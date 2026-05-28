#include<iostream>
#include "graph.h"


using namespace std;  

void menu(){
    cout << "1. Add a City" << endl;
    cout << "2. Add a flight route" << endl;
    cout << "3. Display all cities" << endl;
    cout << "4. Display all routes" << endl;
    cout << "5. Fint CHEAPEST route" << endl;
    cout << "6. Find FASTEST route" << endl;
    cout << "7. Find FEWEST STOPS route" << endl;
    cout << "8. Show All possible paths" << endl;
    cout << "9. Save routes to file " << endl;
    cout << "10. Load routes to file " << endl;
    cout << "11. Exit " << endl;

}

int main(){
    bool running = true;
    cout << " ===== Airline Route System =====" << endl;
    Graph myGraph; // Create an instance of the Graph class
    while (running) {
        menu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Code to add a city
                cout << "Enter city name: ";
                string cityName;
                cin >> cityName;
                myGraph.addCity(cityName);  //Example of adding a city
                break;
            case 2:
                // Code to add a flight route
                string from, to, airline;
                int cost, duration;
                cout << "Enter source city: ";
                cin >> from;
                cout << "Enter destination city: ";
                cin >> to;
                cout << "Enter cost: ";
                cin >> cost;
                cout << "Enter duration: ";
                cin >> duration;
                cout << "Enter airline: ";
                cin >> airline;
                myGraph.addRoute(from, to, cost, duration, airline); //Example of adding a route
                break;
            case 3:
                // Code to display all cities
                myGraph.displayAllCities();
                break;
            case 4:
                // Code to display all routes
                myGraph.displayAllRoutes();
                break;
            case 11:
                running = false; // Exit the program
                cout << "Exiting the program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;

}