#include <iostream>
#include <conio.h>
#include "../include/receptionist.h"
#include "../include/database.h"
#include "../include/validator.h"
#include "../include/guest.h"
#include <string.h>
#include <iomanip>      
#include <regex>          
#include <sstream> 
using namespace std;
Validate a;
int ch, room;




void receptionist::checkRoomAvailability(Database& db, const string& check_in_date, const string& check_out_date) {
    cout << "\n=== ROOM AVAILABILITY CHECK ===" << endl;
    cout << "Current Time: 2025-07-01 05:07:42 (UTC)" << endl;
    cout << "Current User: CrypticLuminary" << endl;
    
    // Display search criteria
    if (!check_in_date.empty() && !check_out_date.empty()) {
        cout << "Check-in Date: " << check_in_date << endl;
        cout << "Check-out Date: " << check_out_date << endl;
        cout << "Searching for available rooms between these dates..." << endl;
    } else if (!check_in_date.empty()) {
        cout << "Check-in Date: " << check_in_date << endl;
        cout << "Searching for rooms available from this date..." << endl;
    } else {
        cout << "Showing all room availability status..." << endl;
    }
    
    cout << string(70, '-') << endl;
    
    // Query to check room availability
    string sql;
    if (!check_in_date.empty() && !check_out_date.empty()) {
        // Check rooms available for specific date range
        sql = "SELECT r.room_id, r.room_number, r.room_type, r.price_per_night, r.status, "
              "CASE WHEN res.room_id IS NULL THEN 'Available' ELSE 'Occupied' END as availability "
              "FROM Rooms r "
              "LEFT JOIN Reservations res ON r.room_id = res.room_id "
              "AND res.status = 'confirmed' "
              "AND NOT (res.check_out_date <= ? OR res.check_in_date >= ?) "
              "ORDER BY r.room_number;";
    } else {
        // Show all rooms with current status
        sql = "SELECT r.room_id, r.room_number, r.room_type, r.price_per_night, r.status, "
              "CASE WHEN res.room_id IS NULL THEN 'Available' ELSE 'Occupied' END as availability "
              "FROM Rooms r "
              "LEFT JOIN Reservations res ON r.room_id = res.room_id "
              "AND res.status = 'confirmed' "
              "AND CURRENT_DATE BETWEEN res.check_in_date AND res.check_out_date "
              "ORDER BY r.room_number;";
    }
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cout << " Error preparing query: " << sqlite3_errmsg(db.getDb()) << endl;
        return;
    }
    
    // Bind parameters if date range is specified
    if (!check_in_date.empty() && !check_out_date.empty()) {
        sqlite3_bind_text(stmt, 1, check_in_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, check_out_date.c_str(), -1, SQLITE_STATIC);
    }
    
    // Display header
    cout << left << setw(8) << "Room ID" 
         << setw(12) << "Room No" 
         << setw(15) << "Type" 
         << setw(12) << "Price/Night" 
         << setw(12) << "Status" 
         << setw(15) << "Availability" << endl;
    cout << string(70, '-') << endl;
    
    int totalRooms = 0;
    int availableRooms = 0;
    int occupiedRooms = 0;
    
    // Execute query and display results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        totalRooms++;
        
        int roomId = sqlite3_column_int(stmt, 0);
        string roomNumber = (char*)sqlite3_column_text(stmt, 1);
        string roomType = (char*)sqlite3_column_text(stmt, 2);
        double price = sqlite3_column_double(stmt, 3);
        string status = (char*)sqlite3_column_text(stmt, 4);
        string availability = (char*)sqlite3_column_text(stmt, 5);
        
        // Count availability
        if (availability == "Available") {
            availableRooms++;
        } else {
            occupiedRooms++;
        }
        
        // Display room info
        cout << left << setw(8) << roomId
             << setw(12) << roomNumber
             << setw(15) << roomType
             << setw(12) << fixed << setprecision(2) << price
             << setw(12) << status
             << setw(15) << availability << endl;
    }
    
    sqlite3_finalize(stmt);
    
    // Display summary
    cout << string(70, '=') << endl;
    cout << "AVAILABILITY SUMMARY:" << endl;
    cout << "Total Rooms: " << totalRooms << endl;
    cout << "Available: " << availableRooms << endl;
    cout << "Occupied: " << occupiedRooms << endl;
    
    if (!check_in_date.empty() && !check_out_date.empty()) {
        cout << "Date Range: " << check_in_date << " to " << check_out_date << endl;
    }
    
    cout << "Query executed by: CrypticLuminary at 2025-07-01 05:07:42 (UTC)" << endl;
    cout << string(70, '=') << endl;
    
    // Show available room types
    if (availableRooms > 0) {
        cout << "\n AVAILABLE ROOM TYPES:" << endl;
        
        // Query available room types
        string typeSQL;
        if (!check_in_date.empty() && !check_out_date.empty()) {
            typeSQL = "SELECT DISTINCT r.room_type, COUNT(*) as count, MIN(r.price_per_night) as min_price "
                     "FROM Rooms r "
                     "LEFT JOIN Reservations res ON r.room_id = res.room_id "
                     "AND res.status = 'confirmed' "
                     "AND NOT (res.check_out_date <= ? OR res.check_in_date >= ?) "
                     "WHERE res.room_id IS NULL AND r.status = 'available' "
                     "GROUP BY r.room_type ORDER BY min_price;";
        } else {
            typeSQL = "SELECT DISTINCT r.room_type, COUNT(*) as count, MIN(r.price_per_night) as min_price "
                     "FROM Rooms r "
                     "LEFT JOIN Reservations res ON r.room_id = res.room_id "
                     "AND res.status = 'confirmed' "
                     "AND CURRENT_DATE BETWEEN res.check_in_date AND res.check_out_date "
                     "WHERE res.room_id IS NULL AND r.status = 'available' "
                     "GROUP BY r.room_type ORDER BY min_price;";
        }
        
        rc = sqlite3_prepare_v2(db.getDb(), typeSQL.c_str(), -1, &stmt, nullptr);
        
        if (!check_in_date.empty() && !check_out_date.empty()) {
            sqlite3_bind_text(stmt, 1, check_in_date.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, check_out_date.c_str(), -1, SQLITE_STATIC);
        }
        
        cout << "Room Type\t\tAvailable\tStarting Price" << endl;
        cout << string(50, '-') << endl;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string roomType = (char*)sqlite3_column_text(stmt, 0);
            int count = sqlite3_column_int(stmt, 1);
            double minPrice = sqlite3_column_double(stmt, 2);
            
            cout << left << setw(20) << roomType
                 << setw(12) << count
                 << "$" << fixed << setprecision(2) << minPrice << endl;
        }
        
        sqlite3_finalize(stmt);
    } else {
        cout << "\n NO ROOMS AVAILABLE for the specified criteria." << endl;
        cout << "Please try different dates or contact management." << endl;
    }
    
    cout << "\nAvailability check completed by: CrypticLuminary" << endl;
    cout << "Report generated at: 2025-07-01 05:07:42 (UTC)" << endl;
}







