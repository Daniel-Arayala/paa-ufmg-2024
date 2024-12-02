#include <queue>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

void readInput(vector<pair<string, string>> &testCases)
{
    int numTestCases;

    cin >> numTestCases;

    for (int i = 0; i < numTestCases; i++)
    {
        string num1, num2;

        cin >> num1 >> num2;

        testCases.push_back(make_pair(num1, num2));
    }
}

int calculateNumPresses(string num1, string num2)
{
    queue<int> numberQueue;
    unordered_set<int> visitedNums;

    int num1Int = stoi(num1);
    int referenceNumber = stoi(num2);

    numberQueue.push(num1Int);
    visitedNums.insert(num1Int);

    int numPresses = 0;

    while (!numberQueue.empty())
    {
        int levelSize = numberQueue.size();

        for (int i = 0; i < levelSize; i++)
        {
            int currentNum = numberQueue.front();
            numberQueue.pop();

            if (currentNum == referenceNumber)
                return numPresses;

            // Plus 1
            int currentNumPlus1 = currentNum + 1;
            if (visitedNums.find(currentNumPlus1) == visitedNums.end())
            {
                numberQueue.push(currentNumPlus1);
                visitedNums.insert(currentNumPlus1);
            }

            // Inverted
            string reverseNumStr = to_string(currentNum);
            reverse(reverseNumStr.begin(), reverseNumStr.end());
            int reversedNum = stoi(reverseNumStr);
            if (visitedNums.find(reversedNum) == visitedNums.end())
            {
                numberQueue.push(reversedNum);
                visitedNums.insert(reversedNum);
            }
        }

        numPresses++;
    }

    return -1; // Caso não encontre o número de referência
}

vector<int> calculateNumPressesForAllTestCases(vector<pair<string, string>> &testCases)
{
    vector<int> numPressesForAllTestCases;

    for (int i = 0; i < testCases.size(); i++)
    {
        numPressesForAllTestCases.push_back(
            calculateNumPresses(testCases[i].first, testCases[i].second));
    }

    return numPressesForAllTestCases;
}

void writeOutput(vector<int> &numPressesForAllTestCases)
{
    for (const int &numPresses : numPressesForAllTestCases)
        cout << numPresses << endl;
}

int main()
{
    vector<pair<string, string>> testCases;
    readInput(testCases);
    vector<int> numPressesForAllTestCases = calculateNumPressesForAllTestCases(testCases);
    writeOutput(numPressesForAllTestCases);
    return 0;
}
