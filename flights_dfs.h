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

int noOfCities = cities.size();

void findAllPaths(int u, int d, vector<vector<int>>& adjMatrix, vector<int>& visited, vector<int>& path, vector<pair<string, int>>& finalPaths) {
    visited[u] = 1;       
    path.push_back(u);    

    if (u == d) {
        int finalCost = 0;
        string pathString = "";

        for (int i = 0; i < path.size() - 1; i++) {
            pathString += to_string(path[i]) + "-";
            finalCost += adjMatrix[path[i]][path[i + 1]];
        }

        pathString += to_string(path.back());

        finalPaths.push_back({ pathString, finalCost });
    }
    else {
        for (int i = 0; i < noOfCities; ++i) {
            if (!visited[i] && (adjMatrix[u][i] < INT_MAX)) {
                findAllPaths(i, d, adjMatrix, visited, path, finalPaths);
            }
        }
    }

    // Backtrack
    path.pop_back();      
    visited[u] = 0;        
}

// Merge Sort Functions to sort paths by cost
void merge(vector<pair<string, int>>& paths, int left, int mid, int right) {
    int n1 = mid - left + 1; 
    int n2 = right - mid;    

    vector<pair<string, int>> leftPaths(n1);  
    vector<pair<string, int>> rightPaths(n2);

    for (int i = 0; i < n1; i++) {
        leftPaths[i] = paths[left + i];
    }
    for (int i = 0; i < n2; i++) {
        rightPaths[i] = paths[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftPaths[i].second <= rightPaths[j].second) {
            paths[k] = leftPaths[i];
            i++;
        }
        else {
            paths[k] = rightPaths[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        paths[k] = leftPaths[i];
        i++;
        k++;
    }

    while (j < n2) {
        paths[k] = rightPaths[j];
        j++;
        k++;
    }
}

void mergeSort(vector<pair<string, int>>& paths, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(paths, left, mid);
        mergeSort(paths, mid + 1, right);

        merge(paths, left, mid, right);
    }
}

// Wrapper function
vector<pair<string, int>> findFlightsUsingDFS(int source, int destination, vector<vector<int>>& adjMatrix, int options = 4) {
    vector<int> visited(noOfCities, 0);      
    vector<int> path;                        
    vector<pair<string, int>> finalPaths;   

    findAllPaths(source, destination, adjMatrix, visited, path, finalPaths);

    mergeSort(finalPaths, 0, finalPaths.size() - 1);
    finalPaths.resize(4);

    return finalPaths;
}