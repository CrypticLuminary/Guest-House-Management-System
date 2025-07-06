#include <conio.h>
#include <sqlite3.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "../include/database.h"


#include "../include/admin.h"

#include "../include/receptionist.h"
#include "../include/validator.h"
#include "../include/user.h"
#include "../include/staff.h"
staffmanager s;
using namespace std;

// string getHiddenPassword() {
//     string password;
//     char ch;
//     while ((ch = _getch()) != '\r') { // Enter key
//         if (ch == '\b') { // Backspace
//             if (!password.empty()) {
//                 password.pop_back();
//                 cout << "\b \b";
//             }
//         } else if (ch == 3) { // Ctrl+C
//             exit(0);
//         } else {
//             password += ch;
//             cout << '*'; // Show * for each character
//         }
//     }
//     cout << endl;
//     return password;
// }


void roomManagementMenu(Database& db);

//____________________MAIN PART OF ADMIN_______________________________
bool Admin::admin(Database& db) {
Admin admin;
Validate v;

    if (!admin.hasAdmin(db)) {
        std::cout << "No admin found. Please create an admin account." << std::endl;
        std::string username, email, password;

        std::cout << "Enter Admin Username: ";
        std::getline(std::cin, username);

        std::cout << "Enter Admin Email: ";
        std::getline(std::cin, email);

        std::cout << "creedential issue in guest.h"
        << "Enter Admin Password: ";
        password = v.getpassword();

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
    Validate v;
    bool loggedIn = false;
    int attempts = 0;
    const int maxAttempts = 3;

    while (!loggedIn && attempts < maxAttempts) {
        std::string username, password;
        std::cout << "\nAdmin Login" << std::endl;
        std::cout << "Enter Username: ";
        std::getline(std::cin, username);
        std::cout << "Enter Password: ";
        password = v.getpassword();

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


bool Admin::updateRoomDetails(Database &db) {
    int room_id;
    string room_type, price_per_night, status;
    int room_no;
    
    cout << "\n--- UPDATE ROOM ---" << endl;
    
    // Show all rooms first
    db.printRoomDetails();
    
    // Get Room ID
    cout << "Enter Room ID to update: ";
    cin >> room_id;
    cin.ignore();
    
    // Verify room exists
    const char* checkSQL = "SELECT room_no, room_type, price_per_night, status FROM RoomDetails WHERE room_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkSQL, -1, &checkStmt, nullptr);
    sqlite3_bind_int(checkStmt, 1, room_id);
    
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "Room ID not found!" << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    
    // Display current room info
    cout << "\nCurrent Details:" << endl;
    cout << "Room Number: " << sqlite3_column_int(checkStmt, 0) << endl;
    cout << "Room Type: " << sqlite3_column_text(checkStmt, 1) << endl;
    cout << "Price: $" << sqlite3_column_double(checkStmt, 2) << endl;
    cout << "Status: " << sqlite3_column_text(checkStmt, 3) << endl;
    sqlite3_finalize(checkStmt);
    
    cout << "\nEnter new values (leave empty to keep current):" << endl;
    
    // Get updated information
    cout << "Room Number: ";
    string room_no_str;
    getline(cin, room_no_str);
    room_no = room_no_str.empty() ? 0 : stoi(room_no_str);
    
    cout << "Room Type: ";
    getline(cin, room_type);
    
    cout << "Price per Night: ";
    getline(cin, price_per_night);
    
    cout << "Status: ";
    getline(cin, status);
    
    // Build update query
    string sql = "UPDATE RoomDetails SET ";
    vector<string> updates;
    
    if (room_no != 0) updates.push_back("room_no = " + to_string(room_no));
    if (!room_type.empty()) updates.push_back("room_type = '" + room_type + "'");
    if (!price_per_night.empty()) updates.push_back("price_per_night = " + price_per_night);
    if (!status.empty()) updates.push_back("status = '" + status + "'");
    
    if (updates.empty()) {
        cout << "No changes made." << endl;
        return false;
    }
    
    // Complete SQL
    for (size_t i = 0; i < updates.size(); ++i) {
        sql += updates[i];
        if (i < updates.size() - 1) sql += ", ";
    }
    sql += " WHERE room_id = " + to_string(room_id) + ";";
    
    // Execute update
    char* errMsg = nullptr;
    rc = sqlite3_exec(db.getDb(), sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc == SQLITE_OK) {
        cout << "Room updated successfully!" << endl;
        return true;
    } else {
        cout << "Update failed: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
}

//______________________DISPLAY PART OF ADMIN__________________________



// Room Management Submenu
void roomManagementMenu(Database& db) {
    int choice;
   Admin a; 
    while (true) {
        cout << "\n=== ROOM MANAGEMENT ===" << endl;
        cout << "Admin: CrypticLuminary | Time: 2025-07-01 05:13:40" << endl;
        cout << "1. Add New Room" << endl;
        cout << "2. View All Rooms" << endl;
        cout << "3. Update Room Details" << endl;
        cout << "4. Delete Room" << endl;
        // cout << "5. Room Status Management" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: {
                int roomNo;
                string roomType;
                double price;

                cout << "\n=== ADD NEW ROOM ===" << endl;
                cout << "Current Date and Time (UTC - YYYY-MM-DD HH:MM:SS formatted): 2025-07-01 05:25:49" << endl;
                cout << "Current User's Login: CrypticLuminary" << endl;
                
                int room_no;
                string room_type, price_per_night, status;
                
                cout << "Enter room number: ";
                cin >> room_no;
                cin.ignore();
                
                cout << "Enter room type: ";
                getline(cin, room_type);
                
                cout << "Enter price per night: ";
                getline(cin, price_per_night);
                
                cout << "Enter status: ";
                getline(cin, status);
                
                db.insertRoomDetails(room_no, room_type, price_per_night, status);
            }
            case 2:
                cout << "\n--- ALL ROOMS ---" << endl;
                db.printRoomDetails();
                cout << "Report generated by: CrypticLuminary at 2025-07-01 05:13:40" << endl;
                break;
                
            case 3:
                cout << "\n--- UPDATE ROOM ---" << endl;
                a.updateRoomDetails(db);
                break;
                
            case 4:
                cout << "\n--- DELETE ROOM ---" << endl;
                db.deleteRoom(db);
                break;
                
            // case 5:
            //     cout << "\n--- ROOM STATUS MANAGEMENT ---" << endl;
            //     // manageRoomStatus(db);
            //     break;
                
            case 0:
                return;
                
            default:
                cout << " Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
}

// Staff Management Submenu
void staffManagementMenu(Database& db) {
    staffmanager staffMgr;
    int choice;
    
    while (true) {
        cout << "\n=== STAFF MANAGEMENT ===" << endl;
        cout << "\n\n";
        cout << "1. View All Staff" << endl;
        cout << "2. Add New Staff" << endl;
        cout << "3. Update Staff Details" << endl;
        cout << "4. Delete Staff Member" << endl;
        // cout << "5. Staff Performance Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                cout << "\n--- ALL STAFF MEMBERS ---" << endl;
                staffMgr.viewAllStaff(db);
                cout << "Report generated by: CrypticLuminary at 2025-07-01 05:13:40" << endl;
                break;
                
            case 2:
                cout << "\n--- ADD NEW STAFF ---" << endl;
                if (staffMgr.insertStaff(db)) {
                    cout << "Staff member added by: CrypticLuminary at 2025-07-01 05:13:40" << endl;
                }
                break;
                
            case 3:
                cout << "\n--- UPDATE STAFF ---" << endl;
                if (staffMgr.updateStaff(db)) {
                    cout << "Staff updated by: CrypticLuminary at 2025-07-01 05:13:40" << endl;
                }
                break;
                
            case 4:
                cout << "\n--- DELETE STAFF ---" << endl;
                if (staffMgr.deleteStaff(db)) {
                    cout << "Staff deletion by: CrypticLuminary at 2025-07-01 05:13:40" << endl;
                }
                break;
                
            // case 5:
            //     cout << "\n--- STAFF PERFORMANCE REPORT ---" << endl;
            //     // generateStaffReport(db);
            //     break;
                
            case 0:
                return;
                
            default:
                cout << " Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
}

// Guest Management Submenu
void guestManagementMenu(Database& db) {
    receptionist rec;
    int choice;
    
    while (true) {
        cout << "\n=== GUEST MANAGEMENT ===" << endl;
        cout << "1. View All Guests" << endl;
        cout << "2. Add New Guest" << endl;
        cout << "3. Update Guest Details" << endl;
        cout << "4. Delete guest" << endl;
        // cout << "4. Guest History Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                cout << "\n--- ALL GUESTS ---" << endl;
                db.printGuests();
                break;
                
            case 2:
                cout << "\n--- ADD NEW GUEST ---" << endl;
                rec.enterGuestDetails(db);
              
                break;
                
            case 3:
                cout << "\n--- UPDATE GUEST ---" << endl;
                rec.updateGuestDetail(db);
                break;

            case 4:
                cout << "\n--- UPDATE GUEST ---" << endl;
                db.deleteGuest(db);
                break;
                
            // case 4:
            //     cout << "\n--- GUEST HISTORY REPORT ---" << endl;
            //     // generateGuestReport(db);
            //     break;
                
            case 0:
                return;
                
            default:
                cout << " Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
}


void bookingManagementMenu(Database& db) {
    while (true) {
        cout << "\n===== Booking Management Menu =====" << endl;
        cout << "1. View All Bookings" << endl;
        cout << "2. Update Booking" << endl;
        cout << "3. Delete Booking" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                db.viewBookings();
                break;
            case 2:
                db.updateBooking();
                break;
            case 3:
                db.deleteBooking();
                break;
            case 0:
                cout << "Returning to Main Menu...\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

void adminManagementMenu(Database& db) {
    Admin admin;
    while (true) {
        cout << "\n===== Admin Management Menu =====" << endl;
        cout << "1. Add Admin" << endl;
        cout << "2. Update Admin" << endl;
        cout << "3. View All Admins" << endl;
        cout << "4. Delete Admin" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                admin.addAdmin(db);
                break;
            case 2:
                admin.updateAdmin(db);
                break;
            case 3:
                admin.viewAdmins(db);
                break;
            case 4:
                admin.deleteAdmin(db);
                break;
            case 0:
                cout << "Returning to Main Menu...\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}


// System Reports Menu
void systemReportsMenu(Database& db) {
    int choice;
    
    while (true) {
        cout << "\n=== SYSTEM REPORTS ===" << endl;
        cout << "1. Daily Revenue Report" << endl;
        cout << "2. Monthly Summary" << endl;
        cout << "3. Room Occupancy Report" << endl;
        cout << "4. Staff Activity Report" << endl;
        cout << "5. Guest Statistics" << endl;
        cout << "6. System Usage Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                cout << "\n--- DAILY REVENUE REPORT ---" << endl;
                // generateDailyRevenueReport(db);
                break;
                
            case 2:
                cout << "\n--- MONTHLY SUMMARY ---" << endl;
                // generateMonthlySummary(db);
                break;
                
            case 3:
                cout << "\n--- ROOM OCCUPANCY REPORT ---" << endl;
                // generateOccupancyReport(db);
                break;
                
            case 4:
                cout << "\n--- STAFF ACTIVITY REPORT ---" << endl;
                // generateStaffActivityReport(db);
                break;
                
            case 5:
                cout << "\n--- GUEST STATISTICS ---" << endl;
                // generateGuestStatistics(db);
                break;
                
            case 6:
                cout << "\n--- SYSTEM USAGE REPORT ---" << endl;
                // generateSystemUsageReport(db);
                break;
                
            case 0:
                return;
                
            default:
                cout << " Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
}

// Change Admin Password
void changeAdminPassword(Database& db) {
    string currentPassword, newPassword, confirmPassword, email;
    
    cout << "\n";
    cout << "================================================================================\n";
    cout << "||                                                                            ||\n";
    cout << "||     CCC   H   H   AAA   N   N   GGG   EEEEE     PPPP    AAA    SSSS      ||\n";
    cout << "||    C   C  H   H  A   A  NN  N  G   G  E         P   P  A   A  S          ||\n";
    cout << "||    C      HHHHH  AAAAA  N N N  G      EEEE      PPPP   AAAAA  SSSS       ||\n";
    cout << "||    C   C  H   H  A   A  N  NN  G   G  E         P      A   A     S       ||\n";
    cout << "||     CCC   H   H  A   A  N   N   GGG   EEEEE     P      A   A  SSSS       ||\n";
    cout << "||                                                                            ||\n";
    cout << "||                        ADMIN PASSWORD CHANGE PORTAL                       ||\n";
    cout << "||                                                                            ||\n";
    cout << "================================================================================\n";
    cout << "\n";
    
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "|                              EMAIL VERIFICATION                             |\n";
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "| Enter admin EMAIL: ";
    getline(cin, email);
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "\n";
    
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "|                           CURRENT PASSWORD                                  |\n";
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "| Enter current password: ";
    getline(cin, currentPassword);
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "\n";
    
    // Verify current password - FIXED SQL
    const char* verifySQL = "SELECT admin_id FROM Admin WHERE email = ? AND password = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), verifySQL, -1, &stmt, nullptr);
    
    // FIXED: Bind both email and password parameters
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, currentPassword.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        cout << "********************************************************************************\n";
        cout << "*                                  ERROR!                                      *\n";
        cout << "*                                                                              *\n";
        cout << "*                        CURRENT PASSWORD INCORRECT!                          *\n";
        cout << "*                                                                              *\n";
        cout << "*                    Please check your email and password                     *\n";
        cout << "*                                                                              *\n";
        cout << "********************************************************************************\n";
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);
    
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "|                              NEW PASSWORD                                    |\n";
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "| Enter new password: ";
    getline(cin, newPassword);
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "\n";
    
    if (newPassword.length() < 3) {  // Your development setting
        cout << "********************************************************************************\n";
        cout << "*                                  ERROR!                                      *\n";
        cout << "*                                                                              *\n";
        cout << "*                   PASSWORD MUST BE AT LEAST 3 CHARACTERS!                  *\n";
        cout << "*                                                                              *\n";
        cout << "********************************************************************************\n";
        return;
    }
    
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "|                           CONFIRM NEW PASSWORD                              |\n";
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "| Confirm new password: ";
    getline(cin, confirmPassword);
    cout << "+------------------------------------------------------------------------------+\n";
    cout << "\n";
    
    if (newPassword != confirmPassword) {
        cout << "********************************************************************************\n";
        cout << "*                                  ERROR!                                      *\n";
        cout << "*                                                                              *\n";
        cout << "*                         PASSWORDS DO NOT MATCH!                            *\n";
        cout << "*                                                                              *\n";
        cout << "*                           Please try again                                  *\n";
        cout << "*                                                                              *\n";
        cout << "********************************************************************************\n";
        return;
    }
    
    cout << ">> Updating password in database...\n";
    
    // Update password - FIXED SQL
    const char* updateSQL = "UPDATE Admin SET password = ? WHERE email = ?;";
    
    rc = sqlite3_prepare_v2(db.getDb(), updateSQL, -1, &stmt, nullptr);
    
    // FIXED: Bind both parameters
    sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << "********************************************************************************\n";
        cout << "*                                SUCCESS!                                      *\n";
        cout << "*                                                                              *\n";
        cout << "*                      ADMIN PASSWORD CHANGED SUCCESSFULLY!                   *\n";
        cout << "*                                                                              *\n";
        cout << "*                         Your new password is now active                     *\n";
        cout << "*                                                                              *\n";
        cout << "********************************************************************************\n";
    } else {
        cout << "********************************************************************************\n";
        cout << "*                                  ERROR!                                      *\n";
        cout << "*                                                                              *\n";
        cout << "*                        FAILED TO CHANGE PASSWORD!                          *\n";
        cout << "*                                                                              *\n";
        cout << "*                    Database error code: " << rc << "                                    *\n";
        cout << "*                                                                              *\n";
        cout << "********************************************************************************\n";
    }
}
bool Admin::adminPower(Database &db) {
    int choice;
    system("CLS");
    cout << "\n" << string(100, '=') << endl;
    cout << "                           ADMIN CONTROL PANEL" << endl;
    cout << string(100, '=') << endl;
    cout << string(100, '=') << endl;

    while (true) {
        cout << "\n" << string(80, '*') << endl;
        cout << "                    ADMIN MANAGEMENT SYSTEM" << endl;

        cout << string(80, '-') << endl;
        
        cout << " HOTEL MANAGEMENT OPTIONS:" << endl;
        cout << "   1. Room Management" << endl;
        cout << "   2. Guest Management" << endl;
        cout << "   3. Booking Management" << endl;
        cout << string(50, '-') << endl;
        
        cout << "👥 STAFF MANAGEMENT OPTIONS:" << endl;
        cout << "   4. Staff Management" << endl;
        cout << "   5. Admin Management" << endl;
        cout << "   6. GRMS reports" << endl;
        cout << string(50, '-') << endl;
        
        // cout << " SYSTEM MANAGEMENT OPTIONS:" << endl;
        // cout << "   7. View System Reports" << endl;
        // cout << "   8. Database Management" << endl;
        // cout << "   9. System Settings" << endl;
        // cout << string(50, '-') << endl;
        
        cout << " ADMIN TOOLS:" << endl;
        // cout << "  10. Backup Database" << endl;
        // cout << "  11. View Activity Logs" << endl;
        cout << "  7. Change Admin Password" << endl;
        cout << string(50, '-') << endl;
        
        cout << "   0. Exit Admin Panel" << endl;
        cout << string(80, '=') << endl;
        cout << "Enter your choice (0-7): ";
        
        cin >> choice;
        cin.ignore(); // Clear input buffer

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << " Invalid input. Please enter a number between 0-12." << endl;
            continue;
        }

        cout << "\nAction initiated by: CrypticLuminary at 2025-07-01 05:13:40" << endl;

        switch (choice) {
            case 1: {
                cout << "\n>>> ROOM MANAGEMENT SELECTED <<<" << endl;
                roomManagementMenu(db);
                break;
            }
            
            case 2: {
                cout << "\n>>> GUEST MANAGEMENT SELECTED <<<" << endl;
                guestManagementMenu(db);
                break;
            }
            
            case 3: {
                cout << "\n>>> BOOKING MANAGEMENT SELECTED <<<" << endl;
                bookingManagementMenu(db);
                break;
            }
            
            case 4: {
                cout << "\n>>> STAFF MANAGEMENT SELECTED <<<" << endl;
                staffManagementMenu(db);
                break;
            }
            
            case 5: {
                cout << "\n>>> ADMIN MANAGEMENT SELECTED <<<" << endl;
                adminManagementMenu(db);
                break;
            }
            
            // case 6: {
            //     cout << "\n>>> USER ACCESS CONTROL SELECTED <<<" << endl;
            //     // userAccessControlMenu(db);
            //     break;
            // }
            
            case 6: {
                cout << "\n>>> GRMS REPORTS SELECTED <<<" << endl;
                db.generateRevenueReport();
                break;
            }
            
            // case 8: {
            //     cout << "\n>>> DATABASE MANAGEMENT SELECTED <<<" << endl;
            //     // databaseManagementMenu(db);
            //     break;
            // }
            
            // case 9: {
            //     cout << "\n>>> SYSTEM SETTINGS SELECTED <<<" << endl;
            //     // systemSettingsMenu(db);
            //     break;
            // }
            
            // case 10: {
            //     cout << "\n>>> DATABASE BACKUP SELECTED <<<" << endl;
            //     // backupDatabase(db);
            //     break;
            // }
            
            // case 11: {
            //     cout << "\n>>> ACTIVITY LOGS SELECTED <<<" << endl;
            //     // viewActivityLogs(db);
            //     break;
            // }
            
            case 7: {
                cout << "\n>>> CHANGE ADMIN PASSWORD SELECTED <<<" << endl;
                changeAdminPassword(db);
                break;
            }
            
            case 0: {
                cout << "\n>>> ADMIN PANEL EXIT SELECTED <<<" << endl;
                cout << "\n" << string(80, '=') << endl;
                cout << "              ADMIN SESSION ENDED" << endl;
                cout << string(80, '=') << endl;

     
                cout << string(80, '=') << endl;
                cout << " Thank you for using the Admin Control Panel!" << endl;
                return true;
            }
            
            default: {
                cout << "\n>>> INVALID CHOICE DETECTED <<<" << endl;
                cout << " ERROR: Invalid menu option selected" << endl;
                cout << "Valid options are: 0-7" << endl;
                cout << "Please try again." << endl;
        
                break;
            }
        }
        
        // Pause before showing menu again
        cout << "\n" << string(60, '-') << endl;
        cout << "Press Enter to return to main admin menu...";
        cin.get();
    }
}


//__________________________________ADMIN FUNCTIONALITY_____________________________________

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



// string getCurrentDateTime() {
//     time_t now = time(0);
//     tm* t = localtime(&now);
//     char buf[20];
//     strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
//     return string(buf);
// }



bool Admin::addAdmin(Database& db) {
    string email, password;
    cout << "\n--- ADD ADMIN ---" << endl;
    cout << "Enter Username: ";
    getline(cin, username);
    cout << "Enter Email: ";
    getline(cin, email);
    cout << "Enter Password: ";
    getline(cin, password);

    const char* sql = "INSERT INTO Admin (username, email, password) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        cout << "Admin added successfully!\n";
        return true;
    } else {
        cerr << "Failed to add admin: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
}


bool Admin::updateAdmin(Database& db) {
    Admin admin;
    admin.viewAdmins(db);
    cout << "\n--- UPDATE ADMIN ---" << endl;
    int aid;
    cout << "Enter Admin ID to update: ";
    cin >> aid; cin.ignore();

    // Check if exists and fetch current info
    const char* checkSql = "SELECT username, email FROM Admin WHERE admin_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkSql, -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(checkStmt, 1, aid);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "No admin found with ID: " << aid << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    string old_username = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 0));
    string old_email = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 1));
    sqlite3_finalize(checkStmt);

    cout << "Updating admin: " << old_username << " (" << old_email << ")" << endl;
    cout << "Enter new Username (leave blank to keep '" << old_username << "'): ";
    string new_username; getline(cin, new_username);
    if (new_username.empty()) new_username = old_username;
    cout << "Enter new Email (leave blank to keep '" << old_email << "'): ";
    string new_email; getline(cin, new_email);
    if (new_email.empty()) new_email = old_email;
    cout << "Enter new Password (leave blank to keep current): ";
    string new_password; getline(cin, new_password);

    string sql;
    if (new_password.empty()) {
        sql = "UPDATE Admin SET username = ?, email = ? WHERE admin_id = ?;";
    } else {
        sql = "UPDATE Admin SET username = ?, email = ?, password = ? WHERE admin_id = ?;";
    }
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db.getDb(), sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, new_username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, new_email.c_str(), -1, SQLITE_STATIC);
    if (new_password.empty()) {
        sqlite3_bind_int(stmt, 3, aid);
    } else {
        sqlite3_bind_text(stmt, 3, new_password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, aid);
    }

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        cout << "Admin updated successfully!\n";
        return true;
    } else {
        cerr << "Failed to update admin: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
}


void Admin::viewAdmins(Database& db) {
    cout << "\n--- VIEW ADMINS ---" << endl;
    const char* sql = "SELECT admin_id, username, email FROM Admin ORDER BY admin_id;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return;
    }
    cout << left << setw(4) << "ID" << setw(18) << "Username"
         << setw(30) << "Email" << endl;
    cout << string(52, '-') << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string uname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string mail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        cout << left << setw(4) << id << setw(18) << uname << setw(30) << mail << endl;
    }
    sqlite3_finalize(stmt);
}

bool Admin::deleteAdmin(Database& db) {
    Admin admin;
    admin.viewAdmins(db);
    cout << "\n--- DELETE ADMIN ---" << endl;
    int aid;
    cout << "Enter Admin ID to delete: ";
    cin >> aid; cin.ignore();

    // Check if exists
    const char* checkSql = "SELECT username, email FROM Admin WHERE admin_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db.getDb(), checkSql, -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(checkStmt, 1, aid);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "No admin found with ID: " << aid << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    string uname = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 0));
    string mail = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 1));
    sqlite3_finalize(checkStmt);

    cout << "Are you sure you want to delete admin '" << uname << " (" << mail << ")'? (y/n): ";
    char confirm;
    cin >> confirm; cin.ignore();
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Delete operation cancelled.\n";
        return false;
    }

    const char* sql = "DELETE FROM Admin WHERE admin_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, aid);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        cout << "Admin deleted successfully!\n";
        return true;
    } else {
        cerr << "Failed to delete admin: " << sqlite3_errmsg(db.getDb()) << endl;
        return false;
    }
}

