// #include <iostream>
// #include <string.h>
// #include <graphics.h>
// #include <winbgim.h>
// #include<string.h>
// #include "../include/database.h"
// #include "../include/receptionist.h"


// #include "../include/admin.h"

// using namespace std;

// // ## ___________TTESTING GRAPHICS_______________

// // No Graphics will be added for this project hahahahahahaha!!(Evil laugh)

// using namespace std;

// //__________________________________________________
// #include "../include/receptionist.h"

// #include "../include/validator.h"
// #include "../include/user.h"

// int main()
// {
//     // // Create a Database object
//     Database db("guest_house.db");

//     // Open the database
//     if (!db.open()) {
//         return 1;
//     }

//     // Create the Guests table
//     if (!db.createTable()) {
//         db.close();
//         return 1;
//     }

//     if (db.createTable()) {
//     db.createTriggrs(); 
//     cout << "Database and triggers created successfully!" << endl;

//     }

//     sqlite3_exec(db.getDb(), "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

//     // // Insert a guest
//     // db.insertGuest("John Doe", "1234567890", "john.doe@example.com");
//     // db.insertGuest("Jane Smith", "9876543210", "jane.smith@example.com");

//     // // Print all guests
//     // db.printGuests();

//     // // Close the database
//     // db.close();

//     // Guest g;
//     // g.add_customer();
//     // g.check_availability();
//     // g.update_reservation();
//     // g.cancel_reservation();
//     // g.view_reservations();
//     // int choice;
//     // do
//     // {
//     //     cout << "Welcome to the Guest House Reservation Management System!" << endl;
//     //     cout << "Please choose an option from the menu below: " << endl;
//     //     cout << "1. Check availability" << endl;
//     //     cout << "2. View Reservations" << endl;
//     //     cout << "3. Cancel Reservation" << endl;
//     //     cout << "4. Exit" << endl;
//     //     cout << "Enter your choice: ";
//     //     cin >> choice;
//     //     switch (choice)
//     //     {
//     //     case 1:
//     //         g.check_availability();
//     //         break;
//     //     case 2:
//     //         g.view_reservations();
//     //         break;
//     //     case 3:
//     //         g.cancel_reservation();
//     //         break;
//     //     case 4:
//     //         cout << "Exiting the program." << endl;
//     //         break;
//     //     }
//     //     cout << "----------------------------------------" << endl;
//     // } while (choice != 4);
//     //_______________TESTING INSERTING GUEST INFO_______________
// //    string fname, lname, contact_info, email, id_proof, address, status,relationship;
// //    int room_no;

// //     // Taking input from the user
// //     cout << "Enter First Name: ";
// //     cin >> fname;
// //     cout << "Enter Last Name: ";
// //     cin >> lname;
// //     cout << "Enter Contact Info: ";
// //     cin >> contact_info;
// //     cout << "Enter Email: ";
// //     cin >> email;
// //     cin.ignore(); // To consume the leftover newline character
// //     cout << "Enter ID Proof Type (e.g., Passport, License): ";
// //     getline(cin, id_proof);
// //     cout << "Enter Address: ";
// //     getline(cin, address);
// //     cout << "Enter relationship: ";
// //     getline(cin, relationship);
// //     cout << "Enter room_no: ";
// //     cin >> room_no;
// //     cin.ignore();
// //     cout << "Confirm Booking Status (occupied,):: ";
// //     getline(cin, status);

// //     // Call the function
// //     int gid = db.insertGuest(fname, lname, contact_info, email, id_proof,relationship, address);
// //     int rid = db.getRoomID(room_no);
// //     db.booking( gid, rid, status);
// //    string fname, lname, contact_info, email, id_proof, address;

//     // Taking input from the user
//     // cout << "Enter First Name: ";
//     // cin >> fname;
//     // cout << "Enter Last Name: ";
//     // cin >> lname;
//     // cout << "Enter Contact Info: ";
//     // cin >> contact_info;
//     // cout << "Enter Email: ";
//     // cin >> email;
//     // cin.ignore(); // To consume the leftover newline character
//     // cout << "Enter ID Proof Type (e.g., Passport, License): ";
//     // getline(cin, id_proof);
//     // cout << "Enter Address: ";
//     // getline(cin, address);

//     // Call the function
//     // db.insertGuest(fname, lname, contact_info, email, id_proof, address);
//     //    string fname, lname, contact_info, email, id_proof, address;

//     //     // Taking input from the user
//     //     cout << "Enter First Name: ";
//     //     cin >> fname;
//     //     cout << "Enter Last Name: ";
//     //     cin >> lname;
//     //     cout << "Enter Contact Info: ";
//     //     cin >> contact_info;
//     //     cout << "Enter Email: ";
//     //     cin >> email;
//     //     cin.ignore(); // To consume the leftover newline character
//     //     cout << "Enter ID Proof Type (e.g., Passport, License): ";
//     //     getline(cin, id_proof);
//     //     cout << "Enter Address: ";
//     //     getline(cin, address);

