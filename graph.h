#ifndef GRAPH_H
#define GRAPH_H


#include<string>
#include<vector>
#include<map>
#include<climits>
#include<fstream>
#include<sstream>
#include<iostream>
#include<climits>
#include<queue>   
#include<algorithm>   
#include<iomanip>
#include<ctime>

using namespace std;

struct  Edge
{   
    string destination;
    int cost;
    int duration;
    string airline;
};

struct RouteResult {
    int totalCost;
    int totalDuration;
    vector<string> path; // list of cities in order
    bool found;          // was a path found at all?
};

class Graph {
private:
    map<string, vector<Edge>> adjList; // Adjacency list to store the graph'

     // private helper for DFS — called recursively
    // private because outside code should use findAllPaths() instead
    void dfsAllPaths(
        string current,
        string destination,
        map<string, bool>& visited,
        vector<string>& path,
        int currentCost,
        int currentDuration,
        int& pathCount
    );
public:
    string addCity(string city);
    string addRoute(string from , string to , int cost, int duration, string airline );
    string displayAllCities();
    string displayAllRoutes();
    void saveToFile(string filename);
    void loadFromFile(string filename);
    RouteResult dijkstraCheapest(string source, string destination);
    RouteResult dijkstraFastest(string source, string destination);
    // Module 3 — BFS
    RouteResult bfsFewestStops(string source, string destination);
 
    // Module 4 — DFS
    void findAllPaths(string source, string destination);
    void performanceComparison(string source, string destination);
};

#endif
    