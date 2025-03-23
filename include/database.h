#ifndef DATABASE_H
#define DATABASE_H

#include "./sqlite3.h"
#include <string>

class Database {
public:
    Database(const std::string& dbFile);
    ~Database();

    bool open();
    void close();
    bool createTable();
    bool insertGuest(const std::string& name, const std::string& phone, const std::string& email);
    void printGuests();

private:
    sqlite3* db;
    std::string dbFile;
};

#endif // DATABASE_H