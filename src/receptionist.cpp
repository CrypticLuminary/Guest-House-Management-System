#include <iostream>
#include <conio.h>
#include "../include/receptionist.h"
#include "../include/database.h"
#include "../include/validator.h"

#include <string.h>
#include <iomanip>      
#include <regex>          
#include <sstream> 
using namespace std;
Validate a;
int ch, room;




void receptionist::checkRoomAvailability(Database& db, const string& check_in_date, const string& check_out_date) {
    cout << "\n=== ROOM AVAILABILITY CHECK ===" << endl;
    cout << string(90, '=') << endl;
    
    // Query to get all rooms with their status and price
    const char* sql = "SELECT rd.room_id, rd.room_no, rd.room_type, rd.status, rd.price_per_night "
                      "FROM RoomDetails rd "
                      "ORDER BY rd.room_type, rd.room_no;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cout << "Error preparing query: " << sqlite3_errmsg(db.getDb()) << endl;
        return;
    }
    
    // Vectors to store available and occupied rooms
    vector<tuple<int, int, string, double, string>> availableRooms;
    vector<tuple<int, int, string, double, string>> occupiedRooms;
    
    int totalRooms = 0;
    
    // Collect room data
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        totalRooms++;
        
        int roomId = sqlite3_column_int(stmt, 0);
        int roomNo = sqlite3_column_int(stmt, 1);
        const char* roomType = (const char*)sqlite3_column_text(stmt, 2);
        const char* status = (const char*)sqlite3_column_text(stmt, 3);
        double price = sqlite3_column_double(stmt, 4);
        
        string roomTypeStr = roomType ? roomType : "N/A";
        string statusStr = status ? status : "N/A";
        
        // Get facilities for this room
        string facilities = getRoomFacilities(db, roomId);
        
        // Categorize rooms
        if (statusStr == "available" || statusStr == "Available") {
            availableRooms.push_back(make_tuple(roomId, roomNo, roomTypeStr, price, facilities));
        } else {
            occupiedRooms.push_back(make_tuple(roomId, roomNo, roomTypeStr, price, facilities));
        }
    }
    
    sqlite3_finalize(stmt);
    
    // Display Available Rooms with Facilities
    cout << "\nAVAILABLE ROOMS:" << endl;
    cout << string(120, '-') << endl;
    cout << left << setw(8) << "Room ID" 
         << setw(8) << "Room #" 
         << setw(15) << "Room Type"
         << setw(12) << "Price/Night"
         << setw(50) << "Facilities" << endl;
    cout << string(120, '-') << endl;
    
    if (availableRooms.empty()) {
        cout << "No available rooms found." << endl;
    } else {
        for (const auto& room : availableRooms) {
            cout << left << setw(8) << get<0>(room)
                 << setw(8) << get<1>(room)
                 << setw(15) << get<2>(room)
                 << setw(12) << fixed << setprecision(2) << "$" << get<3>(room)
                 << setw(50) << get<4>(room) << endl;
        }
    }
    
    cout << string(120, '-') << endl;
    cout << "Total Available Rooms: " << availableRooms.size() << endl;
    
    // Display room type summary with common facilities
    displayRoomTypeSummary(db);
    
    // Rest of your existing code for occupied rooms...
}

// Helper function to get facilities for a specific room
string receptionist::getRoomFacilities(Database& db, int room_id) {
    const char* sql = "SELECT facility_name FROM RoomFacilities WHERE room_id = ? AND availability = 'Available' ORDER BY facility_name;";
    sqlite3_stmt* stmt;
    string facilities = "";
    
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!facilities.empty()) facilities += ", ";
            facilities += (char*)sqlite3_column_text(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    
    return facilities.empty() ? "No facilities" : facilities;
}

