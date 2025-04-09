#include<iostream>
#include<string.h>
#include<conio.h>
#include "../include/user.h"
#include "../include/validator.h"
using namespace std;
Validate v;
void User::loginpage(){
    while(true){
    cout<<"enter your email\n";
    cin>>email;
    cout<<"enter your password\n";
    while ((ch = _getch()) != '\r') // '\r' is the Enter key
    {
        if (ch == '\b') // Handle backspace
        {
            if (!password.empty())
            {
                cout << "\b \b"; // Erase the last character
                password.pop_back();
            }
        }
        else 
        {
            password += ch;
            cout << '*'; // Mask the character
        }
    }
    cout<<endl;
    if(v.isValidEmail(email)){
        cout<<"email is valid\n";
        break;
    }
    else{
        cout<<"email is not valid\n";
    }
}
}


