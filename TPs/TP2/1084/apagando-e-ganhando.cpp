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
    int maxNumber;

public:
    BoardNumber(const string &number, const int &maxErase)
        : number(number), maxErase(maxErase)
    {
        maxSelect = number.size() - maxErase;
    }

    void findMaxBoardNumber()
    {
        vector<int> memoizedNumber(maxSelect + 1, 0);

        for (char ch : number)
        {
            int currentDigit = ch - '0';
            int limit = min(maxSelect, static_cast<int>(number.size() - (&ch - &number[0])));
            for (int select_i = maxSelect; select_i >= 1; select_i--)
            {
                memoizedNumber[select_i] = max(memoizedNumber[select_i], memoizedNumber[select_i - 1] * 10 + currentDigit);
            }
        }

        maxNumber = memoizedNumber[maxSelect];
    }

    int getMaxNumber() const { return maxNumber; }
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
        cout << boardNumber.getMaxNumber() << endl;
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