#ifndef JOBSEEKER_H
#define JOBSEEKER_H

#include "User.h"
#include <vector>

class JobSeeker : public User {
private:
    //  CV Information
    string major;             // Chuyên ngành
    float yearsOfExperience;    // Số năm kinh nghiệm
    vector<string> skills;

    //  Ứng tuyển
    vector<string> appliedJobs; // Applied companies list

    // Thông báo
    vector<string> notifications;

public:
    JobSeeker(string id, string username, string password, string email, string phone, bool isActive)
        : User(id, username, password, email, phone, isActive), yearsOfExperience(0) {
        fetchData();
    };

    explicit JobSeeker(const User& user)
    : User(user.id, user.username, user.password, user.email, user.phone,
           user.fullName, user.dateOfBirth, user.isActive, user.createdAt, user.companyName),
      yearsOfExperience(0) {
        fetchData();
    };

    [[nodiscard]] string getRole() const override { return "jobseeker"; }

    // CV Management
    void updateCVOnline(const string &maj, float yoe) {
        major = maj;
        yearsOfExperience = yoe;

    }

    void addSkill(const string& skill) { skills.push_back(skill); }
    void clearSkills() { skills.clear(); }

    // Getters
    [[nodiscard]] string getCVOnline() const {
        return "Ho ten: " + fullName + "\nTuoi: " + to_string(getAge()) +
               "\nChuyen nganh: " + major +
               "\nKinh nghiem: " + to_string(yearsOfExperience) + " nam";
    }
    [[nodiscard]] string getMajor() const { return major; }
    [[nodiscard]] int getYearsOfExperience() const { return yearsOfExperience; }
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
        if(getAge() > 0) {
            cout << "\n--- THONG TIN CV ---\n";
            cout << "Tuoi: " << getAge() << "\n";
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

    // DATA PROCESSOR
    void fetchData(const string &filename = "jobSeekers.json") {
        json data = PBLJson::loadList(filename)[id];

        major = data["major"].get<std::string>();
        yearsOfExperience = data["yearsOfExperience"].get<float>();
        skills = data["skills"].get<vector<string>>();
        appliedJobs = data["appliedJobs"].get<vector<string>>();
        notifications = data["notifications"].get<vector<string>>();

    }

    void deleteNotifications(const string &filename = "jobSeekers.json") const {
        vector<string> _notifications = {};

        json data = PBLJson::loadList(filename);
        data[id]["notifications"].update(_notifications);
        PBLJson::saveList(data, filename);
    }

    void updateCV(const string &filename = "jobSeekers.json") {
        json data = PBLJson::loadList(filename);
        data[id].update({
            {"major", major},
            {"yearsOfExperience", yearsOfExperience},
            {"skills", skills},
        });

        PBLJson::saveList(data, filename);
    };
};

#endif