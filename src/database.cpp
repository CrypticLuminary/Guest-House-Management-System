#include "../include/database.h"
#include "../include/sqlite3.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include<string>
#include <vector>
using namespace std;

// Constructor
Database::Database(const std::string& dbFile) : db(nullptr), dbFile(dbFile) {}

// Destructor
Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

// Open the database
bool Database::open() {
    int rc = sqlite3_open(dbFile.c_str(), &db);
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::cout << "Database opened successfully!" << std::endl;
    return true;
}

// Close the database
void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        cout << "Database closed." << std::endl;
    }
}

// ##_____________________________________CREATING TABLE__________________________________________________##

bool Database::createTable() {
    sqlite3_busy_timeout(db, 5000);
    // Create the Guests table
    const char* sqlGuest = "CREATE TABLE IF NOT EXISTS Guests ("
                        "guest_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "first_name VARCHAR(100) NOT NULL, "
                        "last_name VARCHAR(100) NOT NULL, "
                        "contact_info TEXT UNIQUE NOT NULL, "
                        "email TEXT NOT NULL UNIQUE, "
                        "id_proof TEXT  NULL, "
                        "relationship TEXT CHECK(relationship IN('single','married')), "
                        "address TEXT NULL);";

    //create the room_details table
    const char* RoomDetails = "CREATE TABLE IF NOT EXISTS RoomDetails ("
                            "room_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "room_no INTEGER UNIQUE NOT NULL ,"
                            "room_type TEXT NOT NULL,"
                            "price_per_night REAL  NULL, "
                            "status TEXT  CHECK(status IN('Available','Occupied','Under Maintainance Reserved')) DEFAULT 'Available');";
    // Facilities
    const char* sqlRoomFacilities = "CREATE TABLE IF NOT EXISTS RoomFacilities ("
                                "facility_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "room_id INTEGER REFERENCES RoomDetails(room_id) ON DELETE CASCADE, "
                                "facility_name TEXT NOT NULL, "
                                "availability TEXT CHECK(availability IN ('Available', 'Not Available', 'Under Maintenance')) DEFAULT 'Available', "
                                "description TEXT NULL, "
                                "created_at TEXT DEFAULT (datetime('now')), "
                                "updated_at TEXT DEFAULT (datetime('now')));";

    // ADMIN TABLE
    const char* sqlAdmin = "CREATE TABLE IF NOT EXISTS Admin("
                           "admin_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "username TEXT UNIQUE NOT NULL, "
                           "email TEXT NOT NULL UNIQUE, "
                           "password TEXT NOT NULL); " ;
                           

//STAFF TABLE
const char* createStaffTable = 
                            "CREATE TABLE IF NOT EXISTS Staff ("
                            "staff_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "username TEXT UNIQUE NOT NULL, "
                            "password TEXT NOT NULL, "
                            "email TEXT UNIQUE NOT NULL, "
                            "role TEXT NOT NULL CHECK(role IN ('admin', 'manager', 'receptionist', 'staff')), "
                            "status TEXT DEFAULT 'active' CHECK(status IN ('active', 'inactive')), "
                            "created_at TEXT NOT NULL, "
                            "created_by TEXT NOT NULL, "
                            "updated_at TEXT, "
                            "updated_by TEXT, "
                            "last_login TEXT"
                            ");";

    const char* sqlStaffDetails = "CREATE TABLE IF NOT EXISTS StaffDetails ("
                        "staffinfo_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "staff_id INTEGER REFERENCES Staff(staff_id) ON DELETE CASCADE, "
                        "first_name VARCHAR(100) NOT NULL, "
                        "last_name VARCHAR(100) NOT NULL, "
                        "contact_info TEXT UNIQUE NOT NULL, "
                        "email TEXT NOT NULL UNIQUE, "
                        "relationship TEXT CHECK(relationship IN('single','married')), "
                        "id_proof TEXT  NULL, "
                        "address TEXT NULL);";

    //BOOKING TABLE
    const char* sqlbooking = "CREATE TABLE IF NOT EXISTS Booking("
                            "booking_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "guest_id INTEGER REFERENCES Guests(guest_id) ON DELETE CASCADE, "
                            "room_id INTEGER REFERENCES RoomDetails(room_id) ON DELETE CASCADE, "
                            "check_in_date DATE DEFAULT (DATE('now')), "
                            "check_out_date DATE NULL, "
                            "total_amount REAL"
                            "stay_duration INTEGER, "
                            "booking_status TEXT CHECK(booking_status IN ('confirmed', 'cancelled', 'checked out')) DEFAULT 'confirmed');";


    //PAYMENT TABLE
    const char* sqlpayment = "CREATE TABLE IF NOT EXISTS Payment("
                            "payment_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "booking_id INTEGER UNIQUE, "
                            "total_bill DECIMAL(10,2) NOT NULL,"
                            "amount_paid REAL NOT NULL, "
                            "balance_due INTEGER NOT NULL,"
                            "payment_date DATE DEFAULT (DATE('now')), "
                            "payment_method TEXT CHECK(payment_method IN ('Cash', 'Card', 'Online')), "
                            "FOREIGN KEY (booking_id) REFERENCES Booking(booking_id) ON DELETE CASCADE);";                        

    
    //MAINTENENCE TABLE
    const char* sqlmaintenence = "CREATE TABLE IF NOT EXISTS Maintenence("
                                "maintenence_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "room_id INTEGER REFERENCES RoomDetails(room_id), "
                                "issue_description TEXT NOT NULL, "
                                "request_date DATE DEFAULT (DATE('now')), "
                                "status TEXT CHECK(status IN('Pending','In Progress', 'Resolved')) );";



    //HOUSEKEEPING TABLE
    const char* sqlhousekeeping = "CREATE TABLE IF NOT EXISTS Housekeeping("
                                "housekeeping_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "room_id INTEGER REFERENCES RoomDetails(room_id), "
                                "last_cleaned DATE NOT NULL, "
                                "next_schedule DATE NOT NULL, "
                                "status TEXT CHECK(status IN('Pending', 'Completed')) DEFAULT'Pending' );";

    

    //GUEST EXPENSESS
    const char* sqlguestexpenses = "CREATE TABLE IF NOT EXISTS Guestexpenses("
                                "expense_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "booking_id INTEGER REFERENCES Booking(booking_id) ON DELETE CASCADE, "
                                "guest_id INTEGER REFERENCES Guests(guest_id) ON DELETE CASCADE, "
                                "expense_on TEXT NOT NULL, "
                                "quantity INTEGER DEFAULT 1, "
                                "unit_price DECIMAL NOT NULL, "
                                "sub_total DECIMAL NOT NULL, "
                                "tax_percent DECIMAL, "
                                "tax_amt DECIMAL );";

    //RESERVATION
    const char* sqlreservation = "CREATE TABLE IF NOT EXISTS Reservations("
                            "reservation_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "check_in_date DATE DEFAULT (DATE('now')), "
                            "check_out_date DATE NULL, "
                            "guest_id INTEGER REFERENCES Guests(guest_id) ON DELETE CASCADE, "
                            "room_id INTEGER UNIQUE REFERENCES RoomDetails(room_id) ON DELETE CASCADE, "
                            "booking_id INTEGER, "
                            "stay_duration INTEGER, "
                            "booking_status TEXT CHECK(booking_status IN ('reserved', 'onboard','cancelled')) DEFAULT 'reserved', "
                            "FOREIGN KEY (booking_id) REFERENCES Booking(booking_id));";
                                
    


    //guest
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sqlGuest, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating guest table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    //room_details
    rc = sqlite3_exec(db, RoomDetails, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK ) {
        cerr << "Error creatinng room table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    //ROOM FACILITIES
    rc = sqlite3_exec(db, sqlRoomFacilities, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating room facilities table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    //Admin
    rc = sqlite3_exec(db, sqlAdmin, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK) {
        cerr << "Error creating admin table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    //Staff
    rc = sqlite3_exec(db, createStaffTable, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK) {
        cerr << "Error creating staff table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    //SatffDetails
    rc = sqlite3_exec(db, sqlStaffDetails, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK) {
        cerr << "Error creating staff details table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    //MAINTENENCE
    rc = sqlite3_exec(db, sqlmaintenence, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating maintenence table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    //PAYMENT
    rc = sqlite3_exec(db, sqlpayment, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating payment table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    // HOUSE_KEEPING 
    rc = sqlite3_exec(db, sqlhousekeeping, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating housekeeping table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

   //BOOKING
    rc = sqlite3_exec(db, sqlbooking, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating booking table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    //RESERVATION
    rc = sqlite3_exec(db, sqlreservation, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating reservation table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    //GUEST_EXPENSES
    rc = sqlite3_exec(db, sqlguestexpenses, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating guest_expenses table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    cout << "Table created successfully!" << std::endl;
    return true;
}



//   ##_________________________________CREATING TRIGGERS POINT_______________________________________________________##

bool Database::createTriggrs() {
    // 1. Auto-create booking when reservation is inserted
    const char* trigger1 = "CREATE TRIGGER IF NOT EXISTS auto_create_booking "
                          "AFTER INSERT ON Reservations "
                          "FOR EACH ROW "
                          "WHEN NEW.booking_id IS NULL "
                          "BEGIN "
                              "INSERT INTO Booking (guest_id, room_id, check_in_date, check_out_date, booking_status) "
                              "VALUES (NEW.guest_id, NEW.room_id, NEW.check_in_date, NEW.check_out_date, 'confirmed'); "
                              
                              "UPDATE Reservations "
                              "SET booking_id = last_insert_rowid() "
                              "WHERE reservation_id = NEW.reservation_id; "
                          "END;";

    // 2. Sync dates when reservation is updated
    const char* trigger2 = "CREATE TRIGGER IF NOT EXISTS sync_booking_on_reservation_update "
                          "AFTER UPDATE OF check_in_date, check_out_date ON Reservations "
                          "FOR EACH ROW "
                          "BEGIN "
                              "UPDATE Booking "
                              "SET check_in_date = NEW.check_in_date, "
                                  "check_out_date = NEW.check_out_date "
                              "WHERE booking_id = NEW.booking_id; "
                          "END;";

    // 3. Update booking on checkout (reservation delete)
    const char* trigger3 = "CREATE TRIGGER IF NOT EXISTS checkout_booking "
                          "BEFORE DELETE ON Reservations "
                          "FOR EACH ROW "
                          "BEGIN "
                              "UPDATE Booking "
                              "SET check_out_date = DATE('now'), "
                                  "booking_status = 'checked out' "
                              "WHERE booking_id = OLD.booking_id; "
                          "END;";
// 4. Trigger 4
    const char* trigger4 = "CREATE TRIGGER IF NOT EXISTS update_room_to_occupied_booking "
                                "AFTER INSERT ON Booking "
                                "FOR EACH ROW "
                                "WHEN NEW.booking_status = 'confirmed' "
                                "BEGIN "
                                    "UPDATE RoomDetails "
                                    "SET status = 'Occupied' "
                                    "WHERE room_id = NEW.room_id; "
                                "END;";
    
    // Trigger 5: Set room to available when reservation is cancelled
    const char* trigger5 = "CREATE TRIGGER IF NOT EXISTS update_room_to_available_on_cancel "
                          "AFTER UPDATE ON Reservations "
                          "FOR EACH ROW "
                          "WHEN NEW.booking_status = 'cancelled' AND OLD.booking_status != 'cancelled' "
                          "BEGIN "
                              "UPDATE RoomDetails "
                              "SET status = 'Available' "
                              "WHERE room_id = NEW.room_id; "
                          "END;";
    
    // Trigger 6: Set room to available when reservation is deleted
    const char* trigger6 = "CREATE TRIGGER IF NOT EXISTS update_room_to_available_on_delete "
                          "AFTER DELETE ON Reservations "
                          "FOR EACH ROW "
                          "BEGIN "
                              "UPDATE RoomDetails "
                              "SET status = 'Available' "
                              "WHERE room_id = OLD.room_id; "
                          "END;";

      const char* trigger7 = R"SQL(
        CREATE TRIGGER IF NOT EXISTS set_total_amount_after_checkout
        AFTER UPDATE OF booking_status ON Booking
        FOR EACH ROW
        WHEN NEW.booking_status = 'checked out'
        BEGIN
            UPDATE Booking
            SET total_amount = (
                (julianday(NEW.check_out_date) - julianday(NEW.check_in_date)) * 
                (SELECT price_per_night FROM RoomDetails WHERE room_id = NEW.room_id)
            )
            WHERE booking_id = NEW.booking_id;
        END;
    )SQL";

    const char* trigger8 = R"SQL(
    CREATE TRIGGER IF NOT EXISTS set_checkout_on_stay_duration
    AFTER UPDATE OF stay_duration ON Booking
    FOR EACH ROW
    WHEN NEW.stay_duration IS NOT NULL AND NEW.stay_duration > 0
    BEGIN
        UPDATE Booking
        SET check_out_date = DATE(NEW.check_in_date, '+' || NEW.stay_duration || ' days')
        WHERE booking_id = NEW.booking_id;
    END;
    )SQL";

    const char* trigger9 = R"SQL(
    CREATE TRIGGER IF NOT EXISTS insert_booking_on_reservation
    AFTER INSERT ON Reservation
    FOR EACH ROW
    WHEN NEW.stay_duration IS NOT NULL AND NEW.stay_duration > 0
    BEGIN
        INSERT INTO Booking (guest_id, room_id, check_in_date, stay_duration, booking_status)
        VALUES (NEW.guest_id, NEW.room_id, NEW.check_in_date, NEW.stay_duration, 'confirmed');
    END;
    )SQL";


    char* errMsg = nullptr;
    
    // Execute all triggers
    sqlite3_exec(db, trigger1, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, trigger2, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, trigger3, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, trigger4, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, trigger5, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, trigger6, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, trigger7, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, trigger8, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, trigger9, nullptr, nullptr, &errMsg);
    
    
    cout << "All triggers created successfully!" << endl;
    return true;
}



//   ##_________________________________INSERTING DATA INTO THE TABLE_________________________________________________##

//ROOM_DETAILS
 bool Database::insertRoomDetails(const int& room_no, const string& room_type, const string& price_per_night, const string& status) {
    const char* RoomDetails = "INSERT INTO RoomDetails (room_no, room_type, price_per_night, status) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, RoomDetails, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, room_no);
    sqlite3_bind_text(stmt, 2, room_type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, price_per_night.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, status.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "Room_Details inserted successfully!" << std::endl;
    sqlite3_finalize(stmt);
    return true;
}

// GUEST_DETAILS
int Database::insertGuest(const string& fname, const string& lname, const string& contact_info, const string& email, const string& id_proof, const string& relationship, const string& address) {
    const char* sqlGuest = "INSERT INTO Guests (first_name, last_name, contact_info, email, id_proof, relationship, address) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlGuest, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, fname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contact_info.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, id_proof.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, relationship.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, address.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "Guest inserted successfully!" << endl;
    sqlite3_finalize(stmt);
    return sqlite3_last_insert_rowid(db);
}

// STAFF_DETAILS
bool Database::insertStaffDetails(const string& fname, const string& lname, const string& contact_info, const string& email, const string& id_proof, const string& relationship, const string& address) {
    const char* sqlStaffDetails = "INSERT INTO StaffDetails (first_name, last_name, contact_info, email, id_proof, address) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlStaffDetails, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, fname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contact_info.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, id_proof.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, relationship.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, address.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "Staff details inserted successfully!" << endl;
    sqlite3_finalize(stmt);
    return true;
}


//ADMIN
bool Database::insertAdmin(const string& username, const string& password, const string& email) {
    const char* sqlAdmin = "INSERT INTO Admin (username, email, password) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlAdmin, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);
   


    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "Admin inserted successfully!" << endl;
    sqlite3_finalize(stmt);
    return true;
}

// STAFF
bool Database::insertStaff(const string& username, const string& password, const string& email, const string& role) {
    const char* sqlStaff = "INSERT INTO Staff (username, email, password, role) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlStaff   , -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, role.c_str(), -1, SQLITE_STATIC);


    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "Staff details inserted successfully!" << endl;
    sqlite3_finalize(stmt);
    return true;
}

//BOOKING
bool Database::booking (int guest_id, int room_id, const string& booking_status) {
    const char* sqlInsertBooking = "INSERT INTO Booking (guest_id, room_id, booking_status) "
    "VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlInsertBooking, -1, &stmt, nullptr);
   
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, guest_id);
    sqlite3_bind_int(stmt, 2, room_id);
    sqlite3_bind_text(stmt, 3, booking_status.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
    std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
    std::cout << "Booking inserted successfully!" << std::endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    return true;
}

//RESERVATION
// Option 1: Pass 0 for booking_id, let trigger handle it

// Option 2: Modify reservation function to auto-generate booking_id
bool Database::reservation(int guest_id, int room_id, int booking_id, const string& booking_status, string &stay_duration) {
    // Let the trigger create the booking automatically
    const char* sqlInsertReservation = "INSERT INTO Reservations (guest_id, room_id, booking_status,stay_duration) "
                                      "VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlInsertReservation, -1, &stmt, nullptr);
   
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, guest_id);
    sqlite3_bind_int(stmt, 2, room_id);
    sqlite3_bind_text(stmt, 3, booking_status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, stay_duration.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    std::cout << "Reservation created and booking auto-generated!" << std::endl;
    sqlite3_finalize(stmt);
    return true;
}



// ##_________________________________________________UPDATING DATA ON THE TABLE______________________________________##

//ADMIN
bool Database::updateAdmin(int admin_id, const std::string& username, const std::string& email, const std::string& password) {
    std::string sql = "UPDATE Admin SET ";
    sqlite3_stmt* stmt;
    bool hasUpdate = false;

    // Add fields to update only if they are non-empty
    if (!username.empty()) {
        sql += "username = ?, ";
        hasUpdate = true;
    }

    if (!email.empty()) {
        sql += "email = ?, ";
        hasUpdate = true;
    }

    if (!password.empty()) {
        sql += "password = ?, ";
        hasUpdate = true;
    }

    // If no fields to update, exit early
    if (!hasUpdate) {
        std::cerr << "No fields to update." << std::endl;
        return false;
    }

    // Remove the trailing comma and space
    sql.pop_back(); // Remove the space
    sql.pop_back(); // Remove the comma

    // Add the WHERE clause to identify the admin by admin_id
    sql += " WHERE admin_id = ?;";

    // Prepare the statement
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters dynamically
    int paramIndex = 1;

    if (!username.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, username.c_str(), -1, SQLITE_STATIC);
    }

    if (!email.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, email.c_str(), -1, SQLITE_STATIC);
    }

    if (!password.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, password.c_str(), -1, SQLITE_STATIC);
    }

    // Bind the admin_id for the WHERE clause
    sqlite3_bind_int(stmt, paramIndex, admin_id);

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    std::cout << "Admin details updated successfully!" << std::endl;

    // Finalize the statement
    sqlite3_finalize(stmt);
    return true;
}


//       _____________________UPDATING GUEST DETAILS________________ 

bool Database::updateGuest(vector<int> guest_id, const string& fname, const string& lname, const string& contact_info, const string& email, const string& id_proof, const string& relationship, const string& address) {
    std::string sql = "UPDATE Guests SET ";
    sqlite3_stmt* stmt;
    bool hasUpdate = false;
    // Add fields to update only if they are non-empty
    if (!fname.empty()) {
        sql += "first_name = ?, ";
        hasUpdate = true;
    }

    if (!lname.empty()) {
        sql += "last_name = ?, ";
        hasUpdate = true;
    }

    if (!contact_info.empty()) {
        sql += "contact_info = ?, ";
        hasUpdate = true;
    }

    if (!email.empty()) {
        sql += "email = ?, ";
        hasUpdate = true;
    }

    if (!id_proof.empty()) {
        sql += "id_proof = ?, ";
        hasUpdate = true;
    }

    if (!relationship.empty()) {
        sql += "relationship = ?, ";
        hasUpdate = true;
    }
    
    if (!address.empty()) {
        sql += "address = ?, ";
        hasUpdate = true;
    }

    // If no fields to update, exit early
    if (!hasUpdate) {
        std::cerr << "No fields to update." << std::endl;
        return false;
    }

    // Remove the trailing comma and space
    sql.pop_back(); // Remove the space
    sql.pop_back(); // Remove the comma

    // Add the WHERE clause to identify the admin by admin_id
    sql += " WHERE guest_id = ?;";

    // Prepare the statement
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters dynamically
    int paramIndex = 1;

    if (!fname.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, fname.c_str(), -1, SQLITE_STATIC);
    }
    if (!lname.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, lname.c_str(), -1, SQLITE_STATIC);
    }
    if (!contact_info.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, contact_info.c_str(), -1, SQLITE_STATIC);
    }
    if (!email.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, email.c_str(), -1, SQLITE_STATIC);
    }
    if (!id_proof.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, id_proof.c_str(), -1, SQLITE_STATIC);
    }
    if (!relationship.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, relationship.c_str(), -1, SQLITE_STATIC);
    }
    if (!address.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, relationship.c_str(), -1, SQLITE_STATIC);
    }

    // Bind the admin_id for the WHERE clause
    for(int ID: guest_id) {
        sqlite3_bind_int(stmt, paramIndex,ID);
    }
   

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    std::cout << "Guest details updated successfully!" << std::endl;

    // Finalize the statement
    sqlite3_finalize(stmt);
    return true;
}

