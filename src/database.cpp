#include "../include/database.h"
#include "../include/sqlite3.h"
#include <iostream>
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
                        "address TEXT NULL);";

    //create the room_details table
    const char* RoomDetails = "CREATE TABLE IF NOT EXISTS RoomDetails ("
                            "room_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "room_no INTEGER NOT NULL,"
                            "room_type TEXT NOT NULL,"
                            "price_per_night REAL  NULL, "
                            "status TEXT  CHECK(status IN('Available','Occupied','Under Maintainance Reserved')) DEFAULT 'Available');";

    // ADMIN TABLE
    const char* sqlAdmin = "CREATE TABLE IF NOT EXISTS Admin("
                           "admin_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "username TEXT UNIQUE NOT NULL, "
                           "email TEXT NOT NULL UNIQUE, "
                           "password TEXT NOT NULL, " 
                           "role TEXT CHECK(role IN('Manager','staff')) NOT NULL);";

    //BOOKING TABLE
    const char* sqlbooking = "CREATE TABLE IF NOT EXISTS Booking("
                            "booking_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "guest_id INTEGER REFERENCES Guests(guest_id) ON DELETE CASCADE, "
                            "room_id INTEGER REFERENCES RoomDetails(room_id) ON DELETE CASCADE, "
                            "check_in_date DATE DEFAULT (DATE('now')), "
                            "check_out_date DATE NOT NULL, "
                            "booking_status TEXT CHECK(booking_status IN ('Confirmed', 'Cancelled', 'Checked Out')) DEFAULT 'Confirmed');";


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

    //Admin
    rc = sqlite3_exec(db, sqlAdmin, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK) {
        cerr << "Error creating admin table: " << errMsg << endl;
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
bool Database::insertGuest(const string& fname, const string& lname, const string& contact_info, const string& email, const string& id_proof, const string& address) {
    const char* sqlGuest = "INSERT INTO Guests (first_name, last_name, contact_info, email, id_proof, address) VALUES (?, ?, ?, ?, ?, ?);";
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
    sqlite3_bind_text(stmt, 6, address.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "Guest inserted successfully!" << endl;
    sqlite3_finalize(stmt);
    return true;
}


//ADMIN
bool Database::insertAdmin(const string& username, const string& password, const string& email, const string& role) {
    const char* sqlGuest = "INSERT INTO Admin (username, email, password, role) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlGuest, -1, &stmt, nullptr);
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

    cout << "Guest inserted successfully!" << endl;
    sqlite3_finalize(stmt);
    return true;
}



//   ##_______________________________________________PRINTING DATA FROM TABLE________________________________________##


// Print all guests from the Guests table
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