#include <iostream>
#include <string.h>
#include <conio.h>
#include <graphics.h>
#include <winbgim.h>
#include<string.h>
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






int main() {
    Database db("guest_house.db");
    if (!db.open()) {
        return 1;
    }

    if (!db.createTable()) {
         db.close();
         return 1;
     }

    if (db.createTable()) {
    db.createTriggrs(); 
    cout << "Database and triggers created successfully!" << endl;

    }

    Admin a;
    staffmanager s;
    a.admin(db);


    sqlite3_exec(db.getDb(), "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);














// ___________________________________________DISPLAY TERMINAL LOGIC________________________________________________________________

    int choice;


    string d = "";
    string b = "";
    system("CLS");
    cout << "\n\n\n";
    cout << "************************************************************************************************************** " << endl ;
    cout << "\n\n";
    cout << "         ******************************      WELCOME TO RESET GUEST HOUSE      *********************************\n\n";
    cout << "\n\n\n";

    cout << "1. Login as receponist" << endl;
    cout << "2. Login as Admin" << endl;
    cin >> choice;
    switch (choice)
    {
        case 1:
            cout << "\n\n************************************************************************************************" << endl;
            cout << "\n\n";
            cout << "____________________________________________    WELCOME STAFF     ____________________________________________ \n\n";
            s.loginStaff(db,d,b);
                break;

        case 2:
           cout << "\n\n************************************************************************************************" << endl;
           cout << "\n\n";
           cout << "____________________________________________    WELCOME ADMIN     ____________________________________________ \n\n";
           cin.ignore();
           a.loginAdmin(db);
            break;
        
    default:
        break;
    }
    

    cout << "hello world \n";
    




//_____________________________________________END OF PROGRAM__________________________________________________________________________

    db.close();
    return 0;

}