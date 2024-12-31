#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

using namespace std;

// Dijkstra's Algorithm implementation
struct Edge {
    string to;
    int weight;
};

void dijkstra(const vector<string>& nodes, const unordered_map<string, vector<Edge>>& graph, const string& start_node) {
    // Initialize distances with infinity
    unordered_map<string, int> distances;
    unordered_map<string, string> predecessors;
    for (const auto& node : nodes) {
        distances[node] = numeric_limits<int>::max();
    }
    distances[start_node] = 0;

    // Priority queue for processing nodes
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({0, start_node});

    while (!pq.empty()) {
        int current_distance = pq.top().first;
        string current_node = pq.top().second;
        pq.pop();

        // Skip processing if the distance is not optimal
        if (current_distance > distances[current_node]) {
            continue;
        }

        // Update distances for neighbors
        for (const auto& edge : graph.at(current_node)) {
            int new_distance = current_distance + edge.weight;
            if (new_distance < distances[edge.to]) {
                distances[edge.to] = new_distance;
                predecessors[edge.to] = current_node;
                pq.push({new_distance, edge.to});
            }
        }
    }

    // Output distances
    cout << "Distances from " << start_node << ":\n";
    for (const auto& node : nodes) {
        cout << node << ": " << distances[node] << endl;
    }

    // Output shortest paths
    cout << "\nShortest Paths:\n";
    for (const auto& node : nodes) {
        if (node == start_node) continue;
        vector<string> path;
        for (string at = node; !at.empty(); at = predecessors[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        if (path.front() == start_node) {
            cout << start_node << " to " << node << ": ";
            for (const auto& step : path) {
                cout << step << (step == node ? "\n" : " -> ");
            }
        } else {
            cout << start_node << " to " << node << ": No path\n";
        }
    }
}

int main() {
    // Nodes
    vector<string> nodes = {
        "IT Industry 1", "IT Industry 2", "Park 1", "Park 2", 
        "University", "Hospital", "Bank 1", "Bank 2", "Residential Area 1", 
        "Residential Area 2", "Transport Hub", "Shopping Mall"
    };

    // Graph edges
    unordered_map<string, vector<Edge>> graph = {
        {"IT Industry 1", {{"Park 1", 6}, {"Park 2", 3}, {"University", 19}, {"Bank 1", 6}, {"Transport Hub", 10}}},
        {"IT Industry 2", {{"Park 1", 15}, {"Residential Area 2", 10}, {"Shopping Mall", 18}}},
        {"Park 1", {{"Bank 2", 19}, {"Residential Area 1", 19}, {"Transport Hub", 16}}},
        {"Park 2", {{"University", 11}, {"Residential Area 1", 19}, {"Shopping Mall", 12}}},
        {"University", {{"Bank 2", 8}, {"Residential Area 1", 14}, {"Residential Area 2", 1}}},
        {"Hospital", {{"Bank 1", 18}, {"Bank 2", 9}, {"Residential Area 2", 1}}},
        {"Bank 1", {{"Residential Area 2", 20}, {"Transport Hub", 8}}},
        {"Bank 2", {{"Transport Hub", 10}}},
        {"Residential Area 1", {{"Residential Area 2", 19}, {"Shopping Mall", 14}}},
        {"Residential Area 2", {{"Transport Hub", 17}}},
        {"Transport Hub", {{"Shopping Mall", 11}}}
    };

    // Run Dijkstra's algorithm from a chosen start node
    string start_node = "IT Industry 1";
    dijkstra(nodes, graph, start_node);

    return 0;
}
