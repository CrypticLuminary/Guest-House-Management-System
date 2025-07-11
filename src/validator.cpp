#include<iostream>
#include <iomanip>
#include<regex>
#include<string>   
#include<cctype>
#include <conio.h>
#include "../include/validator.h"
using namespace std;

bool Validate::isValidEmail(const string& email)
{
    const regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, pattern);
}

bool Validate::isValidNumber(const string& number) 
{
    if(number.length() > 10){
        cout << "Invalid number: Length exceeds 10 digits";
        return false;
    }
    else{
        if(number[0] == '9' && number.length() == 10){
            // cout<<"Valid number";
            return true;
        }
        else{
            cout << "Invalid number";
            return false;
        }
    }
}

string Validate::getpassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else if (ch == 3) { // Ctrl+C
            exit(0);
        } else {
            password += ch;
            cout << '*'; // Show * for each character
        }
    }
    cout << endl;
    return password;
}


bool Validate::isValidName(const std::string& name) {
    std::regex namePattern("^[A-Za-z]+$");
    return std::regex_match(name, namePattern);
}

string Validate::formatCurrency(double amount) {
    stringstream ss;
    ss << "$" << fixed << setprecision(2) << amount;
    return ss.str();
}