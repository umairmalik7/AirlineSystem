#include "graph.h"
#include <iostream>
#include<fstream>
#include <sstream>
#include <queue>
#include <algorithm>

 
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
            result += city + " -> " + edge.destination +
                      " | Cost: $" + to_string(edge.cost) +
                      " | Duration: "  + to_string(edge.duration) + " mins" +
                      " | Airline: "   + edge.airline + "\n";
        }
    }
    return result;  
}

// saveToFile

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





RouteResult Graph::dijkstraCheapest(string source, string destination) {
    RouteResult result;
    result.found = false;
    result.totalCost = 0;
    result.totalDuration = 0;
 
    // check both cities exist first
    if (adjList.find(source) == adjList.end()) {
        cout << "Source city does not exist." << endl;
        return result;
    }
    if (adjList.find(destination) == adjList.end()) {
        cout << "Destination city does not exist." << endl;
        return result;
    }


    map<string, int> dist;
    for (const auto& pair : adjList) {
        dist[pair.first] = INT_MAX;
    }
    dist[source] = 0;

     // duration 
    map<string, int> duration;
    for (const auto& pair : adjList) {
        duration[pair.first] = 0;
    }

    map<string, string> previous;

    priority_queue<
        pair<int,string>,
        vector<pair<int,string>>,
        greater<pair<int,string>>
    > pq;
 
    pq.push({0, source});  

    while (!pq.empty()) {
 
        pair<int,string> top = pq.top();
        pq.pop();
        int currentCost = top.first;
        string currentCity = top.second;
 
        if (currentCost > dist[currentCity]) continue;
 
        if (currentCity == destination) break;

        for (const auto& edge : adjList[currentCity]) {
            string neighbour = edge.destination;
 
            if (dist[currentCity] == INT_MAX) continue;  
            if (adjList.find(neighbour) == adjList.end()) continue;

            int newCost = dist[currentCity] + edge.cost;
 

            if (newCost < dist[neighbour]) {
                dist[neighbour] = newCost;  
                duration[neighbour] = duration[currentCity] + edge.duration;
                previous[neighbour] = currentCity;  
                pq.push({newCost, neighbour});  
            }
        }
    }
      
    if (dist[destination] == INT_MAX) {
    
        result.found = false;
        return result;
    }
 
 
string current = destination;
while (current != source) {
    result.path.push_back(current);
    if (previous.find(current) == previous.end()) {
        result.found = false;
        return result;
    }
    current = previous[current];
}
    result.path.push_back(source);
 
    reverse(result.path.begin(), result.path.end());
 
    result.totalCost = dist[destination];
    result.totalDuration = duration[destination];
    result.found = true;
    return result;
}




RouteResult Graph::dijkstraFastest(string source, string destination) {
    RouteResult result;
    result.found = false;
    result.totalCost = 0;
    result.totalDuration = 0;
 
    if (adjList.find(source) == adjList.end()) {
        cout << "Source city does not exist." << endl;
        return result;
    }
    if (adjList.find(destination) == adjList.end()) {
        cout << "Destination city does not exist." << endl;
        return result;
    }
 

    map<string, int> dist;
    for (const auto& pair : adjList) {
        dist[pair.first] = INT_MAX;
    }
    dist[source] = 0;
 

    map<string, int> cost;
    for (const auto& pair : adjList) {
        cost[pair.first] = 0;
    }
 
    map<string, string> previous;
 
    priority_queue<
        pair<int,string>,
        vector<pair<int,string>>,
        greater<pair<int,string>>
    > pq;
 
    pq.push({0, source});
 
    while (!pq.empty()) {
        pair<int,string> top = pq.top();
        pq.pop();
        int currentDuration = top.first;
        string currentCity = top.second;
 
        if (currentDuration > dist[currentCity]) continue;
        if (currentCity == destination) break;
 
        for (const auto& edge : adjList[currentCity]) {
            string neighbour = edge.destination;
            if (adjList.find(neighbour) == adjList.end()) continue;
 
            int newDuration = dist[currentCity] + edge.duration;
            if (newDuration < dist[neighbour]) {
                dist[neighbour] = newDuration;
                cost[neighbour] = cost[currentCity] + edge.cost;
                previous[neighbour] = currentCity;
                pq.push({newDuration, neighbour});
            }
        }
    }
 
    if (dist[destination] == INT_MAX) {
        result.found = false;
        return result;
    }
 
    string current = destination;
    while (current != source) {
    result.path.push_back(current);
    if (previous.find(current) == previous.end()) {
        result.found = false;
        return result;
    }
    current = previous[current];
}
    result.path.push_back(source);
    reverse(result.path.begin(), result.path.end());
 
    result.totalCost = cost[destination];
    result.totalDuration = dist[destination];
    result.found = true;
    return result;
}

