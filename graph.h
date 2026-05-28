#include<string>
#include<vector>
#include<map>

using namespace std;

struct  Edge
{   
    string source;
    string destination;
    int cost;
    int duration;
    string airline;
};

class Graph {
private:
    map<string, vector<Edge>> adjList; // Adjacency list to store the graph
public:
    string addCity(string city);
    string addRoute(string from , string to , int cost, int duration, string airline );
    string displayAllCities();
    string displayAllRoutes();
};
    