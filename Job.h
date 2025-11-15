#ifndef JOB_H
#define JOB_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

enum JobType {
    FULL_TIME,
    PART_TIME
};

class Job {
private:
    string jobId;
    string employerId;

    // Thông tin cơ bản
    string title;
    string description;      // Mô tả công việc

    // 2.1. Tiêu chí
    string category;         // Ngành nghề
    string location;         // Địa điểm
    JobType jobType;
    double minSalary;
    double maxSalary;
    int minExperience;       // Số năm kinh nghiệm tối thiểu
    int minAge;
    int maxAge;

    // 1.4. Quản lý
    bool isApproved;
    string status;           // "active", "closed"
    string postedDate;

public:
    Job() : jobType(FULL_TIME), minSalary(0), maxSalary(0),
            minExperience(0), minAge(18), maxAge(100),
            isApproved(false), status("active") {}

    Job(string jobId, string employerId, string title)
        : jobId(jobId), employerId(employerId), title(title),
          jobType(FULL_TIME), minSalary(0), maxSalary(0),
          minExperience(0), minAge(18), maxAge(100),
          isApproved(false), status("active") {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        postedDate = to_string(ltm->tm_mday) + "/" +
                     to_string(1 + ltm->tm_mon) + "/" +
                     to_string(1900 + ltm->tm_year);
    }

    // Getters
    string getJobId() const { return jobId; }
    string getEmployerId() const { return employerId; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    string getCategory() const { return category; }
    string getLocation() const { return location; }
    JobType getJobType() const { return jobType; }
    double getMinSalary() const { return minSalary; }
    double getMaxSalary() const { return maxSalary; }
    int getMinExperience() const { return minExperience; }
    int getMinAge() const { return minAge; }
    int getMaxAge() const { return maxAge; }
    bool getIsApproved() const { return isApproved; }
    string getStatus() const { return status; }
    string getPostedDate() const { return postedDate; }

    // Setters
    void setTitle(const string& t) { title = t; }
    void setDescription(const string& desc) { description = desc; }
    void setCategory(const string& cat) { category = cat; }
    void setLocation(const string& loc) { location = loc; }
    void setJobType(JobType type) { jobType = type; }
    void setSalaryRange(double min, double max) {
        minSalary = min;
        maxSalary = max;
    }
    void setMinExperience(int exp) { minExperience = exp; }
    void setAgeRange(int min, int max) { minAge = min; maxAge = max; }
    void setApproved(bool approved) { isApproved = approved; }
    void setStatus(const string& st) { status = st; }

    string getJobTypeString() const {
        return (jobType == FULL_TIME) ? "Full-time" : "Part-time";
    }

    void display() const {
        cout << "\n========================================\n";
        cout << "Job ID: " << jobId << "\n";
        cout << "Tieu de: " << title << "\n";
        cout << "Nganh nghe: " << category << "\n";
        cout << "Dia diem: " << location << "\n";
        cout << "Loai hinh: " << getJobTypeString() << "\n";
        cout << "Muc luong: " << minSalary << " - " << maxSalary << " VND\n";
        cout << "Kinh nghiem: " << minExperience << "+ nam\n";
        cout << "Do tuoi: " << minAge << " - " << maxAge << "\n";
        cout << "Mo ta: " << description << "\n";
        cout << "Trang thai: " << (isApproved ? "Da duyet" : "Cho duyet")
             << " | " << status << "\n";
        cout << "Ngay dang: " << postedDate << "\n";
        cout << "========================================\n";
    }
};

#endif