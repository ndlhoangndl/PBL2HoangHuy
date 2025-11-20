#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
public:
    Admin(string id, string username, string password, string email, string phone, string fullName, bool isActive, string createdAt, string companyName) :
        User(id, username, password, email, phone, fullName, isActive, createdAt, companyName) {}

    string getRole() const override { return "admin"; }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Vai tro: Quan tri vien\n";
    }
};

#endif
