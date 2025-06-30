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

    Admin a;
    a.admin(db);
















// ___________________________________________DISPLAY TERMINAL LOGIC________________________________________________________________

    int choice;



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
           cout << "Hello receponist" << endl;
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