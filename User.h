#ifndef USER_H
#define USER_H

#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "json.h"

using namespace std;

class User {
public:
    string id;
    string username;
    string password;
    string email;
    string phone;
    string fullName;
    bool isActive;
    string createdAt;
    string role;
    string companyName;


    User(string id, string username, string password, string email, string phone, string fullName, bool isActive,
         string createdAt, string companyName = "") :
        id(id), username(username), password(password), email(email), phone(phone), fullName(fullName),
        isActive(isActive), createdAt(createdAt), companyName(companyName) {}
    User(string id, string username, string password, string email, string phone);

    virtual ~User() {} // Virtual destructor

    virtual string getRole() const { return "user"; }

    // Getters
    string getId() const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getFullName() const { return fullName; }
    bool getIsActive() const { return isActive; }
    string getCreatedDate() const { return createdAt; }

    // Setters
    void setPassword(string newPassword) { password = newPassword; }
    void setEmail(string newEmail) { email = newEmail; }
    void setPhone(string newPhone) { phone = newPhone; }
    void setFullName(const string &name) { fullName = name; }
    void setActive(bool active) { isActive = active; }
    // Virtual methods
    virtual void displayInfo() const {
        cout << "ID: " << id << "\n";
        cout << "Username: " << username << "\n";
        cout << "Email: " << email << "\n";
        cout << "Phone: " << phone << "\n";
    }

    // Append new user
    static void addUser(const string &_username, const string &_password, const string &_fullName, const string &_phone,
                        const string &_email, const string &_role, const string &_companyName,
                        const string &filename = "users.json") {
        json users = PBLJson::loadList(filename);

        users.push_back({
                {"id", to_string(PBLJson::getLatestIndex() + 1)},
                {"username", _username},
                {"password", _password},
                {"fullName", _fullName},
                {"phone", _phone},
                {"email", _email},
                {"isActive", true},
                {"role", _role},
                {"createdAt", getCurrentDateISO()},
                {"companyName", _companyName.empty() ? nullptr : _companyName},
        });

        PBLJson::saveList(users, filename);
    }

    // Find user by username
    static User getUser(const std::string &username, const std::string &filename = "users.json") {
        json users = PBLJson::loadList(filename);

        for (const auto &u: users) {

            // Safe username check
            if (u.value("username", "") == username) {

                string companyName = "";

                if (u.contains("companyName") && u["companyName"].is_string())
                    companyName = u["companyName"].get<std::string>();

                User user(
                    u.value("id", "0"),
                    u.value("username", ""),
                    u.value("password", ""),
                    u.value("email", ""),
                    u.value("phone", ""),
                    u.value("fullName", ""),
                    u.value("isActive", true),
                    u.value("createdAt", ""),
                    companyName
                );

                // Optional role assignment
                if (u.contains("role") && u["role"].is_string()) {
                    user.role = u["role"];
                }

                return user;
            }
        }

        return User("", "", "", "", "", "", false, "", "");
    }

    // Find user by phone
    static User getPhone(const std::string &phone, const std::string &filename = "users.json") {
        json users = PBLJson::loadList(filename);

        for (const auto &u: users) {

            // Safe username check
            if (u.value("phone", "") == phone) {

                string companyName = "";

                if (u.contains("companyName") && u["companyName"].is_string())
                    companyName = u["companyName"].get<std::string>();

                User user(
                    u.value("id", "0"),
                    u.value("username", ""),
                    u.value("password", ""),
                    u.value("email", ""),
                    u.value("phone", ""),
                    u.value("fullName", ""),
                    u.value("isActive", true),
                    u.value("createdAt", ""),
                    companyName
                );

                // Optional role assignment
                if (u.contains("role") && u["role"].is_string()) {
                    user.role = u["role"];
                }

                return user;
            }
        }

        return User("", "", "", "", "", "", false, "", "");
    }


protected : [[nodiscard]] static string
            getCurrentDateISO() {
    const std::time_t now = std::time(nullptr);
    const std::tm *gmt = std::gmtime(&now); // UTC time

    std::ostringstream oss;
    oss << std::put_time(gmt, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}
}
;

#endif
