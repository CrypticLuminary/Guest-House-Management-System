#ifndef DATABASE_H
#define DATABASE_H

#include "./sqlite3.h"
#include <string>
#include <vector>
using namespace std;

class Database {
public:
    Database(const std::string& dbFile);
    ~Database();

    bool open();
    void close();
    bool createTable();

    //##----------------INSERTING DATA IN TO THE TABLE--------------------------------##

    //GUEST DATA
    int insertGuest(
        const string& fname,
        const string& lname, 
        const string& contact_info, 
        const string& email, 
        const string& id_proof, 
        const string& relationship, 
        const string& address);

    // STAFF DETAILS
    bool insertStaffDetails(
        const string& fname, 
        const string& lname, 
        const string& contact_info, 
        const string& email, 
        const string& id_proof, 
        const string& relationship,
        const string& address);

    // ADMIN LOGIN DETAILS
    bool insertAdmin(
        const string& username, 
        const string& password, 
        const string& email);

    //STAFF LOGIN DETAIL
    bool insertStaff(
        const string& username, 
        const string& password, 
        const string& email, 
        const string& role);

    // INSERTING ROOM DETAILS
    bool insertRoomDetails(
        const int& room_no, 
        const string& room_type, 
        const string& price_per_night, 
        const string& status);

    //BOOKING
    bool booking (
        int guest_id, 
        int room_id, 
        const string& status);

    //##-----------------UP[DATIING DATA ON THE DATABASE-------------------------------##

    // UPDATING ADMIN LOGIN DETAILS
    bool updateAdmin(
        int admin_id, 
        const std::string& username = "", 
        const std::string& email = "", 
        const std::string& password = "");

    bool updateGuest(
        vector<int> guest_id,
        const string& fname = "", 
        const string& lname = "", 
        const string& contact_info = "", 
        const string& email = "", 
        const string& id_proof = "", 
        const string& relationship = "", 
        const string& address = "");


    //##--------------------SEARCHING DATA ON THE TABLE---------------------------------##

    vector<int> searchGuest(
        const string&email = "",
        const string& contact_info="",
        const string& check_in_date="",
        const string& check_out_date = "",
        int room_no = 0,
        const string& room_type = "");

    void searchRoom(
        const int& room_no, 
        const string& room_type, 
        const string& price_per_night, 
        const string& status);

    //##-------------------PRINTING THE DATA IN THE DATABASE----------------------------##
    void printGuests();
    void printRoomDetails();

    //##____________________GETTING RELATED DATA___________________________________________

    int getRoomID(int room_no);

    //_________________________DELETE FUNCTIONALITY IN THE DATABASE________________________
    bool deleteGuest(int guest_id);
    bool deleteRoom(int room_id);
    bool deleteBooking(int booking_id);




private:
    sqlite3* db;
    std::string dbFile;
};

#endif // 