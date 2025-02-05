#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <sstream>
using namespace std;

vector<string> cities = {
    "Mumbai", "Delhi", "Bengaluru", "Kolkata", "Chennai", "Hyderabad", "Pune", "Ahmedabad",
    "Jaipur", "Lucknow", "Chandigarh", "Bhopal", "Patna", "Thiruvananthapuram"
};

vector<string> flightCompanies = {
    "Air India", "Indigo", "SpiceJet", "GoAir", "Vistara",
    "AirAsia", "Jet Airways", "Zoom Air", "Alliance Air", "Trujet"
};

//(in hours)
vector<vector<int>> adjMatrixTime = {
    {0, 2, INT_MAX, INT_MAX, 2, 1, 1, 1, 2, 3, INT_MAX, INT_MAX, 4, INT_MAX},  // Mumbai
    {3, 0, 3, INT_MAX, INT_MAX, 2, 4, INT_MAX, 3, 1, 2, 3, 3, INT_MAX},  // Delhi
    {INT_MAX, 4, 0, 5, 1, 2, 1, INT_MAX, INT_MAX, 4, INT_MAX, INT_MAX, 5, 1},  // Bengaluru
    {INT_MAX, INT_MAX, 5, 0, 4, 4, INT_MAX, INT_MAX, INT_MAX, 3, 4, INT_MAX, 2, INT_MAX},  // Kolkata
    {3, INT_MAX, 2, 4, 0, 1, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 3, INT_MAX, 3},  // Chennai
    {2, 3, 2, 4, 1, 0, 3, 3, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 2},  // Hyderabad
    {2, 4, 2, INT_MAX, INT_MAX, 3, 0, 2, 3, 2, INT_MAX, 1, 4, 2},  // Pune
    {2, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 3, 2, 0, 3, INT_MAX, INT_MAX, 2, INT_MAX, 3},  // Ahmedabad
    {3, 3, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 3, 3, 0, 3, 2, INT_MAX, 4, INT_MAX},  // Jaipur
    {4, 2, 4, 3, INT_MAX, INT_MAX, 3, INT_MAX, 3, 0, INT_MAX, 2, 3, INT_MAX},  // Lucknow
    {INT_MAX, 2, INT_MAX, 4, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 2, INT_MAX, INT_MAX, 3, INT_MAX, INT_MAX},  // Chandigarh
    {INT_MAX, 3, INT_MAX, INT_MAX, 3, INT_MAX, 2, 2, INT_MAX, 2, 3, 0, 5, INT_MAX},  // Bhopal
    {5, 3, 6, 2, INT_MAX, INT_MAX, 4, INT_MAX, 4, 3, INT_MAX, INT_MAX, 0, 5},  // Patna
    {INT_MAX, INT_MAX, 2, INT_MAX, 3, 3, 3, 3, INT_MAX, INT_MAX, INT_MAX, 4, 5, 0}   // Thiruvananthapuram
};

//(in thousands of INR)
vector<vector<int>> adjMatrixCost = {
    {0, 4, INT_MAX, INT_MAX, 5, 3, 2, 3, 4, 5, INT_MAX, INT_MAX, 7, INT_MAX},  // Mumbai
    {5, 0, 6, INT_MAX, INT_MAX, 6, 7, INT_MAX, 6, 3, 4, 5, 6, INT_MAX},  // Delhi
    {INT_MAX, 6, 0, 8, 5, 4, 3, INT_MAX, INT_MAX, 6, INT_MAX, INT_MAX, 8, 3},  // Bengaluru
    {INT_MAX, INT_MAX, 8, 0, 7, 6, INT_MAX, INT_MAX, INT_MAX, 5, 6, INT_MAX, 4, INT_MAX},  // Kolkata
    {6, INT_MAX, 5, 7, 0, 5, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 5, INT_MAX, 6},  // Chennai
    {4, 6, 4, 6, 5, 0, 4, 6, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 6},  // Hyderabad
    {3, 7, 3, INT_MAX, INT_MAX, 4, 0, 5, 8, 5, INT_MAX, 4, 7, 5},  // Pune
    {4, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 6, 5, 0, 6, INT_MAX, INT_MAX, 5, INT_MAX, 6},  // Ahmedabad
    {5, 6, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 6, 6, 0, 6, 5, INT_MAX, 7, INT_MAX},  // Jaipur
    {6, 4, 6, 5, INT_MAX, INT_MAX, 5, INT_MAX, 6, 0, INT_MAX, 5, 5, INT_MAX},  // Lucknow
    {INT_MAX, 4, INT_MAX, 6, INT_MAX, INT_MAX, INT_MAX, INT_MAX, 5, INT_MAX, INT_MAX, 6, INT_MAX, INT_MAX},  // Chandigarh
    {INT_MAX, 5, INT_MAX, INT_MAX, 5, INT_MAX, 4, 5, INT_MAX, 5, 6, 0, INT_MAX, 8},  // Bhopal
    {8, 6, 9, 4, INT_MAX, INT_MAX, 7, INT_MAX, 7, 5, INT_MAX, INT_MAX, 0, 9},  // Patna
    {INT_MAX, INT_MAX, 4, INT_MAX, 6, 6, 5, 6, INT_MAX, INT_MAX, INT_MAX, 8, 9, 0}   // Thiruvananthapuram
};
