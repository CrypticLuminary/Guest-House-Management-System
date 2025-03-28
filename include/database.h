#ifndef DATABASE_H
#define DATABASE_H

#include "./sqlite3.h"
#include <string>
using namespace std;

class Database {
public:
    Database(const std::string& dbFile);
    ~Database();

    bool open();
    void close();
    bool createTable();
    bool insertGuest(const string& fname, const string& lname, const string& contact_info, const string& email, const string& id_proof, const string& address);
    bool insertAdmin(const string& username, const string& password, const string& email, const string& role);
    bool insertRoomDetails(const int& room_no, const string& room_type, const string& price_per_night, const string& status);
    void printGuests();
    void printRoomDetails();

private:
    sqlite3* db;
    std::string dbFile;
};

#endif // DATABASE_H