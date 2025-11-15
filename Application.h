#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <ctime>
using namespace std;

enum ApplicationStatus {
    PENDING,      // Chờ xử lý
    VIEWED,       // Đã xem
    PROCESSING,   // Đang xử lý
    INTERVIEW,    // Phỏng vấn
    ACCEPTED,     // Chấp nhận
    REJECTED      // Từ chối
};

class Application {
private:
    string applicationId;
    string jobSeekerId;
    string jobId;
    string cvContent;
    ApplicationStatus status;
    string appliedDate;

public:
    Application(string appId, string jsId, string jId, string cv)
        : applicationId(appId), jobSeekerId(jsId), jobId(jId),
          cvContent(cv), status(PENDING) {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        appliedDate = to_string(ltm->tm_mday) + "/" +
                      to_string(1 + ltm->tm_mon) + "/" +
                      to_string(1900 + ltm->tm_year);
    }

    // Getters
    string getApplicationId() const { return applicationId; }
    string getJobSeekerId() const { return jobSeekerId; }
    string getJobId() const { return jobId; }
    string getCVContent() const { return cvContent; }
    ApplicationStatus getStatus() const { return status; }
    string getAppliedDate() const { return appliedDate; }

    // Setters
    void setStatus(ApplicationStatus st) { status = st; }

    string getStatusString() const {
        switch(status) {
            case PENDING: return "Cho xu ly";
            case VIEWED: return "Da xem";
            case PROCESSING: return "Dang xu ly";
            case INTERVIEW: return "Phong van";
            case ACCEPTED: return "Chap nhan";
            case REJECTED: return "Tu choi";
            default: return "Unknown";
        }
    }

    void display() const {
        cout << "\n--- Don ung tuyen " << applicationId << " ---\n";
        cout << "Job ID: " << jobId << "\n";
        cout << "Ngay nop: " << appliedDate << "\n";
        cout << "Trang thai: " << getStatusString() << "\n";
        cout << "CV:\n" << cvContent << "\n";
    }
};

#endif