//##___________________________________________________SEARCHING DATA ON THE TABLE_____________________________________##

//searching guest info
vector<int> Database::searchGuest(const string&email, const string& contact_info, const string& check_in_date, const string& check_out_date, int room_no, const string& room_type)
{   

    vector <int> Guest_ID;
    string sql = "SELECT Guests.*, Booking.check_in_date, Booking.check_out_date, RoomDetails.room_no, RoomDetails.room_type "
                        "FROM Guests "
                        "JOIN Booking ON Guests.guest_id = Booking.guest_id "
                        "JOIN RoomDetails ON Booking.room_id = RoomDetails.room_id "
                        "WHERE 1=1  ";

    bool hasConditions = false;

    if (!email.empty()) {
        sql += " AND Guests.email = ?";
        hasConditions = true;
    }

    if(!contact_info.empty()) {
        sql += " AND Guests.contact_info = ?";
        hasConditions = true;
    }

    if(!check_in_date.empty()) {
        sql += " AND Booking.check_in_date = ?";
        hasConditions = true;
    }

    if(!check_out_date.empty()) {
        sql += " AND Booking.check_out_date = ?";
        hasConditions = true;
    }

    if (room_no != 0) {
        sql += " AND RoomDetails.room_no = ?";
        hasConditions = true;
    }

    if(!room_type.empty()) {
        sql += " AND RoomDetails.room_type = ?";
        hasConditions = true;
    }

    // if no conditions are provided to exixts early
    if(!hasConditions) {
        cerr << "No search criteria provided." << endl;
        
        return Guest_ID;
    }

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if(rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return Guest_ID;
    }


    //Binding parameters to prevent sql injection
    int paramIndex = 1;
    if(!email.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, email.c_str(), -1, SQLITE_STATIC);
    }

    if(!contact_info.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, contact_info.c_str(), -1, SQLITE_STATIC);
    }

    if (!check_in_date.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, check_in_date.c_str(), -1, SQLITE_STATIC);
    }
    if (!check_out_date.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, check_out_date.c_str(), -1, SQLITE_STATIC);
    }

    if (room_no != 0) {
        sqlite3_bind_int(stmt, paramIndex++, room_no);
    }

    if (!room_type.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, room_type.c_str(), -1,SQLITE_STATIC);
    }

    cout << "Search Results:" << endl;
    // std::cout << "Generated SQL Query: " << sql << std::endl;
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int guest_id = sqlite3_column_int(stmt, 0);
        const unsigned char* fname = sqlite3_column_text(stmt, 1);
        const unsigned char* lname = sqlite3_column_text(stmt, 2);
        const unsigned char* contact_info_col = sqlite3_column_text(stmt, 3);
        const unsigned char* email_col = sqlite3_column_text(stmt, 4);
        const unsigned char* id_proof = sqlite3_column_text(stmt, 5);
        const unsigned char* address = sqlite3_column_text(stmt, 6);
        const unsigned char* check_in_date_col = sqlite3_column_text(stmt, 7);
        // const unsigned char* check_out_date_col = sqlite3_column_text(stmt, 8);
        int room_no_col = sqlite3_column_int(stmt, 9);
        const unsigned char* room_type_col = sqlite3_column_text(stmt, 10);

        cout << "ID: " << guest_id
        << ", First Name: " << fname
        << ", Last Name: " << lname
        << ", Contact Info: " << contact_info_col
        << ", Email: " << email_col
        << ", ID Proof: " << id_proof
        << ", Address: " << address
        << ", Check-In Date: " << check_in_date_col
        // << ", Check-Out Date: " << check_out_date_col
        << ", Room No: " << room_no_col
        << ", Room Type: " << room_type_col << endl;
        Guest_ID.push_back(guest_id);
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }


    sqlite3_finalize(stmt);
    return Guest_ID;
}

