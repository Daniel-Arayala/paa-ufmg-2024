#include <iostream>
#include <limits>
#include <vector>
#include <sstream>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <algorithm>

#define INF numeric_limits<float>::infinity()

#define MAXINT numeric_limits<int>::max()

using namespace std;

struct FlightRoute {
    int originCityID;
    int destCityID;
    float ticketPrice;
    int numberOfSeatsUsed = 0;
    int totalNumberOfSeats = 0;
    int numberOfSeatsAvailable = 0; // totalNumberOfSeats - numberOfSeatsUsed
    int priceSignal = 0; // 1 for forward, 0 for backward, 

    FlightRoute(const int& originCityID, const int& destCityID, const float& ticketPrice) {
        this->originCityID = originCityID;
        this->destCityID = destCityID;
        this->ticketPrice = ticketPrice;
    }

    bool hasSeatsAvailable(void) {
        return numberOfSeatsAvailable > 0;
    }
};

struct City{
    int id;
    float knownTotalPriceFromOrigin = INF;
    int knownTotalFriendsThatTraveled = 0;
    bool visited = false;
    int previousCityID = -1;


    City(const int& id){
        this->id = id;
    }

    bool operator<(const City& other) const {
        return this->knownTotalPriceFromOrigin > other.knownTotalPriceFromOrigin;
    }
};

class TripAdvisor {
    private:
        // Data extracted from input
        vector<City> cities;
        unordered_map<int, vector<FlightRoute>> tripGraph;
        int finalDestinyCityID;
        int numFriends;
        int totalFriendsThatTraveled = 0;
        int numSeatsPerFlight;
        float totalPricePaidByFriends = 0;

        bool checkIfCityIsValid(const City& city) {
            bool cityTotalPriceIsUpToDate = city.knownTotalPriceFromOrigin == cities[city.id].knownTotalPriceFromOrigin;
            return cityTotalPriceIsUpToDate && !city.visited;
        }

        void recoverFlightInfoFromDijkstra(int& currentCityID, vector<int>& path) {
            path.emplace_back(currentCityID);
            if (cities[currentCityID].previousCityID == -1)
                return;
            else
                return recoverFlightInfoFromDijkstra(cities[currentCityID].previousCityID, path);
        }

        void resetCities(void) {
            for (int i = 1; i <= cities.size(); i++) {
                cities[i].knownTotalPriceFromOrigin = INF;
                cities[i].visited = false;
                cities[i].previousCityID = -1;
            }
        }

        void accumTotalPrice(int& additionalPrice) {
            this->totalPricePaidByFriends += additionalPrice;
        }

        FlightRoute getRouteFromTripsGraph(const int& originCityID, const int& destCityID){
            for (FlightRoute& flightRoute : this->tripGraph[originCityID]) {
                if (flightRoute.destCityID == destCityID){
                    return flightRoute;
                }
            }
            return FlightRoute(-1, -1, -1);
        }

        void bookSeatsInRoute(int& originCityID, int& destCityID, int& numSeats) {
            for (int i = 0; i < tripGraph[originCityID].size(); i++) {
                if (tripGraph[originCityID][i].destCityID == destCityID) {
                    tripGraph[originCityID][i].numberOfSeatsUsed = numSeats;
                    tripGraph[originCityID][i].numberOfSeatsAvailable = tripGraph[originCityID][i].totalNumberOfSeats - numSeats;
                    if(tripGraph[originCityID][i].priceSignal == 0)
                        tripGraph[originCityID][i].priceSignal = 1;
                    int flowPrice = (int) tripGraph[originCityID][i].ticketPrice * numSeats *tripGraph[originCityID][i].priceSignal;
                    accumTotalPrice(flowPrice);
                    break;
                }
            }

            for (int j = 0; j < tripGraph[destCityID].size(); j++) {
                if (tripGraph[destCityID][j].destCityID == originCityID) {
                    tripGraph[destCityID][j].numberOfSeatsUsed = tripGraph[destCityID][j].totalNumberOfSeats - numSeats;
                    tripGraph[destCityID][j].numberOfSeatsAvailable = numSeats;
                    if(tripGraph[destCityID][j].priceSignal == 0)
                        tripGraph[destCityID][j].priceSignal = -1;
                    break;
                }
            }
        }

    public:
        TripAdvisor(
            const int& numFriends,
            const int& numSeatsPerFlight,
            const int& numCities,
            vector<FlightRoute>& flightRoutes) {

            this->finalDestinyCityID = numCities;
            this->numFriends = numFriends;
            this->numSeatsPerFlight = numSeatsPerFlight;
            this->cities.reserve(numCities);
            // The index 0 will not be used by the algorithm, but will be kept so the indexing can be used with the id of the city
            for (int cityID = 0; cityID <= numCities; cityID++) {
                City city(cityID);
                cities.emplace_back(city);
            }


            for (FlightRoute flightRoute : flightRoutes) {
                flightRoute.totalNumberOfSeats = numSeatsPerFlight;
                flightRoute.numberOfSeatsAvailable = numSeatsPerFlight;
                tripGraph[flightRoute.originCityID].emplace_back(flightRoute);

                FlightRoute flighRouteInversed(flightRoute);
                flighRouteInversed.originCityID = flightRoute.destCityID;
                flighRouteInversed.destCityID = flightRoute.originCityID;
                flighRouteInversed.totalNumberOfSeats = numSeatsPerFlight;
                flighRouteInversed.numberOfSeatsAvailable = numSeatsPerFlight;
                tripGraph[flighRouteInversed.originCityID].emplace_back(flighRouteInversed);
            }
        }

