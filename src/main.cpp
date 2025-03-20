#include <iostream>
// #include "database.h"
#include "guest.h"
#include "booking.h"
#include "billing.h"
#include "reports.h"
#include "admin.h"
#include "housekeeping.h"

int main() {
    std::cout << "Welcome to Guest House Management System" << std::endl;
    
    Guest guest("John Doe", "555-1234", "john@example.com");
    guest.welcomeGuest();
    
    return 0;
}