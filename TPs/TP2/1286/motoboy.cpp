#include <iostream>
#include <vector>

using namespace std;

struct Order
{
    int totalDeliveryTime = 0;
    int numPizzas = 0;

    Order() {};

    Order(const int &totalDeliveryTime, const int &numPizzas)
    {
        this->totalDeliveryTime = totalDeliveryTime;
        this->numPizzas = numPizzas;
    }
};

class PizzaDelivery
{
private:
    int numOrders = 0;
    int maxPizzas = 0;
    int orderIndex = 0;
    vector<Order> orders;
    vector<vector<int>> memoizedDeliveryTimes;

public:
    PizzaDelivery(const int &numOrders, const int &maxPizzas)
        : numOrders(numOrders), maxPizzas(maxPizzas)
    {
        this->orders.resize(numOrders);
        this->memoizedDeliveryTimes.resize(numOrders + 1, vector<int>(maxPizzas + 1, 0));
    }

    void addOrder(const Order &order)
    {
        this->orders[orderIndex++] = order;
    }

    void calculateMaxDelegatedDeliveryTime(void)
    {
        for (int order_i = 1; order_i <= numOrders; order_i++)
        {
            for (int maxPizzas_j = 1; maxPizzas_j <= maxPizzas; maxPizzas_j++)
            {
                if (orders[order_i - 1].numPizzas > maxPizzas_j)
                    this->memoizedDeliveryTimes[order_i][maxPizzas_j] = memoizedDeliveryTimes[order_i - 1][maxPizzas_j];
                else
                    this->memoizedDeliveryTimes[order_i][maxPizzas_j] =
                        max(
                            memoizedDeliveryTimes[order_i - 1][maxPizzas_j],
                            memoizedDeliveryTimes[order_i - 1][maxPizzas_j - orders[order_i - 1].numPizzas] + orders[order_i - 1].totalDeliveryTime);
            }
        }
    }

    int getMaxDelegatedDeliveryTime(void) const
    {
        return this->memoizedDeliveryTimes[numOrders][maxPizzas];
    }

};

void readInput(vector<PizzaDelivery> &pizzaDeliveries)
{
    while (true)
    {
        int numOrders, maxPizzas;

        cin >> numOrders;
        if (numOrders == 0)
            break;

        cin >> maxPizzas;
        PizzaDelivery pizzaDelivery(numOrders, maxPizzas);
        for (int i = 0; i < numOrders; i++)
        {
            int totalDeliveryTime, numPizzas;
            cin >> totalDeliveryTime >> numPizzas;
            Order order(totalDeliveryTime, numPizzas);
            pizzaDelivery.addOrder(order);
        }
        pizzaDeliveries.push_back(pizzaDelivery);
    }
}

void optimizePizzaDeliveries(vector<PizzaDelivery> &pizzaDeliveries)
{
    for (auto &pizzaDelivery : pizzaDeliveries)
        pizzaDelivery.calculateMaxDelegatedDeliveryTime();
}

void writeOutput(const vector<PizzaDelivery> &pizzaDeliveries)
{
    for (const auto &pizzaDelivery : pizzaDeliveries)
        cout << pizzaDelivery.getMaxDelegatedDeliveryTime() << " min." << endl;
}


int main()
{
    vector<PizzaDelivery> pizzaDeliveries;

    readInput(pizzaDeliveries);
    optimizePizzaDeliveries(pizzaDeliveries);
    writeOutput(pizzaDeliveries);

    return 0;
}