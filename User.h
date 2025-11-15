#ifndef USER_H
#define USER_H

#pragma once

#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

class User {
protected:
    string id;
    string username;
    string password;
    string email;
    string phone;
    string fullName;
    bool isActive;
    string createdDate;

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
    string getFullName() const { return fullName; }
    bool getIsActive() const { return isActive; }
    string getCreatedDate() const { return createdDate; }

    // Setters
    void setPassword(string newPassword) { password = newPassword; }
    void setEmail(string newEmail) { email = newEmail; }
    void setPhone(string newPhone) { phone = newPhone; }
    void setFullName(const string& name) { fullName = name; }
    void setActive(bool active) { isActive = active; }
    // Virtual methods
    virtual string getRole() const = 0;
    virtual void displayInfo() const {
        cout << "ID: " << id << "\n";
        cout << "Username: " << username << "\n";
        cout << "Email: " << email << "\n";
        cout << "Phone: " << phone << "\n";

    }
protected:
    string getCurrentDateISO() const {
        const std::time_t now = std::time(nullptr);
        const std::tm* gmt = std::gmtime(&now);   // UTC time

        std::ostringstream oss;
        oss << std::put_time(gmt, "%Y-%m-%dT%H:%M:%SZ");
        return oss.str();
    }
};

#endif