#ifndef VALIDATOR_H
#define VALIDATOR_H
#include<regex>
#include<string>   
#include<cctype>

using namespace std;
class Validate
{
    public:
        bool isValidEmail(const string& email);
        bool isValidNumber(const string& number); 
        bool isValidName(const std::string& name);
        string formatCurrency(double amount);
        string getpassword();
             
};

#endif