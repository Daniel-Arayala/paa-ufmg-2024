#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;

const unsigned short int NUM_SHIRT_TYPES = 6;

typedef enum
{
    XXL=1,
    XL,
    L,
    M,
    S,
    XS
} ShirtSize;

unordered_map<string, ShirtSize> ShirtSizeMap = {
    {"XXL", XXL},
    {"XL", XL},
    {"L", L},
    {"M", M},
    {"S", S},
    {"XS", XS}};

typedef enum
{
    SHIRT,
    VOLUNTEER,
    SOURCE,
    SINK
} NodeType;

struct Node
{
    int id;
    int parentId = -1;
    bool visited = false;
    NodeType type;
    int currentFlow = 0;
    int totalCapacity;

    Node() {};
    Node(int id, NodeType type, int totalCapacity) : id(id), type(type), totalCapacity(totalCapacity) {};
};

struct Edge
{
    int srcNodeId;
    int destNodeId;
    int currentFlow = 0;
    int totalCapacity;
    int residualCapacity; // totalCapacity - currentFlow

    Edge() {};
    Edge(int srcNodeId,
         int destNodeId,
         int currentFlow,
         int totalCapacity) : srcNodeId(srcNodeId),
                              destNodeId(destNodeId),
                              currentFlow(currentFlow),
                              totalCapacity(totalCapacity),
                              residualCapacity(totalCapacity - currentFlow) {};

    void addFlow(int flowToAdd)
    {
        this->currentFlow += flowToAdd;
        this->residualCapacity -= flowToAdd;
    }

    void removeFlow(int flowToRemove)
    {
        this->currentFlow -= flowToRemove;
        this->residualCapacity += flowToRemove;
    }
};

class ShirtDistribution
{
private:
    int idCounter = 0;
    vector<Node> nodes;
    vector<vector<Edge>> adjacencyList;
    unordered_map<NodeType, vector<int>> nodeTypeMap;

    void resetNodes(void)
    {
        for (auto &node : this->nodes)
        {
            node.visited = false;
            node.parentId = -1;
        }
    }

    Edge &getEdge(const int &srcNodeId, const int &destNodeId)
    {
        for (Edge &edge : this->adjacencyList[srcNodeId])
        {
            if (edge.destNodeId == destNodeId)
                return edge;
        }
    }

    bool dfs(int srcNodeId, int destNodeId)
    {
        this->nodes[srcNodeId].visited = true;

        if (srcNodeId == destNodeId)
            return true;

        for (Edge &edge : this->adjacencyList[srcNodeId])
        {
            if ((!this->nodes[edge.destNodeId].visited) && (edge.residualCapacity > 0))
            {
                this->nodes[edge.destNodeId].parentId = srcNodeId;
                if (dfs(edge.destNodeId, destNodeId))
                    return true;
            }
        }

        return false;
    }

    int findBottleneck(const int &srcNodeId, const int &sinkNodeId)
    {
        int currentNodeId = sinkNodeId;
        int bottleneck = INT_MAX;

        while (currentNodeId != srcNodeId)
        {
            const int parentId = this->nodes[currentNodeId].parentId;
            int residualCapacity = this->getEdge(parentId, currentNodeId).residualCapacity;

            if (residualCapacity < bottleneck)
                bottleneck = residualCapacity;

            currentNodeId = this->nodes[currentNodeId].parentId;
        }

        return bottleneck;
    }

    bool applyAugmentingPathFromSourceToSink(void)
    {
        int srcNodeId = this->nodes[nodeTypeMap[SOURCE][0]].id;
        int sinkNodeId = this->nodes[nodeTypeMap[SINK][0]].id;

        bool pathExists = dfs(srcNodeId, sinkNodeId);

        if (pathExists)
        {
            int maxFlow = findBottleneck(srcNodeId, sinkNodeId);

            int currentNodeId = sinkNodeId;

            while (currentNodeId != srcNodeId)
            {
                const int parentId = this->nodes[currentNodeId].parentId;
                Edge &edge = this->getEdge(parentId, currentNodeId);
                edge.addFlow(maxFlow);
                this->nodes[edge.destNodeId].currentFlow += maxFlow;

                Edge &reverseEdge = this->getEdge(currentNodeId, parentId);
                reverseEdge.removeFlow(maxFlow);
                this->nodes[reverseEdge.destNodeId].currentFlow -= maxFlow;
                currentNodeId = this->nodes[currentNodeId].parentId;
            }

            this->resetNodes();

            return true;
        }
        return false;
    }

public:
    int getNodeId(void)
    {
        return this->idCounter++;
    }

