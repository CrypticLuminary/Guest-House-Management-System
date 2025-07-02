#include <iostream>
#include <string.h>
#include <conio.h>
#include <graphics.h>
#include <winbgim.h>
#include<string.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
using namespace std;
#include "../include/database.h"



#include "../include/admin.h"

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



// Current Date and Time (UTC - YYYY-MM-DD HH:MM:SS formatted): 2025-07-02 06:22:32
// Current User's Login: CrypticLuminary

int choice;
string d = "";
string b = "";

system("CLS");
cout << "\n\n";

// Clean ASCII Art Header
cout << "================================================================================\n";
cout << "||                                                                            ||\n";
cout << "||   RRRR   EEEEE  SSSS  EEEEE  TTTTT     GGGGG  U   U  EEEEE  SSSS  TTTTT  ||\n";
cout << "||   R   R  E      S     E        T       G      U   U  E      S       T    ||\n";
cout << "||   RRRR   EEEE   SSSS  EEEE     T       G GGG  U   U  EEEE   SSSS    T    ||\n";
cout << "||   R R    E          S E        T       G   G  U   U  E          S   T    ||\n";
cout << "||   R  R   EEEEE  SSSS  EEEEE    T       GGGGG  UUUUU  EEEEE  SSSS    T    ||\n";
cout << "||                                                                            ||\n";
cout << "||                       H   H   OOO   U   U  SSSS  EEEEE                     ||\n";
cout << "||                       H   H  O   O  U   U  S     E                         ||\n";
cout << "||                       HHHHH  O   O  U   U  SSSS  EEEE                      ||\n";
cout << "||                       H   H  O   O  U   U     S  E                         ||\n";
cout << "||                       H   H   OOO   UUUUU  SSSS  EEEEE                     ||\n";
cout << "||                                                                            ||\n";
cout << "||                    M A N A G E M E N T   S Y S T E M                      ||\n";
cout << "||                                                                            ||\n";
cout << "================================================================================\n";

cout << "\n\n";

// Clean bordered title
cout << "********************************************************************************\n";
cout << "*                   GUEST HOUSE MANAGEMENT SYSTEM LOGIN                         *\n";
cout << "********************************************************************************\n";

cout << "\n\n";

// Menu with clean borders
cout << "+------------------------------------------------------------------------------+\n";
cout << "|                             SELECT YOUR ROLE                                |\n";
cout << "+------------------------------------------------------------------------------+\n";
cout << "|                                                                              |\n";
cout << "|  [1] >> LOGIN AS RECEPTIONIST                                               |\n";
cout << "|      -> Handle guest check-ins, reservations, and daily operations         |\n";
cout << "|                                                                              |\n";
cout << "|  [2] >> LOGIN AS ADMINISTRATOR                                              |\n";
cout << "|      -> Full system access, manage staff, reports, and configurations      |\n";
cout << "|                                                                              |\n";
cout << "|  [0] >> EXIT SYSTEM                                                         |\n";
cout << "|      -> Close the application safely                                        |\n";
cout << "|                                                                              |\n";
cout << "+------------------------------------------------------------------------------+\n";

cout << "\n";
cout << "********************************************************************************\n";
cout << "* Enter your choice: ";
cin >> choice;
cout << "********************************************************************************\n";

