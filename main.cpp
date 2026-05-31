#include <iostream>
#include <string>
#include "graph.h"

using namespace std;

void menu() {
    cout << "\n=========================================" << endl;
    cout << "   AIRLINE ROUTE OPTIMIZATION SYSTEM    " << endl;
    cout << "=========================================" << endl;
    cout << " 1.  Add a City"                          << endl;
    cout << " 2.  Add a Flight Route"                  << endl;
    cout << " 3.  Display All Cities"                  << endl;
    cout << " 4.  Display All Routes"                  << endl;
    cout << " 5.  Find CHEAPEST Route"                 << endl;
    cout << " 6.  Find FASTEST Route"                  << endl;
    cout << " 7.  Find FEWEST STOPS Route"             << endl;
    cout << " 8.  Show ALL Possible Paths"             << endl;
    cout << " 9.  Save Routes to File"                 << endl;
    cout << " 10. Load Routes from File"               << endl;
    cout << " 11. Algorithm Performance Comparison"    << endl;
    cout << " 0.  Exit"                                << endl;
    cout << "=========================================" << endl;
    cout << "Enter your choice: ";
}

void printRoute(RouteResult& r, string label) {
    if (!r.found) {
        cout << "No path found." << endl;
        return;
    }
    cout << "\n===== " << label << " =====" << endl;
    cout << "Path          : ";
    for (int i = 0; i < r.path.size(); i++) {
        cout << r.path[i];
        if (i < r.path.size() - 1) cout << " -> ";
    }
    cout << endl;
    cout << "Total Cost    : $" << r.totalCost << endl;
    int hours = r.totalDuration / 60;
    int mins  = r.totalDuration % 60;
    cout << "Total Duration: " << hours << "h " << mins << "m" << endl;
    cout << "Stops         : " << (int)r.path.size() - 2 << " stop(s)" << endl;
}

int main() {
    Graph myGraph;
    bool running = true;

    cout << "=========================================" << endl;
    cout << "  Welcome to Airline Route Optimizer     " << endl;
    cout << "=========================================" << endl;

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

            case 5: {
                string from, to;
                cout << "Enter source city: ";
                getline(cin, from);
                cout << "Enter destination city: ";
                getline(cin, to);
                RouteResult r = myGraph.dijkstraCheapest(from, to);
                printRoute(r, "Cheapest Route");
                break;
            }

            case 6: {
                string from, to;
                cout << "Enter source city: ";
                getline(cin, from);
                cout << "Enter destination city: ";
                getline(cin, to);
                RouteResult r = myGraph.dijkstraFastest(from, to);
                printRoute(r, "Fastest Route");
                break;
            }

            case 7: {
                string from, to;
                cout << "Enter source city: ";
                getline(cin, from);
                cout << "Enter destination city: ";
                getline(cin, to);
                RouteResult r = myGraph.bfsFewestStops(from, to);
                printRoute(r, "Fewest Stops Route");
                break;
            }

            case 8: {
                string from, to;
                cout << "Enter source city: ";
                getline(cin, from);
                cout << "Enter destination city: ";
                getline(cin, to);
                myGraph.findAllPaths(from, to);
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

            case 11: {
                string from, to;
                cout << "Enter source city: ";
                getline(cin, from);
                cout << "Enter destination city: ";
                getline(cin, to);
                myGraph.performanceComparison(from, to);
                break;
            }

            case 0: {
                cout << "\nGoodbye! Safe travels!" << endl;
                running = false;
                break;
            }

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}