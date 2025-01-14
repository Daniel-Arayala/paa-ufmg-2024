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
    vector<char> result;

public:
    BoardNumber(const string &number, const int &maxErase)
        : number(number), maxErase(maxErase)
    {
        maxSelect = number.size() - maxErase;
        result.resize(maxSelect, '0');
    }

    void findMaxBoardNumber(void)
    {
        for (int i = 0; i < number.size(); i++)
        {
            char currentDigit = number[i];

            bool resultUpdated = false;

            int relativeToMaxSelectPosition = (number.size() - i) - maxSelect;

            int start_index = -min(relativeToMaxSelectPosition, 0);

            for (int r_i = start_index; r_i < maxSelect; r_i++)
            {

                if (resultUpdated)
                {
                    result[r_i] = '0';
                }
                else if ((currentDigit > result[r_i]))
                {
                    result[r_i] = currentDigit;
                    resultUpdated = true;
                }
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