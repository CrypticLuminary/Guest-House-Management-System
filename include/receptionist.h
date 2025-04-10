#ifndef RECEPTIONIST_H
#define RECEPTIONIST_H
#include <string>
#include <iostream>

// class Receptionist {
// public:
    // void greetGuest();
//     void checkInGuest();
//     void checkOutGuest();
//     void handlePayment();
//     void assistWithQueries();
// };

using namespace std;

class guest
{
    string name;
    string email;
    string phone;
    public:
    int add_customer();
    int check_availability();
    int update_reservation();
    int cancel_reservation();
    int view_reservations();
};
// int main()
// {
//     guest g;
//     int choice;
//     cout << "Welcome to the Guest House Reservation Management System!" << endl;
//     cout << "Please choose an option from the menu below: " << endl;
//     cout << "1. Add customer" << endl;
//     cout << "2. Check Availability" << endl;
//     cout << "3. Update Reservation" << endl;
//     cout << "4. Cancel Reservation" << endl;
//     cout << "5. View Reservations" << endl;
//     cout << "6. Exit" << endl;
//     cout << "Enter your choice: ";
//     cin >> choice;
//     switch (choice)
//     {
//     case 1:
//         add_customer(g);
//         break;
//     case 2:
//         check_availability();
//         break;
//     case 3:
//         update_reservation();
//         break;
//     case 4:
//         cancel_reservation();
//         break;
//     case 5:
//         view_reservations();
//         break;
//     case 6:
//         cout << "Exiting the program." << endl;
//         break;
//     }
//     return 0;
// }

#endif