//   __________________SEARCHING ROOM DETAILS__________________________________

void Database::searchRoom(const int& room_no, const std::string& room_type, const std::string& price_per_night, const std::string& status) {

    std::string sql = "SELECT * FROM RoomDetails WHERE 1=1"; // "1=1" is a placeholder to simplify appending conditions
    sqlite3_stmt* stmt;
    bool hasFilters = false;

    // Add filters dynamically based on provided parameters
    if (room_no > 0) { // Check if room_no is valid (greater than 0)
        sql += " AND room_no = ?";
        hasFilters = true;
    }
    if (!room_type.empty()) { // Check if room_type is provided
        sql += " AND room_type = ?";
        hasFilters = true;
    }
    if (!price_per_night.empty()) { // Check if price_per_night is provided
        sql += " AND price_per_night = ?";
        hasFilters = true;
    }
    if (!status.empty()) { // Check if status is provided
        sql += " AND status = ?";
        hasFilters = true;
    }

    // If no filters are provided, notify the user
    if (!hasFilters) {
        std::cout << "No search criteria provided. Returning all rooms." << std::endl;
    }

    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind parameters dynamically
    int paramIndex = 1;
    if (room_no > 0) {
        sqlite3_bind_int(stmt, paramIndex++, room_no);
    }
    if (!room_type.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, room_type.c_str(), -1, SQLITE_STATIC);
    }
    if (!price_per_night.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, price_per_night.c_str(), -1, SQLITE_STATIC);
    }
    if (!status.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, status.c_str(), -1, SQLITE_STATIC);
    }

    // Execute the query and print results
    std::cout << "Search Results:" << std::endl;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int room_id = sqlite3_column_int(stmt, 0);
        int room_number = sqlite3_column_int(stmt, 1);
        const unsigned char* roomType = sqlite3_column_text(stmt, 2);
        const unsigned char* pricePerNight = sqlite3_column_text(stmt, 3);
        const unsigned char* roomStatus = sqlite3_column_text(stmt, 4);

        std::cout << "ID: " << room_id
                  << ", Room Number: " << room_number
                  << ", Room Type: " << (roomType ? reinterpret_cast<const char*>(roomType) : "N/A")
                  << ", Price Per Night: " << (pricePerNight ? reinterpret_cast<const char*>(pricePerNight) : "N/A")
                  << ", Status: " << (roomStatus ? reinterpret_cast<const char*>(roomStatus) : "N/A") << std::endl;
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
}





