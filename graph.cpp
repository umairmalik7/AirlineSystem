#include "graph.h"
#include <iostream>
#include<fstream>
#include <sstream>
 
// addCity

string Graph::addCity(string city) {
    if (adjList.find(city) != adjList.end()) {
        return "City already exists.";
    }
    adjList[city] = vector<Edge>();
    return "City '" + city + "' added successfully.";
}
 
// addRoute

string Graph::addRoute(string from, string to, int cost, int duration, string airline) {
    if (adjList.find(from) == adjList.end()) {
        return "Source city '" + from + "' does not exist.";
    }
    if (adjList.find(to) == adjList.end()) {
        return "Destination city '" + to + "' does not exist.";
    }
 
    Edge newFlight;
    newFlight.destination = to;
    newFlight.cost        = cost;
    newFlight.duration    = duration;
    newFlight.airline     = airline;
 
    adjList[from].push_back(newFlight);
    return "Route from '" + from + "' to '" + to + "' added successfully.";
}
 

// displayAllCities

string Graph::displayAllCities() {
    if (adjList.empty()) {
        return "No cities added yet.";
    }
 
    string result = "\n===== Cities =====\n";
    int count = 1;
    for (const auto& pair : adjList) {
        result += to_string(count) + ". " + pair.first + "\n";
        count++;
    }
    return result;  
}
 

// displayAllRoutes


string Graph::displayAllRoutes() {
    if (adjList.empty()) {
        return "No routes added yet.";
    }
 
    string result = "\n===== All Routes =====\n";
    for (const auto& pair : adjList) {
        string city = pair.first;
 
        if (pair.second.empty()) {
            result += city + " to (no outgoing flights)\n";
            continue;
        }
 
        for (const auto& edge : pair.second) {
            result += city + " to " + edge.destination +
                      " | Cost: $" + to_string(edge.cost) +
                      " | Duration: "  + to_string(edge.duration) + " mins" +
                      " | Airline: "   + edge.airline + "\n";
        }
    }
    return result;  
}

// saveToFile
// IMPORTANT: saves ALL cities first, then ALL routes
// so when loading, all destination cities exist already
// -----------------------------------------------
void Graph::saveToFile(string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file for saving." << endl;
        return;
    }
 
    // first pass — write ALL cities
    for (const auto& pair : adjList) {
        file << "CITY " << pair.first << "\n";
    }
 
    // second pass — write ALL routes
    for (const auto& pair : adjList) {
        for (const auto& edge : pair.second) {
            file << "ROUTE "
                 << pair.first       << " "
                 << edge.destination << " "
                 << edge.cost        << " "
                 << edge.duration    << " "
                 << edge.airline     << "\n";
        }
    }
 
    file.close();
    cout << "Routes saved to '" << filename << "' successfully." << endl;
}


// loadFromFile
// Reads line by line, checks CITY or ROUTE keyword
// -----------------------------------------------
void Graph::loadFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'." << endl;
        return;
    }
 
    string line;
    while (getline(file, line)) {
        if (line.substr(0, 4) == "CITY") {
            string city = line.substr(5);
            addCity(city);
        }
        else if (line.substr(0, 5) == "ROUTE") {
            istringstream ss(line.substr(6));
            string from, to, airline;
            int cost, duration;
            ss >> from >> to >> cost >> duration;
            getline(ss, airline);
            if (!airline.empty() && airline[0] == ' ')
                airline = airline.substr(1);
            addRoute(from, to, cost, duration, airline);
        }
    }
 
    file.close();
    cout << "Routes loaded from '" << filename << "' successfully." << endl;
}