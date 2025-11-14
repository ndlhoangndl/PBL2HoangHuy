#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
using namespace std;

class User {
protected:
    string id;
    string username;
    string password;
    string email;
    string phone;

public:
    User(string id, string username, string password, string email, string phone)
        : id(id), username(username), password(password), email(email), phone(phone) {}

    virtual ~User() {}  // Virtual destructor

    // Getters
    string getId() const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }

    // Setters
    void setPassword(string newPassword) { password = newPassword; }
    void setEmail(string newEmail) { email = newEmail; }
    void setPhone(string newPhone) { phone = newPhone; }

    // Virtual methods
    virtual string getRole() const = 0;
    virtual void displayInfo() const {
        cout << "ID: " << id << "\n";
        cout << "Username: " << username << "\n";
        cout << "Email: " << email << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Role: " << getRole() << "\n";
    }
};

#endif