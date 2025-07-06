// roomfacilities.h
#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include "database.h"

class RoomFacilities {
private:
    int facilityID;
    int roomID;
    std::string facilityName;
    std::string availability;
    std::string description;
    std::string createdAt;
    std::string updatedAt;

public:

    
    // // Constructors
    // RoomFacilities();
    // RoomFacilities(int facilityID, int roomID, const std::string& facilityName, 
    //                const std::string& availability, const std::string& description);
    // ~RoomFacilities();
    
    // Core facility management functions
    // bool createFacilitiesTable(Database& db);
    bool addFacility(Database& db);
    void viewFacilities(Database& db);
    bool updateFacility(Database& db);
    bool deleteFacility(Database& db);
    void manageFacilities(Database& db);
    
    // // Default facilities setup
    // void addDefaultFacilities(Database& db);
    // void addBasicFacilities(Database& db, int roomID);
    // void addDeluxeFacilities(Database& db, int roomID);
    // void addSuiteFacilities(Database& db, int roomID);
    
    // // Search and filter functions
    // void searchFacilitiesByRoom(Database& db);
    // void searchFacilitiesByName(Database& db);
    // void filterFacilitiesByAvailability(Database& db);
    
    // // Bulk operations
    // void bulkUpdateFacilities(Database& db);
    // void copyFacilitiesFromRoom(Database& db);
    // bool deleteFacilitiesByRoom(Database& db);
    
    // // Reporting functions
    // void generateFacilitiesReport(Database& db);
    // void generateRoomWiseFacilitiesReport(Database& db);
    // void generateAvailabilityReport(Database& db);
    
    // // Validation functions
    // bool validateFacilityData(const std::string& facilityName, const std::string& availability);
    // bool checkRoomExists(Database& db, int roomID);
    // bool checkFacilityExists(Database& db, int facilityID);
    
    // // Utility functions
    // void displayFacilitiesMenu();
    // void showAllRooms(Database& db);
    // void showAllFacilities(Database& db);
    // std::string getCurrentTimestamp();
    
    // // Getters
    // int getFacilityID() const { return facilityID; }
    // int getRoomID() const { return roomID; }
    // std::string getFacilityName() const { return facilityName; }
    // std::string getAvailability() const { return availability; }
    // std::string getDescription() const { return description; }
    // std::string getCreatedAt() const { return createdAt; }
    // std::string getUpdatedAt() const { return updatedAt; }
    
    // // Setters
    // void setFacilityID(int id) { facilityID = id; }
    // void setRoomID(int id) { roomID = id; }
    // void setFacilityName(const std::string& name) { facilityName = name; }
    // void setAvailability(const std::string& avail) { availability = avail; }
    // void setDescription(const std::string& desc) { description = desc; }
    // void setCreatedAt(const std::string& created) { createdAt = created; }
    // void setUpdatedAt(const std::string& updated) { updatedAt = updated; }
    
    // // Static utility functions
    // static std::vector<std::string> getStandardFacilities();
    // static std::vector<std::string> getDeluxeFacilities();
    // static std::vector<std::string> getSuiteFacilities();
    // static std::string getAvailabilityStatus(int statusCode);
    // static bool isValidAvailabilityStatus(const std::string& status);
    
    // Friend functions for database operations
    friend class Database;
};




#endif // ROOMFACILITIES_H