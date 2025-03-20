#ifndef GUEST_H
#define GUEST_H

#include <string>

class Guest {
public:
    Guest(std::string name, std::string phone, std::string email);
    void displayInfo();
};

#endif