// ##__________________________________________________PRINTING DATA FROM TABLE_________________________________________##


// _____________________________PRINTING ALL THE GUEST INFO_______________________
void Database::printGuests() {
    const char* guestsql = "SELECT * FROM Guests;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, guestsql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Guests:" << endl;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int guest_id = sqlite3_column_int(stmt, 0);
        const unsigned char* fname = sqlite3_column_text(stmt, 1);
        const unsigned char* lname = sqlite3_column_text(stmt, 2);
        const unsigned char* contact_info = sqlite3_column_text(stmt, 3);
        const unsigned char* email = sqlite3_column_text(stmt, 4);
        const unsigned char* id_proof = sqlite3_column_text(stmt, 5);
        const unsigned char* address = sqlite3_column_text(stmt, 6);

        cout << "ID: " << guest_id
                  << ", FIrst_Name: " << fname
                  << ", Last_Name: " << lname
                  << ", Email: " << email
                  << ", Contact: " << contact_info
                  << ", ID_Proof: " << id_proof
                  << ", Address: " << address << std::endl;
    }

    sqlite3_finalize(stmt);
}

//_________________________________PRINTING ALL THE ROOM DETAILS_______________________
void Database::printRoomDetails() {
    const char* roomDetails = "SELECT * FROM RoomDetails;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, roomDetails, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    cout << "Room_Details:" << endl;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int room_id = sqlite3_column_int(stmt, 0);
        int room_no = sqlite3_column_int(stmt, 1);
        const unsigned char* room_type = sqlite3_column_text(stmt, 2);
        double price = sqlite3_column_double(stmt, 3);
        const unsigned char* status = sqlite3_column_text(stmt, 4); //added the status and price samri le

        cout << "ID: " << room_id
             << ", Room Number: " << room_no
             << ", Room Type: " << room_type
             << ",Price:" <<price
             << ", Status: " << status << endl;
    }
    sqlite3_finalize(stmt);
}