bool isValidEmail(const string& email) {
    const regex pattern(R"((\w+)(\.{1}\w+)*@(\w+)(\.{1}\w+)+)");
    return regex_match(email, pattern);
}


bool isValidPhone(const string& phone) {
    const regex pattern(R"(^[\+]?[0-9\-\s\(\)]{10,15}$)");
    return regex_match(phone, pattern);
}


int receptionist::enterGuestDetails(Database& db) {
    string fname, lname, contact_info, email, id_proof, relationship, address;
    
    cout << "\n========== GUEST REGISTRATION ==========" << endl;
    cout << "Enter guest details:" << endl;
    
    // First Name
    do {
        cout << "First Name: ";
        getline(cin, fname);
        if (fname.empty()) {
            cout << " First name cannot be empty! Please try again." << endl;
        }
    } while (fname.empty());
    
    // Last Name
    do {
        cout << "Last Name: ";
        getline(cin, lname);
        if (lname.empty()) {
            cout << " Last name cannot be empty! Please try again." << endl;
        }
    } while (lname.empty());
    
    // Contact Info
    do {
        cout << "Contact Number: ";
        getline(cin, contact_info);
        if (contact_info.empty()) {
            cout << " Contact number cannot be empty! Please try again." << endl;
        } else if (!isValidPhone(contact_info)) {
            cout << " Invalid phone number format! Please try again." << endl;
            contact_info = ""; // Reset to retry
        }
    } while (contact_info.empty());
    
    // Email
    do {
        cout << "Email: ";
        getline(cin, email);
        if (email.empty()) {
            cout << " Email cannot be empty! Please try again." << endl;
        } else if (!isValidEmail(email)) {
            cout << " Invalid email format! Please try again." << endl;
            email = ""; // Reset to retry
        }
    } while (email.empty());
    
    // ID Proof
    cout << "ID Proof (e.g., Passport, Driver's License): ";
    getline(cin, id_proof);
    if (id_proof.empty()) {
        id_proof = "Not provided";
    }
    
    // Relationship Status
    do {
        cout << "Relationship Status (single/married): ";
        getline(cin, relationship);
        if (!relationship.empty() && relationship != "single" && relationship != "married") {
            cout << " Please enter either 'single' or 'married'!" << endl;
            relationship = ""; 
        }
    } while (!relationship.empty() && relationship != "single" && relationship != "married");
    
    if (relationship.empty()) {
        relationship = "single";
    }
    
    // Address
    cout << "Address: ";
    getline(cin, address);
    if (address.empty()) {
        address = "Not provided";
    }
  
    cout << "\n========== CONFIRMATION ==========" << endl;
    cout << "Please confirm the following details:" << endl;
    cout << "Name: " << fname << " " << lname << endl;
    cout << "Contact: " << contact_info << endl;
    cout << "Email: " << email << endl;
    cout << "ID Proof: " << id_proof << endl;
    cout << "Relationship: " << relationship << endl;
    cout << "Address: " << address << endl;
    
    char confirm;
    cout << "\nIs this information correct? (y/n): ";
    cin >> confirm;
    cin.ignore(); 
    
    if (confirm == 'y' || confirm == 'Y') {
       
        int guest_id = db.insertGuest(fname, lname, contact_info, email, id_proof, relationship, address);
        
        if (guest_id > 0) {
            cout << " Guest registered successfully!" << endl;
            cout << "Guest ID: " << guest_id << endl;
            return guest_id;
        } else {
            cout << " Failed to register guest. Please try again." << endl;
            return -1;
        }
    } else {
        cout << " Guest registration cancelled." << endl;
        return -1;
    }
}

