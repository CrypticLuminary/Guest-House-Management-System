#ifndef ADMIN_H
#define ADMIN_H

#include <string> // For std::string
#include "../include/database.h" // Include Database class definition

class Admin {
private:
    int adminID;
    std::string username;
    std::string role;

public:
    Admin();

    // Room Management
    bool addRoom(Database& db, int roomNo, const std::string& roomType, double price);
    void viewAllRooms(Database& db);
    bool admin(Database& db);
    

    // Admin Management
    bool hasAdmin(Database& db); // Check if any admin exists
    bool createDefaultAdmin(Database& db, const std::string& username, const std::string& email, const std::string& password); // Create an admin
    bool validateAdminLogin(Database& db, const std::string& username, const std::string& password); // Validate admin login
    bool loginAdmin(Database &db);
    bool adminPower(Database &db);
    bool manageRoom(Database &db);
    bool managestaff(Database &db);
    bool manage(Database &db);

    // Getters
    int getAdminID() const { return adminID; }
    std::string getUsername() const { return username; }
    std::string getRole() const { return role; }

    // Setter
    void setAdminInfo(int id, const std::string& name, const std::string& adminRole);
};

#endif