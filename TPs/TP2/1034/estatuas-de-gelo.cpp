#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

#define INF INT_MAX

using namespace std;

class IceStatue
{
private:
    int blockTypes = 0;
    int desiredLength = 0;
    vector<int> blocksLengths;
    int numBlocksUsed = 0;

public:
    IceStatue(
        const int &blockTypes,
        const int &desiredLength,
        const vector<int> &blocksLengths)
        : blockTypes(blockTypes), desiredLength(desiredLength), blocksLengths(blocksLengths)
    {
        sort(this->blocksLengths.begin(), this->blocksLengths.end(), greater<int>());
    };

    void findMinBlocks(void)
    {
        int currentBlockLength = 0;
        int blockIndex = 0;

        while ((currentBlockLength < this->desiredLength) && (blockIndex < this->blocksLengths.size()))
        {
            if (this->blocksLengths[blockIndex] + currentBlockLength <= desiredLength)
            {
                currentBlockLength += this->blocksLengths[blockIndex];
                this->numBlocksUsed++;
            }
            
            blockIndex++;
        }
    }

    int getNumBlocksUsed(void) const { return this->numBlocksUsed; }
};

void readInput(vector<IceStatue> &iceStatues)
{
    int numInstances;
    cin >> numInstances;
    for (int i = 0; i < numInstances; ++i)
    {
        int blockTypes, desiredLength;
        cin >> blockTypes >> desiredLength;
        vector<int> blocksLengths;
        for (int blockLength_i = 0; blockLength_i < blockTypes; blockLength_i++)
        {
            int blockLength;
            cin >> blockLength;
            blocksLengths.push_back(blockLength);
        }
        IceStatue iceStatue(blockTypes, desiredLength, blocksLengths);
        iceStatues.push_back(iceStatue);
    }
}

void minimizeNumberOfBlocks(vector<IceStatue> &iceStatues)
{
    for (int i = 0; i < iceStatues.size(); ++i)
        iceStatues[i].findMinBlocks();
}

void writeOutput(vector<IceStatue> &iceStatues)
{
    for (int i = 0; i < iceStatues.size(); ++i)
        cout << iceStatues[i].getNumBlocksUsed() << endl;
}

int main()
{
    vector<IceStatue> iceStatues;
    readInput(iceStatues);
    minimizeNumberOfBlocks(iceStatues);
    writeOutput(iceStatues);
    return 0;
}