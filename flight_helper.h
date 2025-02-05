#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <windows.h>
#include <conio.h>
#include <unordered_map>
#include <sstream>
using namespace std;

pair<string, int> findPathAndTimeOrCost(const string& pathString, vector<vector<int>>& adjMatrix) {

    stringstream ss(pathString); 
    string city;
    vector<int> cityIndex;

    while (getline(ss, city, '-')) {  
        cityIndex.push_back(stoi(city)); 
    }

    int totalCost = 0;
    string cityPathName = "";

    for (int i = 0; i < cityIndex.size() - 1; ++i) {
        cityPathName += cities[cityIndex[i]] + " -> ";
        totalCost += adjMatrix[cityIndex[i]][cityIndex[i + 1]];
    }
    cityPathName += cities[cityIndex.back()];  

    return { cityPathName, totalCost };
}