        void Dijkstra(const int& startCityID) {
            // Priority Queue Initialization
            priority_queue<City> pq;

            cities[startCityID].knownTotalPriceFromOrigin = 0;

            pq.push(cities[startCityID]);

            while (!pq.empty() && (pq.top().id != this->finalDestinyCityID)) {
                City& currentCity = cities[pq.top().id];
                pq.pop();

                if (!checkIfCityIsValid(currentCity)) {
                    continue;
                }
                currentCity.visited = true;
                for (FlightRoute& flightRoute : tripGraph[currentCity.id]) {
                    // Used in case the flight was already used by the friends
                    if (!flightRoute.hasSeatsAvailable() || cities[flightRoute.destCityID].visited) {
                        continue;
                    }
                    float updatedTotalPriceFromCurrentCity = currentCity.knownTotalPriceFromOrigin + flightRoute.ticketPrice;
                    if (updatedTotalPriceFromCurrentCity < cities[flightRoute.destCityID].knownTotalPriceFromOrigin) {
                        cities[flightRoute.destCityID].knownTotalPriceFromOrigin = updatedTotalPriceFromCurrentCity;
                        cities[flightRoute.destCityID].previousCityID = currentCity.id;
                        pq.push(cities[flightRoute.destCityID]);
                    }
                }
            }   
        }

        int augmentFlow(const vector<int>& dijkstraMinPath){
            int bottleneckFlow = MAXINT;
            int friendsTraveling = 0;
            for (int i = dijkstraMinPath.size() - 1;  i > 0; i--) {
                int currentCityID = dijkstraMinPath[i];
                int nextCityID = dijkstraMinPath[i-1];
                FlightRoute currentFlightRoute = getRouteFromTripsGraph(currentCityID, nextCityID);
                bottleneckFlow = min(bottleneckFlow, currentFlightRoute.numberOfSeatsAvailable);
            }

            for (int i = dijkstraMinPath.size() - 1;  i > 0; i--) {
                int currentCityID = dijkstraMinPath[i];
                int nextCityID = dijkstraMinPath[i-1];
                friendsTraveling = min(bottleneckFlow, cities[currentCityID].knownTotalFriendsThatTraveled);
                cities[currentCityID].knownTotalFriendsThatTraveled -= friendsTraveling;
                cities[nextCityID].knownTotalFriendsThatTraveled += friendsTraveling;

                bookSeatsInRoute(currentCityID, nextCityID, friendsTraveling);
            }
            return friendsTraveling;
        }

        bool findCheapestFlightsRouteForAllFriends(void) {
            cities[1].knownTotalFriendsThatTraveled = this->numFriends;

            while(totalFriendsThatTraveled < numFriends) {
                Dijkstra(1);
                vector<int> dijkstraMinPath; 
                recoverFlightInfoFromDijkstra(finalDestinyCityID, dijkstraMinPath);
                // No possible path from final destiny to origin
                if (dijkstraMinPath.empty() || dijkstraMinPath.back() != 1) {
                    return false;
                }
                else {
                    // Iterating in reverse order due to recovery path starting from sink node
                    int numFriendsTraveling = augmentFlow(dijkstraMinPath);
                    this->totalFriendsThatTraveled += numFriendsTraveling;
                }
                if (totalFriendsThatTraveled < numFriends)
                    resetCities();
            }
            
            return true;
        }

        float getTotalPricePaidByFriends(void) const{
            return this->totalPricePaidByFriends;
        }

};

void readInput(vector<TripAdvisor>& trips) {
    int numCities, numFlightRoutes, numFriends, numSeats;

    while (cin >> numCities >> numFlightRoutes) {
        vector<FlightRoute> flightRoutes;

        for (int i = 0; i < numFlightRoutes; i++) {
            int originCityID, destCityID;
            float ticketPrice;
            if (!(cin >> originCityID >> destCityID >> ticketPrice)) {
                cerr << "Error reading flight route data." << endl;
                return;
            }
            flightRoutes.emplace_back(originCityID, destCityID, ticketPrice);
        }

        if (!(cin >> numFriends >> numSeats)) {
            cerr << "Error reading friends and seats data." << endl;
            return;
        }

        TripAdvisor trip(numFriends, numSeats, numCities, flightRoutes);
        trips.emplace_back(std::move(trip));
    }
}

void readInputFile(vector<TripAdvisor>& trips) {
    ifstream tripsFile("inputs/input1.txt");
    if (tripsFile.is_open()) {
        while (true) {
            int numCities, numFlightRoutes;
            if (!(tripsFile >> numCities >> numFlightRoutes)) {
                break;
            }

            vector<FlightRoute> flightRoutes;
            for (int i = 0; i < numFlightRoutes; i++) {
                int originCityID, destCityID;
                float ticketPrice;

                if (!(tripsFile >> originCityID >> destCityID >> ticketPrice)) {
                    cerr << "Error reading flight route data." << endl;
                    return;
                }
                
                flightRoutes.emplace_back(originCityID, destCityID, ticketPrice);
            }

            int numFriends, numSeats;
            if (!(tripsFile >> numFriends >> numSeats)) {
                cerr << "Error reading friends and seats data." << endl;
                return;
            }

            TripAdvisor trip(numFriends, numSeats, numCities, flightRoutes);
            trips.emplace_back(trip);
        }
    } else {
        cerr << "Failed to open the file." << endl;
    }
    tripsFile.close();
}


int main() {
    // Variables extracted from input data
    vector<TripAdvisor> trips;

    readInput(trips);

    for (int i = 0; i < trips.size(); i++) {
        bool validTrip = trips[i].findCheapestFlightsRouteForAllFriends();
        cout << "Instancia " << i + 1 << endl;
        if (validTrip) {
            cout << fixed << setprecision(0) << trips[i].getTotalPricePaidByFriends() << endl;
        }
        else {
            cout << "impossivel" << endl;
        }
        cout << endl;
    }

    return 0;
}

