#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <string>
using namespace std;

class Admin : public User {
public:
    Admin(string id, string username, string password, string email, string phone)
        : User(id, username, password, email, phone) {}

    string getRole() const override { return "admin"; }

    void manageAccounts() {
        cout << "Quan ly tai khoan (demo)\n";
    }

    void approvePosts() {
        cout << "Kiem duyet bai dang (demo)\n";
    }
};

#endif
