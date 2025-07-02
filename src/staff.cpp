#include <iostream>
#include <string>
#include <conio.h>
#include <sqlite3.h>
#include "../include/database.h"
#include "../include/guest.h"
#include "../include/booking.h"
#include "../include/billing.h"
#include "../include/reports.h"
#include "../include/admin.h"
#include "../include/housekeeping.h"
#include "../include/receptionist.h"
#include "../include/validator.h"
#include "../include/user.h"
#include "../include/staff.h"
using namespace std;

// Fix function signature - use reference parameters (non-const)
bool staffmanager::loginStaff(Database& db, string& loggedUsername, string& loggedRole) {
    receptionist r;
    string username, password;
    
system("cls");
cout << "\n";
cout << "================================================================================\n";
cout << "||                                                                            ||\n";
cout << "||   SSSS  TTTTT   AAA   FFFFF  FFFFF     L       OOO    GGG   III  N   N   ||\n";
cout << "||   S       T    A   A  F      F         L      O   O  G   G   I   NN  N   ||\n";
cout << "||   SSSS    T    AAAAA  FFFF   FFFF      L      O   O  G       I   N N N   ||\n";
cout << "||      S    T    A   A  F      F         L      O   O  G   G   I   N  NN   ||\n";
cout << "||   SSSS    T    A   A  F      F         LLLLL   OOO    GGG   III  N   N   ||\n";
cout << "||                                                                            ||\n";
cout << "||                      RECEPTIONIST AUTHENTICATION                          ||\n";
cout << "||                                                                            ||\n";
cout << "================================================================================\n";

cout << "\n";
cout << "********************************************************************************\n";
cout << "*                            SECURE LOGIN PORTAL                              *\n";
cout << "********************************************************************************\n";
cout << "\n";

// Username Input Section
cout << "+------------------------------------------------------------------------------+\n";
cout << "|                              USERNAME ENTRY                                 |\n";
cout << "+------------------------------------------------------------------------------+\n";
cout << "| Username: ";
cin.ignore();
getline(cin, username);
cout << "+------------------------------------------------------------------------------+\n";

cout << "\n";

// Password Input Section  
cout << "+------------------------------------------------------------------------------+\n";
cout << "|                              PASSWORD ENTRY                                 |\n";
cout << "+------------------------------------------------------------------------------+\n";
cout << "| Password: ";
getline(cin, password);
cout << "+------------------------------------------------------------------------------+\n";

cout << "\n";

// Validation Check
if (username.empty() || password.empty()) {
    cout << "********************************************************************************\n";
    cout << "*                                  ERROR!                                      *\n";
    cout << "*                                                                              *\n";
    cout << "*    X   X     EEEEE  RRRR   RRRR    OOO   RRRR      !!                     *\n";
    cout << "*     X X      E      R   R  R   R  O   O  R   R     !!                     *\n";
    cout << "*      X       EEEE   RRRR   RRRR   O   O  RRRR      !!                     *\n";
    cout << "*     X X      E      R R    R R    O   O  R R       !!                     *\n";
    cout << "*    X   X     EEEEE  R  R   R  R    OOO   R  R      !!                     *\n";
    cout << "*                                                                              *\n";
    cout << "*                    USERNAME AND PASSWORD ARE REQUIRED!                      *\n";
    cout << "*                                                                              *\n";
    cout << "*                         >> Please try again <<                              *\n";
    cout << "*                                                                              *\n";
    cout << "********************************************************************************\n";
    cout << "\n>> Returning to login screen...\n\n";
    return false;
}

// Success Message

cout << "********************************************************************************\n";
cout << "*                            LOGIN SUCCESSFUL!                                 *\n";
cout << "********************************************************************************\n";
cout << "\n>> Welcome, " << username << "!\n";
cout << ">> Access granted to Reception Portal\n\n";
    
    // Check credentials
    const char* loginSQL = "SELECT staff_id, username, role, status FROM Staff "
                          "WHERE username = ? AND password = ?;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), loginSQL, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        cout << " Invalid username or password!" << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
    int staffId = sqlite3_column_int(stmt, 0);
    // Fix: Assign to reference parameters correctly
    loggedUsername = (char*)sqlite3_column_text(stmt, 1);
    loggedRole = (char*)sqlite3_column_text(stmt, 2);
    string status = (char*)sqlite3_column_text(stmt, 3);
    
    sqlite3_finalize(stmt);
    
    if (status != "active") {
        cout << " Account is inactive!" << endl;
        return false;
    }
    
    // Update last login
    const char* updateSQL = "UPDATE Staff SET last_login = DATE('NOW') WHERE staff_id = ?;";
    rc = sqlite3_prepare_v2(db.getDb(), updateSQL, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, staffId);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    cout << " Login successful!" << endl;
    cout << "Welcome " << loggedUsername << " (" << loggedRole << ")" << endl;
    cout << "\n\n";
    r.staffPower(db);
    
    return true;
}

