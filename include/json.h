#pragma once

#include <json.hpp>
#include <fstream>

#ifndef JSON_H
#define JSON_H

using json = nlohmann::json;

namespace PBLJson {
    // Loads JSON array from file. If file doesn't exist, returns empty array.
    inline json loadList(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) return json::array();

        json j;
        in >> j;
        return j;
    }

    // Saves JSON array to file
    inline void saveList(const json& j, const std::string& filename) {
        std::ofstream out(filename);
        out << j.dump(4);   // pretty print
    }

    // Append new user
    inline void addUser(const std::string& filename, const std::string& username, const std::string& email) {
        json users = loadList(filename);

        users.push_back({
            {"username", username},
            {"email", email}
        });

        saveList(users, filename);
    }

// ######################## USER SEARCHING FUNCTIONS ######################## //

    // Find user by username
    inline json getUser(const std::string& username, const std::string& filename = "users.json") {
        json users = loadList(filename);

        for (const auto& user : users) {
            if (user["username"] == username) {
                return user;        // return the whole user object
            }
        }

        return json(); // empty JSON = not found
    }

    [[nodiscard]] inline bool userExists(const std::string& username, const std::string& filename = "users.json") {
        json users = loadList(filename);

        for (const auto& user : users) {
            if (user["username"] == username) {
                return true;
            }
        }

        return false;
    }

    // Find user by phone number
    inline json findPhone(const std::string& phone, const std::string& filename = "users.json") {
        json users = loadList(filename);

        for (const auto& user : users) {
            if (user["phone"] == phone) {
                return user;        // return the whole user object
            }
        }

        return json(); // empty JSON = not found
    }

    [[nodiscard]] inline bool phoneExists(const std::string& phone, const std::string& filename = "users.json") {
        json users = loadList(filename);

        for (const auto& user : users) {
            if (user["phone"] == phone) {
                return true;
            }
        }

        return false;
    }

    
}


#endif