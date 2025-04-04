#include <iostream>
#include "../include/database.h"
#include "../include/guest.h"
#include "../include/booking.h"
#include "../include/billing.h"
#include "../include/reports.h"
#include "../include/admin.h"
#include "../include/housekeeping.h"
#include "../include/receptionist.h" 


int main() {
    // // Create a Database object
    // Database db("guest_house.db");

    // // Open the database
    // if (!db.open()) {
    //     return 1;
    // }

    // // Create the Guests table
    // if (!db.createTable()) {
    //     db.close();
    //     return 1;
    // }

    // // Insert a guest
    // db.insertGuest("John Doe", "1234567890", "john.doe@example.com");
    // db.insertGuest("Jane Smith", "9876543210", "jane.smith@example.com");

    // // Print all guests
    // db.printGuests();

    // // Close the database
    // db.close();
    
    Receptionist r;
    r.greetGuest();

    return 0;
}