#ifndef ADMIN_H
#define ADMIN_H
#include<iostream>
#include <string.h>
using namespace std;

// class Admin {
// public:
    
// void roomDetains(int roomNo, string roomType, int price);
// };


class Database; // Forward declaration

class Admin {
private:
    int adminID;
    std::string username;
    std::string role;
    
public:
    Admin();
    
    // Room Management
    bool addRoom(Database& db, int roomNo, const std::string& roomType, double price /* const std::string& status*/);
    
    // bool updateRoom(Database& db, int roomId, int newRoomNo, const std::string& newRoomType, double newPrice);
    // bool deleteRoom(Database& db, int roomId);
    void viewAllRooms(Database& db);
    
    
    int getAdminID() const { return adminID; }
    std::string getUsername() const { return username; }
    std::string getRole() const { return role; }
    
    
    void setAdminInfo(int id, const std::string& name, const std::string& adminRole);
};



#endif
