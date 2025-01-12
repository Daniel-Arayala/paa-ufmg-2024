#include <iostream>
#include <vector>

using namespace std;

typedef struct
{
    int inputNum = 0;
    int fiboValue = 0;
    int numCalls = 0;
} FibonacciInfo;

int fibonacci(int n, int &numCalls)
{
    if (n <= 1)
        return n;
    else
        return fibonacci(n - 1, ++numCalls) + fibonacci(n - 2, ++numCalls);
}

void calculateFibonaccis(vector<FibonacciInfo> &fiboInfos)
{

    for (int i = 0; i < fiboInfos.size(); i++)
        fiboInfos[i].fiboValue = fibonacci(fiboInfos[i].inputNum, fiboInfos[i].numCalls);
}

void readInput(vector<FibonacciInfo> &fiboInfos)
{
    int numTestCases;

    cin >> numTestCases;

    for (int i = 0; i < numTestCases; i++)
    {
        // Reads the input number
        FibonacciInfo fiboInfo;
        cin >> fiboInfo.inputNum;
        fiboInfos.push_back(fiboInfo);
    }
}

void writeOutput(vector<FibonacciInfo> &fiboInfos)
{
    for (int i = 0; i < fiboInfos.size(); i++)
        cout << "fib(" << fiboInfos[i].inputNum << ") = " << fiboInfos[i].numCalls << " calls = " <<  fiboInfos[i].fiboValue<< endl;
}

int main()
{
    vector<FibonacciInfo> fiboInfos;

    readInput(fiboInfos);
    calculateFibonaccis(fiboInfos);
    writeOutput(fiboInfos);

    return 0;
}