//##___________________________________GETTING RELATED DATA______________________
int Database::getRoomID(int room_no) {
    string sql = " SELECT room_id FROM RoomDetails WHERE room_no = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return -1;
    }

    sqlite3_bind_int(stmt, 1, room_no);

    int room_id = -1;
    if((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        room_id = sqlite3_column_int(stmt, 0);
    }
    else {
        cerr << "No room found with room number: " << room_no << endl;
    }
    sqlite3_finalize(stmt);
    return room_id;
}


//__________________________________DELETE FUNCTIONALITY IN DATABASE____________________________________________

//GUEST
bool Database::deleteGuest(Database &db) {
        int guest_id;
        db.printGuests();
        cout << "Enter Guest ID :: "<< endl;
        cin >> guest_id;
        string sql  = "DELETE FROM Guests WHERE guest_id = ?;";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db.getDb(), sql.c_str(), -1, &stmt, nullptr);
        if(rc != SQLITE_OK) {
            cerr << "Failed to prepare statement (delete_guest): " << sqlite3_errmsg(db.getDb()) << endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, guest_id);
        rc = sqlite3_step(stmt);
        if(rc != SQLITE_DONE) {
            cerr << "Deletion of guest failed: " << sqlite3_errmsg(db.getDb()) << endl;
            sqlite3_finalize(stmt);
            return false;
        }
        cout << "Guest with ID " << guest_id << " has been deleted successfully!" << endl;
        sqlite3_finalize(stmt);
        return true;
    
}

