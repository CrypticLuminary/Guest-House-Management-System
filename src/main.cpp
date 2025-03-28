#include <iostream>
#include "../include/database.h"
#include "../include/guest.h"
#include "../include/booking.h"
#include "../include/billing.h"
#include "../include/reports.h"
#include "../include/admin.h"
#include "../include/housekeeping.h"



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

    // Insert a guest
    string name, email;
    string phone_no;
    
    // cout << "Enter the name phone_no and email of the person :: ";
    // cin >> name >> phone_no >> email ;
    // cout << "\n";

    // db.insertGuest(name, phone_no, email);



    // db.insertRoomDetails(200, "single bed", "reserve");
    // db.insertRoomDetails(201, "double bed", "available");

    // Print all guests
    // db.printGuests();
    // db.printRoomDetails();

    // Close the database
    db.close();

    return 0;
}