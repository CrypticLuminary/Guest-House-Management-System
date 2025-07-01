#ifndef STAFF_H
#define STAFF_H

#include <iostream>
#include <string>
using namespace std;

// Forward declaration
class Database;

class staffmanager {
public:
    // Staff management functions
    bool insertStaff(Database& db);
    bool updateStaff(Database& db);
    bool deleteStaff(Database& db);
    void viewAllStaff(Database& db);
    
    // Login function - NO default parameters in header
    bool loginStaff(Database& db, string& loggedUsername, string& loggedRole);
    
    bool changePassword(Database& db, const string& username);
    void staffMenu(Database& db);
};

#endif