//ROOM
bool Database::deleteRoom(Database &DB) {
    int room_id;
        DB.printGuests();
        cout << " Enter the room ID to be deleted :: " << endl;
        cin >> room_id;
        string sql  = "DELETE FROM RoomDetails WHERE id = ?;";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if(rc != SQLITE_OK) {
            cerr << "Failed to prepare statement (delete_room): " << sqlite3_errmsg(db) << endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, room_id);
        rc = sqlite3_step(stmt);
        if(rc != SQLITE_DONE) {
            cerr << "Deletion of room failed: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            return false;
        }
        cout << "Room with ID " << room_id << " has been deleted successfully!" << endl;
        sqlite3_finalize(stmt);
        return true;
    
}

//BOOKING
bool Database::deleteBooking(int booking_id) {
    const char* sqlDelete = "DELETE FROM Booking WHERE booking_id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

   
    sqlite3_bind_int(stmt, 1, booking_id);

    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "Booking with ID " << booking_id << " deleted successfully!" << endl;

   
    sqlite3_finalize(stmt);
    return true;
}

//RESERVATION
bool Database::deleteReservation(int reservation_id) {
    const char* sqlDelete = "DELETE FROM Reservations WHERE reservation_id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

   
    sqlite3_bind_int(stmt, 1, reservation_id);

    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "Reservation with ID " << reservation_id << " deleted successfully!" << endl;
    cout << "Booking table updated with checkout date automatically!" << endl;

   
    sqlite3_finalize(stmt);
    return true;
}



// string getCurrentDateTime() {
//     time_t now = time(0);
//     tm* t = localtime(&now);
//     char buf[20];
//     strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
//     return string(buf);
// }

void Database::printGuestBill() {
    viewBookings();
    int booking_id;
    cout << "\n--- PRINT GUEST BILL ---" << endl;
    cout << "Enter Booking ID: ";
    cin >> booking_id;
    cin.ignore();

    // Fetch Guest, Room, Booking info
    const char* sql =
        "SELECT b.booking_id, g.first_name, g.last_name, r.room_no, r.room_type, "
        "b.check_in_date, b.check_out_date, b.stay_duration, r.price_per_night, b.total_amount, b.booking_status "
        "FROM Booking b "
        "JOIN Guests g ON b.guest_id = g.guest_id "
        "JOIN RoomDetails r ON b.room_id = r.room_id "
        "WHERE b.booking_id = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_int(stmt, 1, booking_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        cout << "No booking found for Booking ID: " << booking_id << endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Get values
    string guest_name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) + " " +
                        string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    int room_no = sqlite3_column_int(stmt, 3);
    string room_type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    string checkin = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    string checkout = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    int stay_days = sqlite3_column_int(stmt, 7);
    double price_per_night = sqlite3_column_double(stmt, 8);
    double total_room = price_per_night * stay_days;
    double total_amount = sqlite3_column_double(stmt, 9);

    string status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
    sqlite3_finalize(stmt);

    // Fetch Bill Items (if any)
    const char* billSql =
        "SELECT description, amount FROM BillItems WHERE bill_id = (SELECT bill_id FROM Billing WHERE booking_id = ?);";
    sqlite3_stmt* billStmt;
    vector<pair<string, double>> billItems;
    rc = sqlite3_prepare_v2(db, billSql, -1, &billStmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(billStmt, 1, booking_id);
        while (sqlite3_step(billStmt) == SQLITE_ROW) {
            string desc = reinterpret_cast<const char*>(sqlite3_column_text(billStmt, 0));
            double amt = sqlite3_column_double(billStmt, 1);
            billItems.push_back(make_pair(desc, amt));
        }
    }
    sqlite3_finalize(billStmt);


    double subtotal = total_room;
    for (auto& item : billItems) subtotal += item.second;
    double tax = 0.13 * subtotal; 
    double grand_total = subtotal + tax;


    // Payment method
    string payment_method = "Not Recorded";
    const char* paySql = "SELECT payment_method FROM Billing WHERE booking_id = ?;";
    sqlite3_stmt* payStmt;
    rc = sqlite3_prepare_v2(db, paySql, -1, &payStmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(payStmt, 1, booking_id);
        if (sqlite3_step(payStmt) == SQLITE_ROW && sqlite3_column_text(payStmt, 0))
            payment_method = reinterpret_cast<const char*>(sqlite3_column_text(payStmt, 0));
    }
    sqlite3_finalize(payStmt);

    // Print Bill
    cout << "\n====================================\n";
    cout << "      RESET GUEST HOUSE \n";
    cout << "         Guest Bill / Invoice\n";
    // cout << "Bill Date: " << getCurrentDateTime() << endl;
    cout << "Booking ID: " << booking_id << endl;
    cout << "Guest: " << guest_name << endl;
    cout << "Room: " << room_no << " (" << room_type << ")" << endl;
    cout << "Check-in: " << checkin << "\tCheck-out: " << checkout << endl;
    cout << "Stay: " << stay_days << " nights\n";
    cout << "------------------------------------\n";
    cout << left << setw(25) << "Description" << right << setw(10) << "Amount\n";
    cout << "------------------------------------\n";
    cout << left << setw(25) << ("Room (" + to_string(stay_days) + " x $" + to_string((int)price_per_night) + ")")
         << right << setw(10) << fixed << setprecision(2) << total_room << endl;
    for (const auto& item : billItems)
        cout << left << setw(25) << item.first << right << setw(10) << fixed << setprecision(2) << item.second << endl;
    cout << "------------------------------------\n";
    cout << left << setw(25) << "Subtotal" << right << setw(10) << fixed << setprecision(2) << subtotal << endl;
    cout << left << setw(25) << "GST (13%)" << right << setw(10) << fixed << setprecision(2) << tax << endl;
    cout << "------------------------------------\n";
    cout << left << setw(25) << "Grand Total" << right << setw(10) << fixed << setprecision(2) << grand_total << endl;

    cout << "------------------------------------\n";
    cout << "Payment by: " << payment_method << endl;
    cout << "Booking Status: " << status << endl;
    cout << "====================================\n";
    cout << "Thank you for staying with us!\n";
}


