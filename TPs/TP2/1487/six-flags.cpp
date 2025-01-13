#include <iostream>
#include <vector>

using namespace std;

typedef struct
{
    int duration;
    int score;
} Attraction;

class SixFlagsInstance
{
private:
    int numAttractions;
    int totalAvailableTime;
    vector<Attraction> attractions;
    // Solution
    int optimizedTotalScore = 0;
    vector<vector<int>> memoizedScores;
    int result = 0;

    int getMaxItemRepetition(const int& attraction_i, const int& maxDuration)
    {
        int numRepetitions = 0;
        int maxRepScore = 0;
        int maxRepetitions = maxDuration / attractions[attraction_i - 1].duration;

    
        for (int r = 1; r <= maxRepetitions; r++)
        {
            maxRepScore = max(
                maxRepScore,
                r*attractions[attraction_i - 1].score 
                    + memoizedScores[attraction_i - 1][maxDuration - r*attractions[attraction_i - 1].duration]);
        }
        return maxRepScore;
    }


public:
    SixFlagsInstance(int &numAttractions, int &totalAvailableTime, vector<Attraction> &attractions)
        : numAttractions(numAttractions), totalAvailableTime(totalAvailableTime), attractions(attractions)
    {
        memoizedScores.resize(numAttractions + 1, vector<int>(totalAvailableTime + 1, 0));
        // Fill base cases for memoization table
        for (int i = 0; i <= numAttractions; i++)
            for (int j = 0; j <= totalAvailableTime; j++)
                memoizedScores[i][j] = 0;
    }

    void optimize(void)
    {
        for (int i = 1; i <= numAttractions; i++)
        {
            for (int j = 1; j <= totalAvailableTime; j++)
            {
                if (attractions[i - 1].duration > j)
                    memoizedScores[i][j] = memoizedScores[i - 1][j];
                else
                    memoizedScores[i][j] = max(memoizedScores[i - 1][j], getMaxItemRepetition(i, j));
                this->result = max(this->result, memoizedScores[i][j]);
            }
        }
    }

    int getOptimizedTotalScore(void)
    {
        return this->result;
    }
};

void readInput(vector<SixFlagsInstance> &sixFlagInstances)
{
    int numAttractions, totalAvailableTime;

    while (true)
    {
        // Reads the total number of attractions and available time
        cin >> numAttractions >> totalAvailableTime;

        if (numAttractions == 0 && totalAvailableTime == 0)
            break;

        vector<Attraction> attractions;

        for (int i = 0; i < numAttractions; i++)
        {
            Attraction attraction;
            cin >> attraction.duration >> attraction.score;
            attractions.push_back(attraction);
        }

        SixFlagsInstance sixFlagsInstance(numAttractions, totalAvailableTime, attractions);
        sixFlagInstances.push_back(sixFlagsInstance);
    }
}

void optimizeSixFlagsInstances(vector<SixFlagsInstance> &sixFlagInstances)
{
    for (auto &sixFlagsInstance : sixFlagInstances)
        sixFlagsInstance.optimize();
}

void writeOutput(vector<SixFlagsInstance> &sixFlagInstances)
{
    for (int i = 0; i < sixFlagInstances.size(); i++)
    {
        cout << "Instancia " << i + 1 << endl;
        cout << sixFlagInstances[i].getOptimizedTotalScore() << endl;
        cout << endl;
    }
}

int main()
{
    vector<SixFlagsInstance> sixFlagInstances;

    readInput(sixFlagInstances);
    optimizeSixFlagsInstances(sixFlagInstances);
    writeOutput(sixFlagInstances);

    return 0;
}
