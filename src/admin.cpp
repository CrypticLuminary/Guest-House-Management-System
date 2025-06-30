#include <conio.h>

#include "../include/admin.h"
#include "../include/database.h"
#include <sqlite3.h>
#include <iostream>

//____________________MAIN PART OF ADMIN_______________________________
bool Admin::admin(Database& db) {
Admin admin;

    if (!admin.hasAdmin(db)) {
        std::cout << "No admin found. Please create an admin account." << std::endl;
        std::string username, email, password;

        std::cout << "Enter Admin Username: ";
        std::getline(std::cin, username);

        std::cout << "Enter Admin Email: ";
        std::getline(std::cin, email);

        std::cout << "creedential issue in guest.h"
        << "Enter Admin Password: ";
        std::getline(std::cin, password);

        if (admin.createDefaultAdmin(db, username, email, password)) {
            std::cout << "Admin account created successfully!" << std::endl;
            admin.loginAdmin(db);
        } else {
            std::cerr << "Failed to create admin account." << std::endl;
            db.close();
            return 1;
        }
    }
    
return 1;
}

bool Admin::loginAdmin(Database& db) {
    Admin admin;
    bool loggedIn = false;
    int attempts = 0;
    const int maxAttempts = 3;

    while (!loggedIn && attempts < maxAttempts) {
        std::string username, password;
        std::cout << "\nAdmin Login" << std::endl;
        std::cout << "Enter Username: ";
        std::getline(std::cin, username);
        std::cout << "Enter Password: ";
        std::getline(std::cin, password);

        if (admin.validateAdminLogin(db, username, password)) {
            std::cout << "Login successful! Welcome, " << username << "." << std::endl;
            std::cout << "\n\n";
            system("CLS");
            admin.adminPower(db);
            loggedIn = true;
        } else {
            attempts++;
            std::cout << "Invalid username or password. " << (maxAttempts - attempts) << " attempts remaining." << std::endl;
        }
    }

    if (!loggedIn) {
        std::cerr << "Too many failed login attempts. Exiting program." << std::endl;
        db.close();
        return 1;
    }

}

//______________________DISPLAY PART OF ADMIN__________________________






bool Admin::adminPower(Database &db) {
    
    int choice;
    std::cout << "+++++++++++++++++++++++++++++++++++++++    ADMIN INTERFACE    +++++++++++++++++++++++++++++++++++++++++++++++"<<"\n\n\n";
    std::cout << "&$#*&#*&#*#&*#&*&#*&#*&#*&#*&#&#*&#*&#    ENTER YOUR CHPICE   *&#*&#*#&*#&*#&*#&*#&*#&*#&*#&*#&*#&*#*#&*#&#*&*#"<<"\n\n";
while (true)
{

    std::cout << "\nHotel Management Menu:\n";
    std::cout << "1. Add Room\n";
    std::cout << "2. View All Rooms\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter choice (1-3): ";
    std::cin >> choice;

    if (std::cin.fail()) {
        
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            int roomNo;
            std::string roomType;
            double price;

            std::cout << "Enter room number: ";
            std::cin >> roomNo;
            std::cin.ignore(); // Clear newline
            std::cout << "Enter room type (e.g., Single, Double): ";
            std::getline(std::cin, roomType);
            std::cout << "Enter price: ";
            std::cin >> price;

            if (addRoom(db, roomNo, roomType, price)) {
                std::cout << "Room added successfully!\n";
            } else {
                std::cout << "Failed to add room.\n";
            }
            break;
        }
        case 2:
            viewAllRooms(db);
            break;
        case 3:
            std::cout << "Exiting...\n";
            return 0;
        default:
            std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
            break;
        }
        
    }

}



















// _____________________________ADMIN WORK START FROM HERE______________________


Admin::Admin() : adminID(0), username(""), role("") {}

void Admin::setAdminInfo(int id, const std::string& name, const std::string& adminRole) {
    adminID = id;
    username = name;
    role = adminRole;
}


bool Admin::hasAdmin(Database& db) {
    const char* sql = "SELECT COUNT(*) FROM Admin;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.getDb()) << std::endl;
        return false;
    }

    bool hasAdmin = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        hasAdmin = count > 0;
    }

    sqlite3_finalize(stmt);
    return hasAdmin;
}


bool Admin::createDefaultAdmin(Database& db, const std::string& username, const std::string& email, const std::string& password) {
    return db.insertAdmin(username, password ,email);
}


bool Admin::validateAdminLogin(Database& db, const std::string& username, const std::string& password) {
    const char* sql = "SELECT COUNT(*) FROM Admin WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.getDb()) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool isValid = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        isValid = count > 0;
    }

    sqlite3_finalize(stmt);
    return isValid;
}

// Room management methods
bool Admin::addRoom(Database& db, int roomNo, const std::string& roomType, double price) {
    return db.insertRoomDetails(roomNo, roomType, std::to_string(price), "Available");
}

void Admin::viewAllRooms(Database& db) {
    db.printRoomDetails();
}