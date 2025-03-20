#ifndef GUEST_H
#define GUEST_H

#include <string>

class Guest {
    private:
        std::string name;
        std::string phone;
        std::string email;
    public:
        Guest(std::string name, std::string phone, std::string email);
        void displayInfo();
        void welcomeGuest();
    };

#endif