    int getNumNodes(void)
    {
        return this->nodes.size();
    }

    void addNode(const NodeType &nodetype, const int &totalCapacity)
    {
        Node node(this->getNodeId(), nodetype, totalCapacity);
        this->nodes.push_back(node);
        this->adjacencyList.push_back(vector<Edge>());
        this->nodeTypeMap[nodetype].push_back(node.id);
    }

    void addEdge(Edge &edge)
    {
        if ((edge.srcNodeId < this->idCounter) && (edge.destNodeId < this->idCounter))
        {
            this->adjacencyList[edge.srcNodeId].push_back(edge);
        }
        else
        {
            cerr << "Node does not exist!" << endl;
        }
    }

    vector<int> getNodeIdsByType(const NodeType &nodetype)
    {
        return this->nodeTypeMap[nodetype];
    }

    bool isShirtDistributionPossible(void)
    {
        while (this->applyAugmentingPathFromSourceToSink());

        int accumulatedFlowSink = this->nodes[this->nodeTypeMap[SINK][0]].currentFlow;

        return accumulatedFlowSink == this->getNodeIdsByType(VOLUNTEER).size();
    }
};

void readInput(vector<ShirtDistribution> &shirtDistributions)
{
    // Base variables
    int numTestCases;

    cin >> numTestCases;

    for (int tidx = 0; tidx < numTestCases; tidx++)
    {
        ShirtDistribution shirtDistribution;

        int numShirtsAvailable, numVolunteers;

        cin >> numShirtsAvailable >> numVolunteers;

        int numShirtsPerType = numShirtsAvailable / NUM_SHIRT_TYPES;

        // Create source node
        shirtDistribution.addNode(SOURCE, numShirtsAvailable);

        int sourceNodeId = shirtDistribution.getNumNodes() - 1;

        // Create shirt nodes
        for (int shirtTypeidx = 0; shirtTypeidx < NUM_SHIRT_TYPES; shirtTypeidx++)
        {
            shirtDistribution.addNode(SHIRT, numShirtsPerType);
            int shirtNodeId = shirtDistribution.getNumNodes() - 1;
            Edge sourceToShirt(sourceNodeId, shirtNodeId, 0, numShirtsPerType);
            Edge shirtToSource(shirtNodeId, sourceNodeId, numShirtsPerType, numShirtsPerType);
            shirtDistribution.addEdge(sourceToShirt);
            shirtDistribution.addEdge(shirtToSource);
        }

        // Create Sink Node
        shirtDistribution.addNode(SINK, numVolunteers);

        int sinkNodeId = shirtDistribution.getNumNodes() - 1;

        for (int volunteerIdx = 0; volunteerIdx < numVolunteers; volunteerIdx++)
        {
            string shirtSize1, shirtSize2;
            cin >> shirtSize1 >> shirtSize2;

            shirtDistribution.addNode(VOLUNTEER, 1);
            int currentVolunteerId = shirtDistribution.getNumNodes() - 1;
            Edge volunteerToSink(currentVolunteerId, sinkNodeId, 0, 1);
            Edge sinkToVolunteer(sinkNodeId, currentVolunteerId, 1, 1);

            Edge shirt1ToVolunteer(ShirtSizeMap[shirtSize1], currentVolunteerId, 0, 1);
            Edge volunteerToShirt1(currentVolunteerId, ShirtSizeMap[shirtSize1], 1, 1);

            Edge shirt2ToVolunteer(ShirtSizeMap[shirtSize2], currentVolunteerId, 0, 1);
            Edge volunteerToShirt2(currentVolunteerId, ShirtSizeMap[shirtSize2], 1, 1);

            shirtDistribution.addEdge(volunteerToSink);
            shirtDistribution.addEdge(sinkToVolunteer);
            shirtDistribution.addEdge(shirt1ToVolunteer);
            shirtDistribution.addEdge(volunteerToShirt1);
            shirtDistribution.addEdge(shirt2ToVolunteer);
            shirtDistribution.addEdge(volunteerToShirt2);
        }

        shirtDistributions.push_back(shirtDistribution);
    }
}

void writeOutput(vector<ShirtDistribution> &shirtDistributions)
{
    for (ShirtDistribution &shirtDistribution : shirtDistributions)
        shirtDistribution.isShirtDistributionPossible() ? cout << "YES" << endl : cout << "NO" << endl;
}

int main()
{
    vector<ShirtDistribution> shirtDistributions;
    readInput(shirtDistributions);
    writeOutput(shirtDistributions);
    return 0;
}