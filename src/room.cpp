#include <iostream>
#include <conio.h>
#include "../include/room.h"
using namespace std;




void RoomFacilities::manageFacilities(Database &db) {
    
    
    
    int choice;
    
    while (true) {
        cout << "\n=== ROOM FACILITIES MANAGEMENT ===" << endl;
        cout << "1. Add Facility to Room" << endl;
        cout << "2. View Room Facilities" << endl;
        cout << "3. Update Facility Status" << endl;
        cout << "4. Delete Facility" << endl;
        cout << "5. Add Default Facilities" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                addFacility(db);
                break;
                
            case 2:
                viewFacilities(db);
                break;
                
            case 3:
                updateFacility(db);
                break;
                
            case 4:
                deleteFacility(db);
                break;
                
            // case 5:
            //     insertDefaultRoomFacilities(db.getDb());
            //     break;
                
            case 0:
                return;
                
            default:
                cout << "Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
}






bool RoomFacilities::addFacility(Database &db) {
    
    
    
    int room_id;
    string facility_name, availability, description;
    
    cout << "\n--- ADD FACILITY TO ROOM ---" << endl;
    
    // Show available rooms first
    cout << "Available Rooms:" << endl;
    db.printRoomDetails();
    
    cout << "\nEnter Room ID: ";
    cin >> room_id;
    cin.ignore();
    
    // Verify room exists
    const char* checkRoom = "SELECT room_no FROM RoomDetails WHERE room_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkRoom, -1, &checkStmt, nullptr);
    sqlite3_bind_int(checkStmt, 1, room_id);
    
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "Error: Room ID " << room_id << " not found!" << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);
    
    cout << "Enter Facility Name: ";
    getline(cin, facility_name);
    
    cout << "Enter Availability (Available/Not Available/Under Maintenance): ";
    getline(cin, availability);
    
    cout << "Enter Description: ";
    getline(cin, description);
    
    const char* sql = "INSERT INTO RoomFacilities (room_id, facility_name, availability, description) "
                     "VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_text(stmt, 2, facility_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, availability.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, description.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << "Facility added successfully!" << endl;
        
        return true;
    } else {
        cerr << "Failed to insert facility: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
}

void RoomFacilities::viewFacilities(Database &db) {
    
    
    
    int room_id;
    
    cout << "\n--- VIEW ROOM FACILITIES ---" << endl;
    
    // Show available rooms first
    cout << "Available Rooms:" << endl;
    db.printRoomDetails();
    
    cout << "\nEnter Room ID to view facilities: ";
    cin >> room_id;
    cin.ignore();
    
    const char* sql = "SELECT rf.facility_id, rf.facility_name, rf.availability, rf.description, "
                     "rd.room_no, rd.room_type "
                     "FROM RoomFacilities rf "
                     "JOIN RoomDetails rd ON rf.room_id = rd.room_id "
                     "WHERE rf.room_id = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.getDb()) << endl;
        return;
    }
    
    sqlite3_bind_int(stmt, 1, room_id);
    
    bool hasData = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!hasData) {
            cout << "\nRoom Number: " << sqlite3_column_int(stmt, 4) << endl;
            cout << "Room Type: " << sqlite3_column_text(stmt, 5) << endl;
            cout << "Facilities:" << endl;
            cout << "----------------------------------------" << endl;
            hasData = true;
        }
        
        cout << "ID: " << sqlite3_column_int(stmt, 0)
             << " | " << sqlite3_column_text(stmt, 1)
             << " [" << sqlite3_column_text(stmt, 2) << "]" << endl;
        cout << "   Description: " << sqlite3_column_text(stmt, 3) << endl;
    }
    
    if (!hasData) {
        cout << "No facilities found for Room ID: " << room_id << endl;
    }
    
    sqlite3_finalize(stmt);
}

