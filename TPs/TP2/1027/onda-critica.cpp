#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <cstdlib>

using namespace std;

struct Point
{
    int x, y;
    Point() {}
    Point(int x, int y) : x(x), y(y) {}

    bool operator<(const Point &other) const
    {
        return x < other.x || (x == other.x && y < other.y);
    }
};

class CriticalWave
{
private:
    vector<Point> points;
    vector<vector<int>> memoizedPoints;
    int maxCriticalPoints = 0;

    void sortPoints(void)
    {
        sort(points.begin(), points.end());
    }

public:
    CriticalWave(const int &numPoints)
    {
        this->points.reserve(numPoints);
        this->memoizedPoints.resize(numPoints, vector<int>(2, 1));
    }

    void addPoint(const int &x, const int &y)
    {
        points.push_back(Point(x, y));
    }

    void countCriticalPointsInWave(void)
    {
        // Sort points by x-coordinate
        this->sortPoints();
        // cp --> current point
        // pp --> previous point
        for (int cp = 1; cp < points.size(); cp++)
        {
            for (int pp = 0; pp < cp; pp++)
            {
                // Checks if the x coordinate os the previous point is smaller then the current one
                if (points[pp].x < points[cp].x)
                {
                    // Current point --> a + 1 | Previous point --> a - 1
                    if (points[pp].y == points[cp].y - 2)
                    {
                        memoizedPoints[cp][1] = max(memoizedPoints[cp][1], memoizedPoints[pp][0] + 1);
                    }
                    // Current point --> a - 1 | Previous point --> a + 1
                    else if (points[pp].y == points[cp].y + 2)
                    {
                        memoizedPoints[cp][0] = max(memoizedPoints[cp][0], memoizedPoints[pp][1] + 1);
                    }
                }
            }

            maxCriticalPoints = max(maxCriticalPoints, max(memoizedPoints[cp][0], memoizedPoints[cp][1]));
        }
    }

    int getMaxCriticalPoints(void)
    {
        return this->maxCriticalPoints;
    }
};

void readInput(vector<CriticalWave> &criticalWaves)
{
    int numPoints;
    while (cin >> numPoints)
    {
        CriticalWave criticalWave(numPoints);

        for (int i = 0; i < numPoints; ++i)
        {
            int x, y;
            cin >> x >> y;
            criticalWave.addPoint(x, y);
        }

        criticalWaves.push_back(criticalWave);
    }
}

void findOptimalCriticalWaves(vector<CriticalWave> &criticalWaves)
{
    for (auto &criticalWave : criticalWaves)
        criticalWave.countCriticalPointsInWave();
}

void writeOutput(const vector<CriticalWave> &criticalWaves)
{
    for (auto criticalWave : criticalWaves)
        cout << criticalWave.getMaxCriticalPoints() << endl;
}

int main()
{
    vector<CriticalWave> criticalWaves;
    readInput(criticalWaves);
    findOptimalCriticalWaves(criticalWaves);
    writeOutput(criticalWaves);
    return 0;
}