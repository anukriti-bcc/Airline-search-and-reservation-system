#pragma once
#ifndef MYSQL_UTILS_H
#define MYSQL_UTILS_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <windows.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#undef max
extern std::string loggedInUserEmail;
using namespace std;

const std::string server = "tcp://127.0.0.1:3306";
const std::string username = "root";
const std::string password = "root";
const std::string dbName = "flight_mgmt";

bool loginUser(sql::Connection* con) {
    string email_id, password;
    std::cout << "Enter your email: ";
    cin >> email_id;
    std::cout << "Enter your password: ";
    cin >> password;

    try {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM users WHERE email_id = '" + email_id + "' AND password = '" + password + "'");

        if (res->next()) {
            string name = res->getString("name");
            loggedInUserEmail = email_id;
            std::cout << "Login successful!" << endl;
            std::cout << "\nWelcome, " << name << "!" << endl;

            delete res;
            delete stmt;
            return true;
        }
        else {
            std::cout << "Invalid email or password!" << endl;
            delete res;
            delete stmt;
            return false;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: " << e.what() << endl;
        return false;
    }
}

void registerUser(sql::Connection* con) {
    string name, email_id, phone_no, dob, password;
    std::cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);

    std::cout << "Enter your phone number: ";
    cin >> phone_no;

    std::cout << "Enter your email id: ";
    cin >> email_id;

    std::cout << "Enter your Date of Birth (YYYY-MM-DD): ";
    cin >> dob;

    std::cout << "Enter your password: ";
    cin >> password;

    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "INSERT INTO users (name, email_id, phone_no, dob, password) "
            "VALUES (?, ?, ?, ?, ?)"
        );

        pstmt->setString(1, name);
        pstmt->setString(2, email_id);
        pstmt->setString(3, phone_no);
        pstmt->setString(4, dob);
        pstmt->setString(5, password);

        pstmt->executeUpdate();  // Execute the query

        std::cout << "Registration successful!" << endl;

        delete pstmt;
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: " << e.what() << endl;
    }
}

#include <regex>

bool validateDateTime(const std::string& datetime) {
    // Regex for valid datetime: YYYY-MM-DD HH:MM:SS
    std::regex datetimeRegex(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01]) (0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$)");
    return std::regex_match(datetime, datetimeRegex);
}

void book(sql::Connection* con) {
    std::string source, destination, journey_datetime;
    int ticketno;
    time_t now = time(0);
    int flight_no;

    std::cout << "Enter source: ";
    std::cin >> source;
    std::cout << "Enter destination: ";
    std::cin >> destination;
    std::cout << "Enter flight number: ";
    std::cin >> flight_no;
    std::cin.ignore();
    std::cout << "Enter journey date and time (YYYY-MM-DD HH:MM:SS): ";
    std::getline(std::cin, journey_datetime);

    // Validate the date and time
    if (!validateDateTime(journey_datetime)) {
        std::cout << "Error: Invalid journey date and time format. Please use YYYY-MM-DD HH:MM:SS with valid values." << std::endl;
        return;
    }

    ticketno = static_cast<int>(now);

    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT user_id FROM users WHERE email_id = ?");
        pstmt->setString(1, loggedInUserEmail); // Use the stored email
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            int user_id = res->getInt("user_id");

            pstmt = con->prepareStatement("INSERT INTO bookings (ticketno, user_id, user_email, flight_no, source, destination, booking_datetime, status) VALUES (?,?, ?, ?, ?, ?, ?, ?)");
            pstmt->setInt(1, ticketno);
            pstmt->setInt(2, user_id);
            pstmt->setString(3, loggedInUserEmail); // Use the stored email
            pstmt->setInt(4, flight_no);
            pstmt->setString(5, source);
            pstmt->setString(6, destination);
            pstmt->setString(7, journey_datetime);
            pstmt->setString(8, "Confirmed");
            pstmt->executeUpdate();
            std::cout << "\n   Your seat is booked for flight " << flight_no << " from " << source << " to " << destination
                << " with TicketId " << ticketno << " on " << journey_datetime << "." << std::endl;
            std::cout << "   Thank you !!" << std::endl;
        }
        else {
            std::cout << "User not found!" << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}


void logoutUser() {
    if (!loggedInUserEmail.empty()) {
        loggedInUserEmail = ""; // Reset the global variable
        std::cout << "You have successfully logged out!" << std::endl;
    }
    else {
        std::cout << "No user is currently logged in!" << std::endl;
    }
}

