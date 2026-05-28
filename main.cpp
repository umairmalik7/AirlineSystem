#include<iostream>
using namespace std;  

void menu(){
    cout << "1. Add a City" << endl;
    cout << "2. Add a flight route" << endl;
    cout << "3. Remove a route" << endl;
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
    while (running) {
        menu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
         
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