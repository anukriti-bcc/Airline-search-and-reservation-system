#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <sstream>
using namespace std;

pair<string, int> findFlightsUsingDijkstra(int source, int destination, vector<vector<int>>& adjMatrix) {
    int V = adjMatrix.size(); 

    vector<int> prev(V, -1);        
    vector<int> distance(V, INT_MAX);
    vector<bool> visited(V, false);  

    distance[source] = 0;  

    for (int i = 0; i < V; i++) {
        int minDist = INT_MAX;
        int minVertex = -1;

        for (int j = 0; j < V; j++) {
            if (!visited[j] && distance[j] < minDist) {
                minDist = distance[j];
                minVertex = j;
            }
        }

        if (minVertex == -1) break; 

        visited[minVertex] = true; 

        for (int j = 0; j < V; j++) {
            if (adjMatrix[minVertex][j] != INT_MAX && !visited[j]) {
                int newDist = distance[minVertex] + adjMatrix[minVertex][j];
                if (newDist < distance[j]) {
                    distance[j] = newDist;
                    prev[j] = minVertex;
                }
            }
        }
    }

    vector<int> path;
    for (int v = destination; v != -1; v = prev[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    if (distance[destination] == INT_MAX) {
        return { "No flights available.", INT_MAX };
    }
    else {
        string bestPath = "";
        for (int i = 0; i < path.size(); i++) {
            bestPath += to_string(path[i]);
            if (i != path.size() - 1) bestPath += "-";
        }

        return { bestPath, distance[destination] };
    }
}