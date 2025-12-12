#ifndef EMPLOYER_H
#define EMPLOYER_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "User.h"
using namespace std;

class Employer : public User {
private:
    string companyName;
    string companyAddress;
    string companyDescription;
    string industry;
    vector<string> postedJobIds;

public:
    Employer(string id, string username, string password, string email, string phone, string fullName,
             string dateOfBirth, bool isActive, string createdAt, string companyName = nullptr) :
        User(id, username, password, email, phone, fullName, dateOfBirth, isActive, createdAt, companyName),
        companyName(companyName), postedJobIds(vector<string>{}) {}

    Employer(const User &user) :
        User(user.id, user.username, user.password, user.email, user.phone, user.fullName, user.dateOfBirth,
             user.isActive, user.createdAt, user.companyName),
        companyName(user.companyName), postedJobIds(vector<string>{}) {}

    string getRole() const override { return "employer"; }

    // Setters
    void setCompanyName(const string &name) { companyName = name; }
    void setCompanyAddress(const string &addr) { companyAddress = addr; }
    void setCompanyDescription(const string &desc) { companyDescription = desc; }
    void setIndustry(const string &ind) { industry = ind; }

    // Getters
    string getCompanyName() const { return companyName; }
    string getCompanyAddress() const { return companyAddress; }
    string getCompanyDescription() const { return companyDescription; }
    string getIndustry() const { return industry; }

    // Job management
    void addPostedJob(const string &jobId) { postedJobIds.push_back(jobId); }

    void removePostedJob(const string &jobId) {
        auto it = find(postedJobIds.begin(), postedJobIds.end(), jobId);
        if (it != postedJobIds.end()) {
            postedJobIds.erase(it);
        }
    }

    [[nodiscard]] vector<string> getPostedJobIds(const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);
        vector<string> _postedJobIds;

        for (auto &j: data) {
            if (!j.value("employerId", "").empty() && j.value("employerId", "") == id) {
                _postedJobIds.push_back(j.value("id", ""));
            }
        }
        return _postedJobIds;
    }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Vai tro: Nha tuyen dung\n";
        if (!companyName.empty()) {
            cout << "\n--- THONG TIN CONG TY ---\n";
            cout << "Ten cong ty: " << companyName << "\n";
            // cout << "Dia chi: " << companyAddress << "\n";
            // cout << "Nganh nghe: " << industry << "\n";
            // cout << "Mo ta: " << companyDescription << "\n";
            cout << "So tin da dang: " << getPostedJobIds().size() << "\n";
        }
    }

    void updateProfile(const string &filename = "users.json") {
        json data = PBLJson::loadList(filename);
        for (auto &u: data) {
            if (!u.value("id", "").empty() && u.value("id", "") == id) {
                u.update({{"companyName", companyName}, {"email", email}, {"phone", phone}});
            }
        }
        PBLJson::saveList(data, "users.json");
    }
};

#endif
