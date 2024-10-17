#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip> // for setting width
#include <windows.h>

using namespace std;

void SetColor(int ForgC) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, ForgC);
}

class User {
private:
    string username;
    string password;

public:
    void registerUser() {
        SetColor(11); // Light Cyan for input
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ofstream file("users.txt", ios::app);
        file << username << " " << password << "\n";
        file.close();

        SetColor(10); // Green for success message
        cout << "User registered successfully\n" << endl;
        SetColor(7);  // Reset to white
    }

    bool loginUser() {
        string inputUsername, inputPassword;
        cout << "Enter username: ";
        cin >> inputUsername;
        cout << "Enter password: ";
        cin >> inputPassword;
        SetColor(10); // Green for success message
        cout << "Logged in successfully ...\n";
        SetColor(7);  // Reset to white

        ifstream file("users.txt");
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string username, password;
            iss >> username >> password;
            if (username == inputUsername && password == inputPassword) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }
};

class Bus {
private:
    vector<pair<string, string>> routes; // Pair of source and destination
    vector<int> timings;
    vector<int> availability;

public:
    void addBus(const string& source, const string& destination, int timing, int availability) {
        routes.emplace_back(source, destination);
        timings.push_back(timing);
        this->availability.push_back(availability);
    }

    void displayBuses(const string& source, const string& destination) {
        bool found = false;
        for (size_t i = 0; i < routes.size(); i++) {
            if (routes[i].first == source && routes[i].second == destination) {
                SetColor(14); // Yellow for display
                cout << "Route: " << routes[i].first << " to " << routes[i].second 
                     << ", Timing: " << timings[i] 
                     << ", Availability: " << availability[i] << endl;
                found = true;
            }
        }
        if (!found) {
            SetColor(12); // Red for error message
            cout << "No buses available for this route." << endl;
        }
        SetColor(7); // Reset to white
    }

    int getAvailability(const string& source, const string& destination) {
        for (size_t i = 0; i < routes.size(); i++) {
            if (routes[i].first == source && routes[i].second == destination) {
                return availability[i];
            }
        }
        return -1; // No route found
    }

    void updateAvailability(const string& source, const string& destination, int availability) {
        for (size_t i = 0; i < routes.size(); i++) {
            if (routes[i].first == source && routes[i].second == destination) {
                this->availability[i] = availability;
                return;
            }
        }
    }
};

class Reservation {
private:
    vector<string> reservations;

public:
    void bookSeat(Bus& bus, const string& source, const string& destination) {
        int availability = bus.getAvailability(source, destination);
        if (availability > 0) {
            bus.updateAvailability(source, destination, availability - 1);
            reservations.push_back(source + " to " + destination);

            // Get customer information
            string name;
            int age;
            string contact;
            cout << "Enter your name: ";
            cin >> name;
            cout << "Enter your age: ";
            cin >> age;
            cout << "Enter your contact number: ";
            cin >> contact;

            // Save customer information to file
            ofstream passengerFile("passengers_data.txt", ios::app);
            passengerFile << "Name: " << name << ", Age: " << age 
                          << ", Contact: " << contact 
                          << ", Route: " << source << " to " << destination << endl;
            passengerFile.close();

            SetColor(10); // Green for success message
            cout << "Seat booked successfully and customer information saved." << endl;
        } else {
            SetColor(12); // Red for error message
            cout << "No seats available on this route." << endl;
        }
        SetColor(7); // Reset to white
    }

    void viewReservations() {
        if (reservations.empty()) {
            SetColor(12); // Red for error message
            cout << "No reservations found." << endl;
            SetColor(7);  // Reset to white
            return;
        }
        for (const auto& route : reservations) {
            SetColor(14); // Yellow for display
            cout << "Route: " << route << endl;
        }
        SetColor(7);  // Reset to white
    }

    // void cancelReservation(Bus& bus) {
    //     string route;
    //     cout << "Enter route to cancel (format: cityA to cityB): ";
    //     cin >> route;
    //     for (size_t i = 0; i < reservations.size(); i++) {
    //         if (reservations[i] == route) {
    //             reservations.erase(reservations.begin() + i);
    //             bus.updateAvailability(route.substr(0, route.find(" to ")),
    //                                    route.substr(route.find(" to ") + 4), // Extract destination
    //                                    bus.getAvailability(route.substr(0, route.find(" to ")),
    //                                    route.substr(route.find(" to ") + 4)) + 1); // Increase availability
    //             SetColor(10); // Green for success message
    //             cout << "Reservation cancelled successfully." << endl;
    //             SetColor(7);  // Reset to white
    //             return;
    //         }
    //     }
    //     SetColor(12); // Red for error message
    //     cout << "Reservation not found." << endl;
    //     SetColor(7);  // Reset to white
    // }
};

