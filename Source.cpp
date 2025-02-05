#include "mysql_utils.h"
#include "flight_constants.h"
#include "flight_helper.h"
#include "flights_dijkstra.h"
#include "flights_dfs.h"

using namespace std;
#undef max

void dijkstra(int*, int, int) {};
void searchFlight(sql::Connection* con);
void line();
void line2();
std::string loggedInUserEmail = "";

void Sleep(DWORD dwMilliseconds);

unordered_map<string, int> flightNumbers;
int nextFlightNumber = 37641;

int getValidInput() {
    int choice;
    while (true) {
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) { 
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore any remaining input in the buffer
            return choice; 
        }
    }
}

void switchcase(int i, sql::Connection* con, bool& loggedIn) {
    int j;

    if (i == 1) {
        searchFlight(con);
    }
    else if (i == 2) {
        std::cout << std::endl;
        book(con);
    }
    else if (i == 3) {
        std::cout << std::endl;
        Delete(con);  
    }
    else if (i == 4) {
        std::cout << std::endl;
        displayBooking(con);
    }
    else if (i == 5) {
        bool inProfileMenu = true;
        while (inProfileMenu) {
            std::cout << "\n\t\t   ~ USER PROFILE ~ " << std::endl;
            std::cout << "   _____________________________________________";
            std::cout << "\n\n\t * Press 1. View profile\n\t * Press 2. Update profile\n\t * Press 3. Delete profile\n\t * Press 4. View booking history\n\t * Press 5. Go back" << std::endl;
            std::cout << "   _____________________________________________\n";
            j = getValidInput();

            switch (j) {
            case 1:
                std::cout << std::endl;
                view_profile(con);
                break;
            case 2:
                std::cout << std::endl;
                update_profile(con);
                break;
            case 3:
                std::cout << std::endl;
                delete_profile(con, loggedIn);
                if (!loggedIn) {
                    // If loggedOut, exit the profile menu and return to the login menu
                    inProfileMenu = false;
                }
                break;
            case 4:
                booking_history(con);
                break;
            case 5:
                inProfileMenu = false; // Exit the profile menu loop and go back to the main menu
                break;
            default:
                std::cout << "Invalid choice. Please select again." << std::endl;
                break;
            }
        }
    }
    else if (i == 6) {
        return;
    }
    else {
        std::cout << "Invalid choice. Please enter a valid option." << std::endl;
    }
}

int getFlightNumber(const string& route) {
    if (flightNumbers.find(route) == flightNumbers.end()) {
        flightNumbers[route] = nextFlightNumber++;
    }
    return flightNumbers[route];
}

void searchFlight(sql::Connection* con) {
    int i, j, l = 0;
    int source = 0, destination = 0;

        for (i = 0; i < 14; i++) {
            for (j = i; j < i + 3; j++) {
                cout << "    ";
                if (j <= 13) {
                    cout << "Press " << j + 1 << "." << cities[j] << "\t\t";
                }
                else {
                    break;
                }
            }
            i = j - 1;
            cout << endl;
        }
        do {
            cout << endl << "* Enter source city : ";
        cin >> source;
        source--;
        if (source < 0 || source > 13) {
            cout << "PLEASE ENTER A VALID OPTION" << endl;
        }
    } while (source < 0 || source > 13);

    do {
        cout << endl << "* Enter destination city : ";
        cin >> destination;

        destination--;
        if (destination < 0 || destination > 13) {
            cout << "PLEASE ENTER A VALID OPTION" << endl;
        }
        if (destination == source) {
            cout << "PLEASE ENTER A VALID DESTINATION OTHER THAN THE SOURCE" << endl;
        }
    } while ((destination < 0 || destination >= 13) || destination == source);

    cout <<endl<< "       Searching all flights ";
    for (int wait = 0; wait < 5; wait++) {
        cout << "./";
        Sleep(300);
    }
    cout << "\n\n";

    vector<pair<string, int>> shortestFlights = findFlightsUsingDFS(source, destination, adjMatrixTime, 4);
    pair<string, int> leastExpensiveFlight = findFlightsUsingDijkstra(source, destination, adjMatrixCost);

    cout << setw(55) << "\n\t\t~~ FLIGHT TABLE ~~" << endl;
    line2();
    cout << setw(15) << "FlightNo" << setw(15) << "Airline" << setw(15) << "Time"
        << setw(25) << "Cost of ticket" << setw(30) << "Route" << endl;
    line2();

    cout << "Shortest Paths (Time Efficient): " << endl << endl;
    for (int i = 0; i < shortestFlights.size(); i++) {
        pair<string, int> pathNameAndWeight = findPathAndTimeOrCost(shortestFlights[i].first, adjMatrixCost);
        string route = pathNameAndWeight.first;
        int flightNo = getFlightNumber(route);  
        string airline = flightCompanies[flightNo % flightCompanies.size()];  // Cycle through airlines

        cout << setw(12) << flightNo
            << setw(16) << airline
            << setw(14) << shortestFlights[i].second << " Hrs"
            << setw(17) << "Rs. " << (pathNameAndWeight.second * 1000)
            << setw(50) << route << endl;
    }
    line2();
    cout << endl;

    pair<string, int> pathNameAndWeight = findPathAndTimeOrCost(leastExpensiveFlight.first, adjMatrixTime);
    string route = pathNameAndWeight.first;
    int flightNo = getFlightNumber(route);  
    string airline = flightCompanies[flightNo % flightCompanies.size()];  

    cout << "Most pocket-friendly flight: " << endl << endl;
    cout << setw(12) << flightNo
        << setw(16) << airline
        << setw(14) << pathNameAndWeight.second << " Hrs"
        << setw(17) << "Rs. " << (leastExpensiveFlight.second * 1000)
        << setw(50) << route << endl << endl;
    line2();
    cout << endl;
    
    do {
        std::cout << "\n\t\t   ~~ MENU ~~" << std::endl;
        std::cout << "   _____________________________________________";
        cout << "\n\n\t * Press 1. Search Flight\n\t * Press 2. Book Flight\n\t * Press 3. Exit" << endl;
        std::cout << "   _____________________________________________";
        cout << endl << endl << "Enter your choice : ";
        cin >> i;
        if (i < 1 || i > 3) {
            cout << "PLEASE ENTER A VALID OPTION" << endl;
        }
    } while (i <= 0 || i >= 4);

    if (i == 1) {
        searchFlight(con);  
    }
    if (i == 2) {
        long int fno = 0;
        book(con); 
    }
    if (i == 3) {
    
    }
}

