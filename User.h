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
    string dateOfBirth;
    bool isActive;
    string createdAt;
    string role;
    string companyName;

    User() :
        id(""), username(""), password(""), email(""), phone(""), fullName(""), dateOfBirth(""), isActive(false),
        createdAt(""), role(""), companyName("") {};

    User(const string &id, const string &username, const string &password, const string &email, const string &phone,
         const string &fullName, const string &dateOfBirth, const bool isActive, const string &createdAt,
         const string &companyName = "") :
        id(id), username(username), password(password), email(email), phone(phone), fullName(fullName),
        dateOfBirth(dateOfBirth), isActive(isActive), createdAt(createdAt), companyName(companyName) {}

    User(string id, string username, string password, string email, string phone, bool isActive) :
        id(id), username(username), password(password), email(email), phone(phone), isActive(isActive) {};

    virtual ~User() = default; // Virtual destructor

    [[nodiscard]] virtual string getRole() const { return "user"; }

    // Getters
    [[nodiscard]] string getId() const { return id; }
    [[nodiscard]] string getUsername() const { return username; }
    [[nodiscard]] string getPassword() const { return password; }
    [[nodiscard]] string getEmail() const { return email; }
    [[nodiscard]] string getPhone() const { return phone; }
    [[nodiscard]] string getFullName() const { return fullName; }
    [[nodiscard]] string getDateOfBirth() const { return dateOfBirth; }
    [[nodiscard]] virtual string getCompanyName() const { return companyName; }

    [[nodiscard]] virtual int getAge() const {
        int y, m, d;
        std::istringstream iss(dateOfBirth);
        if (char sep1, sep2; !(iss >> y >> sep1 >> m >> sep2 >> d))
            return -1; // invalid format
        const std::time_t t = std::time(nullptr);
        const std::tm now = *std::localtime(&t);
        const int year = now.tm_year + 1900;
        const int month = now.tm_mon + 1;
        const int day = now.tm_mday;
        int age = year - y;
        if (month < m || (month == m && day < d))
            --age;
        return age;
    }

    [[nodiscard]] bool getStatus() const { return isActive; }
    [[nodiscard]] string getCreatedDate() const { return createdAt; }

    // Setters
    void setPassword(const string &_password) {
        password = _password;
        updateData();
    }
    void setEmail(const string &_email) {
        email = _email;
        updateData();
    }
    void setPhone(const string &_phone) {
        phone = _phone;
        updateData();
    }
    void setFullName(const string &name) {
        fullName = name;
        updateData();
    }
    void setDateOfBirth(const string &_dateOfBirth) {
        dateOfBirth = _dateOfBirth;
        updateData();
    }
    void setActive(const bool active) {
        isActive = active;
        updateData();
    }

    void updateData(const string &filename = "users.json") {
        json data = PBLJson::loadList(filename);
        for (auto &u: data) {
            if (u.value("id", "") == id) {
                u.update({
                        {"id", id},
                        {"username", username},
                        {"password", password},
                        {"fullName", fullName},
                        {"dateOfBirth", dateOfBirth},
                        {"phone", phone},
                        {"email", email},
                        {"isActive", isActive},
                        {"role", role},
                        {"createdAt", createdAt},
                        {"companyName", companyName},
                });
                break;
            }
        }
        PBLJson::saveList(data, filename);
    }

    // Virtual methods
    virtual void displayInfo() const {
        cout << "ID: " << id << "\n";
        cout << "Username: " << username << "\n";
        cout << "Email: " << email << "\n";
        cout << "Phone: " << phone << "\n";
    }

    // Append new user
    static void addUser(const string &_username, const string &_password, const string &_fullName,
                        const string &_dateOfBirth, const string &_phone, const string &_email, const string &_role,
                        const string &_companyName, const string &filename = "users.json") {
        json users = PBLJson::loadList(filename);

        users.push_back({
                {"id", to_string(PBLJson::getLatestIndex() + 1)},
                {"username", _username},
                {"password", _password},
                {"fullName", _fullName},
                {"dateOfBirth", _dateOfBirth},
                {"phone", _phone},
                {"email", _email},
                {"isActive", true},
                {"role", _role},
                {"createdAt", getCurrentDateISO()},
                {"companyName", _companyName.empty() ? "" : _companyName},
        });

        PBLJson::saveList(users, filename);
    }

    // Delete user
    static void removeUser(const User &user, const string &filename = "users.json") {
        json data = PBLJson::loadList(filename);

        for (auto it = data.begin(); it != data.end(); ++it) {
            if ((*it).value("id", "") == user.id) {
                data.erase(it); // ✅ correct!
                break;
            }
        }

        PBLJson::saveList(data, filename);
    }

    // Find user by username
    static User findUser(const std::string &username, const std::string &filename = "users.json") {
        json users = PBLJson::loadList(filename);

        for (const auto &u: users) {

            // Safe username check
            if (u.value("username", "") == username) {

                string companyName;

                if (u.contains("companyName") && u["companyName"].is_string())
                    companyName = u["companyName"].get<std::string>();

                User user(u.value("id", "0"), u.value("username", ""), u.value("password", ""), u.value("email", ""),
                          u.value("phone", ""), u.value("fullName", ""), u.value("dateOfBirth", "0000-00-00"),
                          u.value("isActive", true), u.value("createdAt", ""), companyName);

                // Optional role assignment
                if (u.contains("role") && u["role"].is_string()) {
                    user.role = u["role"];
                }

                return user;
            }
        }

        return {"", "", "", "", "", "", "", false, "", ""};
    }

    // Find user by phone
    static User findPhone(const std::string &phone, const std::string &filename = "users.json") {
        json users = PBLJson::loadList(filename);

        for (const auto &u: users) {

            // Safe username check
            if (u.value("phone", "") == phone) {

                string companyName;

                if (u.contains("companyName") && u["companyName"].is_string())
                    companyName = u["companyName"].get<std::string>();

                User user(u.value("id", "0"), u.value("username", ""), u.value("password", ""), u.value("email", ""),
                          u.value("phone", ""), u.value("fullName", ""), u.value("dateOfBirth", "0000-00-00"),
                          u.value("isActive", true), u.value("createdAt", ""), companyName);

                // Optional role assignment
                if (u.contains("role") && u["role"].is_string()) {
                    user.role = u["role"];
                }

                return user;
            }
        }

        return {"", "", "", "", "", "", "", false, "", ""};
    }

    // Find user by email
    static User findEmail(const std::string &email, const std::string &filename = "users.json") {
        json users = PBLJson::loadList(filename);

        for (const auto &u: users) {

            // Safe username check
            if (u.value("email", "") == email) {

                string companyName;

                if (u.contains("companyName") && u["companyName"].is_string())
                    companyName = u["companyName"].get<std::string>();

                User user(u.value("id", "0"), u.value("username", ""), u.value("password", ""), u.value("email", ""),
                          u.value("phone", ""), u.value("fullName", ""), u.value("dateOfBirth", "0000-00-00"),
                          u.value("isActive", true), u.value("createdAt", ""), companyName);

                // Optional role assignment
                if (u.contains("role") && u["role"].is_string()) {
                    user.role = u["role"];
                }

                return user;
            }
        }

        return {"", "", "", "", "", "", "", false, "", ""};
    }


protected:
    [[nodiscard]] static string getCurrentDateISO() {
        const std::time_t now = std::time(nullptr);
        const std::tm *gmt = std::gmtime(&now); // UTC time

        std::ostringstream oss;
        oss << std::put_time(gmt, "%Y-%m-%dT%H:%M:%SZ");
        return oss.str();
    }
};

#endif
