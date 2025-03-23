#include "../include/database.h"
#include "../include/sqlite3.h"
#include <iostream>

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
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
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
        std::cout << "Database closed." << std::endl;
    }
}

// Create the Guests table
bool Database::createTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS Guests ("
                      "guest_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "phone TEXT, "
                      "email TEXT);";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    std::cout << "Table created successfully!" << std::endl;
    return true;
}

// Insert a guest into the Guests table
bool Database::insertGuest(const std::string& name, const std::string& phone, const std::string& email) {
    const char* sql = "INSERT INTO Guests (name, phone, email) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    std::cout << "Guest inserted successfully!" << std::endl;
    sqlite3_finalize(stmt);
    return true;
}

// Print all guests from the Guests table
void Database::printGuests() {
    const char* sql = "SELECT guest_id, name, phone, email FROM Guests;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::cout << "Guests:" << std::endl;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int guest_id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* phone = sqlite3_column_text(stmt, 2);
        const unsigned char* email = sqlite3_column_text(stmt, 3);

        std::cout << "ID: " << guest_id
                  << ", Name: " << name
                  << ", Phone: " << phone
                  << ", Email: " << email << std::endl;
    }

    sqlite3_finalize(stmt);
}