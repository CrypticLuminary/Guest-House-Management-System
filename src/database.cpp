#include "../include/database.h"
#include "../include/sqlite3.h"
#include <iostream>
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
                            "room_no INTEGER NOT NULL,"
                            "room_type TEXT NOT NULL,"
                            "price_per_night REAL  NULL, "
                            "status TEXT  CHECK(status IN('Available','Occupied','Under Maintainance Reserved')) DEFAULT 'Available');";

    // ADMIN TABLE
    const char* sqlAdmin = "CREATE TABLE IF NOT EXISTS Admin("
                           "admin_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "username TEXT UNIQUE NOT NULL, "
                           "email TEXT NOT NULL UNIQUE, "
                           "password TEXT NOT NULL); " ;
                           

    const char* sqlStaff = "CREATE TABLE IF NOT EXISTS Staff("
                            "staff_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "username TEXT UNIQUE NOT NULL, "
                           "email TEXT NOT NULL UNIQUE, "
                           "password TEXT NOT NULL, " 
                           "role TEXT CHECK(role IN('receptionist','manager')) NOT NULL);";

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

    //Staff
    rc = sqlite3_exec(db, sqlStaff, nullptr, nullptr, &errMsg);
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



// ##_________________________________________________UPDATIN DATA ON THE TABLE______________________________________##

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

    std::cout << "Admin details updated successfully!" << std::endl;

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
        const unsigned char* status = sqlite3_column_text(stmt, 3);

        cout << "ID: " << room_id
             << ", Room Number: " << room_no
             << ", Room Type: " << room_type
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
bool Database::deleteGuest(int guest_id) {
        string sql  = "DELETE FROM Guests WHERE id = ?;";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if(rc != SQLITE_OK) {
            cerr << "Failed to prepare statement (delete_guest): " << sqlite3_errmsg(db) << endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, guest_id);
        rc = sqlite3_step(stmt);
        if(rc != SQLITE_DONE) {
            cerr << "Deletion of guest failed: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            return false;
        }
        cout << "Guest with ID " << guest_id << " has been deleted successfully!" << endl;
        sqlite3_finalize(stmt);
        return true;
    
}

//ROOM
bool Database::deleteRoom(int room_id) {
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