bool staffmanager::insertStaff(Database& db) {
    Validate validator;
    string username, password, email, role;
    
    cout << "\n=== ADD NEW STAFF ===" << endl;
    cout << "Current Time: 2025-07-01 04:54:27" << endl;
    cout << "Current User: CrypticLuminary" << endl;
    
    // Get username
    cout << "Username: ";
    getline(cin, username);
    if (username.empty()) {
        cout << " Username cannot be empty!" << endl;
        return false;
    }
    
    // Get email using validator
    cout << "Email: ";
    getline(cin, email);
    if (!validator.isValidEmail(email)) {
        cout << " Invalid email format!" << endl;
        return false;
    }
    
    // Get password
    cout << "Password: ";
    getline(cin, password);
    if (password.length() < 6) {
        cout << " Password must be at least 6 characters!" << endl;
        return false;
    }
    
    // Get role
    cout << "Role (admin/manager/receptionist/staff): ";
    getline(cin, role);
    if (role != "admin" && role != "manager" && role != "receptionist" && role != "staff") {
        cout << " Invalid role!" << endl;
        return false;
    }
    
    // Check if username exists
    const char* checkSQL = "SELECT username FROM Staff WHERE username = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db.getDb(), checkSQL, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << " Username already exists!" << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    
    // Insert staff
    const char* insertSQL = "INSERT INTO Staff (username, password, email, role, status, created_at, created_by) "
                           "VALUES (?, ?, ?, ?, 'active', '2025-07-01 04:54:27', 'CrypticLuminary');";
    
    int rc = sqlite3_prepare_v2(db.getDb(), insertSQL, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, role.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << " Staff created successfully!" << endl;
        cout << "Staff ID: " << sqlite3_last_insert_rowid(db.getDb()) << endl;
        cout << "Created by: CrypticLuminary at 2025-07-01 04:54:27" << endl;
        return true;
    } else {
        cout << " Failed to create staff!" << endl;
        return false;
    }
}

bool staffmanager::updateStaff(Database& db) {
    Validate validator;
    int staffId;
    string newEmail, newRole;
    
    cout << "\n=== UPDATE STAFF ===" << endl;
    cout << "Current Time: 2025-07-01 04:54:27" << endl;
    cout << "Current User: CrypticLuminary" << endl;
    
    // Get staff ID
    cout << "Enter Staff ID: ";
    cin >> staffId;
    cin.ignore();
    
    // Check if staff exists
    const char* checkSQL = "SELECT username FROM Staff WHERE staff_id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db.getDb(), checkSQL, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, staffId);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        cout << " Staff ID not found!" << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    
    // Get new email
    cout << "New Email: ";
    getline(cin, newEmail);
    if (!validator.isValidEmail(newEmail)) {
        cout << " Invalid email format!" << endl;
        return false;
    }
    
    // Get new role
    cout << "New Role (admin/manager/receptionist/staff): ";
    getline(cin, newRole);
    if (newRole != "admin" && newRole != "manager" && newRole != "receptionist" && newRole != "staff") {
        cout << " Invalid role!" << endl;
        return false;
    }
    
    // Update staff
    const char* updateSQL = "UPDATE Staff SET email = ?, role = ?, updated_at = '2025-07-01 04:54:27', "
                           "updated_by = 'CrypticLuminary' WHERE staff_id = ?;";
    
    int rc = sqlite3_prepare_v2(db.getDb(), updateSQL, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, newEmail.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, newRole.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, staffId);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << " Staff updated successfully!" << endl;
        cout << "Updated by: CrypticLuminary at 2025-07-01 04:54:27" << endl;
        return true;
    } else {
        cout << " Failed to update staff!" << endl;
        return false;
    }
}

bool staffmanager::deleteStaff(Database& db) {
    int staffId;
    char confirm;
    
    cout << "\n=== DELETE STAFF ===" << endl;
    cout << "Current Time: 2025-07-01 04:54:27" << endl;
    cout << "Current User: CrypticLuminary" << endl;
    
    // Get staff ID
    cout << "Enter Staff ID: ";
    cin >> staffId;
    cin.ignore();
    
    // Check if staff exists
    const char* checkSQL = "SELECT username, role FROM Staff WHERE staff_id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db.getDb(), checkSQL, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, staffId);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        cout << " Staff ID not found!" << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
    string username = (char*)sqlite3_column_text(stmt, 0);
    string role = (char*)sqlite3_column_text(stmt, 1);
    sqlite3_finalize(stmt);
    
    // Don't delete admin
    if (role == "admin") {
        cout << " Cannot delete admin users!" << endl;
        return false;
    }
    
    // Confirm deletion
    cout << "Delete staff: " << username << " (" << role << ")? (y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (confirm != 'y' && confirm != 'Y') {
        cout << " Deletion cancelled!" << endl;
        return false;
    }
    
    // Delete staff
    const char* deleteSQL = "DELETE FROM Staff WHERE staff_id = ?;";
    
    int rc = sqlite3_prepare_v2(db.getDb(), deleteSQL, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, staffId);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << " Staff deleted successfully!" << endl;
        cout << "Deleted by: CrypticLuminary at 2025-07-01 04:54:27" << endl;
        return true;
    } else {
        cout << " Failed to delete staff!" << endl;
        return false;
    }
}

