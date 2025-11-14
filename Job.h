#ifndef JOB_H
#define JOB_H

#include <string>
#include <iostream>
using namespace std;

class Job {
private:
    string jobId;
    string title;
    string employerId;
    string description;
    string location;
    double salary;
    string category;
    bool isApproved;

public:
    Job() : salary(0.0), isApproved(false) {}

    Job(string jobId, string title, string employerId,
        string description = "", string location = "",
        double salary = 0.0, string category = "")
        : jobId(jobId), title(title), employerId(employerId),
          description(description), location(location),
          salary(salary), category(category), isApproved(false) {}

    // Getters
    string getJobId() const { return jobId; }
    string getTitle() const { return title; }
    string getEmployerId() const { return employerId; }
    string getDescription() const { return description; }
    string getLocation() const { return location; }
    double getSalary() const { return salary; }
    string getCategory() const { return category; }
    bool getIsApproved() const { return isApproved; }

    // Setters
    void setApproved(bool status) { isApproved = status; }
    void setDescription(string desc) { description = desc; }
    void setLocation(string loc) { location = loc; }
    void setSalary(double sal) { salary = sal; }

    void display() const;
};

#endif