bool RoomFacilities::updateFacility(Database &db) {
    
    
    
    int facility_id;
    string availability;
    
    cout << "\n--- UPDATE FACILITY STATUS ---" << endl;
    
    // Show all facilities first
    cout << "All Facilities:" << endl;
    const char* showAll = "SELECT rf.facility_id, rf.facility_name, rf.availability, rd.room_no "
                         "FROM RoomFacilities rf "
                         "JOIN RoomDetails rd ON rf.room_id = rd.room_id "
                         "ORDER BY rd.room_no;";
    sqlite3_stmt* showStmt;
    sqlite3_prepare_v2(db.getDb(), showAll, -1, &showStmt, nullptr);
    
    cout << "ID | Room | Facility Name | Status" << endl;
    cout << "----------------------------------------" << endl;
    while (sqlite3_step(showStmt) == SQLITE_ROW) {
        cout << sqlite3_column_int(showStmt, 0) << " | "
             << sqlite3_column_int(showStmt, 3) << " | "
             << sqlite3_column_text(showStmt, 1) << " | "
             << sqlite3_column_text(showStmt, 2) << endl;
    }
    sqlite3_finalize(showStmt);
    
    cout << "\nEnter Facility ID to update: ";
    cin >> facility_id;
    cin.ignore();
    
    // Verify facility exists
    const char* checkFacility = "SELECT facility_name FROM RoomFacilities WHERE facility_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkFacility, -1, &checkStmt, nullptr);
    sqlite3_bind_int(checkStmt, 1, facility_id);
    
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "Error: Facility ID " << facility_id << " not found!" << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);
    
    cout << "Enter new availability (Available/Not Available/Under Maintenance): ";
    getline(cin, availability);
    
    const char* sql = "UPDATE RoomFacilities SET availability = ?, updated_at = datetime('now') "
                     "WHERE facility_id = ?;";
    sqlite3_stmt* stmt;
    
    rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, availability.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, facility_id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << "Facility updated successfully!" << endl;
        cout << "Updated by: CrypticLuminary at 2025-07-04 13:25:46" << endl;
        return true;
    } else {
        cerr << "Failed to update facility: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
}

bool RoomFacilities::deleteFacility(Database &db) {
    
    
    
    int facility_id;
    
    cout << "\n--- DELETE FACILITY ---" << endl;
    
    // Show all facilities first
    cout << "All Facilities:" << endl;
    const char* showAll = "SELECT rf.facility_id, rf.facility_name, rf.availability, rd.room_no "
                         "FROM RoomFacilities rf "
                         "JOIN RoomDetails rd ON rf.room_id = rd.room_id "
                         "ORDER BY rd.room_no;";
    sqlite3_stmt* showStmt;
    sqlite3_prepare_v2(db.getDb(), showAll, -1, &showStmt, nullptr);
    
    cout << "ID | Room | Facility Name | Status" << endl;
    cout << "----------------------------------------" << endl;
    while (sqlite3_step(showStmt) == SQLITE_ROW) {
        cout << sqlite3_column_int(showStmt, 0) << " | "
             << sqlite3_column_int(showStmt, 3) << " | "
             << sqlite3_column_text(showStmt, 1) << " | "
             << sqlite3_column_text(showStmt, 2) << endl;
    }
    sqlite3_finalize(showStmt);
    
    cout << "\nEnter Facility ID to delete: ";
    cin >> facility_id;
    cin.ignore();
    
    // Verify facility exists
    const char* checkFacility = "SELECT facility_name FROM RoomFacilities WHERE facility_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkFacility, -1, &checkStmt, nullptr);
    sqlite3_bind_int(checkStmt, 1, facility_id);
    
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "Error: Facility ID " << facility_id << " not found!" << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);
    
    char confirm;
    cout << "Are you sure you want to delete this facility? (y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Delete operation cancelled." << endl;
        return false;
    }
    
    const char* sql = "DELETE FROM RoomFacilities WHERE facility_id = ?;";
    sqlite3_stmt* stmt;
    
    rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, facility_id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << "Facility deleted successfully!" << endl;
        
        return true;
    } else {
        cerr << "Failed to delete facility: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
}