int receptionist::update_reservation(Database& db) {
    cout << "\n========== UPDATE RESERVATION ==========" << endl;
    
    
    cout << "Current reservations:" << endl;
    if (!view_reservations(db)) {
        cout << " No reservations found to update." << endl;
        return -1;
    }
    
    int reservation_id;
    cout << "\nEnter Reservation ID to update: ";
    cin >> reservation_id;
    cin.ignore(); // Clear input buffer
    
    // Check if reservation exists
    const char* checkReservation = "SELECT r.reservation_id, r.check_in_date, r.check_out_date, "
                                  "r.guest_id, r.room_id, r.booking_id, r.booking_status, "
                                  "g.fname, g.lname, rd.room_no "
                                  "FROM Reservations r "
                                  "JOIN Guests g ON r.guest_id = g.guest_id "
                                  "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                                  "WHERE r.reservation_id = ?;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkReservation, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << " Error preparing statement: " << sqlite3_errmsg(db.getDb()) << endl;
        return -1;
    }
    
    sqlite3_bind_int(stmt, 1, reservation_id);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        cout << " Reservation ID " << reservation_id << " not found!" << endl;
        sqlite3_finalize(stmt);
        return -1;
    }
    
    // Display current reservation details
    cout << "\nCurrent Reservation Details:" << endl;
    cout << "Reservation ID: " << sqlite3_column_int(stmt, 0) << endl;
    cout << "Check-in Date: " << sqlite3_column_text(stmt, 1) << endl;
    cout << "Check-out Date: " << sqlite3_column_text(stmt, 2) << endl;
    cout << "Guest: " << sqlite3_column_text(stmt, 7) << " " << sqlite3_column_text(stmt, 8) << endl;
    cout << "Room Number: " << sqlite3_column_int(stmt, 9) << endl;
    cout << "Status: " << sqlite3_column_text(stmt, 6) << endl;
    
    // Store current values
    string current_checkin = (char*)sqlite3_column_text(stmt, 1);
    string current_checkout = (char*)sqlite3_column_text(stmt, 2);
    int current_guest_id = sqlite3_column_int(stmt, 3);
    int current_room_id = sqlite3_column_int(stmt, 4);
    int current_booking_id = sqlite3_column_int(stmt, 5);
    string current_status = (char*)sqlite3_column_text(stmt, 6);
    
    sqlite3_finalize(stmt);
    
    // Get update options
    cout << "\nWhat would you like to update?" << endl;
    cout << "1. Check-in Date" << endl;
    cout << "2. Check-out Date" << endl;
    cout << "3. Room" << endl;
    cout << "4. Status" << endl;
    cout << "5. All Details" << endl;
    cout << "Enter your choice (1-5): ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string new_checkin = current_checkin;
    string new_checkout = current_checkout;
    int new_room_id = current_room_id;
    string new_status = current_status;
    
    switch (choice) {
        case 1: {
            cout << "Current check-in date: " << current_checkin << endl;
            cout << "Enter new check-in date (YYYY-MM-DD): ";
            getline(cin, new_checkin);
            break;
        }
        case 2: {
            cout << "Current check-out date: " << current_checkout << endl;
            cout << "Enter new check-out date (YYYY-MM-DD): ";
            getline(cin, new_checkout);
            break;
        }
        case 3: {
            cout << "Available rooms:" << endl;
            checkRoomAvailability(db, new_checkin, new_checkout);
            cout << "Enter new room ID: ";
            cin >> new_room_id;
            cin.ignore();
            break;
        }
        case 4: {
            cout << "Current status: " << current_status << endl;
            cout << "Enter new status (reserved/confirmed/cancelled): ";
            getline(cin, new_status);
            break;
        }
        case 5: {
            cout << "Enter new check-in date (YYYY-MM-DD) [" << current_checkin << "]: ";
            getline(cin, new_checkin);
            if (new_checkin.empty()) new_checkin = current_checkin;
            
            cout << "Enter new check-out date (YYYY-MM-DD) [" << current_checkout << "]: ";
            getline(cin, new_checkout);
            if (new_checkout.empty()) new_checkout = current_checkout;
            
            cout << "Available rooms:" << endl;
            checkRoomAvailability(db, new_checkin, new_checkout);
            cout << "Enter new room ID [" << current_room_id << "]: ";
            string room_input;
            getline(cin, room_input);
            if (!room_input.empty()) new_room_id = stoi(room_input);
            
            cout << "Enter new status [" << current_status << "]: ";
            getline(cin, new_status);
            if (new_status.empty()) new_status = current_status;
            break;
        }
        default:
            cout << " Invalid choice!" << endl;
            return -1;
    }
    
    // Confirm update
    cout << "\n========== CONFIRMATION ==========" << endl;
    cout << "New Details:" << endl;
    cout << "Check-in Date: " << new_checkin << endl;
    cout << "Check-out Date: " << new_checkout << endl;
    cout << "Room ID: " << new_room_id << endl;
    cout << "Status: " << new_status << endl;
    
    char confirm;
    cout << "\nConfirm update? (y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (confirm != 'y' && confirm != 'Y') {
        cout << " Update cancelled." << endl;
        return -1;
    }
    
    // Update reservation
    const char* updateSQL = "UPDATE Reservations SET "
                           "check_in_date = ?, check_out_date = ?, "
                           "room_id = ?, booking_status = ? "
                           "WHERE reservation_id = ?;";
    
    rc = sqlite3_prepare_v2(db.getDb(), updateSQL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << " Error preparing update statement: " << sqlite3_errmsg(db.getDb()) << endl;
        return -1;
    }
    
    sqlite3_bind_text(stmt, 1, new_checkin.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, new_checkout.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, new_room_id);
    sqlite3_bind_text(stmt, 4, new_status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, reservation_id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << " Reservation updated successfully!" << endl;
        
        // Log the update
        cout << "Updated by: CrypticLuminary" << endl;
        cout << "Update time: 2025-07-01 04:09:47" << endl;
        
        return reservation_id;
    } else {
        cerr << " Failed to update reservation: " << sqlite3_errmsg(db.getDb()) << endl;
        return -1;
    }
}