RouteResult Graph::bfsFewestStops(string source, string destination) {
    RouteResult result;
    result.found = false;
    result.totalCost = 0;
    result.totalDuration = 0;
 
    if (adjList.find(source) == adjList.end()) {
        cout << "Source city does not exist." << endl;
        return result;
    }
    if (adjList.find(destination) == adjList.end()) {
        cout << "Destination city does not exist." << endl;
        return result;
    }
 
    if (source == destination) {
        result.path.push_back(source);
        result.found = true;
        return result;
    }
 
   
    map<string, bool> visited;

    map<string, string> previous;
 
    map<string, int> totalCost;
    map<string, int> totalDuration;
 
    for (const auto& pair : adjList) {
        visited[pair.first]      = false;
        totalCost[pair.first]    = 0;
        totalDuration[pair.first]= 0;
    }
 
    queue<string> q;
 
    q.push(source);
    
    while (!q.empty()) {

        string currentCity = q.front();
        q.pop();
        for (const auto& edge : adjList[currentCity]) {
            string neighbour = edge.destination;
 
            if (!visited[neighbour]) {
                visited[neighbour]       = true;
                previous[neighbour]      = currentCity;
                totalCost[neighbour]     = totalCost[currentCity] + edge.cost;
                totalDuration[neighbour] = totalDuration[currentCity] + edge.duration;
 
                if (neighbour == destination) {
                    // reconstruct path
                    string current = destination;
                    while (current != source) {
                        result.path.push_back(current);
                        current = previous[current];
                    }
                    result.path.push_back(source);
                    reverse(result.path.begin(), result.path.end());
                    result.totalCost     = totalCost[destination];
                    result.totalDuration = totalDuration[destination];
                    result.found         = true;
                    return result;
                }
 
                q.push(neighbour); 
            }
        }
    }
 
  
    result.found = false;
    return result;
}


void Graph::dfsAllPaths(
    string current,
    string destination,
    map<string, bool>& visited,
    vector<string>& path,
    int currentCost,
    int currentDuration,
    int& pathCount
) {
     if (path.size() > 3) return;
    // mark current city as visited 
    visited[current] = true;
    path.push_back(current);
 
    // BASE CASE: if we reached destination, print this path
    if (current == destination) {
        pathCount++;
        cout << "\nPath " << pathCount << ": ";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        int hours = currentDuration / 60;
        int mins  = currentDuration % 60;
        cout << "\n         Cost: $" << currentCost
             << " | Duration: " << hours << "h " << mins << "m"
             << " | Stops: " << path.size() - 2 << endl;
    }
    else {
        // RECURSIVE CASE: explore all unvisited neighbours
        for (const auto& edge : adjList[current]) {
            if (!visited[edge.destination]) {
                // go deeper into this path
                dfsAllPaths(
                    edge.destination,
                    destination,
                    visited,
                    path,
                    currentCost + edge.cost,
                    currentDuration + edge.duration,
                    pathCount
                );
                // when it returns we BACKTRACK automatically
                // because the next iteration tries a different neighbour
            }
        }
    }
 
    
    visited[current] = false;
    path.pop_back();
}
 

void Graph::findAllPaths(string source, string destination) {
    if (adjList.find(source) == adjList.end()) {
        cout << "Source city does not exist." << endl;
        return;
    }
    if (adjList.find(destination) == adjList.end()) {
        cout << "Destination city does not exist." << endl;
        return;
    }
 
    map<string, bool> visited;
    for (const auto& pair : adjList) {
        visited[pair.first] = false;
    }
 
    vector<string> path;
    int pathCount = 0;
 
    cout << "\n===== All Paths from " << source << " to " << destination << " =====" << endl;
 
    dfsAllPaths(source, destination, visited, path, 0, 0, pathCount);
 
    if (pathCount == 0) {
        cout << "No paths found." << endl;
    } else {
        cout << "\nTotal paths found: " << pathCount << endl;
    }
}