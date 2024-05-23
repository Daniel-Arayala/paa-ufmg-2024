#include <ios>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include <iomanip>

using namespace std;

typedef struct {
    int id;
    double x;
    double y;    
} Person;


class WebConnection {
    public:
        int person1ID;
        int person2ID;
        double distance;
        WebConnection(const int& person1ID, const int& person2ID, const double& distance) {
            this->person1ID = person1ID;
            this->person2ID = person2ID;
            this->distance = distance;
        }

        // for use with the 
        bool operator<(const WebConnection& other) const {
            return distance < other.distance;
        }
};

class DisjointSet {
    private:
        vector<int> disjointSetParents;
        vector<int> rank;
        bool allElementsInSameSet = false;

        void makeSet(const Person& person) {
            disjointSetParents.emplace_back(person.id);
            rank.emplace_back(1); // Number of nodes in the set
        }

        int findSetRecursive(const int& personID) {
            if(personID == disjointSetParents[personID]) {
                return personID;
            }
            else {
                return findSetRecursive(disjointSetParents[personID]);
            }
        }

    public:
        DisjointSet(const vector<Person>& people){
            rank.reserve(people.size());
            disjointSetParents.reserve(people.size());
            for(const Person& person: people) {
                makeSet(person);
            }
        }

        bool areAllElementsInSameSet(void) const {
            return allElementsInSameSet;
        }

        bool unionSet(const WebConnection& webConnection) {
            if (webConnection.person1ID == webConnection.person2ID) 
                return false;

            int representativePerson1 = findSetRecursive(webConnection.person1ID);
            int representativePerson2 = findSetRecursive(webConnection.person2ID);

            if (representativePerson1 == representativePerson2){
                return false;
            }
            else if(rank[representativePerson1] <= rank[representativePerson2]) {
                disjointSetParents[representativePerson1] = representativePerson2;
                rank[representativePerson2] += rank[representativePerson1];
                if (rank[representativePerson2] == disjointSetParents.size())
                    allElementsInSameSet = true;
            }
            else {
                disjointSetParents[representativePerson2] = representativePerson1;
                rank[representativePerson1] += rank[representativePerson2];
                if (rank[representativePerson1] == disjointSetParents.size())
                    allElementsInSameSet = true;
            }

            return true;
        }

};

class TestCase {
    private:
        // VARIABLES --------------------------------------------------
        vector<Person> people;
        vector<WebConnection> webConnections;

        // FUNCTIONS --------------------------------------------------
        double calculateDistance(const Person& person1, const Person& person2) {
            return sqrt(pow(person1.x - person2.x, 2) + pow(person1.y - person2.y, 2));
        }
    public:

        void addPerson(Person& newPerson){
            for(const Person& person: people) {
                double webDistance = calculateDistance(person, newPerson);
                WebConnection webConnection(person.id, newPerson.id, webDistance);
                webConnections.emplace_back(webConnection);
            }            
            people.push_back(newPerson);
        }

        // Kruskal implementation
        double minimizeWebTotalDistance(void) {
            sort(webConnections.begin(), webConnections.end());

            double webTotalDistance = 0;
            DisjointSet disjointSet(people);
            for(const WebConnection webConnection: webConnections) {
                if(disjointSet.areAllElementsInSameSet())
                    break;

                if(disjointSet.unionSet(webConnection))
                    webTotalDistance += webConnection.distance;                
            }

            return webTotalDistance;
        }
};

void readInput(vector<TestCase>& testCases) {
    int numTestCases;
    cin >> numTestCases;
    testCases.reserve(numTestCases);

    for (int i = 0; i < numTestCases; i++){
        int numPeople;
        cin >> numPeople;
        TestCase testCase;
        for (int j = 0; j < numPeople; j++){
            Person personCoord;
            cin >> personCoord.x >> personCoord.y;
            personCoord.id = j;
            testCase.addPerson(personCoord);
        }

        testCases.emplace_back(testCase);
    }
}

int main() {
    // Disable synchronization between C and C++ I/O operations
    ios_base::sync_with_stdio(false);
    // Holds the entire input data and operates over it
    vector<TestCase> testCases;

    readInput(testCases);

    for(TestCase& testCase: testCases) {
        double minWebTotalDistanceMeters = testCase.minimizeWebTotalDistance() / 100;
        cout  << fixed << setprecision(2) << minWebTotalDistanceMeters << endl;
        
    }

    return 0;
}