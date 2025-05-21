#include "../include/admin.h"
#include "../include/database.h"
#include <iostream>
#include<string.h>
using namespace std;


Admin::Admin() : adminID(0) {}

void Admin::setAdminInfo(int id, const std::string& name, const std::string& adminRole) {
    adminID = id;
    username = name;
    role = adminRole;
}

bool Admin::addRoom(Database& db, int roomNo, const std::string& roomType, double price/*const std::string& status*/ ) {
    if (roomNo <= 0 || price <= 0) {
        cout << "Error: Invalid room number or price\n";
        return false;
    }
//status ko lagi k garne baki xaaa
    if (roomType.empty()) {
        cout << "Error: Room type cannot be empty\n";
        return false;
    }

    return db.insertRoomDetails(roomNo, roomType, std::to_string(price), "Available");
}

// bool Admin::updateRoom(Database& db, int roomId, int newRoomNo, 
//                       const std::string& newRoomType, double newPrice) {
//     if (roomId <= 0 || newRoomNo <= 0 || newPrice <= 0) {
//         cout << "Error: Invalid room parameters\n";
//         return false;
//     }

//     if (newRoomType.empty()) {
//         cout << "Error: Room type cannot be empty\n";
//         return false;
//     }

    
//     return db.updateRoomDetails(roomId, newRoomNo, newRoomType, 
//                               std::to_string(newPrice), "Available");
// }

// bool Admin::deleteRoom(Database& db, int roomId) {
//     if (roomId <= 0) {
//         cout << "Error: Invalid room ID\n";
//         return false;
//     }

    
//     return db.deleteRoom(roomId);
// }

void Admin::viewAllRooms(Database& db) {
    db.printRoomDetails();
}