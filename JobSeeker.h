#ifndef JOBSEEKER_H
#define JOBSEEKER_H

#include "User.h"
#include <vector>

class JobSeeker : public User {
private:
    //  CV Information
    string cvOnline;
    string cvFilePath;
    int age;
    string major;             // Chuyên ngành
    int yearsOfExperience;    // Số năm kinh nghiệm
    vector<string> skills;

    //  Ứng tuyển
    vector<string> appliedJobs;

    // Thông báo
    vector<string> notifications;

public:
    JobSeeker(string id, string username, string password, string email, string phone)
        : User(id, username, password, email, phone),
          age(0), yearsOfExperience(0) {}

    string getRole() const override { return "jobseeker"; }

    // CV Management
    void updateCVOnline(const string& name, int a, const string& maj, int yoe) {
        fullName = name;
        age = a;
        major = maj;
        yearsOfExperience = yoe;
    }

    void setCVFilePath(const string& path) { cvFilePath = path; }
    void addSkill(const string& skill) { skills.push_back(skill); }
    void clearSkills() { skills.clear(); }

    // Getters
    string getCVOnline() const {
        return "Ho ten: " + fullName + "\nTuoi: " + to_string(age) +
               "\nChuyen nganh: " + major +
               "\nKinh nghiem: " + to_string(yearsOfExperience) + " nam";
    }
    string getCVFilePath() const { return cvFilePath; }
    int getAge() const { return age; }
    string getMajor() const { return major; }
    int getYearsOfExperience() const { return yearsOfExperience; }
    vector<string> getSkills() const { return skills; }

    // Ứng tuyển
    void applyForJob(const string& jobId) {
        appliedJobs.push_back(jobId);
    }
    vector<string> getAppliedJobs() const { return appliedJobs; }

    //Thông báo
    void addNotification(const string& msg) {
        notifications.push_back(msg);
    }
    vector<string> getNotifications() const { return notifications; }
    void clearNotifications() { notifications.clear(); }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Vai tro: Ung vien\n";
        if(age > 0) {
            cout << "\n--- THONG TIN CV ---\n";
            cout << "Tuoi: " << age << "\n";
            cout << "Chuyen nganh: " << major << "\n";
            cout << "Kinh nghiem: " << yearsOfExperience << " nam\n";
            if(!skills.empty()) {
                cout << "Ky nang: ";
                for(size_t i = 0; i < skills.size(); i++) {
                    cout << skills[i];
                    if(i < skills.size() - 1) cout << ", ";
                }
                cout << "\n";
            }
        }
    }
};

#endif