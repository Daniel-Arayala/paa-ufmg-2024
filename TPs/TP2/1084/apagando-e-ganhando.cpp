#include <iostream>
#include <vector>
#include <string>

using namespace std;

class BoardNumber
{
private:
    string number;
    int maxErase = 0;
    int maxSelect = 0;
    int maxNumber = 0;

public:
    BoardNumber(const string &number, const int &maxErase)
        : number(number), maxErase(maxErase)
    {
        this->maxSelect = number.size() - maxErase;
    }

    void findMaxBoardNumber(void)
    {
        vector<int> prevMemoization(maxSelect + 1, 0);
        vector<int> currMemoization(maxSelect + 1, 0);

        for (int digit_i = 1; digit_i <= number.size(); digit_i++)
        {
            int currentDigit = number[digit_i - 1] - '0';
            int selectionLimit = min(digit_i, maxSelect);

            for (int select_i = 1; select_i <= selectionLimit; select_i++)
            {
                currMemoization[select_i] = max(
                    prevMemoization[select_i],
                    prevMemoization[select_i - 1] * 10 + currentDigit);
            }
            prevMemoization.swap(currMemoization);
        }

        maxNumber = prevMemoization[maxSelect];
    }

    int getMaxNumber(void) const { return maxNumber; }
};

void readInput(vector<BoardNumber> &boardNumberInstances)
{
    int numDigits, maxErase;
    while (true)
    {
        cin >> numDigits >> maxErase;
        if (numDigits == 0 && maxErase == 0)
            break;

        string number;
        cin >> number;
        BoardNumber boardNumber(number, maxErase);
        boardNumberInstances.push_back(boardNumber);
    }
}

void getMaxBoardNumbers(vector<BoardNumber> &boardNumberInstances)
{
    for (auto &boardNumber : boardNumberInstances)
        boardNumber.findMaxBoardNumber();
}

void writeOutput(vector<BoardNumber> &boardNumberInstances)
{
    for (auto &boardNumber : boardNumberInstances)
        cout << boardNumber.getMaxNumber() << endl;
}

int main()
{
    vector<BoardNumber> boardNumberInstances;

    readInput(boardNumberInstances);
    getMaxBoardNumbers(boardNumberInstances);
    writeOutput(boardNumberInstances);

    return 0;
}
