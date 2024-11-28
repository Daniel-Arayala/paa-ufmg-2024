#include <vector>
#include <iostream>
#include <unordered_map>

#define BRANCO 0
#define CINZA 1
#define PRETO 2

using namespace std;

struct Intersection
{
    unsigned short int id;
    unsigned short int cor = BRANCO;

    Intersection() {};
    Intersection(unsigned short int id) : id(id) {};
};

struct Road
{
    unsigned short int sourceIntersectionId;
    unsigned short int destIntersectionId;

    Road() {};
    Road(unsigned short int sourceIntersectionId, unsigned short int destIntersectionId)
        : sourceIntersectionId(sourceIntersectionId), destIntersectionId(destIntersectionId) {};
};

struct City
{
    vector<Intersection> intersections;
    unordered_map<unsigned short int, vector<Road>> roads;

    City() {};
    City(vector<Intersection> intersections, unordered_map<unsigned short int, vector<Road>> roads)
        : intersections(intersections), roads(roads) {};
};

class ConnectedCities
{
private:
    vector<City> cities;
    vector<bool> areCitiesConnected;

    void dfs(City &city, unsigned short int rootIntersectionId)
    {
        city.intersections[rootIntersectionId].cor = CINZA;

        for (Road destRoad : city.roads[rootIntersectionId])
        {
            if (city.intersections[destRoad.destIntersectionId].cor == BRANCO)
                dfs(city, destRoad.destIntersectionId);
        }

        city.intersections[rootIntersectionId].cor = PRETO;
    }

    void resetCityColors(City &city)
    {
        for (Intersection &intersection : city.intersections)
            intersection.cor = BRANCO;
    }

    bool checkConnectedCity(City &city)
    {

        for (Intersection intersectionRoot : city.intersections)
        {
            this->dfs(city, intersectionRoot.id);

            for (Intersection intersection : city.intersections)
            {
                if (intersection.cor != PRETO)
                {
                    return false;
                }
            }
            resetCityColors(city);
        }

        return true;
    }

public:
    void addCity(City city)
    {
        cities.push_back(city);
    }

    void checkConnectedCities(void)
    {
        for (City city : cities)
            areCitiesConnected.push_back(this->checkConnectedCity(city));
    }

    void writeOutput(void)
    {
        for (bool isCityConnected : areCitiesConnected)
            cout << (isCityConnected ? 1 : 0) << endl;
    }
};

void readInput(ConnectedCities &connectedCities)
{
    unsigned short int numIntersections, numRoads;

    while (true)
    {
        // Reads the total number of translations and word pairs
        cin >> numIntersections >> numRoads;

        if (numIntersections == 0 && numRoads == 0)
            break;

        City city;

        // Creates intersections
        for (unsigned short int id = 0; id < numIntersections; id++)
            city.intersections.push_back(Intersection(id));

        // Reads the word pairs and stores them in the translation dictionary
        for (unsigned short int i = 0; i < numRoads; i++)
        {
            unsigned short int sourceIntersectionId, destIntersectionId, numWays;

            cin >> sourceIntersectionId >> destIntersectionId >> numWays;

            // Shifting indexes to start at 0;
            sourceIntersectionId -= 1;
            destIntersectionId -= 1;

            city.roads[sourceIntersectionId].push_back(
                Road(sourceIntersectionId, destIntersectionId));

            if (numWays == 2)
                city.roads[destIntersectionId].push_back(
                    Road(destIntersectionId, sourceIntersectionId));
        }
        connectedCities.addCity(city);
    }
}

int main()
{
    ios::sync_with_stdio(false);
    ConnectedCities connectedCities;

    readInput(connectedCities);
    connectedCities.checkConnectedCities();
    connectedCities.writeOutput();

    return 0;
}