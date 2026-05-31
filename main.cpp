
#include <iostream>
#include <string>
#include "graph.h"
 
using namespace std;
 
void menu() {
    cout << "\n===== Airline Route System =====" << endl;
    cout << " 1.  Add a City"                    << endl;
    cout << " 2.  Add a Flight Route"             << endl;
    cout << " 3.  Display All Cities"             << endl;
    cout << " 4.  Display All Routes"             << endl;
    cout << " 5.  Find CHEAPEST Route"            << endl;
    cout << " 6.  Find FASTEST Route"             << endl;
    cout << " 7.  Find FEWEST STOPS Route"        << endl;
    cout << " 8.  Show ALL Possible Paths"        << endl;
    cout << " 9.  Save Routes to File"            << endl;
    cout << " 10. Load Routes from File"          << endl;
    cout << " 0.  Exit"                           << endl;
    cout << "================================="   << endl;
    cout << "Enter your choice: ";
}
 
int main() {
    Graph myGraph;
    bool running = true;
 
    cout << "===== Welcome to Airline Route Optimization System =====" << endl;
 
    while (running) {
        menu();
        int choice;
        cin >> choice;
        cin.ignore(); 
 
        switch (choice) {
 
            case 1: {
                string cityName;
                cout << "Enter city name: ";
                getline(cin, cityName);
                cout << myGraph.addCity(cityName) << endl;
                break;
            }
 
            case 2: {
                string from, to, airline;
                int cost, duration;
                cout << "Enter source city: ";
                getline(cin, from);
                cout << "Enter destination city: ";
                getline(cin, to);
                cout << "Enter cost ($): ";
                cin >> cost;
                cout << "Enter duration (mins): ";
                cin >> duration;
                cin.ignore();
                cout << "Enter airline name: ";
                getline(cin, airline);
                cout << myGraph.addRoute(from, to, cost, duration, airline) << endl;
                break;
            }
 
            case 3: {
                cout << myGraph.displayAllCities() << endl;
                break;
            }
 
            case 4: {
                cout << myGraph.displayAllRoutes() << endl;
                break;
            }
            case 9: {
                string filename;
                cout << "Enter filename to save (e.g. routes.txt): ";
                getline(cin, filename);
                myGraph.saveToFile(filename);
                break;
            }
            case 10: {
                string filename;
                cout << "Enter filename to load (e.g. routes.txt): ";
                getline(cin, filename);
                myGraph.loadFromFile(filename);
                break;
            }
 
            case 0: {
                cout << "Goodbye!" << endl;
                running = false;
                break;
            }
 
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
 
    return 0;
}