// Utility: Get first day of month from any date "YYYY-MM-DD"
string firstDayOfMonth(const string& date) {
    if (date.size() < 7) return "";
    return date.substr(0, 7) + "-01";
}

// Utility: Get last day of month from any date "YYYY-MM-DD"
string lastDayOfMonth(const string& date) {
    int year, month;
    if (sscanf(date.c_str(), "%d-%d", &year, &month) != 2) return "";
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if ((year%4==0 && year%100!=0) || (year%400==0)) daysInMonth[1] = 29; // Leap year
    char buf[11];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", year, month, daysInMonth[month-1]);
    return string(buf);
}

// Prompt for month (YYYY-MM) and return first and last day
void askMonthRange(string& fromDate, string& toDate) {
    cout << "Enter month (YYYY-MM), or leave blank for current month: ";
    string month;
    getline(cin, month);
    if (month.empty()) {
        time_t now = time(0);
        tm* t = localtime(&now);
        char buf[8], buf2[11];
        strftime(buf, sizeof(buf), "%Y-%m", t);
        month = buf;
        strftime(buf2, sizeof(buf2), "%Y-%m-%d", t);
        fromDate = firstDayOfMonth(buf2);
        toDate = buf2; // up to today
    } else {
        fromDate = month + "-01";
        toDate = lastDayOfMonth(fromDate);
    }
}

void Database::generateRevenueReport() {
    cout << "\n--- REVENUE REPORT ---" << endl;


    string fromDate, toDate;
    askMonthRange(fromDate, toDate);

    cout << "\nRevenue Report for period: " << fromDate << " to " << toDate << endl;

    // 1. Total revenue for all bookings in this date range (by check-out date)
    const char* totalSql =
        "SELECT IFNULL(SUM(total_amount), 0) FROM Booking "
        "WHERE check_out_date BETWEEN ? AND ? AND booking_status IN ('Confirmed', 'Completed');";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, totalSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_text(stmt, 1, fromDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, toDate.c_str(), -1, SQLITE_STATIC);

    double total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);

    cout << fixed << setprecision(2);
    cout << "\nTotal Revenue: $" << total << endl;

    // 2. Revenue by Room Type
    const char* typeSql =
        "SELECT r.room_type, IFNULL(SUM(b.total_amount),0) as sum "
        "FROM Booking b JOIN RoomDetails r ON b.room_id = r.room_id "
        "WHERE b.check_out_date BETWEEN ? AND ? AND b.booking_status IN ('Confirmed', 'Completed') "
        "GROUP BY r.room_type;";
    rc = sqlite3_prepare_v2(db, typeSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_text(stmt, 1, fromDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, toDate.c_str(), -1, SQLITE_STATIC);

    cout << "\nRevenue by Room Type:\n";
    cout << left << setw(15) << "Room Type" << right << setw(12) << "Revenue\n";
    cout << "------------------------------\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string t = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        double s = sqlite3_column_double(stmt, 1);
        cout << left << setw(15) << t << right << setw(12) << s << endl;
    }
    sqlite3_finalize(stmt);

    // 3. Number of Bookings and Average Revenue per Booking
    const char* countSql =
        "SELECT COUNT(*), IFNULL(AVG(total_amount),0) FROM Booking "
        "WHERE check_out_date BETWEEN ? AND ? AND booking_status IN ('checked_out');";
    rc = sqlite3_prepare_v2(db, countSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_text(stmt, 1, fromDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, toDate.c_str(), -1, SQLITE_STATIC);

    int nbook = 0;
    double avg = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        nbook = sqlite3_column_int(stmt, 0);
        avg = sqlite3_column_double(stmt, 1);
    }
    sqlite3_finalize(stmt);

    cout << "\nTotal Bookings: " << nbook << endl;
    cout << "Average Revenue per Booking: $" << avg << endl;

    // 4. Optionally: List of bookings in this period
    cout << "\nShow list of bookings in this period? (y/n): ";
    char show;
    cin >> show;
    cin.ignore();
    if (show == 'y' || show == 'Y') {
        const char* listSql =
            "SELECT b.booking_id, g.fname, g.lname, r.room_no, r.room_type, "
            "b.check_in_date, b.check_out_date, b.total_amount, b.booking_status "
            "FROM Booking b "
            "JOIN Guests g ON b.guest_id = g.guest_id "
            "JOIN RoomDetails r ON b.room_id = r.room_id "
            "WHERE b.check_out_date BETWEEN ? AND ? AND b.booking_status IN ('Confirmed', 'Completed') "
            "ORDER BY b.check_out_date;";
        rc = sqlite3_prepare_v2(db, listSql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
            return;
        }
        sqlite3_bind_text(stmt, 1, fromDate.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, toDate.c_str(), -1, SQLITE_STATIC);

        cout << "\nID | Guest           | Room | Type     | Check-in   | Check-out  |  Amount  | Status\n";
        cout << "-------------------------------------------------------------------------------------\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string fname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string lname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int room = sqlite3_column_int(stmt, 3);
            string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            string ci = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            string co = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            double amt = sqlite3_column_double(stmt, 7);
            string st = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
            cout << id << " | "
                << fname << " " << lname << setw(13 - (fname.size() + lname.size()))
                << " | " << room << "   | " << setw(8) << type << " | "
                << ci << " | " << co << " | "
                << setw(8) << amt << " | " << st << endl;
        }
        sqlite3_finalize(stmt);
    }

    cout << "\nEnd of Revenue Report.\n";
}


