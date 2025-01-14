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
        vector<vector<int>> memoizedNumber;
        memoizedNumber.resize(2, vector<int>(maxSelect + 1, 0));
        int prev_i = 0; // Previous index
        int curr_i = 1; // Current index
        for (int digit_i = 1; digit_i <= number.size(); digit_i++)
        {
            for (int select_i = 1; select_i <= maxSelect; select_i++)
            {
                memoizedNumber[curr_i][select_i] = max(
                    memoizedNumber[prev_i][select_i],
                    memoizedNumber[prev_i][select_i - 1] * 10 + (number[digit_i - 1] - '0'));

                maxNumber = max(maxNumber, memoizedNumber[curr_i][select_i]);
            }
            int temp = prev_i;
            prev_i = curr_i;
            curr_i = temp;
        }
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
