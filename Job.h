#ifndef JOB_H
#define JOB_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "json.h"
using namespace std;

inline pmr::unordered_map<int, string> jobTypeInterface = {
        {1, "Full-time"},
        {2, "Part-time"},
};

inline pmr::unordered_map<int, string> placeOfWorkInterface = {
        {1, "On-site"},
        {2, "Hybrid"},
        {3, "Remote"},
};


class Job {
private:
    string id;
    string employerId;

    // Thông tin cơ bản
    string title;
    string description; // Mô tả công việc

    // 2.1. Tiêu chí
    int jobType;
    vector<int> placeOfWork;
    double minSalary, maxSalary;
    int minExperience; // Số năm kinh nghiệm tối thiểu
    int minAge;
    int maxAge;

    // 1.4. Quản lý
    bool status; // "active", "closed"
    string postedDate;

public:
    Job() : jobType(), minSalary(0), maxSalary(0), minExperience(0), minAge(18), maxAge(100), status(true) {}

    Job(string Id, string employerId, string title) :
        id(Id), employerId(employerId), title(title), jobType(1), placeOfWork(1), minSalary(), maxSalary(),
        minExperience(0), minAge(18), maxAge(100), status(true) {

        const std::time_t now = std::time(nullptr);
        const std::tm *gmt = std::gmtime(&now); // UTC time

        std::ostringstream oss;
        oss << std::put_time(gmt, "%Y-%m-%dT%H:%M:%SZ");
        postedDate = oss.str();
    }

    explicit Job(const json &_) :
        id(_.value("id", "")), employerId(_.value("employerId", "")), title(_.value("title", "")),
        description(_.value("description", "")), jobType(_.value("jobType", 1)),
        placeOfWork(_.value("placeOfWork", vector{1})), minSalary(_.value("minSalary", 0)),
        maxSalary(_.value("maxSalary", 0)), minExperience(_.value("minExperience", 0)), minAge(_.value("minAge", 0)),
        maxAge(_.value("maxAge", 0)), status(_.value("status", true)), postedDate(_.value("postedDate", "")) {}

    // Getters
    [[nodiscard]] string getJobId() const { return id; }
    [[nodiscard]] string getEmployerId() const { return employerId; }
    [[nodiscard]] string getTitle() const { return title; }
    [[nodiscard]] string getDescription() const { return description; }
    [[nodiscard]] string getJobType() const { return jobTypeInterface[jobType]; }
    [[nodiscard]] vector<string> getPlaceOfWork() const {
        vector<string> result;
        for (auto &i: placeOfWork)
            result.push_back(placeOfWorkInterface[i]);
        return result;
    }
    [[nodiscard]] double getMinSalary() const { return minSalary; }
    [[nodiscard]] double getMaxSalary() const { return maxSalary; }
    [[nodiscard]] string getSalaryRange() const { return to_string(minSalary) + " - " + to_string(maxSalary); }
    [[nodiscard]] int getMinExperience() const { return minExperience; }
    [[nodiscard]] int getMinAge() const { return minAge; }
    [[nodiscard]] int getMaxAge() const { return maxAge; }
    [[nodiscard]] bool getStatus() const { return status; }
    [[nodiscard]] string getPostedDate() const { return postedDate; }

    // Setters
    void setTitle(const string &t) { title = t; }
    void setDescription(const string &desc) { description = desc; }
    void setJobType(const int type) { jobType = type; }
    void setPlaceOfWork(const vector<int> &pl) { placeOfWork = pl; }
    void setSalaryRange(const double _minSalary, const double _maxSalary) {
        minSalary = _minSalary;
        maxSalary = _maxSalary;
    }
    void setMinExperience(int exp) { minExperience = exp; }
    void setAgeRange(int min, int max) {
        minAge = min;
        maxAge = max;
    }
    void setStatus(const string &_status) { status = _status == "Active"; }
    void setStatus(const bool &_status) { status = _status; }

    [[nodiscard]] string getJobTypeString() const { return jobTypeInterface[jobType]; }

    void display() const {
        cout << "\n========================================\n";
        cout << "Tieu de: " << title << "\n";
        cout << "Loai hinh: " << getJobTypeString() << "\n";
        cout << "Muc luong: " << std::fixed << std::setprecision(2) << minSalary << " - " << maxSalary << " VND\n";
        cout << "Kinh nghiem: " << minExperience << "+ nam\n";
        cout << "Do tuoi: " << minAge << " - " << maxAge << "\n";
        cout << "Mo ta: " << description << "\n";
        cout << "Ngay dang: " << postedDate << "\n";
        cout << "========================================\n";
    }

    static vector<Job> getAllActiveJob(const string &filename = "jobs.json") {
        vector<Job> result;
        json data = PBLJson::loadList(filename);

        for (auto &j: data) {
            if (j.value("status", true) == true) {
                auto _ = Job(j);
                result.emplace_back(_);
            }
        }
        return result;
    }

    static Job getJobById(const string &id, const string &filename = "jobs.json") {
        json data = PBLJson::loadList(filename);

        for (auto &j: data) {
            if (!j.value("id", "").empty() && j.value("id", "") == id) {
                return Job(j);
            }
        }
        return {};
    }

    static vector<Job> getJobByEmployerId(const string &id, const string &filename = "jobs.json") {
        vector<Job> result;
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("employerId", "").empty() && j.value("EmployerId", "") == id) {
                auto _ = Job(j);
                result.emplace_back(_);
            }
        }
        return result;
    }
};

#endif
