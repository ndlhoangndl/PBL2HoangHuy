#ifndef JOB_H
#define JOB_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "JobSeeker.h"
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
    float minExperience; // Số năm kinh nghiệm tối thiểu
    int minAge;
    int maxAge;
    vector<string> applicants;
    vector<string> accepted;

    // 1.4. Quản lý
    bool status; // "active", "closed"
    string postedDate;

public:
    Job() :
        jobType(), minSalary(0), maxSalary(0), minExperience(0), minAge(18), maxAge(100), applicants({}), accepted({}),
        status(true) {}

    Job(string Id, string employerId, string title) :
        id(Id), employerId(employerId), title(title), jobType(1), placeOfWork(1), minSalary(), maxSalary(),
        minExperience(0), minAge(18), maxAge(100), applicants({}), accepted({}), status(true) {

        const std::time_t now = std::time(nullptr);
        const std::tm *gmt = std::gmtime(&now); // UTC time

        std::ostringstream oss;
        oss << std::put_time(gmt, "%Y-%m-%dT%H:%M:%SZ");
        postedDate = oss.str();
    }

    explicit Job(const json &_) :
        id(_.value("id", "")), employerId(_.value("employerId", "")), title(_.value("title", "")),
        description(_.value("description", "")), jobType(_.value("jobType", 1)),
        placeOfWork(_.value("placeOfWork", vector<int>{})), minSalary(_.value("minSalary", 0)),
        maxSalary(_.value("maxSalary", 0)), minExperience(_.value("minExperience", 0.0)), minAge(_.value("minAge", 0)),
        maxAge(_.value("maxAge", 0)), applicants(_.value("applicants", vector<string>{})),
        accepted(_.value("accepted", vector<string>{})), status(_.value("status", true)),
        postedDate(_.value("postedDate", "")) {}

    // Getters
    [[nodiscard]] string getJobId() const { return id; }
    [[nodiscard]] string getEmployerId() const { return employerId; }
    [[nodiscard]] string getTitle() const { return title; }
    [[nodiscard]] string getDescription() const { return description; }
    [[nodiscard]] string getJobType() const { return jobTypeInterface[jobType]; }
    [[nodiscard]] int getJobTypeId() const { return jobType; }
    [[nodiscard]] vector<string> getPlaceOfWork() const {
        vector<string> result;
        for (auto &i: placeOfWork)
            result.push_back(placeOfWorkInterface[i]);
        return result;
    }
    [[nodiscard]] vector<int> getPlaceOfWorkId() const { return placeOfWork; }
    [[nodiscard]] double getMinSalary() const { return minSalary; }
    [[nodiscard]] double getMaxSalary() const { return maxSalary; }
    [[nodiscard]] string getSalaryRange() const { return to_string(minSalary) + " - " + to_string(maxSalary); }
    [[nodiscard]] float getMinExperience() const { return minExperience; }
    [[nodiscard]] int getMinAge() const { return minAge; }
    [[nodiscard]] int getMaxAge() const { return maxAge; }
    [[nodiscard]] vector<string> getApplicants() const { return applicants; }
    [[nodiscard]] vector<string> getAccepted() const { return accepted; }
    [[nodiscard]] bool getStatus() const { return status; }
    [[nodiscard]] string getPostedDate() const { return postedDate; }
    [[nodiscard]] static int getLatestJobId() { return PBLJson::getLatestIndex("jobs.json"); }
    [[nodiscard]] vector<string> getApplicantsId() { return applicants; }

    // Setters
    void setTitle(const string &t) { title = t; }
    void setDescription(const string &desc) { description = desc; }
    void setJobType(const int type) { jobType = type; }
    void setPlaceOfWork(const vector<int> &pl) { placeOfWork = pl; }
    void setSalaryRange(const double _minSalary, const double _maxSalary) {
        minSalary = _minSalary;
        maxSalary = _maxSalary;
    }
    void setMinExperience(float exp) { minExperience = exp; }
    void setAgeRange(int min, int max) {
        minAge = min;
        maxAge = max;
    }
    void setStatus(const string &_status) { status = _status == "Active"; }
    void setStatus(const bool &_status) { status = _status; }
    void setApplicants(const vector<string> &_applicants = {}) { applicants = _applicants; }
    void setAccepted(const vector<string> &_accepted = {}) { accepted = _accepted; }

    [[nodiscard]] string getJobTypeString() const { return jobTypeInterface[jobType]; }

    void display() const;
    void acceptedView() const;
    void notAcceptedView() const;

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

    static vector<Job> getAllJob(const string &filename = "jobs.json") {
        vector<Job> result;
        json data = PBLJson::loadList(filename);

        for (auto &j: data) {
            auto _ = Job(j);
            result.emplace_back(_);
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

    static vector<Job> getJobsByEmployerId(const string &id, const string &filename = "jobs.json") {
        vector<Job> result;
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("employerId", "").empty() && j.value("employerId", "") == id) {
                auto _ = Job(j);
                result.emplace_back(_);
            }
        }

        return result;
    }

    static vector<Job> getJobByEmployerCompanyName(const string &companyName, const string &filenameJobs = "jobs.json",
                                                   const string &filenameUsers = "users.json") {
        vector<Job> result;
        json jobsData = PBLJson::loadList(filenameJobs);
        json usersData = PBLJson::loadList(filenameUsers);

        for (auto &u: usersData) {
            if (u.value("role", "") == "employer" && !u.value("companyName", "").empty() &&
                u.value("companyName", "") == companyName) {
                for (auto &j: jobsData) {
                    if (j.value("employerId", "") == u.value("employerId", "")) {
                        auto _ = Job(j);
                        result.emplace_back(_);
                    }
                }
            }
        }

        return result;
    }

    static vector<Job> getJobByEmployerEmail(const string &employerEmail, const string &filenameJobs = "jobs.json",
                                             const string &filenameUsers = "users.json") {
        vector<Job> result;
        json jobsData = PBLJson::loadList(filenameJobs);
        json usersData = PBLJson::loadList(filenameUsers);

        for (auto &u: usersData) {
            if (u.value("role", "") == "employer" && !u.value("email", "").empty() &&
                u.value("email", "") == employerEmail) {
                for (auto &j: jobsData) {
                    if (j.value("employerId", "") == u.value("employerId", "")) {
                        auto _ = Job(j);
                        result.emplace_back(_);
                    }
                }
            }
        }

        return result;
    }

    static void editJob(const Job &editedJob, const string &filename = "jobs.json") {
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("id", "").empty() && j.value("id", "") == editedJob.getJobId()) {
                j.update({{"title", editedJob.getTitle()},
                          {"description", editedJob.getDescription()},
                          {"jobType", editedJob.getJobTypeId()},
                          {"placeOfWork", editedJob.getPlaceOfWorkId()},
                          {"minSalary", editedJob.getMinSalary()},
                          {"maxSalary", editedJob.getMaxSalary()},
                          {"minExperience", editedJob.getMinExperience()},
                          {"minAge", editedJob.getMinAge()},
                          {"maxAge", editedJob.getMaxAge()},
                          {"status", editedJob.getStatus()}});
                PBLJson::saveList(data, filename);
                return;
            }
        }
    }

    void addApplicant(const string &applicantId, const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);

        for (auto &j: data) {
            if (j.value("id", "") == id) {

                if (!j.contains("applicants") || !j["applicants"].is_array())
                    j["applicants"] = json::array();

                j["applicants"].push_back(applicantId);

                PBLJson::saveList(data, filename);
                return;
            }
        }
    }


    void addAccepted(const string &applicantId, const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("id", "").empty() && j.value("id", "") == id) {
                vector<string> acceptedIds = j.value("accepted", vector<string>{});
                acceptedIds.push_back(applicantId);
                j.update({"accepted", acceptedIds});
                PBLJson::saveList(data, filename);
                return;
            }
        }
    }

    void addAccepted(const vector<string> &applicantIds, const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("id", "").empty() && j.value("id", "") == id) {
                vector<string> acceptedIds = j.value("accepted", vector<string>{});
                for (auto &i: applicantIds) {
                    acceptedIds.push_back(i);
                }
                j.update({"accepted", acceptedIds});
                PBLJson::saveList(data, filename);
                return;
            }
        }
    }

    void removeAccepted(const string &applicantId, const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("id", "").empty() && j.value("id", "") == id) {
                vector<string> acceptedIds = j.value("accepted", vector<string>{});
                for (int i = 0; i < acceptedIds.size(); i++) {
                    if (applicantId == acceptedIds[i]) {
                        acceptedIds.erase(acceptedIds.begin() + i);
                        j.update({"accepted", acceptedIds});
                        PBLJson::saveList(data, filename);
                        return;
                    }
                }
                j.update({"accepted", acceptedIds});
                PBLJson::saveList(data, filename);
                return;
            }
        }
    }

    void removeAccepted(vector<string> &applicantIds, const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("id", "").empty() && j.value("id", "") == id) {
                vector<string> acceptedIds = j.value("accepted", vector<string>{});
                std::sort(applicantIds.begin(), applicantIds.end(), [](const std::string& a, const std::string& b) {
                    try {
                        if (a.empty()) return false;
                        if (b.empty()) return true;

                        return std::stoi(a) < std::stoi(b);
                    } catch (...) {
                        return a < b;
                    }
                });
                sort(acceptedIds.begin(), acceptedIds.end(), [](const std::string& a, const std::string& b) {
                    try {
                        if (a.empty()) return false;
                        if (b.empty()) return true;

                        return std::stoi(a) < std::stoi(b);
                    } catch (...) {
                        return a < b;
                    }
                });
                for (int i = 0, j = 0; i < acceptedIds.size(); i++) {
                    if (j == applicantIds.size()) break;
                    if (applicantIds[j] == applicantIds[j+1]) {
                        j++;
                        i--;
                        continue;
                    }
                    if (acceptedIds[i] == applicantIds[j]) {
                        acceptedIds.erase(acceptedIds.begin() + i);
                        j++;
                    }
                }
                j.update({"accepted", acceptedIds});
                PBLJson::saveList(data, filename);
                return;
            }
        }
    }

    void remove(const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);

        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it->value("id", "") == id) {
                data.erase(it);
                break;
            }
        }

        PBLJson::saveList(data, filename);
    }

    void postJob(const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);

        data.push_back({{"id", id},
                        {"employerId", employerId},
                        {"title", title},
                        {"description", description},
                        {"jobType", jobType},
                        {"placeOfWork", placeOfWork},
                        {"minSalary", minSalary},
                        {"maxSalary", maxSalary},
                        {"minExperience", minExperience},
                        {"minAge", minAge},
                        {"maxAge", maxAge},
                        {"applicants", applicants},
                        {"accepted", accepted},
                        {"status", status},
                        {"postedDate", postedDate}});

        PBLJson::saveList(data, filename);
    }
    // 0: accepted | -1: not accepted | 1: waiting for reviews
    [[nodiscard]] int checkApplyStatus(const string &applicantId, const string &filename = "jobs.json") const {
        if (std::any_of(accepted.begin(), accepted.end(),
                        [&](const string &acceptedId) { return acceptedId == applicantId; }))
            return 0;
        if (!status)
            return -1;
        return 1;
    }

    [[nodiscard]] vector<JobSeeker> getApplications(const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("id", "").empty() && j.value("id", "") == id) {
                vector<JobSeeker> applications;
                for (auto &i: j.value("applicants", vector<string>{})) {
                    applications.push_back(JobSeeker::getJobSeekerById(i));
                }
                return applications;
            }
        }
        return {};
    };

    // end = start + offset
    [[nodiscard]] vector<JobSeeker> getApplicationsByIndex(int start, int offset,
                                                           const string &filename = "jobs.json") const {
        json data = PBLJson::loadList(filename);
        for (auto &j: data) {
            if (!j.value("id", "").empty() && j.value("id", "") == id) {
                vector<JobSeeker> applications;
                for (int i = start; i < start + offset; ++i) {
                    if (i == j.value("applicants", vector<string>{}).size())
                        return applications;
                    applications.push_back(JobSeeker::getJobSeekerById(j.value("applicants", vector<string>{}).at(i)));
                }
                return applications;
            }
        }
        return {};
    };

    // ########## FILTERING TERRITORY ########## //

    enum class FilterType { Experience, Age };

    enum class SortType { ASC, DESC, RANGE };

    [[nodiscard]] vector<JobSeeker> getApplicationsFiltered(FilterType filterType, SortType sortType, float minExp = -1,
                                                            int _minAge = -1, int _maxAge = -1,
                                                            const string &filename = "jobs.json") const {
        vector<JobSeeker> applications = getApplications(filename);
        auto cmp = [&](const JobSeeker &a, const JobSeeker &b) {
            switch (filterType) {
                case FilterType::Experience:
                    return sortType == SortType::DESC ? a.getYearsOfExperience() > b.getYearsOfExperience()
                                                      : a.getYearsOfExperience() < b.getYearsOfExperience();

                case FilterType::Age:
                    return sortType == SortType::DESC ? a.getAge() > b.getAge() : a.getAge() < b.getAge();
            }
            return false;
        };
        sort(applications.begin(), applications.end(), cmp);

        if (sortType == SortType::RANGE) {
            for (int i = 0; i < applications.size(); ++i) {
                if ((filterType == FilterType::Age &&
                     (applications.at(i).getAge() < _minAge || applications.at(i).getAge() > _maxAge)) ||
                    (filterType == FilterType::Experience && applications.at(i).getYearsOfExperience() < minExp))
                    applications.erase(applications.begin() + i);
            }
        }

        return applications;
    }
};

#endif
