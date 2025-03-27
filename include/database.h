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
    bool insertGuest(const string& name, const string& phone, const string& email);
    bool insertRoomDetails(const int& room_no, const string& room_type, const string& status);
    void printGuests();
    void printRoomDetails();

private:
    sqlite3* db;
    std::string dbFile;
};

#endif // DATABASE_H