#include<iostream>
#include<string.h>
#include "../include/user.h"
#include "../include/validator.h"
using namespace std;
Validate v;
void User::loginpage(){
    while(true){
    cout<<"enter your email\n";
    cin>>email;
    if(v.isValidEmail(email)){
    cout<<"enter your password\n";
    cin>>password;
    break;
    }
    else{
        cout<<"this email cant be used\n";
    }
}
}