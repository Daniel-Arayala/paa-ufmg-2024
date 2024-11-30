#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

#define BRANCO 0
#define CINZA 1
#define PRETO 2

typedef enum
{
    ONE_LIBRARY_PER_CITY,
    ONE_LIBRARY_CONNECTED_BY_ROADS
} Strategy;

struct City
{
    unsigned short int cor = BRANCO;
    unsigned short int connectedComponentID;
};

class DisjointSet
{
private:
    vector<int> parent;
    vector<int> rank;

    void makeSet(int id)
    {
        parent[id] = id;
        rank[id] = 1;
    }

public:
    DisjointSet(int size)
    {
        parent.resize(size);
        rank.resize(size);
        for (int i = 0; i < size; ++i)
        {
            makeSet(i);
        }
    }

    int findSet(int id)
    {
        if (id != parent[id])
        {
            parent[id] = findSet(parent[id]);
        }
        return parent[id];
    }

    bool unionSet(int id1, int id2)
    {
        int root1 = findSet(id1);
        int root2 = findSet(id2);

        if (root1 == root2)
        {
            return false;
        }

        if (rank[root1] < rank[root2])
        {
            parent[root1] = root2;
        }
        else if (rank[root1] > rank[root2])
        {
            parent[root2] = root1;
        }
        else
        {
            parent[root2] = root1;
            rank[root1]++;
        }
        return true;
    }
};

class LivrolandiaMap
{
private:
    Strategy strategy;
    DisjointSet connectedCities;
    long int libraryConstructionCost, roadConstructionCost;
    int numCities;

public:
    LivrolandiaMap(
        Strategy strategy,
        int numCities,
        DisjointSet &connectedCities,
        long int libraryConstructionCost,
        long int roadConstructionCost)
        : strategy(strategy),
          connectedCities(connectedCities),
          libraryConstructionCost(libraryConstructionCost),
          roadConstructionCost(roadConstructionCost),
          numCities(numCities) {}

    long int calculateMinReconstructionCost()
    {
        unordered_map<int, int> componentSize;
        for (int i = 0; i < numCities; ++i)
        {
            int root = connectedCities.findSet(i);
            componentSize[root]++;
        }

        long int totalCost = 0;
        for (const auto &entry : componentSize)
        {
            int size = entry.second;
            if (strategy == ONE_LIBRARY_PER_CITY)
            {
                totalCost += size * libraryConstructionCost;
            }
            else
            {
                totalCost += libraryConstructionCost + (size - 1) * roadConstructionCost;
            }
        }

        return totalCost;
    }
};

class ReconstructionPlan
{
private:
    vector<LivrolandiaMap> livrolandiaMaps;
    vector<long int> minReconstructionCosts;

public:
    void addMap(LivrolandiaMap &map)
    {
        livrolandiaMaps.push_back(map);
    }

    void calculateMinReconstructionCosts()
    {
        for (LivrolandiaMap &map : livrolandiaMaps)
        {
            minReconstructionCosts.push_back(map.calculateMinReconstructionCost());
        }
    }

    vector<long int> getMinReconstructionCosts()
    {
        return minReconstructionCosts;
    }
};

void readInput(ReconstructionPlan &reconstructionPlan)
{
    unsigned short int numLivroMaps;
    cin >> numLivroMaps;

    for (unsigned short int livroMap = 0; livroMap < numLivroMaps; livroMap++)
    {
        unsigned int numCities, numRoads;
        long int libraryConstructionCost, roadConstructionCost;

        cin >> numCities >> numRoads >> libraryConstructionCost >> roadConstructionCost;

        Strategy strategy = (libraryConstructionCost < roadConstructionCost) ? ONE_LIBRARY_PER_CITY : ONE_LIBRARY_CONNECTED_BY_ROADS;

        DisjointSet connectedCities(numCities);

        for (int roadCounter = 0; roadCounter < numRoads; roadCounter++)
        {
            unsigned int srcCity, destCity;
            cin >> srcCity >> destCity;
            connectedCities.unionSet(srcCity - 1, destCity - 1);
        }

        LivrolandiaMap livrolandiaMap(strategy, numCities, connectedCities, libraryConstructionCost, roadConstructionCost);
        reconstructionPlan.addMap(livrolandiaMap);
    }
}

void writeOutput(ReconstructionPlan &reconstructionPlan)
{
    for (long int minReconstructionCost : reconstructionPlan.getMinReconstructionCosts())
    {
        cout << minReconstructionCost << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReconstructionPlan reconstructionPlan;
    readInput(reconstructionPlan);
    reconstructionPlan.calculateMinReconstructionCosts();
    writeOutput(reconstructionPlan);

    return 0;
}
