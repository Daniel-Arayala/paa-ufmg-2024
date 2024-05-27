#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <cmath>
#include <iomanip>

using namespace std;

#define INF numeric_limits<float>::infinity()

struct Road {
    int destStrategicPoint;

    Road(int& destStrategicPoint) {
        this->destStrategicPoint = destStrategicPoint;
    }
};

struct StrategicPoint {
    int id;
    int numShooters;
    float knownMaxProbabilityOfSuccess = 0;
    bool visited = false;
    int previousPointID = -1;

    StrategicPoint() {}
    
    StrategicPoint(int& id, int& numShooters) {
        this->id = id;
        this->numShooters = numShooters;
    }

    bool operator<(const StrategicPoint& other) const {
        return this->knownMaxProbabilityOfSuccess < other.knownMaxProbabilityOfSuccess;
    }
};

class AsssassinSoldier {
    private:
        int numStrategicPoints;
        int numRoads;
        int numBullets;
        int soldierStartPoint;
        int soldierEndPoint;
        float killProbability;
        float totalProbabilityOfSuccess = 1;
        vector<vector<Road>> roads;
        vector<StrategicPoint> strategicPoints;

        bool checkIfStrategicPointIsValid(const StrategicPoint& strategicPoint) {
            bool strategicPointMaxProbIsUpToDate = \
                strategicPoint.knownMaxProbabilityOfSuccess == strategicPoints[strategicPoint.id].knownMaxProbabilityOfSuccess;
            bool totalProbabilityOfSuccessIsZero = strategicPoint.knownMaxProbabilityOfSuccess == 0.0;

            return strategicPointMaxProbIsUpToDate && !totalProbabilityOfSuccessIsZero && !strategicPoint.visited;
        }

        float calculateProbabilityOfSuccess(const StrategicPoint& strategicPoint) {
            if (this->numBullets < strategicPoint.numShooters) {
                return 0.0;
            }
            return pow(this->killProbability, strategicPoint.numShooters);
        }

        void dijkstra(void) {
            // Priority Queue Initialization
            priority_queue<StrategicPoint> pq;
            
            this->strategicPoints[this->soldierStartPoint].knownMaxProbabilityOfSuccess = calculateProbabilityOfSuccess(this->strategicPoints[this->soldierStartPoint]);

            pq.push(this->strategicPoints[this->soldierStartPoint]);

            while (!pq.empty() && (pq.top().id != this->soldierEndPoint)) {
                StrategicPoint& currentStrategicPoint = this->strategicPoints[pq.top().id];
                pq.pop();

                if (this->numBullets <= 0) {
                    this->strategicPoints[this->soldierEndPoint].knownMaxProbabilityOfSuccess == 0.0;
                    return;
                }
                else {
                    this->numBullets -= currentStrategicPoint.numShooters;
                }


                if (!checkIfStrategicPointIsValid(currentStrategicPoint)) {
                    continue;
                }

                currentStrategicPoint.visited = true;

                if (currentStrategicPoint.id == this->soldierEndPoint)
                    break;

                for (Road& road : this->roads[currentStrategicPoint.id]) {
                    StrategicPoint& nextStrategicPoint = strategicPoints[road.destStrategicPoint];
                    if (nextStrategicPoint.visited) {
                        continue;
                    }
                    float currentProbabilityOfSuccess = currentStrategicPoint.knownMaxProbabilityOfSuccess;
                    float updatedTotalProbabilityOfSuccess = currentProbabilityOfSuccess * calculateProbabilityOfSuccess(nextStrategicPoint);
                    bool hasEnoughtBullets = this->numBullets >= nextStrategicPoint.numShooters;
                    if (hasEnoughtBullets && (updatedTotalProbabilityOfSuccess > nextStrategicPoint.knownMaxProbabilityOfSuccess)) {
                        nextStrategicPoint.knownMaxProbabilityOfSuccess = updatedTotalProbabilityOfSuccess;
                        nextStrategicPoint.previousPointID = currentStrategicPoint.id;
                        pq.push(nextStrategicPoint);
                    }
                }
            }   
        }