void displayBooking(sql::Connection* con) {
    int ticketno;
    std::cout << "Enter ticket no.: ";
    std::cin >> ticketno;

    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM bookings WHERE ticketno = ?");
        pstmt->setInt(1, ticketno);
        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next()) {
            cout << "\nTicket no.: " << res->getInt("ticketno") << endl;
            cout << "User Email: " << res->getString("user_email") << endl;
            cout << "Flight No: " << res->getInt("flight_no") << endl;
            cout << "Source: " << res->getString("source") << endl;
            cout << "Destination: " << res->getString("destination") << endl;
            cout << "Booking Date and Time: " << res->getString("booking_datetime") << endl;
            cout << "Status: " << res->getString("status") << endl;
        }
        else {
            cout << "Booking not found!" << endl;
        }
        delete res;
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Delete(sql::Connection* con) {
    int ticket_no;
    std::cout << "Enter ticket no. to cancel: ";
    std::cin >> ticket_no;

    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE bookings SET status = 'Cancelled' WHERE ticketno = ?");
        pstmt->setInt(1, ticket_no);
        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected > 0) {
            cout << "The booking has been cancelled!" << endl;
        }
        else {
            cout << "No booking found with the given ticket ID!" << endl;
        }
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void view_profile(sql::Connection* con) {
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM users WHERE email_id = ?");
        pstmt->setString(1, loggedInUserEmail);
        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next()) {
            std::cout << "User Profile:\n";
            std::cout << "Name: " << res->getString("name") << std::endl;
            std::cout << "Email: " << res->getString("email_id") << std::endl;
            std::cout << "Phone No: " << res->getString("phone_no") << std::endl;
            std::cout << "DOB: " << res->getString("dob") << std::endl;
            std::cout << "Created At: " << res->getString("created_at") << std::endl;
        }
        else {
            std::cout << "No profile found!" << std::endl;
        }

        delete res;
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}


void booking_history(sql::Connection* con) {
    try {
        // Assume 'loggedInUserEmail' is set after successful login
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT user_id FROM users WHERE email_id = ?");
        pstmt->setString(1, loggedInUserEmail); // Use the logged-in user's email
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            int user_id = res->getInt("user_id");

            // Query the bookings for the user
            pstmt = con->prepareStatement("SELECT * FROM bookings WHERE user_id = ?");
            pstmt->setInt(1, user_id);
            res.reset(pstmt->executeQuery());

            bool hasHistory = false; // Flag to check if there's booking history
            while (res->next()) {
                hasHistory = true; // Set flag to true if at least one record exists
                std::cout << "\nTicket No.: " << res->getInt("ticketno") << std::endl;
                std::cout << "Flight No.: " << res->getInt("flight_no") << std::endl;
                std::cout << "Source: " << res->getString("source") << std::endl;
                std::cout << "Destination: " << res->getString("destination") << std::endl;
                std::cout << "Booking Date & Time: " << res->getString("booking_datetime") << std::endl;
                std::cout << "Status: " << res->getString("status") << std::endl;
                std::cout << "----------------------------------------------" << std::endl;
            }

            if (!hasHistory) {
                std::cout << "No booking history available!" << std::endl;
            }
        }
        else {
            std::cout << "An unexpected error occurred: User not found!" << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void update_profile(sql::Connection* con) {
    try {
        std::string name, phone_no, dob;

        std::cout << "Enter new name (or press enter to keep current): ";
        std::getline(std::cin, name);  // Read the name input

        std::cout << "Enter new phone number (or press enter to keep current): ";
        std::getline(std::cin, phone_no);  // Read the phone number input

        std::cout << "Enter new date of birth (YYYY-MM-DD) (or press enter to keep current): ";
        std::getline(std::cin, dob);  // Read the date of birth input

        std::string query = "UPDATE users SET ";
        bool hasUpdate = false;

        if (!name.empty()) {
            query += "name = ?";
            hasUpdate = true;
        }
        if (!phone_no.empty()) {
            if (hasUpdate) query += ", ";
            query += "phone_no = ?";
            hasUpdate = true;
        }
        if (!dob.empty()) {
            if (hasUpdate) query += ", ";
            query += "dob = ?";
        }

        query += " WHERE email_id = ?";  // Use parameterized query for email_id

        sql::PreparedStatement* pstmt = con->prepareStatement(query);

        // Bind the input parameters
        int paramIndex = 1;
        if (!name.empty()) {
            pstmt->setString(paramIndex++, name);
        }
        if (!phone_no.empty()) {
            pstmt->setString(paramIndex++, phone_no);
        }
        if (!dob.empty()) {
            pstmt->setString(paramIndex++, dob);
        }

        pstmt->setString(paramIndex, loggedInUserEmail);  // Bind the logged-in user email as the last parameter

        pstmt->executeUpdate();  // Execute the update query

        std::cout << "Profile updated successfully!" << std::endl;

        delete pstmt;  // Clean up the prepared statement
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void delete_profile(sql::Connection* con, bool& loggedIn) {
    try {
        std::string confirm;

        std::cout << "Are you sure you want to delete your profile? (yes/no): ";
        std::cin >> confirm;
        std::cout << std::endl;

        if (confirm == "yes") {
            // Declare the prepared statement as a pointer
            sql::PreparedStatement* pstmt;

            // Step 1: Delete the user's bookings from the bookings table
            pstmt = con->prepareStatement("DELETE FROM bookings WHERE user_email = ?");
            pstmt->setString(1, loggedInUserEmail);
            pstmt->executeUpdate();
            std::cout << "All bookings associated with your profile have been deleted." << std::endl;

            // Ensure to delete the pointer after use
            delete pstmt;

            // Step 2: Delete the user's profile from the users table
            pstmt = con->prepareStatement("DELETE FROM users WHERE email_id = ?");
            pstmt->setString(1, loggedInUserEmail);
            pstmt->executeUpdate();

            std::cout << "Profile deleted successfully!" << std::endl;

            // Ensure to delete the pointer after use
            delete pstmt;

            // Logout the user after deleting the profile
            loggedIn = false; // Set loggedIn to false to log out the user

            std::cout << "You have been logged out." << std::endl;
        }
        else {
            std::cout << "Profile deletion canceled." << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

#endif 
