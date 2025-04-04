#include <iostream>
#include "../include/database.h"
#include "../include/guest.h"
#include "../include/booking.h"
#include "../include/billing.h"
#include "../include/reports.h"
#include "../include/admin.h"
#include "../include/housekeeping.h"
#include "../include/validator.h"
#include "../include/user.h"gi



int main() {
    // Create a Database object
    Database db("guest_house.db");

    // Open the database
    if (!db.open()) {
        return 1;
    }

    // Create the Guests table
    if (!db.createTable()) {
        db.close();
        return 1;
    }


    //_______________TESTING INSERTING GUEST INFO_______________
//    string fname, lname, contact_info, email, id_proof, address;

//     // Taking input from the user
//     cout << "Enter First Name: ";
//     cin >> fname;
//     cout << "Enter Last Name: ";
//     cin >> lname;
//     cout << "Enter Contact Info: ";
//     cin >> contact_info;
//     cout << "Enter Email: ";
//     cin >> email;
//     cin.ignore(); // To consume the leftover newline character
//     cout << "Enter ID Proof Type (e.g., Passport, License): ";
//     getline(cin, id_proof);
//     cout << "Enter Address: ";
//     getline(cin, address);

//     // Call the function
//     db.insertGuest(fname, lname, contact_info, email, id_proof, address);


    //______________TESTING INSERTING ROOM INFO_________________________
    // int room_no;
    // string room_type, price_per_night, status;

    // // Taking input from the user
    // cout << "Enter Room Number: ";
    // cin >> room_no;
    // cin.ignore(); // To consume the leftover newline character

    // cout << "Enter Room Type (e.g., Single, Double, Suite): ";
    // getline(cin, room_type);

    // cout << "Enter Price per Night (e.g., 100, 150.50): ";
    // getline(cin, price_per_night);

    // cout << "Enter Room Status (Available/Booked): ";
    // getline(cin, status);

    // // Call the function
    // db.insertRoomDetails(room_no, room_type, price_per_night, status);

    
//__________________________TESTING INSERTING ADMIN DETAILS___________________
// string username, password, email, role;

// // Taking input from the user
// cout << "Enter Username: ";
// cin >> username;

// cout << "Enter Password: ";
// cin >> password;

// cout << "Enter Email: ";
// cin >> email;
// cin.ignore(); // To consume the leftover newline character

// cout << "Enter Role (e.g., Manager, Staff): ";
// getline(cin, role);

// // Call the function
// db.insertAdmin(username, password, email, role);



// _____________________login with email VALIDATOR __________________________

User u;
u.loginpage();

// db.printGuests();

    db.close();

    return 0;
}