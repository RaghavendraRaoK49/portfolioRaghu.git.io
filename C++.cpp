#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <cfloat>  // Include for FLT_MAX

using namespace std;

class CongestionGraph 
{
private:
    int V;  // Number of places (nodes)
    vector<vector<pair<int, pair<int, float>>>> adjList;  // {destination, {weight, congestion_factor}}

public:
    CongestionGraph(int V);
    void addRoad(int u, int v, int w, float congestionFactor);
    void removeRoad(int u, int v);
    void displayNetwork();
    float findShortestPath(int src, int dest);  // Changed to return float
    float calculateTrafficCongestion();
    void updateRoadCongestion(int u, int v, float congestionFactor);
    vector<int> findAllPaths(int src, int dest);
    pair<int, float> getMaxCongestedRoad();
    bool isConnected();
    void displayCongestionInformation();
    int findBestPlaceToGo();  // New function to find the best place with minimum congestion
};

// Constructor
CongestionGraph::CongestionGraph(int V) {
    this->V = V;
    adjList.resize(V);
}

// Add road with congestion factor
void CongestionGraph::addRoad(int u, int v, int w, float congestionFactor) {
    adjList[u].push_back({v, {w, congestionFactor}});
    adjList[v].push_back({u, {w, congestionFactor}});
}

// Remove road between u and v
void CongestionGraph::removeRoad(int u, int v) {
    adjList[u].erase(remove_if(adjList[u].begin(), adjList[u].end(),
                                [v](const pair<int, pair<int, float>>& edge) { return edge.first == v; }),
                      adjList[u].end());
    adjList[v].erase(remove_if(adjList[v].begin(), adjList[v].end(),
                                [u](const pair<int, pair<int, float>>& edge) { return edge.first == u; }),
                      adjList[v].end());
}

// Display the entire network
void CongestionGraph::displayNetwork() {
    cout << "\nNetwork Connections:\n";
    for (int u = 0; u < V; u++) {
        cout << "\nPlace " << u << " is connected to:\n";
        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            int weight = edge.second.first;
            float congestionFactor = edge.second.second;
            cout << "  - Place " << v << " (Distance: " << weight << " km, Congestion: " << congestionFactor << ")\n";
        }
    }
}

// Find the shortest path considering congestion
float CongestionGraph::findShortestPath(int src, int dest) {
    vector<float> dist(V, FLT_MAX);  // Changed to float for precise weight calculations
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    pq.push({0.0, src});
    dist[src] = 0.0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (const auto& neighbor : adjList[u]) {
            int v = neighbor.first;
            int weight = neighbor.second.first;
            float congestionFactor = neighbor.second.second;
            float newWeight = weight * (1 + congestionFactor); // Adjusting weight by congestion

            if (dist[u] + newWeight < dist[v]) {
                dist[v] = dist[u] + newWeight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist[dest];
}

// Calculate the total traffic congestion in the network
float CongestionGraph::calculateTrafficCongestion() {
    float totalCongestion = 0;
    for (int u = 0; u < V; u++) {
        for (const auto& edge : adjList[u]) {
            totalCongestion += edge.second.second; // Adding congestion factors
        }
    }
    return totalCongestion;
}

// Update the congestion factor for a road
void CongestionGraph::updateRoadCongestion(int u, int v, float congestionFactor) {
    for (auto& edge : adjList[u]) {
        if (edge.first == v) {
            edge.second.second = congestionFactor;
        }
    }
    for (auto& edge : adjList[v]) {
        if (edge.first == u) {
            edge.second.second = congestionFactor;
        }
    }
}

// Find all paths between two nodes (using DFS or BFS)
vector<int> CongestionGraph::findAllPaths(int src, int dest) {
    vector<int> paths;
    // For simplicity, we are just showing direct connection.
    // You can extend this to find actual paths.
    for (const auto& edge : adjList[src]) {
        if (edge.first == dest) {
            paths.push_back(dest);
        }
    }
    return paths;
}

// Get the most congested road
pair<int, float> CongestionGraph::getMaxCongestedRoad() {
    int maxRoad = -1;
    float maxCongestion = -1;
    for (int u = 0; u < V; u++) {
        for (const auto& edge : adjList[u]) {
            if (edge.second.second > maxCongestion) {
                maxCongestion = edge.second.second;
                maxRoad = u;
            }
        }
    }
    return {maxRoad, maxCongestion};
}

// Check if the network is fully connected
bool CongestionGraph::isConnected() {
    vector<bool> visited(V, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    for (bool v : visited) {
        if (!v) return false;
    }
    return true;
}

// Display congestion information for each road
void CongestionGraph::displayCongestionInformation() {
    cout << "\nCurrent Congestion Information for all roads:\n";
    for (int u = 0; u < V; u++) {
        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            float congestion = edge.second.second;
            cout << "Road from Place " << u << " to Place " << v << " has congestion factor: " << congestion << "\n";
        }
    }
}

// Find the best place to go with the least congestion (considering shortest path)
int CongestionGraph::findBestPlaceToGo() {
    float minCongestion = FLT_MAX;
    int bestPlace = -1;

    for (int u = 0; u < V; u++) {
        float totalCongestionForPlace = 0;

        // Calculate total congestion considering shortest path to all other places
        for (int v = 0; v < V; v++) {
            if (u != v) {
                float shortestPath = findShortestPath(u, v);  // Get the shortest path between u and v
                totalCongestionForPlace += shortestPath;  // Sum up the congestion considering shortest paths
            }
        }

        if (totalCongestionForPlace < minCongestion) {
            minCongestion = totalCongestionForPlace;
            bestPlace = u;
        }
    }
    return bestPlace;
}

int main() {
    int V;
    cout << "Welcome to the Traffic Management System!\n";
    cout << "First, let's set up the number of places (nodes) in the city.\n";
    cout << "Enter the total number of places in your city: ";
    cin >> V;

    CongestionGraph cg(V);

    int roadCount;
    cout << "\nNow, let's add some roads! How many roads do you want to input? ";
    cin >> roadCount;

    cout << "\nPlease enter the details for each road:\n";
    for (int i = 0; i < roadCount; i++) {
        int u, v, w;
        float congestionFactor;
        cout << "\nEnter details for road " << i + 1 << " (Place1 Place2 Distance CongestionFactor): ";
        cin >> u >> v >> w >> congestionFactor;
        cg.addRoad(u, v, w, congestionFactor);
    }

    cg.displayNetwork();  // Display the network with congestion

    int src, dest;
    cout << "\nGreat! Now, let's calculate the shortest path between two places.\n";
    cout << "Enter the starting place and the destination place: ";
    cin >> src >> dest;
    float shortestPath = cg.findShortestPath(src, dest);
    cout << "The shortest path from Place " << src << " to Place " << dest << " is: " << shortestPath << " km.\n";

    cout << "\nThe total congestion in the city is: " << cg.calculateTrafficCongestion() << "\n";

    cout << "\nNow, let's find out the best place to go considering the least congestion.\n";
    int bestPlace = cg.findBestPlaceToGo();
    cout << "The best place to go with the least congestion is: Place " << bestPlace << "\n";

    return 0;
}