#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <map>
#include "JobSeeker.h"
#include "Employer.h"
#include "Admin.h"
#include "Job.h"
using namespace std;

class System {
private:
    vector<JobSeeker> jobSeekers;
    vector<Employer> employers;
    vector<Admin> admins;
    vector<Job> jobs;

    int nextJobId;

    // Menu functions
    void jobSeekerMenu(JobSeeker &js);
    void employerMenu(Employer &e);
    void adminMenu(Admin &a);

    // Job functions
    void searchJobs(JobSeeker &js);
    void applyForJob(JobSeeker &js);
    void viewAppliedJobs(JobSeeker &js);
    void updateProfile(JobSeeker &js);

    // Employer functions
    void createJob(Employer &e);
    void viewMyJobs(Employer &e);
    void editJob(Employer &e);
    void deleteJob(Employer &e);

    // Admin functions
    void viewAllJobs();
    void approveJobs();
    void viewStatistics();

    // Helper functions
    string generateJobId();
    Job* findJobById(string jobId);

public:
    System() : nextJobId(1) {}

    void run();
    void registerUser();
    void loginUser();
};

#endif