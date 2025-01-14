#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class BoardNumber
{
private:
    string number;
    int maxErase;
    int maxSelect;
    vector<char> result;

public:
    BoardNumber(const string &number, const int &maxErase)
        : number(number), maxErase(maxErase)
    {
        maxSelect = number.size() - maxErase;
        result.reserve(maxSelect);
    }

    void findMaxBoardNumber(void)
    {
        int eraseCount = 0;
        for (char currentDigit : number)
        {
            while (!result.empty() && result.back() < currentDigit && eraseCount < maxErase)
            {
                result.pop_back();
                eraseCount++;
            }
            if (result.size() < maxSelect)
            {
                result.push_back(currentDigit);
            }
            else
            {
                eraseCount++;
            }
        }
    }

    vector<char> getResult() const { return result; }
};

void readInput(vector<BoardNumber> &boardNumberInstances)
{
    int numDigits, maxErase;
    while (cin >> numDigits >> maxErase && (numDigits != 0 || maxErase != 0))
    {
        string number;
        cin >> number;
        boardNumberInstances.emplace_back(number, maxErase);
    }
}

void getMaxBoardNumbers(vector<BoardNumber> &boardNumberInstances)
{
    for (auto &boardNumber : boardNumberInstances)
        boardNumber.findMaxBoardNumber();
}

void writeOutput(vector<BoardNumber> &boardNumberInstances)
{
    for (const auto &boardNumber : boardNumberInstances)
    {
        for (const auto &digit : boardNumber.getResult())
        {
            cout << digit;
        }
        cout << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<BoardNumber> boardNumberInstances;

    readInput(boardNumberInstances);
    getMaxBoardNumbers(boardNumberInstances);
    writeOutput(boardNumberInstances);

    return 0;
}