#ifndef EMPLOYER_H
#define EMPLOYER_H

#include "User.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Employer : public User {
private:
    string companyName;
    string companyAddress;
    string companyDescription;
    string industry;
    vector<string> postedJobIds;

public:
    Employer(string id, string username, string password, string email, string phone, string fullName, bool isActive, string createdAt, string companyName = nullptr)
        : User(id, username, password, email, phone, fullName, isActive, createdAt, companyName) {}

    string getRole() const override { return "employer"; }

    // Setters
    void setCompanyName(const string& name) { companyName = name; }
    void setCompanyAddress(const string& addr) { companyAddress = addr; }
    void setCompanyDescription(const string& desc) { companyDescription = desc; }
    void setIndustry(const string& ind) { industry = ind; }

    // Getters
    string getCompanyName() const { return companyName; }
    string getCompanyAddress() const { return companyAddress; }
    string getCompanyDescription() const { return companyDescription; }
    string getIndustry() const { return industry; }

    // Job management
    void addPostedJob(const string& jobId) {
        postedJobIds.push_back(jobId);
    }

    void removePostedJob(const string& jobId) {  // ✅ THÊM HÀM NÀY
        auto it = find(postedJobIds.begin(), postedJobIds.end(), jobId);
        if(it != postedJobIds.end()) {
            postedJobIds.erase(it);
        }
    }

    vector<string> getPostedJobIds() const {
        return postedJobIds;
    }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Vai tro: Nha tuyen dung\n";
        if(!companyName.empty()) {
            cout << "\n--- THONG TIN CONG TY ---\n";
            cout << "Ten cong ty: " << companyName << "\n";
            cout << "Dia chi: " << companyAddress << "\n";
            cout << "Nganh nghe: " << industry << "\n";
            cout << "Mo ta: " << companyDescription << "\n";
            cout << "So tin da dang: " << postedJobIds.size() << "\n";
        }
    }
};

#endif