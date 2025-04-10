#include<iostream>
#include<regex>
#include<string>   
#include<cctype>
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
        cout<<"Invalid number: Length exceeds 10 digits";
        return false;
    }
    else{
        if(number[0]=='9'&& number.length() == 10){
            // cout<<"Valid number";
            return true;
        }
        else{
            cout<<"Invalid number";
            return false;
 }
}
}