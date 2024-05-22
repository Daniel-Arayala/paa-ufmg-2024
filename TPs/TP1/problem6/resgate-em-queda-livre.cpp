#include <ios>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include <unordered_set>
#include <iomanip>

using namespace std;

typedef struct {
    int id;
    float x;
    float y;    
} Person;


class WebConnection {
    public:
        Person person1;
        Person person2;
        float distance;
        WebConnection(const Person& person1, const Person& person2, const float distance) {
            this->person1 = person1;
            this->person2 = person2;
            this->distance = distance;
        }

        // for use with the 
        bool operator<(const WebConnection& other) const {
            return distance < other.distance;
        }
};

class DisjointSet {
    private:
        unordered_map<int, int> disjointSetParents;
        unordered_map<int, int> rank;
        bool allElementsInSameSet = false;

        void makeSet(const Person& person) {
            disjointSetParents[person.id] = person.id;
            rank[person.id] = 1; // Number of nodes in the set
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
            for(const Person person: people) {
                makeSet(person);
            }
        }

        bool areAllElementsInSameSet(void) {
            return allElementsInSameSet;
        }

        int findSet(const Person& person) {
            return findSetRecursive(person.id);
        }

        bool unionSet(const WebConnection& webConnection) {
            if (webConnection.person1.id == webConnection.person2.id) 
                return false;

            int representativePerson1 = findSetRecursive(webConnection.person1.id);
            int representativePerson2 = findSetRecursive(webConnection.person2.id);

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
        float calculateDistance(const Person& person1, const Person& person2) {
            return sqrt(pow(person1.x - person2.x, 2) + pow(person1.y - person2.y, 2));
        }
    public:

        void addPerson(Person& newPerson){
            for(const Person person: people) {
                float webDistance = calculateDistance(person, newPerson);
                WebConnection webConnection(person, newPerson, webDistance);
                webConnections.push_back(webConnection);
            }            
            people.push_back(newPerson);
        }

        // Kruskal implementation
        float minimizeWebTotalDistance(void) {
            sort(webConnections.begin(), webConnections.end());

            float webTotalDistance = 0;
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

        testCases.push_back(testCase);
    }
}

int main() {
    // Disable synchronization between C and C++ I/O operations
    ios_base::sync_with_stdio(false);
    // Holds the entire input data and operates over it
    vector<TestCase> testCases;

    readInput(testCases);

    for(TestCase testCase: testCases) {
        float minWebTotalDistanceMeters = testCase.minimizeWebTotalDistance() / 100;
        cout  << fixed << setprecision(2) << minWebTotalDistanceMeters << endl;
        
    }

    return 0;
}