switch (choice)
{
    case 1:
        system("CLS");
        cout << "\n\n";
        
        // Receptionist Header
        cout << "================================================================================\n";
        cout << "||                                                                            ||\n";
        cout << "||   RRRR   EEEEE   CCC   EEEEE  PPPP   TTTTT  III   OOO   N   N  III  SSSS ||\n";
        cout << "||   R   R  E      C   C  E      P   P    T     I   O   O  NN  N   I   S     ||\n";
        cout << "||   RRRR   EEEE   C      EEEE   PPPP     T     I   O   O  N N N   I   SSSS  ||\n";
        cout << "||   R R    E      C   C  E      P        T     I   O   O  N  NN   I      S  ||\n";
        cout << "||   R  R   EEEEE   CCC   EEEEE  P        T    III   OOO   N   N  III  SSSS  ||\n";
        cout << "||                                                                            ||\n";
        cout << "||                       WELCOME TO RECEPTION PORTAL                        ||\n";
        cout << "||                                                                            ||\n";
        cout << "================================================================================\n";
        
        cout << "\n";
        cout << ">> Initializing Reception System...\n";
        cout << ">> Loading Reception Portal...\n";
        cout << ">> Reception System Ready!\n\n";
        
        cout << "********************************************************************************\n";
        cout << "*                         RECEPTION PORTAL ACTIVATED                          *\n";
        cout << "********************************************************************************\n\n";
        
        s.loginStaff(db, d, b);
        break;

    case 2:
        system("CLS");
        cout << "\n\n";
        
        // Administrator Header
        cout << "================================================================================\n";
        cout << "||                                                                            ||\n";
        cout << "||      AAA   DDDD   M   M  III  N   N  III  SSSS  TTTTT  RRRR    AAA   TTTTT||\n";
        cout << "||     A   A  D   D  MM MM   I   NN  N   I   S        T    R   R  A   A    T  ||\n";
        cout << "||     AAAAA  D   D  M M M   I   N N N   I   SSSS     T    RRRR   AAAAA    T  ||\n";
        cout << "||     A   A  D   D  M   M   I   N  NN   I      S     T    R R    A   A    T  ||\n";
        cout << "||     A   A  DDDD   M   M  III  N   N  III  SSSS     T    R  R   A   A    T  ||\n";
        cout << "||                                                                            ||\n";
        cout << "||    OOO   RRRR      CCC   OOO   N   N  TTTTT  RRRR    OOO   L              ||\n";
        cout << "||   O   O  R   R    C   C O   O  NN  N    T    R   R  O   O  L              ||\n";
        cout << "||   O   O  RRRR     C     O   O  N N N    T    RRRR   O   O  L              ||\n";
        cout << "||   O   O  R R      C   C O   O  N  NN    T    R R    O   O  L              ||\n";
        cout << "||    OOO   R  R      CCC   OOO   N   N    T    R  R    OOO   LLLLL          ||\n";
        cout << "||                                                                            ||\n";
        cout << "||                   ADMINISTRATIVE CONTROL CENTER                           ||\n";
        cout << "||                                                                            ||\n";
        cout << "================================================================================\n";
        
        cout << "\n";
        cout << ">> Accessing Administrator Portal...\n";
        cout << ">> Authenticating Administrator Credentials...\n";
        cout << ">> Administrator Access Granted!\n\n";
        
        cout << "********************************************************************************\n";
        cout << "*                       ADMIN CONTROL PANEL ACTIVATED                         *\n";
        cout << "********************************************************************************\n\n";
        
        cin.ignore();
        a.loginAdmin(db);
        break;
    
    case 0:
        system("CLS");
        cout << "\n\n";
        
        // Exit Screen
        cout << "================================================================================\n";
        cout << "||                                                                            ||\n";
        cout << "||   TTTTT  H   H   AAA   N   N  K   K     Y   Y   OOO   U   U              ||\n";
        cout << "||     T    H   H  A   A  NN  N  K  K      Y   Y  O   O  U   U              ||\n";
        cout << "||     T    HHHHH  AAAAA  N N N  KKK        YYY   O   O  U   U              ||\n";
        cout << "||     T    H   H  A   A  N  NN  K  K        Y    O   O  U   U              ||\n";
        cout << "||     T    H   H  A   A  N   N  K   K       Y     OOO   UUUUU              ||\n";
        cout << "||                                                                            ||\n";
        cout << "||   FFFFF   OOO   RRRR      U   U  SSSS  III  N   N   GGG                  ||\n";
        cout << "||   F      O   O  R   R     U   U  S      I   NN  N  G   G                 ||\n";
        cout << "||   FFFF   O   O  RRRR      U   U  SSSS   I   N N N  G                     ||\n";
        cout << "||   F      O   O  R R       U   U     S   I   N  NN  G   G                 ||\n";
        cout << "||   F       OOO   R  R      UUUUU  SSSS  III  N   N   GGG                  ||\n";
        cout << "||                                                                            ||\n";
        cout << "||              RESET GUEST HOUSE MANAGEMENT SYSTEM                          ||\n";
        cout << "||                                                                            ||\n";
        cout << "||                         Have a wonderful day!                             ||\n";
        cout << "||                                                                            ||\n";
        cout << "||                       Created by: CrypticLuminary                         ||\n";
        cout << "||                       Date: 2025-07-02 06:22:32                          ||\n";
        cout << "||                                                                            ||\n";
        cout << "================================================================================\n";
        
        cout << "\n";
        cout << ">> Shutting down system safely...\n";
        cout << ">> Saving user data...\n";
        cout << ">> Closing database connections...\n";
        cout << ">> System closed successfully!\n\n";
        
        cout << "********************************************************************************\n";
        cout << "*                                GOODBYE!                                      *\n";
        cout << "********************************************************************************\n\n";
        
        exit(0);
        break;
        
    default:
        cout << "\n";
        cout << "********************************************************************************\n";
        cout << "*                                 ERROR!                                       *\n";
        cout << "*                        Invalid choice! Please select:                       *\n";
        cout << "*                            1 - Receptionist                                  *\n";
        cout << "*                            2 - Administrator                                 *\n";
        cout << "*                            0 - Exit                                          *\n";
        cout << "********************************************************************************\n";
        cout << "\n>> Please try again...\n\n";
        break;
}



//_____________________________________________END OF PROGRAM__________________________________________________________________________

    db.close();
    return 0;

}