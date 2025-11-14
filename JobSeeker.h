#ifndef JOBSEEKER_H
#define JOBSEEKER_H

#include "User.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class JobSeeker : public User {
private:
    string cv;  // Thông tin CV
    vector<string> appliedJobs;  // Danh sách công việc đã ứng tuyển

public:
    JobSeeker(string id, string username, string password, string email, string phone)
        : User(id, username, password, email, phone), cv("") {}

    string getRole() const override { return "jobseeker"; }

    // CV Management
    void updateCV(string newCV) { cv = newCV; }
    string getCV() const { return cv; }

    // Apply for jobs
    void applyForJob(string jobId) {
        appliedJobs.push_back(jobId);
        cout << "Da ung tuyen cong viec ID: " << jobId << "\n";
    }

    vector<string> getAppliedJobs() const { return appliedJobs; }

    void displayInfo() const override {
        User::displayInfo();
        cout << "CV: " << (cv.empty() ? "Chua cap nhat" : cv) << "\n";
        cout << "So cong viec da ung tuyen: " << appliedJobs.size() << "\n";
    }

    void searchJob() {
        cout << "Tim viec theo: Nganh, Vi tri, Luong, Dia diem\n";
    }
};

#endif