#ifndef EMPLOYER_H
#define EMPLOYER_H

#include "User.h"
#include "Job.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Employer : public User {
private:
    string companyName;
    vector<string> postedJobIds;  // Danh sách ID công việc đã đăng

public:
    Employer(string id, string username, string password, string email, string phone)
        : User(id, username, password, email, phone), companyName("") {}

    string getRole() const override { return "employer"; }

    void setCompanyName(string name) { companyName = name; }
    string getCompanyName() const { return companyName; }

    void addPostedJob(string jobId) {
        postedJobIds.push_back(jobId);
    }

    vector<string> getPostedJobIds() const { return postedJobIds; }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Company: " << (companyName.empty() ? "Chua cap nhat" : companyName) << "\n";
        cout << "So tin da dang: " << postedJobIds.size() << "\n";
    }

    void postJob() {
        cout << "Dang tin tuyen dung\n";
    }
};

#endif