#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include "Admin.h"
#include "Application.h"
#include "Employer.h"
#include "Job.h"
#include "JobSeeker.h"


class Job;
using namespace std;

class System {
private:
    vector<JobSeeker> jobSeekers;
    vector<Employer> employers;
    Admin admin;
    vector<Job> jobs;
    vector<Application> applications;

    vector<string> categories;  // Ngành nghề
    vector<string> locations;   // Khu vực

    int nextJobId;
    int nextAppId;

    // ========== MENU FUNCTIONS ==========
    void adminMenu(Admin &a);
    void employerMenu(Employer &e);
    void jobSeekerMenu(JobSeeker &js);

    // ========== ADMIN FUNCTIONS ==========
    // Quản lý danh sách
    void admin_ViewAllUsers();

    //  Quản lý tài khoản nhà tuyển dụng
    // void admin_UpdateEmployer();
    void admin_DeleteEmployer();

    //  Quản lý danh mục
    void admin_ManageCategories();

    //  Quản lý tin tuyển dụng
    void admin_ManageJobs();
    void admin_ApproveJobs();
    void admin_EditJob();
    void admin_DeleteJob();

    // Thống kê
    void admin_ViewStatistics();

    // ========== EMPLOYER FUNCTIONS ==========
    //  Quản lý tin tuyển dụng
    void employer_PostJob(Employer &e);
    void employer_ViewMyJobs(Employer &e);
    void employer_EditJob(Employer &e);
    void employer_DeleteJob(Employer &e);

    //  Quản lý ứng viên
    void employer_ViewApplications(Employer &e);
    void employer_FilterApplications(Employer &e);
    void employer_SelectCandidate(Employer &e);

    //  Quản lý tài khoản
    void employer_UpdateProfile(Employer &e);
    void employer_ViewHistory(Employer &e);

    // ========== JOBSEEKER FUNCTIONS ==========
    //  Quản lý hồ sơ
    void jobseeker_UpdateCV(JobSeeker &js);
    void jobseeker_UploadCVFile(JobSeeker &js);
    void jobseeker_UpdateProfile(JobSeeker &js);

    //Tìm kiếm việc làm
    void jobseeker_SearchJobs(JobSeeker &js);
    void jobseeker_ViewJobDetail(JobSeeker &js);

    // Ứng tuyển
    void jobseeker_ApplyJob(JobSeeker &js);
    void jobseeker_ViewApplicationStatus(JobSeeker &js);

    //  Quản lý tài khoản
    void jobseeker_ViewHistory(JobSeeker &js);
    // void jobseeker_ViewNotifications(JobSeeker &js);

    // ========== HELPER FUNCTIONS ==========
    string generateJobId();
    string generateAppId();
    Job* findJobById(const string &jobId);
    JobSeeker* findJobSeekerById(const string &id);
    Employer* findEmployerById(const string &id);
    void initializeDefaultAdmin();
    void initializeDefaultCategories();
    // void sendNotificationToJobSeeker(const string& jobSeekerId, const string& message);

public:
    explicit System(const Admin & admin);
    System();

    void run();
    void registerUser();
    void loginUser();
};

#endif