int receptionist::cancel_reservation(Database& db) {
    cout << "\n========== CANCEL RESERVATION ==========" << endl;
    
    // Show all active reservations
    cout << "Active reservations:" << endl;
    const char* showActive = "SELECT r.reservation_id, r.check_in_date, r.check_out_date, "
                            "g.fname, g.lname, rd.room_no, r.booking_status "
                            "FROM Reservations r "
                            "JOIN Guests g ON r.guest_id = g.guest_id "
                            "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                            "WHERE r.booking_status != 'cancelled' "
                            "ORDER BY r.reservation_id;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), showActive, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << " Error preparing statement: " << sqlite3_errmsg(db.getDb()) << endl;
        return -1;
    }
    
    bool hasReservations = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasReservations = true;
        cout << "ID: " << sqlite3_column_int(stmt, 0)
             << " | " << sqlite3_column_text(stmt, 1) << " to " << sqlite3_column_text(stmt, 2)
             << " | Guest: " << sqlite3_column_text(stmt, 3) << " " << sqlite3_column_text(stmt, 4)
             << " | Room: " << sqlite3_column_int(stmt, 5)
             << " | Status: " << sqlite3_column_text(stmt, 6) << endl;
    }
    sqlite3_finalize(stmt);
    
    if (!hasReservations) {
        cout << " No active reservations found to cancel." << endl;
        return -1;
    }
    
    int reservation_id;
    cout << "\nEnter Reservation ID to cancel: ";
    cin >> reservation_id;
    cin.ignore();
    
    // Check if reservation exists and is not already cancelled
    const char* checkSQL = "SELECT r.reservation_id, r.booking_status, r.booking_id, "
                          "g.fname, g.lname, rd.room_no, r.check_in_date "
                          "FROM Reservations r "
                          "JOIN Guests g ON r.guest_id = g.guest_id "
                          "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                          "WHERE r.reservation_id = ?;";
    
    rc = sqlite3_prepare_v2(db.getDb(), checkSQL, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, reservation_id);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        cout << " Reservation ID " << reservation_id << " not found!" << endl;
        sqlite3_finalize(stmt);
        return -1;
    }
    
    string current_status = (char*)sqlite3_column_text(stmt, 1);
    int booking_id = sqlite3_column_int(stmt, 2);
    string guest_name = string((char*)sqlite3_column_text(stmt, 3)) + " " + string((char*)sqlite3_column_text(stmt, 4));
    int room_no = sqlite3_column_int(stmt, 5);
    string checkin_date = (char*)sqlite3_column_text(stmt, 6);
    
    sqlite3_finalize(stmt);
    
    if (current_status == "cancelled") {
        cout << " Reservation is already cancelled!" << endl;
        return -1;
    }
    
    // Show reservation details
    cout << "\nReservation to cancel:" << endl;
    cout << "Guest: " << guest_name << endl;
    cout << "Room: " << room_no << endl;
    cout << "Check-in Date: " << checkin_date << endl;
    cout << "Current Status: " << current_status << endl;
    
    // Cancellation reason
    cout << "\nReason for cancellation:" << endl;
    cout << "1. Guest request" << endl;
    cout << "2. No show" << endl;
    cout << "3. Payment issue" << endl;
    cout << "4. Other" << endl;
    cout << "Enter reason (1-4): ";
    
    int reason_choice;
    cin >> reason_choice;
    cin.ignore();
    
    string cancellation_reason;
    switch (reason_choice) {
        case 1: cancellation_reason = "Guest request"; break;
        case 2: cancellation_reason = "No show"; break;
        case 3: cancellation_reason = "Payment issue"; break;
        case 4: {
            cout << "Enter custom reason: ";
            getline(cin, cancellation_reason);
            break;
        }
        default: cancellation_reason = "Administrative cancellation"; break;
    }
    
    // Confirm cancellation
    char confirm;
    cout << "\n  WARNING: This action cannot be undone!" << endl;
    cout << "Confirm cancellation? (y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (confirm != 'y' && confirm != 'Y') {
        cout << " Cancellation aborted." << endl;
        return -1;
    }
    
    // Begin transaction
    sqlite3_exec(db.getDb(), "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    
    // Update reservation status
    const char* cancelSQL = "UPDATE Reservations SET "
                           "booking_status = 'cancelled', "
                           "cancellation_reason = ?, "
                           "cancelled_by = 'CrypticLuminary', "
                           "cancellation_date = '2025-07-01 04:09:47' "
                           "WHERE reservation_id = ?;";
    
    rc = sqlite3_prepare_v2(db.getDb(), cancelSQL, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, cancellation_reason.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, reservation_id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        sqlite3_exec(db.getDb(), "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << " Failed to cancel reservation: " << sqlite3_errmsg(db.getDb()) << endl;
        return -1;
    }
    
    // Update booking status
    const char* updateBookingSQL = "UPDATE Booking SET booking_status = 'cancelled' WHERE booking_id = ?;";
    rc = sqlite3_prepare_v2(db.getDb(), updateBookingSQL, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, booking_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    // Commit transaction
    sqlite3_exec(db.getDb(), "COMMIT;", nullptr, nullptr, nullptr);
    
    cout << " Reservation cancelled successfully!" << endl;
    cout << "Reservation ID: " << reservation_id << endl;
    cout << "Reason: " << cancellation_reason << endl;
    cout << "Cancelled by: CrypticLuminary" << endl;
    cout << "Cancellation time: 2025-07-01 04:09:47" << endl;
    
    return reservation_id;
}


int receptionist::view_reservations(Database& db) {
    cout << "\n========== VIEW RESERVATIONS ==========" << endl;
    
    cout << "Filter options:" << endl;
    cout << "1. All reservations" << endl;
    cout << "2. Active reservations only" << endl;
    cout << "3. Cancelled reservations only" << endl;
    cout << "4. Today's check-ins" << endl;
    cout << "5. Today's check-outs" << endl;
    cout << "6. Search by guest name" << endl;
    cout << "Enter your choice (1-6): ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string sql;
    string searchTerm = "";
    
    switch (choice) {
        case 1:
            sql = "SELECT r.reservation_id, r.check_in_date, r.check_out_date, "
                  "g.fname, g.lname, g.contact_info, rd.room_no, rd.room_type, "
                  "r.booking_status, r.booking_id "
                  "FROM Reservations r "
                  "JOIN Guests g ON r.guest_id = g.guest_id "
                  "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                  "ORDER BY r.reservation_id DESC;";
            break;
            
        case 2:
            sql = "SELECT r.reservation_id, r.check_in_date, r.check_out_date, "
                  "g.fname, g.lname, g.contact_info, rd.room_no, rd.room_type, "
                  "r.booking_status, r.booking_id "
                  "FROM Reservations r "
                  "JOIN Guests g ON r.guest_id = g.guest_id "
                  "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                  "WHERE r.booking_status != 'cancelled' "
                  "ORDER BY r.check_in_date;";
            break;
            
        case 3:
            sql = "SELECT r.reservation_id, r.check_in_date, r.check_out_date, "
                  "g.fname, g.lname, g.contact_info, rd.room_no, rd.room_type, "
                  "r.booking_status, r.booking_id, r.cancellation_reason, r.cancellation_date "
                  "FROM Reservations r "
                  "JOIN Guests g ON r.guest_id = g.guest_id "
                  "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                  "WHERE r.booking_status = 'cancelled' "
                  "ORDER BY r.cancellation_date DESC;";
            break;
            
        case 4:
            sql = "SELECT r.reservation_id, r.check_in_date, r.check_out_date, "
                  "g.fname, g.lname, g.contact_info, rd.room_no, rd.room_type, "
                  "r.booking_status, r.booking_id "
                  "FROM Reservations r "
                  "JOIN Guests g ON r.guest_id = g.guest_id "
                  "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                  "WHERE DATE(r.check_in_date) = DATE('2025-07-01') "
                  "AND r.booking_status != 'cancelled' "
                  "ORDER BY r.check_in_date;";
            break;
            
        case 5:
            sql = "SELECT r.reservation_id, r.check_in_date, r.check_out_date, "
                  "g.fname, g.lname, g.contact_info, rd.room_no, rd.room_type, "
                  "r.booking_status, r.booking_id "
                  "FROM Reservations r "
                  "JOIN Guests g ON r.guest_id = g.guest_id "
                  "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                  "WHERE DATE(r.check_out_date) = DATE('2025-07-01') "
                  "AND r.booking_status != 'cancelled' "
                  "ORDER BY r.check_out_date;";
            break;
            
        case 6:
            cout << "Enter guest name to search: ";
            getline(cin, searchTerm);
            sql = "SELECT r.reservation_id, r.check_in_date, r.check_out_date, "
                  "g.fname, g.lname, g.contact_info, rd.room_no, rd.room_type, "
                  "r.booking_status, r.booking_id "
                  "FROM Reservations r "
                  "JOIN Guests g ON r.guest_id = g.guest_id "
                  "JOIN RoomDetails rd ON r.room_id = rd.room_id "
                  "WHERE (g.fname LIKE ? OR g.lname LIKE ?) "
                  "ORDER BY r.reservation_id DESC;";
            break;
            
        default:
            cout << " Invalid choice!" << endl;
            return -1;
    }
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << " Error preparing statement: " << sqlite3_errmsg(db.getDb()) << endl;
        return -1;
    }
    
    // Bind search term if needed
    if (choice == 6 && !searchTerm.empty()) {
        string searchPattern = "%" + searchTerm + "%";
        sqlite3_bind_text(stmt, 1, searchPattern.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, searchPattern.c_str(), -1, SQLITE_STATIC);
    }
    
    // Display header
    cout << "\n" << string(120, '=') << endl;
    cout << left << setw(5) << "ID" 
         << setw(12) << "Check-in" 
         << setw(12) << "Check-out" 
         << setw(20) << "Guest Name" 
         << setw(15) << "Contact" 
         << setw(8) << "Room" 
         << setw(12) << "Room Type" 
         << setw(12) << "Status" 
         << setw(10) << "Booking ID";
    
    if (choice == 3) {
        cout << setw(15) << "Cancel Reason";
    }
    cout << endl;
    cout << string(120, '-') << endl;
    
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        count++;
        
        cout << left << setw(5) << sqlite3_column_int(stmt, 0)  // reservation_id
             << setw(12) << sqlite3_column_text(stmt, 1)        // check_in_date
             << setw(12) << sqlite3_column_text(stmt, 2)        // check_out_date
             << setw(20) << (string((char*)sqlite3_column_text(stmt, 3)) + " " + 
                           string((char*)sqlite3_column_text(stmt, 4)))  // guest name
             << setw(15) << sqlite3_column_text(stmt, 5)        // contact
             << setw(8) << sqlite3_column_int(stmt, 6)          // room_no
             << setw(12) << sqlite3_column_text(stmt, 7)        // room_type
             << setw(12) << sqlite3_column_text(stmt, 8)        // booking_status
             << setw(10) << sqlite3_column_int(stmt, 9);        // booking_id
        
        if (choice == 3 && sqlite3_column_text(stmt, 10)) {
            cout << setw(15) << sqlite3_column_text(stmt, 10);  // cancellation_reason
        }
        cout << endl;
    }
    
    sqlite3_finalize(stmt);
    
    cout << string(120, '=') << endl;
    cout << "Total reservations found: " << count << endl;

    
    return count;
}

