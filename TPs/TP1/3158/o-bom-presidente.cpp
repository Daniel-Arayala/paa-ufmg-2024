#include <iostream>
#include <vector>
#include <unordered_set>
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

struct Road
{
    unsigned int srcCity;
    unsigned int destCity;

    Road() {};
    Road(unsigned int &srcCity, unsigned int &destCity)
    {
        this->srcCity = srcCity;
        this->destCity = destCity;
    }
};

struct City
{
    unsigned short int cor = BRANCO;
    unsigned short int connectedComponentID;
};

class DisjointSet
{
private:
    unordered_map<int, int> disjointSetParents;
    unordered_map<int, int> rank;
    bool allElementsInSameSet = false;

    void makeSet(int id)
    {
        disjointSetParents[id] = id;
        rank[id] = 1; // Number of nodes in the set
    }

    int findSetRecursive(const int &id)
    {
        if (id != disjointSetParents[id])
        {
            disjointSetParents[id] = findSetRecursive(disjointSetParents[id]); // Path compression
        }
        return disjointSetParents[id];
    }

public:
    DisjointSet() {};

    DisjointSet(const vector<City> cities)
    {
        for (int cityIdx = 0; cityIdx < cities.size(); cityIdx++)
        {
            makeSet(cityIdx);
        }
    }

    bool areAllElementsInSameSet(void) const
    {
        return allElementsInSameSet;
    }

    bool unionSet(const int &id1, const int &id2)
    {
        if (id1 == id2)
            return false;

        int representative1 = findSetRecursive(id1);
        int representative2 = findSetRecursive(id2);

        if (representative1 == representative2)
        {
            return false;
        }

        else if (rank[representative1] <= rank[representative2])
        {
            disjointSetParents[representative1] = representative2;
            rank[representative2] += rank[representative1];
            if (rank[representative2] == disjointSetParents.size())
                allElementsInSameSet = true;
        }
        else
        {
            disjointSetParents[representative2] = representative1;
            rank[representative1] += rank[representative2];
            if (rank[representative1] == disjointSetParents.size())
                allElementsInSameSet = true;
        }
        return true;
    }

    int findSet(const int &id)
    {
        return findSetRecursive(id);
    }
};

class LivrolandiaMap
{
private:
    Strategy strategy;
    vector<City> cities;
    DisjointSet connectedCities;

    long int libraryConstructionCost, roadConstructionCost;
    vector<vector<unsigned int>> connectedComponents;

public:
    LivrolandiaMap() {};
    LivrolandiaMap(
        Strategy &strategy,
        vector<City> &cities,
        DisjointSet &connectedCities,
        long int &libraryConstructionCost,
        long int &roadConstructionCost) : strategy(strategy),
                                          cities(cities),
                                          connectedCities(connectedCities),
                                          libraryConstructionCost(libraryConstructionCost),
                                          roadConstructionCost(roadConstructionCost) {};

    long int calculateMinReconstructionCost(void)
    {

        unordered_map<int, long int> totalCostPerConnectedComponent;

        long int totalRecontructionCosts = 0;

        for (int cityIdx = 0; cityIdx < this->cities.size(); cityIdx++)
        {
            int connectedComponentID = this->connectedCities.findSet(cityIdx);

            if (this->strategy == ONE_LIBRARY_PER_CITY)
                totalCostPerConnectedComponent[connectedComponentID] += this->libraryConstructionCost;
            else
                totalCostPerConnectedComponent[connectedComponentID] += this->roadConstructionCost;
        }

        for (auto const &connectedComponent : totalCostPerConnectedComponent)
        {
            totalRecontructionCosts += connectedComponent.second;
            if (this->strategy == ONE_LIBRARY_CONNECTED_BY_ROADS) // adjustement
                totalRecontructionCosts = totalRecontructionCosts + this->libraryConstructionCost - this->roadConstructionCost;
        }

        return totalRecontructionCosts;
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
        this->livrolandiaMaps.push_back(map);
    }

    void calculateMinReconstructionCosts(void)
    {
        for (LivrolandiaMap &map : this->livrolandiaMaps)
            this->minReconstructionCosts.push_back(map.calculateMinReconstructionCost());
    }

    vector<long int> getMinReconstructionCosts(void)
    {
        return this->minReconstructionCosts;
    }
};

void readInput(ReconstructionPlan &reconstructionPlan)
{
    unsigned short int numLivroMaps;
    // Reads the total number of translations and word pairs
    cin >> numLivroMaps;

    // Reading translations
    for (unsigned short int livroMap = 0; livroMap < numLivroMaps; livroMap++)
    {
        unsigned int numCities, numRoads;
        vector<City> cities;

        long int libraryConstructionCost, roadConstructionCost;
        Strategy strategy;

        cin >> numCities >> numRoads >> libraryConstructionCost >> roadConstructionCost;

        // Strategy
        if (libraryConstructionCost < roadConstructionCost)
            strategy = ONE_LIBRARY_PER_CITY;
        else
            strategy = ONE_LIBRARY_CONNECTED_BY_ROADS;

        // Initialize cities
        for (int cityCounter = 0; cityCounter < numCities; cityCounter++)
        {
            cities.push_back(City());
        }

        // Initialize roads
        DisjointSet connectedCities(cities);

        for (int roadCounter = 0; roadCounter < numRoads; roadCounter++)
        {
            unsigned int srcCity, destCity;
            cin >> srcCity >> destCity;
            // Adjusting the indexes
            srcCity -= 1;
            destCity -= 1;

            connectedCities.unionSet(srcCity, destCity);
        }

        LivrolandiaMap livrolandiaMap(strategy, cities, connectedCities, libraryConstructionCost, roadConstructionCost);
        reconstructionPlan.addMap(livrolandiaMap);
    }
}

void writeOutput(ReconstructionPlan &reconstructionPlan)
{
    for (int minReconstructionCost : reconstructionPlan.getMinReconstructionCosts())
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