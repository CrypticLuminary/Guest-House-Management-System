#include <iostream>
#include "../include/receptionist.h"
#include "../include/validator.h"
#include "../include/guest.h"
#include <string.h>
using namespace std;
Validate a;
int ch, room;

int guest::check_availability()
{
    cout << "Which room do you want book??" << endl;
    cout << "1. Single Room" << endl;
    cout << "2. Double Room" << endl;   
    cout << "3. Suite Room" << endl;
    cin>>ch;
    switch (ch)
    {
        case 1:
            cout << "There is 1 single room in each floor." << endl;
            cout<<"Room 101 is available."<<endl;
            cout<<"Room 201 is available."<<endl;
            cout<<"Room 301 is available."<<endl;
            cin>>room;
            if(room==101 || room==201 || room==301)
            {
                add_customer();
            }
            break;
            case 2:
            cout << "There is 1 double room in each floor." << endl;
            cout<<"Room 102 is available."<<endl;
            cout<<"Room 202 is available."<<endl;
            cout<<"Room 302 is available."<<endl;
            cin>>room;
            if(room==102 || room==202 || room==302)
            {
                add_customer();
            }
            break;
            case 3:
            cout << "There is 1 suite room in each floor." << endl;
            cout<<"Room 103 is available."<<endl;
            cout<<"Room 203 is available."<<endl;
            cout<<"Room 303 is available."<<endl;
            cin>>room;
            if(room==103 || room==203 || room==303)
            {
                add_customer();
            }
            break;
        default:
            cout << "Invalid choice!" << endl;
    }
    return 0;
}
int guest::add_customer()
{
    cout << "Enter customer details: " << endl;
    cout << "Name: ";
    cin >> name;
    while (true)
    {
        cout << "Email: ";
        cin >> email;
        if (a.isValidEmail(email))
        {
            break;
        }
        else
        {
            cout << "Invalid email. Please try again.\n";
        }
    }
    while (true)
    {
        cout << "Phone: ";
        cin >> phone;
        if (a.isValidNumber(phone))
        {
            break;
        }
        else
        {
            cout << "Invalid phone number. Please try again.\n" << endl;
        }
    }
    
    cout << "Customer added successfully!" << endl;
    update_reservation();
}
int guest::update_reservation()
{
    cout << "Updating reservation..." << endl;
    // Implement the logic to update reservation
    return 0;
}
int guest::cancel_reservation()
{
    cout << "Cancelling reservation..." << endl;
    // Implement the logic to cancel reservation
    return 0;
}
int guest::view_reservations()
{
    cout << "Viewing reservations..." << endl;
    // Implement the logic to view reservations
    return 0;
}