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
public:
    vector<JobSeeker> jobSeekers;
    vector<Employer> employers;
    Admin admin;
    vector<Job> jobs;
    vector<Application> applications;

    int latestJobId;
    int nextAppId;

    // ========== MENU FUNCTIONS ==========
    void adminMenu(const Admin &a);
    void employerMenu(Employer &e);
    static void jobSeekerMenu(JobSeeker &js);

    // ========== ADMIN FUNCTIONS ==========
    // Quản lý danh sách
    void admin_ViewAllUsers();

    //  Quản lý tài khoản nhà tuyển dụng
    // void admin_UpdateEmployer();
    void admin_DeleteEmployer();

    //  Quản lý danh mục
    static void admin_removeUser();

    //  Quản lý tin tuyển dụng
    static void admin_ManageJobs() ;
    static void admin_FindJobs() ;
    void admin_EditJob();
    static void admin_removeJob();

    // Thống kê
    void admin_RemoveJob();

    // ========== EMPLOYER FUNCTIONS ==========
    //  Quản lý tin tuyển dụng
    void employer_PostJob(Employer &e);
    static void employer_ViewMyJobs(Employer &e);
    static void employer_EditJob(Employer &e);
    static void employer_DeleteJob(Employer &e);

    //  Quản lý ứng viên
    static void employer_ViewApplications(Employer &e);
    void employer_FilterApplications(Employer &e);
    static void employer_CloseJob(Employer &e);

    //  Quản lý tài khoản
    static void employer_UpdateProfile(Employer &e);
    void employer_ViewHistory(Employer &e);

    // ========== JOBSEEKER FUNCTIONS ==========
    //  Quản lý hồ sơ
    static void jobseeker_UpdateCV(JobSeeker &js);
    void jobseeker_UploadCVFile(JobSeeker &js);
    static void jobseeker_UpdateProfile(JobSeeker &js);

    // Tìm kiếm việc làm
    static void jobseeker_SearchJobs(JobSeeker &js);
    void jobseeker_ViewJobDetail(JobSeeker &js);

    // Ứng tuyển
    static void jobseeker_ApplyJob(JobSeeker &js);
    static void jobseeker_ViewAppliesStatus(JobSeeker &js);

    //  Quản lý tài khoản
    void jobseeker_ViewHistory(JobSeeker &js);
    // void jobseeker_ViewNotifications(JobSeeker &js);

    // ========== HELPER FUNCTIONS ==========
    static string generateJobId();
    string generateAppId();
    JobSeeker *findJobSeekerById(const string &id);
    Employer *findEmployerById(const string &id);
    void initializeDefaultAdmin();
    // void sendNotificationToJobSeeker(const string& jobSeekerId, const string& message);

    explicit System(const Admin &admin);
    System();

    void run();
    static void registerUser();
    void loginUser();
};

#endif
