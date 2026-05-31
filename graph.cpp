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



// ================================================================
// DIJKSTRA'S ALGORITHM — Find Cheapest Route

// HOW IT WORKS STEP BY STEP:
//
// Imagine you are standing in Karachi with a notebook.
// In the notebook you write the cheapest known cost to reach
// every city. At the start everything is INFINITY except
// Karachi itself which is 0.
//
// You then use a Priority Queue (Min Heap) — think of it as
// a queue that always gives you the CHEAPEST city first.
//
// Step 1: Put source city in the queue with cost 0
// Step 2: Take out the cheapest city from the queue
// Step 3: Look at all its neighbours (direct flights)
// Step 4: If going THROUGH this city is cheaper than what
//         we knew before → UPDATE the cost (this is called RELAXATION)
// Step 5: Repeat until destination is reached
//
// TIME COMPLEXITY:  O((V + E) log V)
//   V = number of cities, E = number of routes
//   log V comes from the priority queue operations
//
// SPACE COMPLEXITY: O(V)
//   We store cost and previous city for each vertex
// ================================================================

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

     // --- STEP 1: Setup ---
 
    // dist stores the cheapest known cost to reach each city
    // at the start everything is INT_MAX (infinity)
    map<string, int> dist;
    for (const auto& pair : adjList) {
        dist[pair.first] = INT_MAX;
    }
    dist[source] = 0;

     // duration stores total flight time along the cheapest path
    map<string, int> duration;
    for (const auto& pair : adjList) {
        duration[pair.first] = 0;
    }

     // previous stores WHICH city we came from to reach this city
    // this is how we reconstruct the path at the end
    // e.g. previous["London"] = "Dubai" means we came from Dubai
    map<string, string> previous;

     // Priority Queue (Min Heap)
    // stores pairs of (cost, cityName)
    // priority_queue by default is MAX heap so we use greater<>
    // to make it MIN heap — always gives smallest cost first
    priority_queue<
        pair<int,string>,
        vector<pair<int,string>>,
        greater<pair<int,string>>
    > pq;
 
    pq.push({0, source});  // start from source with cost 0

     // --- STEP 2: Main Loop ---
    while (!pq.empty()) {
 
        // take out the city with SMALLEST cost
        pair<int,string> top = pq.top();
        pq.pop();
        int currentCost = top.first;
        string currentCity = top.second;
 
        // if we already found a cheaper way to this city, skip it
        if (currentCost > dist[currentCity]) continue;
 
        // if we reached destination, we are done
        if (currentCity == destination) break;
 
        // --- STEP 3: Look at all neighbours ---
        for (const auto& edge : adjList[currentCity]) {
            string neighbour = edge.destination;
 
            if (adjList.find(neighbour) == adjList.end()) continue;

                        // --- STEP 4: RELAXATION ---
            // new cost = cost to reach currentCity + cost of this flight
            if (dist[currentCity] == INT_MAX) continue;  // ← add this chec
            int newCost = dist[currentCity] + edge.cost;
 
            // if this new cost is CHEAPER than what we knew before
            if (newCost < dist[neighbour]) {
                dist[neighbour] = newCost;  // update the cost
                duration[neighbour] = duration[currentCity] + edge.duration;
                previous[neighbour] = currentCity;  // remember we came from here
                pq.push({newCost, neighbour});  // add to queue for processing
            }
        }
    }
        // --- STEP 5: Reconstruct the path ---
    // we follow the 'previous' map backwards from destination to source
    if (dist[destination] == INT_MAX) {
        // never reached destination
        result.found = false;
        return result;
    }
 
    // trace back from destination to source
    string current = destination;
    while (current != source) {
        result.path.push_back(current);
        current = previous[current];
    }
    result.path.push_back(source);
 
    // path was built backwards so reverse it
    reverse(result.path.begin(), result.path.end());
 
    result.totalCost = dist[destination];
    result.totalDuration = duration[destination];
    result.found = true;
    return result;
}


// ================================================================
// DIJKSTRA'S ALGORITHM — Find Fastest Route (by duration)
//
// Exact same algorithm as dijkstraCheapest
// The ONLY difference is we compare duration instead of cost
// This shows how flexible Dijkstra is — just change the weight!
// ================================================================

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
 
    // this time dist stores DURATION (time) not cost
    map<string, int> dist;
    for (const auto& pair : adjList) {
        dist[pair.first] = INT_MAX;
    }
    dist[source] = 0;
 
    // cost stores total price along the fastest path
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
 
            // relaxation by DURATION this time
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
        current = previous[current];
    }
    result.path.push_back(source);
    reverse(result.path.begin(), result.path.end());
 
    result.totalCost = cost[destination];
    result.totalDuration = dist[destination];
    result.found = true;
    return result;
}
