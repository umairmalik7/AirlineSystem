#include "graph.h"

string Graph::addCity(string city){
    if(adjList.find(city) != adjList.end()){
        return "City already exists.";
    }
    adjList[city] = vector<Edge>(); // Initialize an empty list of edges for the new city
    return "City added successfully.";

}

string Graph::addRoute(string from , string to , int cost , int duration, string ariline){
    if(adjList.find(from) == adjList.end() || adjList.find(to) == adjList.end()){
        return "One or both cities do not exist.";
    } 
    Edge newFlight;
    newFlight.destination = to;
    newFlight.cost = cost;
    newFlight.duration = duration;
    newFlight.airline = ariline;
    adjList[from].push_back(newFlight); // Add the new flight to the adjacency list
    
}

string Graph::displayAllCities(){
    string result = "Cities:\n";

    for (const auto& pair: adjList){
        result == pair.first + "\n";
        return result;
    }
}

string Graph::displayAllRoutes(){
    string result = "Routes:\n";
    for (const auto& pair: adjList){
        string city = pair.first;
        for (const auto& edge: pair.second){
            result += city + " -> " + edge.destination + " | Cost: " + to_string(edge.cost) + " | Duration: " + to_string(edge.duration) + " | Airline: " + edge.airline + "\n";
        }
    }
}