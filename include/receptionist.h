#ifndef RECEPTIONIST_H
#define RECEPTIONIST_H

#include <iostream>
#include <string>
using namespace std;

// Forward declaration
class Database;

class receptionist {

  
public:
    // Guest management
    int enterGuestDetails(Database& db);
    
    // Room and reservation management
    void checkRoomAvailability(Database& db, const string& check_in_date = "", const string& check_out_date = "");
    int update_reservation(Database& db);
    int cancel_reservation(Database& db);
    int view_reservations(Database& db);
    bool staffPower(Database& db);
};

#endif