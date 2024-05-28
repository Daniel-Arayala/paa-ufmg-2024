#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <cmath>
#include <iomanip>


using namespace std;


struct Wall {
    int id;
    float x;
    float y;

    Wall () {};

    Wall(const int& id, const float& x, const float& y)
        : id(id), x(x), y(y) {}
};

struct Sensor {
    int id;
    float x;
    float y;
    float range;

    Sensor(const int& id, const float& x, const float& y, const float& range)
        : id(id), x(x), y(y), range(range) {}
};


struct Saloon {
    float width;
    float length;

    Saloon(){};

    Saloon(const float& width, const float& length)
        : width(width), length(length) {}
};


class DisjointSet {
    private:
        unordered_map<int, int> disjointSetParents;
        unordered_map<int, int> rank;
        bool allElementsInSameSet = false;

        void makeSet(int id) {
            disjointSetParents[id] = id;
            rank[id] = 1; // Number of nodes in the set
        }

        int findSetRecursive(const int& id) {
            if(id == disjointSetParents[id]) {
                return id;
            }
            else {
                return findSetRecursive(disjointSetParents[id]);
            }
        }

    public:
        DisjointSet(){};

        DisjointSet(const vector<Wall>& walls, const vector<Sensor>& sensors) {
            int totalSize = walls.size() + sensors.size();
            rank.reserve(totalSize);
            disjointSetParents.reserve(totalSize);
            for(const Wall& wall: walls) {
                makeSet(wall.id);
            }

            for(const Sensor& sensor: sensors) {
                makeSet(sensor.id);
            }

        }

        bool areAllElementsInSameSet(void) const {
            return allElementsInSameSet;
        }

        bool unionSet(const int& id1, const int& id2) {
            if (id1 == id2) 
                return false;

            int representative1 = findSetRecursive(id1);
            int representative2 = findSetRecursive(id2);

            if (representative1 == representative2){
                return false;
            }

            else if(rank[representative1] <= rank[representative2]) {
                disjointSetParents[representative1] = representative2;
                rank[representative2] += rank[representative1];
                if (rank[representative2] == disjointSetParents.size())
                    allElementsInSameSet = true;
            }
            else {
                disjointSetParents[representative2] = representative1;
                rank[representative1] += rank[representative2];
                if (rank[representative1] == disjointSetParents.size())
                    allElementsInSameSet = true;
            }
            return true;
        }

        int findSet(const int& id) {
            return findSetRecursive(id);
        }

};

class ArtRobbery {
    private:
        Wall leftWall;
        Wall rightWall;
        Wall frontWall;
        Wall backWall;
        vector<Sensor> sensors;
        Saloon saloon;
        DisjointSet disjointSet;

        float calculateDistanceBetweenSensors(const Sensor& sensor1, const Sensor& sensor2) {
            return sqrt(pow(sensor1.x - sensor2.x, 2) + pow(sensor1.y - sensor2.y, 2));
        }

        void assignSensorsToWalls() {
            for (Sensor& sensor: sensors) {
                // Checks intersection with left wall
                if (sensor.x - sensor.range <= leftWall.x) {
                    disjointSet.unionSet(sensor.id, leftWall.id);
                }
                // Checks intersection with right wall 
                else if (sensor.x + sensor.range >= rightWall.x) {
                    disjointSet.unionSet(sensor.id, rightWall.id);
                }
                // Checks intersection with top wall
                else if (sensor.y + sensor.range >= frontWall.y) {
                    disjointSet.unionSet(sensor.id, frontWall.id);
                }
                else if (sensor.y - sensor.range <= backWall.y) {
                    disjointSet.unionSet(sensor.id, backWall.id);
                }
            }
        }

        void findIntersectionBetweenSensors() {
            for (int i = 0; i < sensors.size(); i++) {
                for (int j = i + 1; j < sensors.size(); j++) {
                    float distanceBetweenSensors = calculateDistanceBetweenSensors(sensors[i], sensors[j]);
                    if (distanceBetweenSensors <= sensors[i].range + sensors[j].range) {
                        disjointSet.unionSet(sensors[i].id, sensors[j].id);
                    }
                }
            }
        }

    public:
        ArtRobbery(const Saloon& saloon, vector<Sensor>& sensors) {
            this->sensors = sensors;
            this->saloon = saloon;
            // Initializing walls

            int baseID = sensors.back().id;
            this->leftWall = Wall(++baseID, 0, 0);
            this->rightWall = Wall(++baseID, saloon.width, 0);
            this->frontWall = Wall(++baseID, 0, saloon.length);
            this->backWall = Wall(++baseID, 0, 0);
            vector<Wall> walls({leftWall, rightWall, frontWall, backWall});
            this->disjointSet = DisjointSet(walls, sensors);
        }

        bool isRobberyPossible(void) {
            assignSensorsToWalls();
            findIntersectionBetweenSensors();

            // Identifying cutting planes that blocks the thief from robbing the painting
            bool frontAndRightWallsIntersection = disjointSet.findSet(frontWall.id) == disjointSet.findSet(rightWall.id);
            bool leftAndRightWallsIntersection = disjointSet.findSet(leftWall.id) == disjointSet.findSet(rightWall.id);
            bool frontAndBackWallsIntersection = disjointSet.findSet(frontWall.id) == disjointSet.findSet(backWall.id);
            bool leftAndBackWallsIntersection = disjointSet.findSet(leftWall.id) == disjointSet.findSet(backWall.id);
            return !(frontAndRightWallsIntersection || leftAndRightWallsIntersection || frontAndBackWallsIntersection || leftAndBackWallsIntersection);
            
        }

};

ArtRobbery readInput(void) {
    float saloonWidth, saloonLength;
    int numSensors;

    cin >> saloonWidth >> saloonLength >> numSensors;

    Saloon saloon(saloonWidth, saloonLength);
    
    vector<Sensor> sensors;

    for (int i = 0; i < numSensors; i++) {
        int x, y, range;
        cin >> x >> y >> range;
        Sensor sensor(i, x, y, range);
        sensors.emplace_back(sensor);
    }
    ArtRobbery artRobbery(saloon, sensors);

    return artRobbery;
}


int main() {

    ArtRobbery artRobbery = readInput();

    if (artRobbery.isRobberyPossible()) {
        cout << "S" << endl;        
    }
    else {
        cout << "N" << endl;
    }

    return 0;   

}
