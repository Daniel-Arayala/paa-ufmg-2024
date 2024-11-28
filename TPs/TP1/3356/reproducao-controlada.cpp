#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_set>
#include <queue>
#include <memory>

using namespace std;

struct Animal
{
    string name;
    int level = 0;
    bool visited = false;

    Animal() {};

    Animal(string name) : name(name) {};
};

void readRelationships(
    unordered_map<string, vector<string>> &animalGenTree,
    unordered_map<string, Animal> &animals,
    const int &numAnimals,
    const int &numRelationships)
{
    // Reads the animal names and their translations
    for (int i = 0; i < numRelationships; i++)
    {
        string animalParent1Name, animalParent2Name, animalChildName;

        cin >> animalParent1Name >> animalParent2Name >> animalChildName;

        if (animals.find(animalParent1Name) == animals.end())
        { // Doesn't exist

            Animal animalParent1(animalParent1Name);
            animals[animalParent1Name] = animalParent1;
        }
        if (animals.find(animalParent2Name) == animals.end())
        { // Doesn't exist
            Animal animalParent2(animalParent2Name);
            animals[animalParent2Name] = animalParent2;
        }

        if (animals.find(animalChildName) == animals.end())
        { // Doesn't exist
            Animal animalChild(animalChildName);
            animals[animalChildName] = animalChild;
        }

        animals[animalChildName].level = max(animals[animalParent1Name].level, animals[animalParent2Name].level) + 1;
        animalGenTree[animalParent1Name].push_back(animalChildName);
        animalGenTree[animalParent2Name].push_back(animalChildName);
    }
}

void readTestCases(vector<pair<string, string>> &testCases, const int &numTestCases)
{
    // Reads the test cases
    for (int i = 0; i < numTestCases; i++)
    {
        string animalName1, animalName2;

        cin >> animalName1 >> animalName2;

        testCases.push_back(make_pair(animalName1, animalName2));
    }
}

void readInput(
    unordered_map<string, vector<string>> &animalGenTree,
    unordered_map<string, Animal> &animals,
    vector<pair<string, string>> &testCases)
{
    int numAnimals, numRelationships, numTestCases;

    // Reads the total number of translations and word pairs
    cin >> numAnimals >> numRelationships >> numTestCases;

    readRelationships(animalGenTree, animals, numAnimals, numRelationships);
    readTestCases(testCases, numTestCases);
}

int checkAnimalFromOldestAncientBFS(
    const unordered_map<string, vector<string>> &animalGenTree,
    unordered_map<string, Animal> &animals,
    const string &rootAnimal,
    const string &animal1,
    const string &animal2)
{
    int animalCounter = 0;

    if (animal1 == animal2)
        return 2;

    queue<string> animalToVisit;

    animalToVisit.push(rootAnimal);

    while (!animalToVisit.empty())
    {
        string currentAnimal = animalToVisit.front();
        animalToVisit.pop();

        if (animalGenTree.find(currentAnimal) != animalGenTree.end())
        {
            for (string child : animalGenTree.at(currentAnimal))
            {
                if (!animals[child].visited)
                    animalToVisit.push(child);
            }
        }

        animals[currentAnimal].visited = true;

        if ((currentAnimal.compare(animal1) == 0) || (currentAnimal.compare(animal2) == 0))
            animalCounter++;
    }
    return animalCounter;
}

void resetVisited(unordered_map<string, Animal> &animals)
{
    for (auto animal = animals.begin(); animal != animals.end(); animal++)
        animal->second.visited = false;
}

vector<bool> areAnimalsRelated(
    unordered_map<string, Animal> &animals,
    const vector<pair<string, string>> &testCases,
    const unordered_map<string, vector<string>> &animalGenTree)
{

    vector<bool> results;

    // Check if animals in testCase are related to each other
    for (pair<string, string> animalPair : testCases)
    {
        string animal1 = animalPair.first;
        string animal2 = animalPair.second;
        bool relationshipFound = false;

        for (auto rootAnimal = animals.begin(); rootAnimal != animals.end(); rootAnimal++)
        {
            if (rootAnimal->second.level == 0)
            {
                int animalCounter = checkAnimalFromOldestAncientBFS(animalGenTree, animals, rootAnimal->first, animal1, animal2);
                if (animalCounter == 2)
                {
                    relationshipFound = true;
                    resetVisited(animals);
                    break;
                }
                resetVisited(animals);
            }
        }
        results.push_back(relationshipFound);
    }

    return results;
}

void writeOutput(vector<bool> &results)
{
    for (bool result : results)
        cout << (result ? "verdadeiro" : "falso") << endl;
}

int main()
{
    unordered_map<string, vector<string>> animalGenTree;
    unordered_map<string, Animal> animals;
    vector<pair<string, string>> testCases;

    readInput(animalGenTree, animals, testCases);
    vector<bool> results = areAnimalsRelated(animals, testCases, animalGenTree);
    writeOutput(results);
}
