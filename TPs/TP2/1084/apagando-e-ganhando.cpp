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
    vector<vector<string>> memoizedNumber;
    int maxNumber = 0;

    int parseToInt(const string &strNumber) {
        if (strNumber.empty()) 
            return 0;
        return stoi(strNumber); 
    }

public:
    BoardNumber(const string &number, const int &maxErase)
        : number(number), maxErase(maxErase)
    {
        this->maxSelect = number.size() - maxErase;
        memoizedNumber.resize(number.size() + 1, vector<string>(maxSelect + 1, ""));
    }

    void findMaxBoardNumber(void)
    {
        for (int digit_i = 1; digit_i <= number.size(); digit_i++)
        {
            for (int select_i = 1; select_i <= maxSelect; select_i++)
            {

                memoizedNumber[digit_i][select_i] = to_string(max(
                    parseToInt(memoizedNumber[digit_i - 1][select_i]),
                    parseToInt(memoizedNumber[digit_i - 1][select_i - 1] + number[digit_i-1])));

                maxNumber = max(maxNumber, parseToInt(memoizedNumber[digit_i][select_i]));
            }
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