bool receptionist::staffPower(Database& db) {
    int choice;
    system("CLS");
    cout << "\n" << string(100, '=') << endl;
    cout << "                           STAFF CONTROL PANEL" << endl;
    cout << string(100, '=') << endl;

    cout << string(100, '=') << endl;

    while (true) {
        cout << "\n" << string(80, '*') << endl;
        cout << "                    STAFF MANAGEMENT SYSTEM" << endl;
        cout << string(80, '*') << endl;
        
        cout << string(80, '-') << endl;
        
        cout << "GUEST MANAGEMENT OPTIONS:" << endl;
        cout << "   1. Enter Guest Details" << endl;
        cout << string(50, '-') << endl;
        
        cout << "ROOM MANAGEMENT OPTIONS:" << endl;
        cout << "   2. Check Room Availability" << endl;
        cout << string(50, '-') << endl;
        
        cout << "RESERVATION MANAGEMENT OPTIONS:" << endl;
        cout << "   3. Update Reservation" << endl;
        cout << "   4. Cancel Reservation" << endl;
        cout << "   5. View Reservations" << endl;
        cout << string(50, '-') << endl;
        
        cout << "   0. Exit Staff Panel" << endl;
        cout << string(80, '=') << endl;
        cout << "Enter your choice (0-5): ";
        
        cin >> choice;
        cin.ignore(); // Clear input buffer

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number between 0-5." << endl;
            continue;
        }

      
        

        switch (choice) {
            case 1: {
                cout << "\n>>> ENTER GUEST DETAILS SELECTED <<<" << endl;
                receptionist rec;
                if (rec.enterGuestDetails(db)) {
                    cout << "Guest details entered successfully!" << endl;
                   
                } else {
                    cout << "Failed to enter guest details!" << endl;
                   
                }
                break;
            }
            
            case 2: {
                cout << "\n>>> CHECK ROOM AVAILABILITY SELECTED <<<" << endl;
                string check_in, check_out;
                cout << "Enter check-in date (YYYY-MM-DD) or press Enter for today: ";
                getline(cin, check_in);
                cout << "Enter check-out date (YYYY-MM-DD) or press Enter for tomorrow: ";
                getline(cin, check_out);
                
                receptionist rec;
                rec.checkRoomAvailability(db, check_in, check_out);
               
                break;
            }
            
            case 3: {
                cout << "\n>>> UPDATE RESERVATION SELECTED <<<" << endl;
                receptionist rec;
                if (rec.update_reservation(db)) {
                    cout << "Reservation updated successfully!" << endl;
                   
                } else {
                    cout << "Failed to update reservation!" << endl;
                   
                }
                break;
            }
            
            case 4: {
                cout << "\n>>> CANCEL RESERVATION SELECTED <<<" << endl;
                receptionist rec;
                if (rec.cancel_reservation(db)) {
                    cout << "Reservation cancelled successfully!" << endl;
                   
                } else {
                    cout << "Failed to cancel reservation!" << endl;
                   
                }
                break;
            }
            
            case 5: {
                cout << "\n>>> VIEW RESERVATIONS SELECTED <<<" << endl;
                receptionist rec;
                if (rec.view_reservations(db)) {
                    cout << "Reservations viewed successfully!" << endl;
                   
                } else {
                    cout << "Failed to view reservations!" << endl;
                   
                }
                break;
            }
            
            case 0: {
                cout << "\n>>> STAFF PANEL EXIT SELECTED <<<" << endl;
                cout << "\n" << string(80, '=') << endl;
                cout << "              STAFF SESSION ENDED" << endl;
                cout << string(80, '=') << endl;

                cout << string(80, '=') << endl;
                cout << "Thank you for using the Staff Control Panel!" << endl;
                return true;
            }
            
            default: {
                cout << "\n>>> INVALID CHOICE DETECTED <<<" << endl;
                cout << "ERROR: Invalid menu option selected" << endl;
                cout << "Valid options are: 0-5" << endl;
                cout << "Please try again." << endl;
                cout << "Error logged by: CrypticLuminary at 2025-07-01 06:45:24" << endl;
                break;
            }
        }
        
        // Pause before showing menu again
        cout << "\n" << string(60, '-') << endl;
        cout << "Press Enter to return to main staff menu...";
        cin.get();
    }
}


