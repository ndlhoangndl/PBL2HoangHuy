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
    bool updated;

public:
    JobSeeker(string id, string username, string password, string email, string phone, bool isActive, bool _updated = false) :
        User(id, username, password, email, phone, isActive), yearsOfExperience(0), updated(_updated) {
        ensureJobSeekerExists();
        fetchData();
    };

    explicit JobSeeker(const User &user) :
        User(user.id, user.username, user.password, user.email, user.phone, user.fullName, user.dateOfBirth,
             user.isActive, user.createdAt, user.role, user.companyName),
        yearsOfExperience(0) {
        ensureJobSeekerExists();
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
        json data = PBLJson::loadAny(filename);
        return data[id].value("appliedJobs", vector<string>{});
    }
    [[nodiscard]] bool getUpdated() const { return updated; }

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
    [[nodiscard]] float getYearsOfExperience() const { return yearsOfExperience; }
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
            cout << "Ky nang: ";
            for (size_t i = 0; i < skills.size(); i++) {
                cout << skills[i];
                if (i < skills.size() - 1)
                    cout << ", ";
            }
            cout << "\n";
        }
    }

    // DATA PROCESSOR
    void ensureJobSeekerExists(const string &filename = "jobSeekers.json") const {
        json data = PBLJson::loadAny(filename);

        if (!data.contains(id)) {
            data[id] = {{"yearsOfExperience", 0.0},
                        {"major", ""},
                        {"skills", json::array()},
                        {"appliedJobs", json::array()},
                        {"notifications", {"Welcome to our app, feel free exploring and giving us feedbacks."}},
                        {"updated", false}};

            PBLJson::saveAny(data, filename);
        }
    }

    void fetchData(const string &filename = "jobSeekers.json") {
        json data = PBLJson::loadAny(filename);

        if (!data.contains(id) || !data[id].is_object()) {
            yearsOfExperience = 0;
            major = "";
            skills.clear();
            notifications.clear();
            appliedJobs.clear();
            updated = false;
            return;
        }

        const json &js = data[id];

        yearsOfExperience = js.value("yearsOfExperience", 0.0);
        major = js.value("major", "");
        skills = js.value("skills", vector<string>{});
        notifications = js.value("notifications", vector<string>{});
        appliedJobs = js.value("appliedJobs", vector<string>{});
        updated = js.value("updated", false);
    }


    void deleteNotifications(const string &filename = "jobSeekers.json") const {
        vector<string> _notifications = {};

        json data = PBLJson::loadAny(filename);
        data[id]["notifications"].update(_notifications);
        PBLJson::saveAny(data, filename);
    }

    void updateCV(const string &filename = "jobSeekers.json") {
        json data = PBLJson::loadAny(filename);
        data[id].update({
                {"major", major},
                {"yearsOfExperience", yearsOfExperience},
                {"skills", skills},
                {"updated", true}
        });

        PBLJson::saveAny(data, filename);
        fetchData();
    };

    static JobSeeker getJobSeekerById(const string &id, const string &filename = "jobSeekers.json") {
        json data = PBLJson::loadAny(filename);
        return JobSeeker(data[id], id);
    }

    void addAppliedJob(const string &jobId, const string &filename = "jobSeekers.json") const {
        json data = PBLJson::loadAny(filename);

        if (!data[id].contains("appliedJobs") || !data[id]["appliedJobs"].is_array()) {
            data[id]["appliedJobs"] = json::array();
        }

        data[id]["appliedJobs"].push_back(jobId);

        PBLJson::saveAny(data, filename);
    }
};

#endif