#include <iostream>
#include <iomanip>
#include <string>
#include <sqlite3.h>
#include "database.h"

using namespace std;

// View all bookings
void Database::viewBookings() {
    cout << "\n--- ALL BOOKINGS ---\n";
    const char* sql = R"(
        SELECT b.booking_id, b.guest_id, g.first_Name, g.last_Name, 
               b.room_id, b.check_in_date, b.check_out_date, b.booking_status
        FROM Booking b
        LEFT JOIN Guests g ON b.guest_id = g.guest_id
        ORDER BY b.booking_id;
    )";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(getDb()) << endl;
        return;
    }
    cout << left << setw(5) << "ID"
         << setw(10) << "GuestID"
         << setw(15) << "First Name"
         << setw(15) << "Last Name"
         << setw(8) << "RoomID"
         << setw(15) << "Check-in"
         << setw(15) << "Check-out"
         << setw(13) << "Status" << endl;
    cout << string(95, '-') << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int booking_id = sqlite3_column_int(stmt, 0);
        int guest_id = sqlite3_column_int(stmt, 1);
        string fname = (const char*)sqlite3_column_text(stmt, 2) ? (const char*)sqlite3_column_text(stmt, 2) : "";
        string lname = (const char*)sqlite3_column_text(stmt, 3) ? (const char*)sqlite3_column_text(stmt, 3) : "";
        int room_id = sqlite3_column_int(stmt, 4);
        string checkin = (const char*)sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : "";
        string checkout = (const char*)sqlite3_column_text(stmt, 6) ? (const char*)sqlite3_column_text(stmt, 6) : "";
        string status = (const char*)sqlite3_column_text(stmt, 7) ? (const char*)sqlite3_column_text(stmt, 7) : "";
        cout << left << setw(5) << booking_id
             << setw(10) << guest_id
             << setw(15) << fname
             << setw(15) << lname
             << setw(8) << room_id
             << setw(15) << checkin
             << setw(15) << checkout
             << setw(13) << status << endl;
    }
    sqlite3_finalize(stmt);
}

// Delete a booking
bool Database::deleteBooking() {
    viewBookings();
    cout << "\n--- DELETE BOOKING ---\n";
    int booking_id;
    cout << "Enter Booking ID to delete: ";
    cin >> booking_id;
    cin.ignore();

    // Confirm existence
    const char* checkSql = "SELECT booking_id FROM Booking WHERE booking_id = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(getDb(), checkSql, -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(checkStmt, 1, booking_id);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "No booking found with ID: " << booking_id << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);

    // Delete
    const char* sql = "DELETE FROM Booking WHERE booking_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, booking_id);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        cout << "Booking deleted successfully!\n";
        return true;
    } else {
        cerr << "Failed to delete booking: " << sqlite3_errmsg(getDb()) << endl;
        return false;
    }
}

// Update a booking
bool Database::updateBooking() {
    viewBookings();
    cout << "\n--- UPDATE BOOKING ---\n";
    int booking_id;
    cout << "Enter Booking ID to update: ";
    cin >> booking_id;
    cin.ignore();

    // Check existence and fetch current details
    const char* checkSql = R"(
        SELECT guest_id, room_id, check_in_date, check_out_date, booking_status 
        FROM Booking WHERE booking_id = ?;
    )";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(getDb(), checkSql, -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(checkStmt, 1, booking_id);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "No booking found with ID: " << booking_id << endl;
        sqlite3_finalize(checkStmt);
        return false;
    }
    int old_guest_id = sqlite3_column_int(checkStmt, 0);
    int old_room_id = sqlite3_column_int(checkStmt, 1);
    string old_check_in = (const char*)sqlite3_column_text(checkStmt, 2) ? (const char*)sqlite3_column_text(checkStmt, 2) : "";
    string old_check_out = (const char*)sqlite3_column_text(checkStmt, 3) ? (const char*)sqlite3_column_text(checkStmt, 3) : "";
    string old_status = (const char*)sqlite3_column_text(checkStmt, 4) ? (const char*)sqlite3_column_text(checkStmt, 4) : "";
    sqlite3_finalize(checkStmt);

    cout << "Leave field blank to keep current value." << endl;
    cout << "Current Guest ID: " << old_guest_id << endl;
    cout << "Enter new Guest ID: ";
    string input; getline(cin, input);
    int new_guest_id = input.empty() ? old_guest_id : stoi(input);

    cout << "Current Room ID: " << old_room_id << endl;
    cout << "Enter new Room ID: ";
    getline(cin, input);
    int new_room_id = input.empty() ? old_room_id : stoi(input);

    cout << "Current Check-in Date: " << old_check_in << endl;
    cout << "Enter new Check-in Date (YYYY-MM-DD): ";
    getline(cin, input);
    string new_check_in = input.empty() ? old_check_in : input;

    cout << "Current Check-out Date: " << old_check_out << endl;
    cout << "Enter new Check-out Date (YYYY-MM-DD or blank for NULL): ";
    getline(cin, input);
    string new_check_out = input; // can be empty

    cout << "Current Status: " << old_status << endl;
    cout << "Enter new Status (confirmed, cancelled, checked out): ";
    getline(cin, input);
    string new_status = input.empty() ? old_status : input;

    const char* sql = R"(
        UPDATE Booking SET 
            guest_id = ?, 
            room_id = ?, 
            check_in_date = ?, 
            check_out_date = ?, 
            booking_status = ?
        WHERE booking_id = ?;
    )";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(getDb()) << endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, new_guest_id);
    sqlite3_bind_int(stmt, 2, new_room_id);
    sqlite3_bind_text(stmt, 3, new_check_in.c_str(), -1, SQLITE_STATIC);
    if (new_check_out.empty())
        sqlite3_bind_null(stmt, 4);
    else
        sqlite3_bind_text(stmt, 4, new_check_out.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, new_status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, booking_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        cout << "Booking updated successfully!\n";
        return true;
    } else {
        cerr << "Failed to update booking: " << sqlite3_errmsg(getDb()) << endl;
        return false;
    }
}
//_______________________________ADMIN FUNCTIIONALITIES____________________
