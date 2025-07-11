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
    bool checkRoomAvailability(Database& db, const string& check_in_date = "", const string& check_out_date = "");
    int update_reservation(Database& db);
    int cancel_reservation(Database& db);
    int view_reservations(Database& db);
    bool staffPower(Database& db);
    bool updateGuestDetail(Database& db);
    bool deleteGuestDetail(Database& db);
    string getRoomFacilities(Database& db, int room_id);
    void displayRoomTypeSummary(Database& db);


    // int enterGuestDetails(Database& db);
    bool checkSimpleRoomAvailability(Database& db);
    int selectRoomSimple(Database& db, int stay_duration);
    string getRoomFacilitiesSimple(Database& db, int room_id);
    bool isRoomAvailable(Database& db, int room_id);
    // int cancel_reservation(Database& db);
};

#endif