//     //     // Call the function
//     //     db.insertGuest(fname, lname, contact_info, email, id_proof, address);

//     //______________TESTING INSERTING ROOM INFO_________________________
//     // int room_no;
//     // string room_type, price_per_night, status;

//     // // Taking input from the user
//     // cout << "Enter Room Number: ";
//     // cin >> room_no;
//     // cin.ignore(); // To consume the leftover newline character

//     // cout << "Enter Room Type (e.g., Single, Double, Suite): ";
//     // getline(cin, room_type);

//     // cout << "Enter Price per Night (e.g., 100, 150.50): ";
//     // getline(cin, price_per_night);

//     // cout << "Enter Room Status (Available/Booked): ";
//     // getline(cin, status);

//     // // Call the function
//     // db.insertRoomDetails(room_no, room_type, price_per_night, status);

//     //__________________________TESTING INSERTING ADMIN DETAILS___________________
//     // string username, password, email, role;

//     // // Taking input from the user
//     // cout << "Enter Username: ";
//     // cin >> username;

//     // cout << "Enter Password: ";
//     // cin >> password;

//     // cout << "Enter Email: ";
//     // cin >> email;
//     // cin.ignore(); // To consume the leftover newline character

//     // cout << "Enter Role (e.g., Manager, Staff): ";
//     // getline(cin, role);

// // // Call the function
// // db.insertAdmin(username, password, email);
//     // // Call the function
//     // db.insertAdmin(username, password, email, role);

//     // _____________________login with email VALIDATOR __________________________

// //______________________________SEARCHING GUEST INFO___________________________
// // cout << "\nSearching ..." << endl;
// // db.searchGuest("", "9827981557", "", "");

// //##____________________________TESTING THE UPDATE ADMIN DATA__________________

// // db.updateAdmin(1,"sitaula", "praj123@gmail.com", "pass");

// // db.updateAdmin(1,"", "", "pass");

// //##   ________________________TESTING UPDATING GUEST DATA______________________
// // vector <int> ID = db.searchGuest("","9827981557");      

// // db.updateGuest(ID,"qwerty","","");
//     // User u;
//     // u.loginpage();

//     // db.printGuests();

//     // db.close();

// // db.printGuests();
// // db.searchRoom(0, "single", "", "Available");
// // TESTING SEARCH FOR ROOM DETAILS

// // db.printGuests();


// // _____________________login with email VALIDATOR __________________________

// // Admin A;
// // int roomNo;
// // string roomType;
// // int price;

// // Admin admin;
// // admin.admin(db);
// // admin.setAdminInfo(1, "admin_user", "Manager");

// // // Test room management
// // cout << "\nAdding rooms..." << endl;
// // admin.addRoom(db, 101, "Deluxe", 150.00);
// // admin.addRoom(db, 102, "Standard", 100.00);
// // admin.addRoom(db, 103, "Suite", 250.00);

// // cout << "\nAll rooms:" << endl;
// // admin.viewAllRooms(db);

// //  cout << "\nUpdating room 102..." << endl;
// // // admin.updateRoom(db, 2, 102, "Superior", 125.00); 
// // cout << "\nAll rooms after update:" << endl;
// // admin.viewAllRooms(db);

// // // cout << "\nDeleting room 103..." << endl;
// // // admin.deleteRoom(db, 3); 

// // cout << "\nFinal room list:" << endl;
// // admin.viewAllRooms(db);

// // _______________________________________TESTING RESERVATION__________________________
// db.reservation(1,1,1,"reserved",2);
// // db.deleteReservation(1);

// // receptionist r;
// // r.cancel_reservation(db);
// // r.checkRoomAvailability(db);

// // int enterTestGuestDetails(Database& db) {
// //     cout << "\n========== ADDING TEST GUEST ==========" << endl;
    
// //     // Pre-filled test data
// //     string fname = "John";
// //     string lname = "Doe";
// //     string contact_info = "+1-555-123-4567";
// //     string email = "john.doe@email.com";
// //     string id_proof = "Passport: A12345678";
// //     string relationship = "married";
// //     string address = "123 Main St, New York, NY 10001";
    
// //     cout << "Adding test guest: " << fname << " " << lname << endl;
    
// //     int guest_id = db.insertGuest(fname, lname, contact_info, email, id_proof, relationship, address);
    
// //     if (guest_id > 0) {
// //         cout << " Test guest added successfully!" << endl;
// //         cout << "Guest ID: " << guest_id << endl;
// //         return guest_id;
// //     } else {
// //         cout << "Failed to add test guest." << endl;
// //         return -1;
// //     }
// // }










// db.close();
// return (0);
// }


  