class Payment {
public:
    bool makePayment() {
        // Simulate payment gateway
        return (rand() % 2 == 0);
    }
};

class Admin {
private:
    Bus& bus; // Reference to Bus object

public:
    Admin(Bus& b) : bus(b) {} // Constructor to initialize reference

    void addBus() {
        string source, destination;
        int timing, availability;
        cout << "Enter source: ";
        cin >> source;
        cout << "Enter destination: ";
        cin >> destination;
        cout << "Enter timing: ";
        cin >> timing;
        cout << "Enter availability: ";
        cin >> availability;
        bus.addBus(source, destination, timing, availability);
        SetColor(10); // Green for success message
        cout << "Bus added successfully." << endl;
        SetColor(7);  // Reset to white
    }

    void displayBuses(const string& source, const string& destination) {
        bus.displayBuses(source, destination);
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    User user;
    Bus bus;
    Reservation reservation;
    Payment payment;

    // Admin object initialized with bus reference
    Admin admin(bus);

    // Pre-add some buses
    bus.addBus("cityA", "cityB", 10, 20);
    bus.addBus("cityA", "cityC", 11, 30);
    bus.addBus("cityB", "cityD", 12, 13);
    bus.addBus("narhe", "katraj", 13, 15);
    bus.addBus("swargate", "katraj", 14, 35);
    bus.addBus("shivaji nagar", "katraj", 15, 2);
    bus.addBus("manapa", "katraj", 16, 12);

    while (true) {
        SetColor(14); // Yellow for menu display
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                user.registerUser();
                break;
            case 2:
                if (user.loginUser()) {
                    while (true) {
                        SetColor(14); // Yellow for menu display
                        cout << "\n1. View Buses" << endl;
                        cout << "2. Book Seat" << endl;
                        cout << "3. View Reservations" << endl;
                        // cout << "4. Cancel Reservation" << endl;
                        cout << "4. Make Payment" << endl;
                        cout << "5. Admin Mode" << endl;
                        cout << "6. Logout\n" << endl;
                        cout << "Enter your choice: ";
                        int subChoice;
                        cin >> subChoice;
                        switch (subChoice) {
                            case 1: {
                                string source, destination;
                                cout << "Enter source: ";
                                cin >> source;
                                cout << "Enter destination: ";
                                cin >> destination;
                                admin.displayBuses(source, destination);
                                break;
                            }
                            case 2: {
                                string source, destination;
                                cout << "Enter source: ";
                                cin >> source;
                                cout << "Enter destination: ";
                                cin >> destination;
                                reservation.bookSeat(bus, source, destination);
                                break;
                            }
                            case 3:
                                reservation.viewReservations();
                                break;
                            // case 4:
                            //     // reservation.cancelReservation(bus);
                            //     break;
                            case 4:
                                if (payment.makePayment()) {
                                    SetColor(10); // Green for success message
                                    cout << "Payment successful." << endl;
                                } else {
                                    SetColor(12); // Red for error message
                                    cout << "Payment failed. Try again." << endl;
                                }
                                SetColor(7); // Reset to white
                                break;
                            case 5:
                                admin.addBus();
                                break;
                            case 6:
                                SetColor(10); // Green for success message
                                cout << "Logged out successfully." << endl;
                                SetColor(7); // Reset to white
                                return 0; // Exit loop
                            default:
                                SetColor(12); // Red for error message
                                cout << "Invalid choice." << endl;
                                SetColor(7); // Reset to white
                                break;
                        }
                    }
                } else {
                    SetColor(12); // Red for error message
                    cout << "Invalid username or password." << endl;
                    SetColor(7); // Reset to white
                }
                break;
            case 3:
                SetColor(10); // Green for success message
                cout << "Exiting the program." << endl;
                SetColor(7); // Reset to white
                return 0;
            default:
                SetColor(12); // Red for error message
                cout << "Invalid choice." << endl;
                SetColor(7); // Reset to white
                break;
        }
    }
    return 0;
}