    public:
        AsssassinSoldier(
                int& numStrategicPoints, 
                int& numRoads, 
                int& numBullets,
                int& soldierStartPoint,
                int& soldierEndPoint,
                float& killProbability,
                vector<pair<int, int>>& roadPairs, 
                unordered_map<int, int>& numShootersPerStrategicPoint
        ) {
            this->numStrategicPoints = numStrategicPoints;
            this->numRoads = numRoads;
            this->numBullets = numBullets;
            this->soldierStartPoint = soldierStartPoint;
            this->soldierEndPoint = soldierEndPoint;
            this->killProbability = killProbability;

            // Road initialization
            this->roads.reserve(this->numRoads + 1);
            this->roads.resize(this->numRoads + 1);

            for(pair<int, int>& roadPair : roadPairs) {
                Road road(roadPair.second);
                this->roads[roadPair.first].emplace_back(road);
            }

            // Strategic Point initialization

            this->strategicPoints.reserve(this->numStrategicPoints + 1);
            this->strategicPoints.resize(this->numStrategicPoints + 1);

            for(int i = 1; i <= this->numStrategicPoints; i++) {
                StrategicPoint strategicPoint(i, numShootersPerStrategicPoint[i]);
                this->strategicPoints[i] = strategicPoint;
            }
        }  

        double calculateMaxProbabilityOfSuccess(void) {
            dijkstra();
            return this->strategicPoints[this->soldierEndPoint].knownMaxProbabilityOfSuccess;
        }
};


void readInput(vector<AsssassinSoldier>& assassinSoldiers) {
    int numStrategicPoints, numRoads, numBullets;
    float killProbability;

    while (cin >> numStrategicPoints >> numRoads >> numBullets >> killProbability) {

        vector<pair<int, int>> roadPairs;

        for (int i = 0; i < numRoads; i++) {
            int srcStrategicPoint, destStrategicPoint;
            if (!(cin >> srcStrategicPoint >> destStrategicPoint)) {
                cerr << "Error reading road data." << endl;
                return;
            }
            roadPairs.emplace_back(make_pair(srcStrategicPoint, destStrategicPoint));
        }

        unordered_map<int, int> numShootersPerStrategicPoint;
        int numShooters;
        cin >> numShooters;
        for (int i = 0; i < numShooters; i++) {
            int shooterStrategicPoint;
            cin >> shooterStrategicPoint;
            numShootersPerStrategicPoint[shooterStrategicPoint]++;
        }
        
        int soldierStartPoint, soldierEndPoint;
        cin >> soldierStartPoint >> soldierEndPoint;

        AsssassinSoldier assassinSoldier(
            numStrategicPoints, 
            numRoads, 
            numBullets, 
            soldierStartPoint, 
            soldierEndPoint, 
            killProbability,
            roadPairs, 
            numShootersPerStrategicPoint); 

        assassinSoldiers.emplace_back(assassinSoldier);
    }
}

void readInputFile(vector<AsssassinSoldier>& assassinSoldiers) {
    ifstream massiveMurderFile("inputs/4.txt");

    if (massiveMurderFile.is_open()) {
        while (true) {
            int numStrategicPoints, numRoads, numBullets;
            float killProbability;
            if (!(massiveMurderFile >> numStrategicPoints >> numRoads >> numBullets >> killProbability)) {
                break;
            }

            vector<pair<int, int>> roadPairs;
            for (int i = 0; i < numRoads; i++) {
                int srcStrategicPoint, destStrategicPoint;
                if (!(massiveMurderFile >> srcStrategicPoint >> destStrategicPoint)) {
                    cerr << "Error reading road data." << endl;
                    return;
                }
                roadPairs.emplace_back(make_pair(srcStrategicPoint, destStrategicPoint));
            }

            unordered_map<int, int> numShootersPerStrategicPoint;
            int numShooters;
            massiveMurderFile >> numShooters;
            for (int i = 0; i < numShooters; i++) {
                int shooterStrategicPoint;
                massiveMurderFile >> shooterStrategicPoint;
                numShootersPerStrategicPoint[shooterStrategicPoint]++;
            }
        
            int soldierStartPoint, soldierEndPoint;
            massiveMurderFile >> soldierStartPoint >> soldierEndPoint;

            AsssassinSoldier assassinSoldier(
                numStrategicPoints, 
                numRoads, 
                numBullets, 
                soldierStartPoint, 
                soldierEndPoint, 
                killProbability,
                roadPairs, 
                numShootersPerStrategicPoint); 

            assassinSoldiers.emplace_back(assassinSoldier);
        }
    } else {
        cerr << "Failed to open the file." << endl;
    }
    massiveMurderFile.close();
}

int main() {
    ios::sync_with_stdio(false);

    vector<AsssassinSoldier> assassinSoldiers;
    readInputFile(assassinSoldiers);
    //readInput(assassinSoldiers);

    for (AsssassinSoldier& assassinSoldier : assassinSoldiers) {
        float probabilityOfSuccess = assassinSoldier.calculateMaxProbabilityOfSuccess();
        cout  << fixed << setprecision(3) << probabilityOfSuccess << endl;
    }   

    return 0;
}  