// Function to display room type summary with facilities
void receptionist::displayRoomTypeSummary(Database& db) {
    cout << "\n=== ROOM TYPES & COMMON FACILITIES ===" << endl;
    
    const char* sql = R"(
        SELECT rd.room_type, 
               GROUP_CONCAT(DISTINCT rf.facility_name) as facilities,
               COUNT(DISTINCT rd.room_id) as total_rooms,
               AVG(rd.price_per_night) as avg_price
        FROM RoomDetails rd
        LEFT JOIN RoomFacilities rf ON rd.room_id = rf.room_id AND rf.availability = 'Available'
        GROUP BY rd.room_type
        ORDER BY rd.room_type;
    )";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK) {
        cout << string(100, '-') << endl;
        cout << left << setw(15) << "Room Type" 
             << setw(10) << "Rooms"
             << setw(12) << "Avg Price"
             << setw(50) << "Common Facilities" << endl;
        cout << string(100, '-') << endl;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string roomType = (char*)sqlite3_column_text(stmt, 0);
            string facilities = sqlite3_column_text(stmt, 1) ? (char*)sqlite3_column_text(stmt, 1) : "None";
            int totalRooms = sqlite3_column_int(stmt, 2);
            double avgPrice = sqlite3_column_double(stmt, 3);
            
            cout << left << setw(15) << roomType
                 << setw(10) << totalRooms
                 << setw(12) << fixed << setprecision(2) << "$" << avgPrice
                 << setw(50) << facilities << endl;
        }
        cout << string(100, '-') << endl;
    }
    sqlite3_finalize(stmt);
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
    receptionist r;
    int stay_duration;
    string fname, lname, contact_info, email, id_proof, relationship, address;
    r.checkRoomAvailability(db);
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
        Validate v;
        cout << "Contact Number: ";
        getline(cin, contact_info);
        if (contact_info.empty()) {
            cout << " Contact number cannot be empty! Please try again." << endl;
        } else if (!v.isValidNumber(contact_info)) {
            cout << " Invalid phone number format! Please try again." << endl;
            contact_info = ""; // Reset to retry
        }
    } while (contact_info.empty());
    
    // Email
    do {
        Validate v;
        cout << "Email: ";
        getline(cin, email);
        if (email.empty()) {
            cout << " Email cannot be empty! Please try again." << endl;
        } else if (!v.isValidEmail(email)) {
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

    cout << "Stay Duration: ";
    cin >> stay_duration;

    // do {
    // if (stay_duration.empty()) {
    //     cout << " Stay Duration cannot be empty! Please try again." << endl;
    // }
    // } while (stay_duration.empty());
  
    cout << "\n========== CONFIRMATION ==========" << endl;
    cout << "Please confirm the following details:" << endl;
    cout << "Name: " << fname << " " << lname << endl;
    cout << "Contact: " << contact_info << endl;
    cout << "Email: " << email << endl;
    cout << "ID Proof: " << id_proof << endl;
    cout << "Relationship: " << relationship << endl;
    cout << "Address: " << address << endl;
    cout << "Stay Duration: " << stay_duration << endl;
    
    char confirm;
    cout << "\nIs this information correct? (y/n): ";
    cin >> confirm;
    cin.ignore(); 
    
    if (confirm == 'y' || confirm == 'Y') {
       
        int guest_id = db.insertGuest(fname, lname, contact_info, email, id_proof, relationship, address);
        
        if (guest_id > 0) {
            cout << " Guest registered successfully!" << endl;
            cout << "Guest ID: " << guest_id << endl;
          
            int room_id;
            cout << "\n\n";

            cout << setw(15) << " Enter the room_id of available room " << endl;
            cin >> room_id;
            db.reservation(guest_id,room_id,0,"reserved",stay_duration);
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
            receptionist rec;
    cout << "\n========== CANCEL RESERVATION ==========" << endl;
            rec.view_reservations(db);
            int ID;
            cout << "  Enter the ID of reservation for checkout " << endl;
            cin >> ID;
            db.deleteReservation(ID);
    return ID;
}


int receptionist::view_reservations(Database& db) {
 
    
    cout << "\n========== VIEW RESERVATIONS ==========" << endl;
   
    
    string sql;
    string searchTerm = "";
    
        sql = "SELECT r.reservation_id, r.check_in_date, "
                "COALESCE(r.check_out_date, 'N/A') as check_out_date, "
                "g.first_name, g.last_name, g.contact_info, rd.room_no, rd.room_type, "
                "r.booking_status, r.booking_id "
                "FROM Reservations r "
                "INNER JOIN Guests g ON r.guest_id = g.guest_id "
                "INNER JOIN RoomDetails rd ON r.room_id = rd.room_id "
                "ORDER BY r.reservation_id DESC;";
       
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Error preparing statement: " << sqlite3_errmsg(db.getDb()) << endl;
        cout << "Error logged by: CrypticLuminary at 2025-07-01 09:26:53" << endl;
        return -1;
    }
    
    
        string searchPattern = "%" + searchTerm + "%";
        sqlite3_bind_text(stmt, 1, searchPattern.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, searchPattern.c_str(), -1, SQLITE_STATIC);
    
    
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
    

       

    cout << endl;
    cout << string(120, '-') << endl;
    
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        count++;
        
        cout << left << setw(5) << sqlite3_column_int(stmt, 0)  // reservation_id
             << setw(12) << sqlite3_column_text(stmt, 1)        // check_in_date
             << setw(12) << sqlite3_column_text(stmt, 2)        // check_out_date
             << setw(20) << (string((char*)sqlite3_column_text(stmt, 3)) + " " + 
                           string((char*)sqlite3_column_text(stmt, 4)))  // first_name + last_name
             << setw(15) << sqlite3_column_text(stmt, 5)        // contact_info
             << setw(8) << sqlite3_column_int(stmt, 6)          // room_no
             << setw(12) << sqlite3_column_text(stmt, 7)        // room_type
             << setw(12) << sqlite3_column_text(stmt, 8)        // booking_status
             << setw(10) << sqlite3_column_int(stmt, 9);        // booking_id
      
    }
    
    sqlite3_finalize(stmt);
    
    cout << string(120, '=') << endl;

    
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
        system("CLS");

        cout << "                    STAFF MANAGEMENT SYSTEM" << endl;
        cout << string(80, '*') << endl;
        
        cout << string(80, '-') << endl;
        
        cout << "GUEST MANAGEMENT OPTIONS:" << endl;
        cout << "   1. Enter Guest Details" << endl;
        cout << "   2. Checkout" << endl;
        cout << "   3. Update Guest Details" << endl;
        cout << string(50, '-') << endl;
        
        cout << "ROOM MANAGEMENT OPTIONS:" << endl;
        cout << "   4. Check Room Availability" << endl;
        cout << string(50, '-') << endl;
        
        cout << "RESERVATION MANAGEMENT OPTIONS:" << endl;
        cout << "   5. Update Reservation" << endl;
        cout << "   6. Cancel Reservation" << endl;
        cout << "   7. View Reservations" << endl;
        cout << string(50, '-') << endl;

        cout << "BILLING MANAGEMENT OPTIONS:" << endl;
        cout << "   8. View Guest Billing" << endl;
        cout << "   0. Exit Staff Panel" << endl;
        cout << string(80, '=') << endl;
        cout << "Enter your choice (0-7): ";
        
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
                receptionist rec;
                
                cout << "\n>>> CHECK OUT SELECTED SELECTED <<<" << endl;
               
                cout << "\n";

                rec.cancel_reservation(db);
                
                break;
            }

             case 3: {
                receptionist rec;
                
                cout << "\n>>> UPDATE GUEST DETAILS SELECTED SELECTED <<<" << endl;
               
                cout << "\n";

                updateGuestDetail(db);
                
                break;
            }
            
            case 4: {
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
            
            case 5: {
                cout << "\n>>> UPDATE RESERVATION SELECTED <<<" << endl;
                receptionist rec;
                if (rec.update_reservation(db)) {
                    cout << "Reservation updated successfully!" << endl;
                   
                } else {
                    cout << "Failed to update reservation!" << endl;
                   
                }
                break;
            }
            
            case 6: {
                cout << "\n>>> CANCEL RESERVATION SELECTED <<<" << endl;
                receptionist rec;
                if (rec.cancel_reservation(db)) {
                    cout << "Reservation cancelled successfully!" << endl;
                   
                } else {
                    cout << "Failed to cancel reservation!" << endl;
                   
                }
                break;
            }
            
            case 7: {
                cout << "\n>>> VIEW RESERVATIONS SELECTED <<<" << endl;
                receptionist rec;
                if (rec.view_reservations(db)) {
                    cout << "Reservations viewed successfully!" << endl;
                   
                } else {
                    cout << "Failed to view reservations!" << endl;
                   
                }
                break;
            }

            case 8: {
                cout << "\n>>> VIEW GUEST BILL SELECTED <<<" << endl;
                db.printGuestBill();
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

bool receptionist::updateGuestDetail(Database &db) {
    db.printGuests();
    cout << "\n--- UPDATE GUEST DETAIL ---" << endl;
    int guest_id;
    cout << "Enter Guest ID to update: ";
    cin >> guest_id;
    cin.ignore();

    // Check if guest exists
    const char* checkSql = "SELECT first_name, last_name FROM Guests WHERE guest_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkSql, -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(checkStmt, 1, guest_id);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "No guest found with ID: " << guest_id << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    string old_fname = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 0));
    string old_lname = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 1));
    sqlite3_finalize(checkStmt);

    cout << "Updating guest: " << old_fname << " " << old_lname << endl;

    // Get updated details
    string fname, lname, contact_info, email, id_proof, relationship, address;
    cout << "Enter new First Name (leave blank to keep '" << old_fname << "'): ";
    getline(cin, fname);
    if (fname.empty()) fname = old_fname;

    cout << "Enter new Last Name (leave blank to keep '" << old_lname << "'): ";
    getline(cin, lname);
    if (lname.empty()) lname = old_lname;

    cout << "Enter new Contact Info: "; getline(cin, contact_info);
    cout << "Enter new Email: "; getline(cin, email);
    cout << "Enter new ID Proof: "; getline(cin, id_proof);
    cout << "Enter new Relationship: "; getline(cin, relationship);
    cout << "Enter new Address: "; getline(cin, address);

    const char* sql = "UPDATE Guests SET first_name = ?, last_name = ?, contact_info = ?, email = ?, id_proof = ?, relationship = ?, address = ? WHERE guest_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, fname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contact_info.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, id_proof.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, relationship.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, guest_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        cout << "Guest details updated successfully!" << endl;
        return true;
    } else {
        cerr << "Failed to update guest: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
}

// Delete Guest Detail
bool receptionist::deleteGuestDetail(Database &db) {
    
    cout << "\n--- DELETE GUEST DETAIL ---" << endl;
    int guest_id;
    cout << "Enter Guest ID to delete: ";
    cin >> guest_id;
    cin.ignore();

    // Optionally confirm deletion
    cout << "Are you sure you want to delete guest ID " << guest_id << "? (y/n): ";
    char confirm;
    cin >> confirm; cin.ignore();
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Delete operation cancelled." << endl;
        return false;
    }

    // Check if guest exists
    const char* checkSql = "SELECT fname, lname FROM Guests WHERE guest_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkSql, -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(checkStmt, 1, guest_id);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "No guest found with ID: " << guest_id << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    string fname = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 0));
    string lname = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 1));
    sqlite3_finalize(checkStmt);

    const char* sql = "DELETE FROM Guests WHERE guest_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, guest_id);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        cout << "Guest '" << fname << " " << lname << "' deleted successfully!" << endl;
        return true;
    } else {
        cerr << "Failed to delete guest: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
}

