#ifndef JOBSEEKER_H
#define JOBSEEKER_H

#include <vector>
#include "User.h"
#include "json.h"

class JobSeeker : public User {
private:
    //  CV Information
    string major; // Chuyên ngành
    float yearsOfExperience; // Số năm kinh nghiệm
    vector<string> skills;

    //  Ứng tuyển
    vector<string> appliedJobs; // Applied companies list

    // Thông báo
    vector<string> notifications;

public:
    JobSeeker(string id, string username, string password, string email, string phone, bool isActive) :
        User(id, username, password, email, phone, isActive), yearsOfExperience(0) {
        fetchData();
    };

    explicit JobSeeker(const User &user) :
        User(user.id, user.username, user.password, user.email, user.phone, user.fullName, user.dateOfBirth,
             user.isActive, user.createdAt, user.companyName),
        yearsOfExperience(0) {
        fetchData();
    };

    explicit JobSeeker(const json &_, const string &uid) :
        User(
                // Load base user info from users.json
                [&]() {
                    json users = PBLJson::loadList("users.json");

                    for (auto &u: users) {
                        if (u.value("id", "") == uid) {
                            return User(u.value("id", ""), u.value("username", ""), u.value("password", ""),
                                        u.value("email", ""), u.value("phone", ""), u.value("fullName", ""),
                                        u.value("dateOfBirth", "0000-00-00"), u.value("isActive", true),
                                        u.value("createdAt", ""), u.value("companyName", ""));
                        }
                    }

                    // Fallback if user not found
                    return User();
                }()),
        major(_.value("major", "")), yearsOfExperience(_.value("yearsOfExperience", 0.0)),
        skills(_.value("skills", std::vector<std::string>{})),
        appliedJobs(_.value("appliedJobs", std::vector<std::string>{})),
        notifications(_.value("notifications", std::vector<std::string>{})) {

        fetchData();
    }


    [[nodiscard]] string getRole() const override { return "jobseeker"; }
    [[nodiscard]] vector<string> getAppliedJobs(const string &filename = "jobSeekers.json") const {
        json data = PBLJson::loadList(filename);
        return data[id].value("appliedJobs", vector<string>{});
    }

    // CV Management
    void updateCVOnline(const string &maj, float yoe) {
        major = maj;
        yearsOfExperience = yoe;
    }

    void addSkill(const string &skill) { skills.push_back(skill); }
    void clearSkills() { skills.clear(); }

    // Getters
    [[nodiscard]] string getCVOnline() const {
        return "Ho ten: " + fullName + "\nTuoi: " + to_string(getAge()) + "\nChuyen nganh: " + major +
               "\nKinh nghiem: " + to_string(yearsOfExperience) + " nam";
    }
    [[nodiscard]] string getMajor() const { return major; }
    [[nodiscard]] int getYearsOfExperience() const { return yearsOfExperience; }
    vector<string> getSkills() const { return skills; }

    // Ứng tuyển
    void applyForJob(const string &jobId) { appliedJobs.push_back(jobId); }

    // Thông báo
    void addNotification(const string &msg) { notifications.push_back(msg); }
    vector<string> getNotifications() const { return notifications; }
    void clearNotifications() { notifications.clear(); }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Vai tro: Ung vien\n";
        if (getAge() > 0) {
            cout << "\n--- THONG TIN CV ---\n";
            cout << "Tuoi: " << getAge() << "\n";
            cout << "Chuyen nganh: " << major << "\n";
            cout << "Kinh nghiem: " << yearsOfExperience << " nam\n";
            if (!skills.empty()) {
                cout << "Ky nang: ";
                for (size_t i = 0; i < skills.size(); i++) {
                    cout << skills[i];
                    if (i < skills.size() - 1)
                        cout << ", ";
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

    static JobSeeker getJobSeekerById(const string &id, const string &filename = "jobSeekers.json") {
        json data = PBLJson::loadList(filename);
        return JobSeeker(data[id], id);
    }

    void addAppliedJob(const string &jobId, const string &filename = "jobSeekers.json") const {
        json data = PBLJson::loadList(filename);

        if (!data[id].contains("appliedJobs") || !data[id]["appliedJobs"].is_array()) {
            data[id]["appliedJobs"] = json::array();
        }

        data[id]["appliedJobs"].push_back(jobId);

        PBLJson::saveList(data, filename);
    }
};

#endif
