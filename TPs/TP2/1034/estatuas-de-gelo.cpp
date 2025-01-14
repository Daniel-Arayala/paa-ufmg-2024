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
    IceStatue(const int &blockTypes, const int &desiredLength, const vector<int> &blocksLengths)
        : blockTypes(blockTypes), desiredLength(desiredLength), blocksLengths(blocksLengths){}

    void findMinBlocks()
    {   
        vector<int> memoizedNumBlocks;
        memoizedNumBlocks = vector<int>(desiredLength + 1, INF);
        memoizedNumBlocks[0] = 0;

        for (int maxLength_i = 1; maxLength_i <= desiredLength; maxLength_i++)
        {
            for (int blockType_i = 0; blockType_i < blockTypes; blockType_i++)
            {
                if (maxLength_i >= this->blocksLengths[blockType_i])
                {
                    memoizedNumBlocks[maxLength_i] = min(
                        memoizedNumBlocks[maxLength_i], 
                        memoizedNumBlocks[maxLength_i - blocksLengths[blockType_i]] + 1
                    );
                }
            }
        }

        numBlocksUsed = memoizedNumBlocks[desiredLength];
    }

    int getNumBlocksUsed() const { return numBlocksUsed; }
};

void readInput(vector<IceStatue> &iceStatues)
{
    int numInstances;
    cin >> numInstances;
    for (int i = 0; i < numInstances; ++i)
    {
        int blockTypes, desiredLength;
        cin >> blockTypes >> desiredLength;
        vector<int> blocksLengths(blockTypes);
        for (int &blockLength : blocksLengths)
        {
            cin >> blockLength;
        }
        IceStatue iceStatue(blockTypes, desiredLength, blocksLengths);
        iceStatues.push_back(iceStatue);
    }
}

void minimizeNumberOfBlocks(vector<IceStatue> &iceStatues)
{
    for (auto &iceStatue : iceStatues)
        iceStatue.findMinBlocks();
}

void writeOutput(const vector<IceStatue> &iceStatues)
{
    for (const auto &iceStatue : iceStatues)
        cout << iceStatue.getNumBlocksUsed() << endl;
}

int main()
{
    vector<IceStatue> iceStatues;
    readInput(iceStatues);
    minimizeNumberOfBlocks(iceStatues);
    writeOutput(iceStatues);
    return 0;
}