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

class LivrolandiaMap
{
private:
    Strategy strategy;
    vector<City> cities;
    vector<vector<Road>> cityRoads;
    float libraryConstructionCost, roadConstructionCost;
    vector<vector<unsigned int>> connectedComponents;

    void dfs(unsigned int &rootCityId, unsigned short int &connectedComponentId)
    {
        this->cities[rootCityId].cor = CINZA;

        for (Road road : cityRoads[rootCityId])
        {
            if (this->cities[road.destCity].cor == BRANCO)
                dfs(road.destCity, connectedComponentId);
        }

        this->cities[rootCityId].cor = PRETO;
        this->cities[rootCityId].connectedComponentID = connectedComponentId;
        this->connectedComponents[connectedComponentId].push_back(rootCityId);
    }

    void findConnectedComponents(void)
    {

        unsigned short int connectedComponentId = 0;

        for (unsigned int cityIdx = 0; cityIdx < this->cities.size(); cityIdx++)
        {
            if (this->cities[cityIdx].cor == BRANCO)
            {
                this->connectedComponents.push_back(vector<unsigned int>());
                this->dfs(cityIdx, connectedComponentId);
                connectedComponentId++;
            }
        }
    }

public:
    LivrolandiaMap() {};
    LivrolandiaMap(
        Strategy &strategy,
        vector<City> &cities,
        vector<vector<Road>> &roads,
        float &libraryConstructionCost,
        float &roadConstructionCost) : strategy(strategy),
                                       cities(cities),
                                       cityRoads(roads),
                                       libraryConstructionCost(libraryConstructionCost),
                                       roadConstructionCost(roadConstructionCost) {};

    float calculateMinReconstructioCost(void)
    {
        this->findConnectedComponents();

        float totalRecontructionCosts = 0.0;

        if (this->strategy == ONE_LIBRARY_PER_CITY)
        {
            for (unsigned short int connCompIdx = 0; connCompIdx < this->connectedComponents.size(); connCompIdx++)
            {
                totalRecontructionCosts += this->connectedComponents[connCompIdx].size() * this->libraryConstructionCost;
            }
            
        }
        else
        {
            for (unsigned short int connCompIdx = 0; connCompIdx < this->connectedComponents.size(); connCompIdx++)
            {
                totalRecontructionCosts += this->libraryConstructionCost + (this->connectedComponents[connCompIdx].size() - 1) * this->roadConstructionCost;
            }
        }
        return totalRecontructionCosts;
    }
};

class ReconstructionPlan
{
private:
    vector<LivrolandiaMap> livrolandiaMaps;
    vector<float> minReconstructionCosts;

public:
    void addMap(LivrolandiaMap &map)
    {
        this->livrolandiaMaps.push_back(map);
    }

    void calculateMinReconstructionCosts(void)
    {
        for (LivrolandiaMap &map : this->livrolandiaMaps)
            this->minReconstructionCosts.push_back(map.calculateMinReconstructioCost());
    }

    vector<float> getMinReconstructionCosts(void)
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
        vector<vector<Road>> roads;
        float libraryConstructionCost, roadConstructionCost;
        Strategy strategy;

        cin >> numCities >> numRoads >> libraryConstructionCost >> roadConstructionCost;

        // Strategy
        if (libraryConstructionCost <= (roadConstructionCost / 2))
            strategy = ONE_LIBRARY_PER_CITY;
        else
            strategy = ONE_LIBRARY_CONNECTED_BY_ROADS;

        for (int cityCounter = 0; cityCounter < numCities; cityCounter++)
        {
            cities.push_back(City());
            roads.push_back(vector<Road>());
        }

        for (int roadCounter = 0; roadCounter < numRoads; roadCounter++)
        {
            unsigned int srcCity, destCity;
            cin >> srcCity >> destCity;
            // Adjusting the indexes
            srcCity -= 1;
            destCity -= 1;
            // Undirected graph
            roads[srcCity].push_back(Road(srcCity, destCity));
            roads[destCity].push_back(Road(destCity, srcCity));
        }

        LivrolandiaMap livrolandiaMap(strategy, cities, roads, libraryConstructionCost, roadConstructionCost);
        reconstructionPlan.addMap(livrolandiaMap);
    }
}

void writeOutput(ReconstructionPlan &reconstructionPlan)
{
    for (float minReconstructionCost : reconstructionPlan.getMinReconstructionCosts())
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
}