void line()
{
    for (int i = 1; i < 70; i++)
        std::cout << "--";
    std::cout << "+";
    std::cout << "\n";
}
void line2()
{
    std::cout << "+";
    for (int i = 1; i < 60; i++)
        std::cout << "--";
    std::cout << "+";
    std::cout << "\n";
}

int main() {
    sql::Driver* driver;
    sql::Connection* con;

    try {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        con->setSchema(dbName);

        int choice;
        bool loggedIn = false;

        std::cout << "\a";
        Sleep(2000);
        std::cout << "\n\n\tWelcome To Flight Reservation System" << std::endl << std::endl;
        std::cout << "   <><><><><><><><><><><><><><><><><><><><><><><>\n";
        std::cout << "   Book your Flight tickets at affordable prices!" << std::endl;
        std::cout << "   <><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;

        while (true) { 
            if (!loggedIn) {
                std::cout << "\n\t\t   ~~ LOGIN MENU ~~" << std::endl;
                std::cout << "   _____________________________________________";
                std::cout << "\n\n\t * Press 1.Login\n\t * Press 2.Register\n\t * Press 3.Exit" << std::endl;
                std::cout << "   _____________________________________________";
                std::cout << std::endl << std::endl << "Enter your choice: ";
                std::cin >> choice;

                if (choice == 1) {
                    if (loginUser(con)) {
                        loggedIn = true;
                    }
                }
                else if (choice == 2) {
                    registerUser(con);
                }
                else if (choice == 3) {
                    std::cout << "Exiting the program. Goodbye!" << std::endl;
                    break; 
                }
                else {
                    std::cout << "Invalid choice. Please select again." << std::endl;
                }
            }
            else {
                std::cout << "\n\t\t   ~~ MAIN MENU ~~" << std::endl;
                std::cout << "   _____________________________________________";
                std::cout << "\n\n\t * Press 1. Search Flight\n\t * Press 2. Book Flight\n\t * Press 3. Cancel Flight\n\t * Press 4. Display Booking\n\t * Press 5. User Profile\n\t * Press 6. Exit" << std::endl;
                std::cout << "   _____________________________________________";
                std::cout << std::endl << std::endl << "Enter your choice: ";

                std::cin >> choice;

                // Validate input
                if (std::cin.fail() || choice < 1 || choice > 6) {
                    std::cin.clear(); // Clear the error flag
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid choice. Please select a valid option (1-6)." << std::endl;
                }
                else {
                    switchcase(choice, con, loggedIn); 
                }

                if (choice == 6) {
                    std::cout << "Exiting the main menu. Goodbye!" << std::endl;
                    break; 
                }
            }

        }

        std::cout << "\t\t\t\t Exiting ";
        for (int wait = 0; wait < 5; wait++) {
            std::cout << "..";
            Sleep(200);
        }
        std::cout << "\n\t\t\t\t Thank You, Have a Nice day ! :D" << endl;
        delete con;
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: " << e.what() << endl;
    }

    return 0;
}