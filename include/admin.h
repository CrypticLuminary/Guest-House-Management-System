#ifndef ADMIN_H
#define ADMIN_H

#include <string>

class Admin {
public:
    Admin(std::string username, std::string password);
    bool login();
};

#endif