void staffmanager::viewAllStaff(Database& db) {
    cout << "\n=== ALL STAFF ===" << endl;
    cout << "\n\n";
    
    const char* sql = "SELECT staff_id, username, email, role, status FROM staff ORDER BY staff_id;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cout << " Error preparing statement!" << endl;
        return;
    }
    
    cout << "\nID\tUsername\t\tEmail\t\t\tRole\t\tStatus" << endl;
    cout << "----------------------------------------------------------------" << endl;
    
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        count++;
        cout << sqlite3_column_int(stmt, 0) << "\t"
             << sqlite3_column_text(stmt, 1) << "\t\t"
             << sqlite3_column_text(stmt, 2) << "\t\t"
             << sqlite3_column_text(stmt, 3) << "\t\t"
             << sqlite3_column_text(stmt, 4) << endl;
    }
    
    sqlite3_finalize(stmt);
    
    cout << "----------------------------------------------------------------" << endl;
    cout << "Total staff: " << count << endl;
}

bool staffmanager::changePassword(Database& db, const string& username) {
    string currentPassword, newPassword;
    
    cout << "\n=== CHANGE PASSWORD ===" << endl;
    cout << "Current Time: 2025-07-01 04:54:27" << endl;
    cout << "Current User: CrypticLuminary" << endl;
    cout << "User: " << username << endl;
    
    cout << "Current Password: ";
    getline(cin, currentPassword);
    
    // Verify current password
    const char* verifySQL = "SELECT staff_id FROM Staff WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDb(), verifySQL, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, currentPassword.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        cout << " Current password incorrect!" << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
    int staffId = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    
    // Get new password
    cout << "New Password: ";
    getline(cin, newPassword);
    
    if (newPassword.length() < 6) {
        cout << " Password must be at least 6 characters!" << endl;
        return false;
    }
    
    if (newPassword == currentPassword) {
        cout << " New password must be different!" << endl;
        return false;
    }
    
    // Update password
    const char* updateSQL = "UPDATE Staff SET password = ?, updated_at = '2025-07-01 04:54:27', "
                           "updated_by = ? WHERE staff_id = ?;";
    
    rc = sqlite3_prepare_v2(db.getDb(), updateSQL, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, staffId);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << " Password changed successfully!" << endl;
        cout << "Changed by: " << username << " at 2025-07-01 04:54:27" << endl;
        return true;
    } else {
        cout << " Failed to change password!" << endl;
        return false;
    }
}

void staffmanager::staffMenu(Database& db) {
    string loggedUsername, loggedRole;
    
    if (!loginStaff(db, loggedUsername, loggedRole)) {
        cout << "Login failed!" << endl;
        return;
    }
    
    int choice;
    while (true) {
        cout << "\n=== STAFF MANAGEMENT ===" << endl;
        cout << "Time: 2025-07-01 04:54:27" << endl;
        cout << "User: " << loggedUsername << " (" << loggedRole << ")" << endl;
        cout << "1. View All Staff" << endl;
        cout << "2. Add Staff" << endl;
        cout << "3. Update Staff" << endl;
        cout << "4. Delete Staff" << endl;
        cout << "5. Change Password" << endl;
        cout << "0. Logout" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                viewAllStaff(db);
                break;
            case 2:
                if (loggedRole == "admin" || loggedRole == "manager") {
                    insertStaff(db);
                } else {
                    cout << " Access denied!" << endl;
                }
                break;
            case 3:
                if (loggedRole == "admin" || loggedRole == "manager") {
                    updateStaff(db);
                } else {
                    cout << " Access denied!" << endl;
                }
                break;
            case 4:
                if (loggedRole == "admin") {
                    deleteStaff(db);
                } else {
                    cout << " Access denied!" << endl;
                }
                break;
            case 5:
                changePassword(db, loggedUsername);
                break;
            case 0:
                cout << " Logged out by CrypticLuminary at 2025-07-01 04:54:27!" << endl;
                return;
            default:
                cout << " Invalid choice!" << endl;
        }
    }
}