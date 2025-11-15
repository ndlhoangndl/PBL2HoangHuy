#include "System.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <regex>
#include "hash.h"
#include "json.h"
using namespace std;

// ========== CONSTRUCTOR ==========
System::System()
    : defaultAdmin("102240308", "ndlhoang", "ndlhoang", "ndlhoangndl@gmail.com", "0852237258"),
      nextJobId(1), nextAppId(1) {
    initializeDefaultAdmin();
    initializeDefaultCategories();
}

void System::initializeDefaultAdmin() {
    defaultAdmin.setFullName("Quan Tri Vien");

}

void System::initializeDefaultCategories() {
    // Ngành nghề
    categories = {"IT", "Marketing", "Kinh doanh", "Ke toan", "Nhan su",
                  "Xay dung", "Y te", "Giao duc", "Du lich", "Thiet ke"};

    // Khu vực
    locations = {"Ha Noi", "Ho Chi Minh", "Da Nang", "Hai Phong",
                 "Can Tho", "Binh Duong", "Dong Nai", "Hai Duong"};
}

// ========== HELPER FUNCTIONS ==========
string System::generateJobId() {
    return "JOB" + to_string(nextJobId++);
}

string System::generateAppId() {
    return "APP" + to_string(nextAppId++);
}

Job* System::findJobById(const string& jobId) {
    for(auto &job : jobs) {
        if(job.getJobId() == jobId) {
            return &job;
        }
    }
    return nullptr;
}

JobSeeker* System::findJobSeekerById(const string& id) {
    for(auto &js : jobSeekers) {
        if(js.getId() == id) {
            return &js;
        }
    }
    return nullptr;
}

Employer* System::findEmployerById(const string& id) {
    for(auto &emp : employers) {
        if(emp.getId() == id) {
            return &emp;
        }
    }
    return nullptr;
}

void System::sendNotificationToJobSeeker(const string& jobSeekerId, const string& message) {
    JobSeeker* js = findJobSeekerById(jobSeekerId);
    if(js) {
        js->addNotification(message);
    }
}

// ========================================
// ADMIN MENU
// ========================================
void System::adminMenu(Admin &a) {
    int choice;
    do {
        cout << "             MENU QUAN TRI VIEN                   \n";
        cout << "Xin chao, " << a.getUsername() << "!\n\n";
        cout << "1. Quan ly danh sach nguoi dung\n";
        cout << "2. Quan ly tai khoan nha tuyen dung\n";
        cout << "3. Quan ly danh muc\n";
        cout << "4. Quan ly tin tuyen dung\n";
        cout << "5. Thong ke hoat dong\n";
        cout << "0. Dang xuat\n";
        cout << "\nChon: "; cin >> choice;

        switch(choice) {
            case 1: admin_ViewAllUsers(); break;
            case 2: {
                cout << "\n--- QUAN LY TAI KHOAN NHA TUYEN DUNG ---\n";
                cout << "1. Tao tai khoan nha tuyen dung\n";
                cout << "2. Cap nhat thong tin nha tuyen dung\n";
                cout << "3. Xoa tai khoan nha tuyen dung\n";
                cout << "Chon: ";
                int subChoice;
                cin >> subChoice;
                switch(subChoice) {
                    case 1: admin_CreateEmployer(); break;
                    case 2: admin_UpdateEmployer(); break;
                    case 3: admin_DeleteEmployer(); break;
                }
                break;
            }
            case 3: admin_ManageCategories(); break;
            case 4: admin_ManageJobs(); break;
            case 5: admin_ViewStatistics(); break;
            case 0: cout << "Dang xuat...\n"; break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while(choice != 0);
}

// 1.1. Quản lý danh sách người dùng
void System::admin_ViewAllUsers() {
  cout << "        DANH SACH NGUOI DUNG                      \n";
    cout << "\n--- UNG VIEN (JOBSEEKER) ---\n";
    if(jobSeekers.empty()) {
        cout << "Khong co ung vien nao.\n";
    } else {
        for(const auto &js : jobSeekers) {
            cout << "\nID: " << js.getId()
                 << " | Username: " << js.getUsername()
                 << " | Email: " << js.getEmail()
                 << " | Status: " << (js.getIsActive() ? "Active" : "Inactive") << "\n";
            cout << "Ho ten: " << js.getFullName()
                 << " | Tuoi: " << js.getAge()
                 << " | Chuyen nganh: " << js.getMajor()
                 << " | KN: " << js.getYearsOfExperience() << " nam\n";
        }
        cout << "\nTong: " << jobSeekers.size() << " ung vien\n";
    }

    cout << "\n--- NHA TUYEN DUNG (EMPLOYER) ---\n";
    if(employers.empty()) {
        cout << "Khong co nha tuyen dung nao.\n";
    } else {
        for(const auto &emp : employers) {
            cout << "\nID: " << emp.getId()
                 << " | Username: " << emp.getUsername()
                 << " | Email: " << emp.getEmail()
                 << " | Status: " << (emp.getIsActive() ? "Active" : "Inactive") << "\n";
            cout << "Cong ty: " << emp.getCompanyName()
                 << " | Nganh: " << emp.getIndustry()
                 << " | So tin: " << emp.getPostedJobIds().size() << "\n";
        }
        cout << "\nTong: " << employers.size() << " nha tuyen dung\n";
    }
}

// 1.2. Tạo tài khoản nhà tuyển dụng
void System::admin_CreateEmployer() {
    cin.ignore();
    cout << "\n=== TAO TAI KHOAN NHA TUYEN DUNG ===\n";

    string id, username, password, email, phone, companyName;

    cout << "Nhap ID: "; getline(cin, id);
    cout << "Nhap username: "; getline(cin, username);

    // Check username exists
    for(const auto &emp : employers) {
        if(emp.getUsername() == username) {
            cout << "### Username da ton tai!\n";
            return;
        }
    }

    cout << "Nhap password: "; getline(cin, password);
    cout << "Nhap email: "; getline(cin, email);
    cout << "Nhap phone: "; getline(cin, phone);
    cout << "Nhap ten cong ty: "; getline(cin, companyName);

    employers.emplace_back(id, username, password, email, phone);
    employers.back().setCompanyName(companyName);

    cout << "✓ Tao tai khoan nha tuyen dung thanh cong!\n";
}

// 1.2. Cập nhật thông tin nhà tuyển dụng
void System::admin_UpdateEmployer() {
    cin.ignore();
    cout << "\n=== CAP NHAT THONG TIN NHA TUYEN DUNG ===\n";
    cout << "Nhap ID nha tuyen dung: ";
    string id;
    getline(cin, id);

    Employer* emp = findEmployerById(id);
    if(!emp) {
        cout << "### Khong tim thay nha tuyen dung!\n";
        return;
    }

    cout << "\n--- Thong tin hien tai ---\n";
    emp->displayInfo();

    cout << "\n--- Cap nhat ---\n";
    cout << "1. Ten cong ty\n";
    cout << "2. Dia chi\n";
    cout << "3. Nganh nghe\n";
    cout << "4. Mo ta\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    string value;
    switch(choice) {
        case 1:
            cout << "Nhap ten cong ty moi: ";
            getline(cin, value);
            emp->setCompanyName(value);
            break;
        case 2:
            cout << "Nhap dia chi moi: ";
            getline(cin, value);
            emp->setCompanyAddress(value);
            break;
        case 3:
            cout << "Nhap nganh nghe moi: ";
            getline(cin, value);
            emp->setIndustry(value);
            break;
        case 4:
            cout << "Nhap mo ta moi: ";
            getline(cin, value);
            emp->setCompanyDescription(value);
            break;
        default:
            cout << "Lua chon khong hop le!\n";
            return;
    }

    cout << "✓ Cap nhat thanh cong!\n";
}

// 1.2. Xóa tài khoản nhà tuyển dụng vi phạm
void System::admin_DeleteEmployer() {
    cin.ignore();
    cout << "\n=== XOA TAI KHOAN NHA TUYEN DUNG ===\n";
    cout << "Nhap ID nha tuyen dung can xoa: ";
    string id;
    getline(cin, id);

    auto it = remove_if(employers.begin(), employers.end(),
                        [&](const Employer &e) { return e.getId() == id; });

    if(it != employers.end()) {
        // Xóa tất cả tin tuyển dụng của employer này
        string empId = it->getId();
        auto jobIt = remove_if(jobs.begin(), jobs.end(),
                              [&](const Job &j) { return j.getEmployerId() == empId; });
        jobs.erase(jobIt, jobs.end());

        employers.erase(it, employers.end());
        cout << "✓ Xoa tai khoan va tat ca tin tuyen dung thanh cong!\n";
    } else {
        cout << "### Khong tim thay ID!\n";
    }
}

// 1.3. Quản lý danh mục
void System::admin_ManageCategories() {
    int choice;
    do {
        cout << "\n=== QUAN LY DANH MUC ===\n";
        cout << "1. Xem danh sach nganh nghe\n";
        cout << "2. Them nganh nghe\n";
        cout << "3. Xoa nganh nghe\n";
        cout << "4. Xem danh sach khu vuc\n";
        cout << "5. Them khu vuc\n";
        cout << "6. Xoa khu vuc\n";
        cout << "0. Quay lai\n";
        cout << "Chon: "; cin >> choice;
        cin.ignore();

        string value;
        switch(choice) {
            case 1:
                cout << "\n--- DANH SACH NGANH NGHE ---\n";
                for(size_t i = 0; i < categories.size(); i++) {
                    cout << i+1 << ". " << categories[i] << "\n";
                }
                break;
            case 2:
                cout << "Nhap ten nganh nghe: ";
                getline(cin, value);
                categories.push_back(value);
                cout << "✓ Them thanh cong!\n";
                break;
            case 3:
                cout << "Nhap ten nganh nghe can xoa: ";
                getline(cin, value);
                categories.erase(remove(categories.begin(), categories.end(), value),
                               categories.end());
                cout << "✓ Xoa thanh cong!\n";
                break;
            case 4:
                cout << "\n--- DANH SACH KHU VUC ---\n";
                for(size_t i = 0; i < locations.size(); i++) {
                    cout << i+1 << ". " << locations[i] << "\n";
                }
                break;
            case 5:
                cout << "Nhap ten khu vuc: ";
                getline(cin, value);
                locations.push_back(value);
                cout << "✓ Them thanh cong!\n";
                break;
            case 6:
                cout << "Nhap ten khu vuc can xoa: ";
                getline(cin, value);
                locations.erase(remove(locations.begin(), locations.end(), value),
                              locations.end());
                cout << "✓ Xoa thanh cong!\n";
                break;
            case 0:
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while(choice != 0);
}

// 1.4. Quản lý tin tuyển dụng
void System::admin_ManageJobs() {
    int choice;
    do {
        cout << "\n=== QUAN LY TIN TUYEN DUNG ===\n";
        cout << "1. Duyet tin tuyen dung\n";
        cout << "2. Chinh sua tin tuyen dung\n";
        cout << "3. Xoa tin tuyen dung\n";
        cout << "4. Xem tat ca tin tuyen dung\n";
        cout << "0. Quay lai\n";
        cout << "Chon: "; cin >> choice;

        switch(choice) {
            case 1: admin_ApproveJobs(); break;
            case 2: admin_EditJob(); break;
            case 3: admin_DeleteJob(); break;
            case 4:
                cout << "\n--- TAT CA TIN TUYEN DUNG ---\n";
                for(const auto &job : jobs) {
                    job.display();
                }
                break;
            case 0: break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while(choice != 0);
}

void System::admin_ApproveJobs() {
    cout << "\n=== DUYET TIN TUYEN DUNG ===\n";

    bool hasPending = false;
    for(auto &job : jobs) {
        if(!job.getIsApproved()) {
            hasPending = true;
            job.display();

            cout << "Duyet tin nay? (1: Duyet, 0: Bo qua): ";
            int approve;
            cin >> approve;

            if(approve == 1) {
                job.setApproved(true);
                cout << "✓ Da duyet tin tuyen dung!\n";

                // Thông báo cho employer
                Employer* emp = findEmployerById(job.getEmployerId());
                if(emp) {
                    cout << "Thong bao da duoc gui den " << emp->getUsername() << "\n";
                }
            }
            cout << "\n";
        }
    }

    if(!hasPending) {
        cout << "Khong co tin tuyen dung nao can duyet!\n";
    }
}

void System::admin_EditJob() {
    cin.ignore();
    cout << "\n=== CHINH SUA TIN TUYEN DUNG ===\n";
    cout << "Nhap Job ID: ";
    string jobId;
    getline(cin, jobId);

    Job* job = findJobById(jobId);
    if(!job) {
        cout << "### Khong tim thay tin tuyen dung!\n";
        return;
    }

    job->display();

    cout << "\n1. Cap nhat trang thai (active/closed)\n";
    cout << "2. Chinh sua mo ta\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    if(choice == 1) {
        cout << "Trang thai (active/closed): ";
        string status;
        getline(cin, status);
        job->setStatus(status);
        cout << "✓ Cap nhat thanh cong!\n";
    } else if(choice == 2) {
        cout << "Nhap mo ta moi: ";
        string desc;
        getline(cin, desc);
        job->setDescription(desc);
        cout << "✓ Cap nhat thanh cong!\n";
    }
}

void System::admin_DeleteJob() {
    cin.ignore();
    cout << "\n=== XOA TIN TUYEN DUNG ===\n";
    cout << "Nhap Job ID can xoa: ";
    string jobId;
    getline(cin, jobId);

    auto it = remove_if(jobs.begin(), jobs.end(),
                       [&](const Job &j) { return j.getJobId() == jobId; });

    if(it != jobs.end()) {
        jobs.erase(it, jobs.end());
        cout << "✓ Xoa tin tuyen dung thanh cong!\n";
    } else {
        cout << "### Khong tim thay Job ID!\n";
    }
}

// 1.5. Thống kê hoạt động
void System::admin_ViewStatistics() {

    cout << "            THONG KE HOAT DONG                     \n";


    // Thống kê tổng quan
    cout << "\n--- TONG QUAN ---\n";
    cout << "Tong so ung vien: " << jobSeekers.size() << "\n";
    cout << "Tong so nha tuyen dung: " << employers.size() << "\n";
    cout << "Tong so tin tuyen dung: " << jobs.size() << "\n";
    cout << "Tong so don ung tuyen: " << applications.size() << "\n";

    // Thống kê tin tuyển dụng theo ngành nghề
    cout << "\n--- THONG KE THEO NGANH NGHE ---\n";
    map<string, int> categoryCount;
    for(const auto &job : jobs) {
        if(job.getIsApproved()) {
            categoryCount[job.getCategory()]++;
        }
    }
    for(const auto &pair : categoryCount) {
        cout << pair.first << ": " << pair.second << " tin\n";
    }

    // Thống kê ứng tuyển
    cout << "\n--- THONG KE UNG TUYEN ---\n";
    int pendingApps = 0, acceptedApps = 0, rejectedApps = 0;
    for(const auto &app : applications) {
        if(app.getStatus() == PENDING || app.getStatus() == VIEWED ||
           app.getStatus() == PROCESSING || app.getStatus() == INTERVIEW) {
            pendingApps++;
        } else if(app.getStatus() == ACCEPTED) {
            acceptedApps++;
        } else if(app.getStatus() == REJECTED) {
            rejectedApps++;
        }
    }
    cout << "Don dang xu ly: " << pendingApps << "\n";
    cout << "Don trung tuyen: " << acceptedApps << "\n";
    cout << "Don tu choi: " << rejectedApps << "\n";
}

// ========================================
// EMPLOYER MENU
// ========================================
void System::employerMenu(Employer &e) {
    int choice;
    do {

        cout << "            MENU NHA TUYEN DUNG                    \n";

        cout << "Xin chao, " << e.getUsername();
        if(!e.getCompanyName().empty()) {
            cout << " (" << e.getCompanyName() << ")";
        }
        cout << "!\n\n";
        cout << "1. Dang tin tuyen dung\n";
        cout << "2. Xem tin da dang\n";
        cout << "3. Chinh sua tin tuyen dung\n";
        cout << "4. Xoa tin tuyen dung\n";
        cout << "5. Xem danh sach ung vien ung tuyen\n";
        cout << "6. Loc ho so ung vien\n";
        cout << "7. Chon ung vien trung tuyen\n";
        cout << "8. Cap nhat thong tin cong ty\n";
        cout << "9. Xem lich su tin da dang\n";
        cout << "10. Xem thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "\nChon: "; cin >> choice;

        switch(choice) {
            case 1: employer_PostJob(e); break;
            case 2: employer_ViewMyJobs(e); break;
            case 3: employer_EditJob(e); break;
            case 4: employer_DeleteJob(e); break;
            case 5: employer_ViewApplications(e); break;
            case 6: employer_FilterApplications(e); break;
            case 7: employer_SelectCandidate(e); break;
            case 8: employer_UpdateProfile(e); break;
            case 9: employer_ViewHistory(e); break;
            case 10: e.displayInfo(); break;
            case 0: cout << "Dang xuat...\n"; break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while(choice != 0);
}

// 2.1. Đăng tin tuyển dụng
void System::employer_PostJob(Employer &e) {
    cin.ignore();
    cout << "\n=== DANG TIN TUYEN DUNG ===\n";

    string title, description, category, location;
    int jobType, minExp, minAge, maxAge;
    double minSalary, maxSalary;

    cout << "Tieu de: "; getline(cin, title);

    // Chọn ngành nghề
    cout << "\n--- NGANH NGHE ---\n";
    for(size_t i = 0; i < categories.size(); i++) {
        cout << i+1 << ". " << categories[i] << "\n";
    }
    cout << "Chon nganh nghe (nhap so): ";
    int catChoice;
    cin >> catChoice;
    if(catChoice > 0 && catChoice <= (int)categories.size()) {
        category = categories[catChoice - 1];
    }
    cin.ignore();

    // Chọn địa điểm
    cout << "\n--- DIA DIEM ---\n";
    for(size_t i = 0; i < locations.size(); i++) {
        cout << i+1 << ". " << locations[i] << "\n";
    }
    cout << "Chon dia diem (nhap so): ";
    int locChoice;
    cin >> locChoice;
    if(locChoice > 0 && locChoice <= (int)locations.size()) {
        location = locations[locChoice - 1];
    }
    cin.ignore();

    // Loại hình công việc
    cout << "\nLoai hinh (1: Full-time, 2: Part-time): ";
    cin >> jobType;

    // Mức lương
    cout << "Muc luong toi thieu: ";
    cin >> minSalary;
    cout << "Muc luong toi da: ";
    cin >> maxSalary;

    // Kinh nghiệm
    cout << "Kinh nghiem toi thieu (nam): ";
    cin >> minExp;

    // Độ tuổi
    cout << "Do tuoi toi thieu: ";
    cin >> minAge;
    cout << "Do tuoi toi da: ";
    cin >> maxAge;

    cin.ignore();
    cout << "Mo ta cong viec: ";
    getline(cin, description);

    // Tạo job
    string jobId = generateJobId();
    Job newJob(jobId, e.getId(), title);
    newJob.setDescription(description);
    newJob.setCategory(category);
    newJob.setLocation(location);
    newJob.setJobType((jobType == 1) ? FULL_TIME : PART_TIME);
    newJob.setSalaryRange(minSalary, maxSalary);
    newJob.setMinExperience(minExp);
    newJob.setAgeRange(minAge, maxAge);

    jobs.push_back(newJob);
    e.addPostedJob(jobId);

    cout << "\n✓ Dang tin thanh cong! Job ID: " << jobId << "\n";
    cout << "ℹ️  Tin tuyen dung dang cho Admin duyet.\n";
}

// 2.1. Xem tin đã đăng
void System::employer_ViewMyJobs(Employer &e) {
    cout << "\n=== TIN TUYEN DUNG DA DANG ===\n";
    auto postedJobIds = e.getPostedJobIds();

    if(postedJobIds.empty()) {
        cout << "Ban chua dang tin tuyen dung nao!\n";
        return;
    }

    for(const auto &jobId : postedJobIds) {
        Job* job = findJobById(jobId);
        if(job) {
            job->display();
        }
    }
}

// 2.1. Chỉnh sửa tin tuyển dụng
void System::employer_EditJob(Employer &e) {
    cin.ignore();
    cout << "\n=== CHINH SUA TIN TUYEN DUNG ===\n";
    cout << "Nhap Job ID: ";
    string jobId;
    getline(cin, jobId);

    Job* job = findJobById(jobId);
    if(!job || job->getEmployerId() != e.getId()) {
        cout << "### Khong tim thay hoac ban khong co quyen chinh sua!\n";
        return;
    }

    job->display();

    cout << "\n--- CHINH SUA ---\n";
    cout << "1. Mo ta\n";
    cout << "2. Muc luong\n";
    cout << "3. Kinh nghiem yeu cau\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    switch(choice) {
        case 1: {
            string desc;
            cout << "Nhap mo ta moi: ";
            getline(cin, desc);
            job->setDescription(desc);
            cout << "✓ Cap nhat thanh cong!\n";
            break;
        }
        case 2: {
            double minSal, maxSal;
            cout << "Muc luong toi thieu: ";
            cin >> minSal;
            cout << "Muc luong toi da: ";
            cin >> maxSal;
            job->setSalaryRange(minSal, maxSal);
            cout << "✓ Cap nhat thanh cong!\n";
            break;
        }
        case 3: {
            int minExp;
            cout << "Kinh nghiem toi thieu (nam): ";
            cin >> minExp;
            job->setMinExperience(minExp);
            cout << "✓ Cap nhat thanh cong!\n";
            break;
        }
        default:
            cout << "Lua chon khong hop le!\n";
    }
}

// 2.1. Xóa tin tuyển dụng
void System::employer_DeleteJob(Employer &e) {
    cin.ignore();
    cout << "\n=== XOA TIN TUYEN DUNG ===\n";
    cout << "Nhap Job ID: ";
    string jobId;
    getline(cin, jobId);

    auto it = find_if(jobs.begin(), jobs.end(),
                      [&](const Job &j) {
                          return j.getJobId() == jobId &&
                                 j.getEmployerId() == e.getId();
                      });

    if(it != jobs.end()) {
        jobs.erase(it);
        e.removePostedJob(jobId);
        cout << "✓ Xoa tin tuyen dung thanh cong!\n";
    } else {
        cout << "### Khong tim thay hoac ban khong co quyen xoa!\n";
    }
}

// 2.2. Xem danh sách ứng viên
void System::employer_ViewApplications(Employer &e) {
    cout << "\n=== DANH SACH UNG VIEN UNG TUYEN ===\n";

    auto postedJobIds = e.getPostedJobIds();
    if(postedJobIds.empty()) {
        cout << "Ban chua co tin tuyen dung nao!\n";
        return;
    }

    bool found = false;
    for(const auto &jobId : postedJobIds) {
        cout << "\n--- Job ID: " << jobId << " ---\n";
        Job* job = findJobById(jobId);
        if(job) {
            cout << "Tieu de: " << job->getTitle() << "\n";
        }

        for(const auto &app : applications) {
            if(app.getJobId() == jobId) {
                found = true;
                app.display();

                // Hiển thị thông tin ứng viên
                JobSeeker* js = findJobSeekerById(app.getJobSeekerId());
                if(js) {
                    cout << "Thong tin ung vien:\n";
                    cout << "  Username: " << js->getUsername() << "\n";
                    cout << "  Email: " << js->getEmail() << "\n";
                    cout << "  Phone: " << js->getPhone() << "\n";
                }
                cout << "---\n";
            }
        }
    }

    if(!found) {
        cout << "Chua co ung vien nao ung tuyen!\n";
    }
}

// 2.2. Lọc hồ sơ ứng viên
void System::employer_FilterApplications(Employer &e) {
    cout << "\n=== LOC HO SO UNG VIEN ===\n";
    cout << "1. Loc theo ky nang\n";
    cout << "2. Loc theo kinh nghiem\n";
    cout << "3. Loc theo tuoi\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    auto postedJobIds = e.getPostedJobIds();

    switch(choice) {
        case 1: {
            string skill;
            cout << "Nhap ky nang can tim: ";
            getline(cin, skill);

            cout << "\n--- KET QUA LOC ---\n";
            for(const auto &app : applications) {
                if(find(postedJobIds.begin(), postedJobIds.end(),
                       app.getJobId()) != postedJobIds.end()) {
                    JobSeeker* js = findJobSeekerById(app.getJobSeekerId());
                    if(js) {
                        auto skills = js->getSkills();
                        if(find(skills.begin(), skills.end(), skill) != skills.end()) {
                            app.display();
                            cout << "Ung vien: " << js->getUsername() << "\n---\n";
                        }
                    }
                }
            }
            break;
        }
        case 2: {
            int minExp;
            cout << "Nhap so nam kinh nghiem toi thieu: ";
            cin >> minExp;

            cout << "\n--- KET QUA LOC ---\n";
            for(const auto &app : applications) {
                if(find(postedJobIds.begin(), postedJobIds.end(),
                       app.getJobId()) != postedJobIds.end()) {
                    JobSeeker* js = findJobSeekerById(app.getJobSeekerId());
                    if(js && js->getYearsOfExperience() >= minExp) {
                        app.display();
                        cout << "Ung vien: " << js->getUsername()
                             << " | KN: " << js->getYearsOfExperience() << " nam\n---\n";
                    }
                }
            }
            break;
        }
        case 3: {
            int minAge, maxAge;
            cout << "Nhap tuoi toi thieu: ";
            cin >> minAge;
            cout << "Nhap tuoi toi da: ";
            cin >> maxAge;

            cout << "\n--- KET QUA LOC ---\n";
            for(const auto &app : applications) {
                if(find(postedJobIds.begin(), postedJobIds.end(),
                       app.getJobId()) != postedJobIds.end()) {
                    JobSeeker* js = findJobSeekerById(app.getJobSeekerId());
                    if(js && js->getAge() >= minAge && js->getAge() <= maxAge) {
                        app.display();
                        cout << "Ung vien: " << js->getUsername()
                             << " | Tuoi: " << js->getAge() << "\n---\n";
                    }
                }
            }
            break;
        }
        default:
            cout << "Lua chon khong hop le!\n";
    }
}

// 2.2. Chọn ứng viên trúng tuyển
void System::employer_SelectCandidate(Employer &e) {
    cin.ignore();
    cout << "\n=== CHON UNG VIEN TRUNG TUYEN ===\n";
    cout << "Nhap Application ID: ";
    string appId;
    getline(cin, appId);

    for(auto &app : applications) {
        if(app.getApplicationId() == appId) {
            // Check nếu job thuộc về employer này
            auto postedJobIds = e.getPostedJobIds();
            if(find(postedJobIds.begin(), postedJobIds.end(),
                   app.getJobId()) == postedJobIds.end()) {
                cout << "### Ban khong co quyen thao tac voi don nay!\n";
                return;
            }

            app.setStatus(ACCEPTED);
            cout << "✓ Da chon ung vien trung tuyen!\n";

            // Gửi thông báo cho ứng viên
            JobSeeker* js = findJobSeekerById(app.getJobSeekerId());
            if(js) {
                string notif = "Chuc mung! Ban da trung tuyen cong viec " +
                              app.getJobId() + " tai " + e.getCompanyName();
                sendNotificationToJobSeeker(app.getJobSeekerId(), notif);
                cout << "Thong bao da duoc gui den ung vien " << js->getUsername() << "\n";
            }
            return;
        }
    }

    cout << "### Khong tim thay Application ID!\n";
}

// 2.3. Cập nhật thông tin công ty
void System::employer_UpdateProfile(Employer &e) {
    cin.ignore();
    cout << "\n=== CAP NHAT THONG TIN CONG TY ===\n";
    cout << "1. Ten cong ty\n";
    cout << "2. Dia chi\n";
    cout << "3. Nganh nghe\n";
    cout << "4. Mo ta\n";
    cout << "5. Email\n";
    cout << "6. So dien thoai\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    string value;
    switch(choice) {
        case 1:
            cout << "Nhap ten cong ty: ";
            getline(cin, value);
            e.setCompanyName(value);
            break;
        case 2:
            cout << "Nhap dia chi: ";
            getline(cin, value);
            e.setCompanyAddress(value);
            break;
        case 3:
            cout << "Nhap nganh nghe: ";
            getline(cin, value);
            e.setIndustry(value);
            break;
        case 4:
            cout << "Nhap mo ta: ";
            getline(cin, value);
            e.setCompanyDescription(value);
            break;
        case 5:
            cout << "Nhap email: ";
            getline(cin, value);
            e.setEmail(value);
            break;
        case 6:
            cout << "Nhap so dien thoai: ";
            getline(cin, value);
            e.setPhone(value);
            break;
        default:
            cout << "Lua chon khong hop le!\n";
            return;
    }

    cout << "✓ Cap nhat thanh cong!\n";
}

// 2.3. Xem lịch sử tin đã đăng
void System::employer_ViewHistory(Employer &e) {
    cout << "\n=== LICH SU TIN TUYEN DUNG ===\n";
    auto postedJobIds = e.getPostedJobIds();

    if(postedJobIds.empty()) {
        cout << "Ban chua dang tin tuyen dung nao!\n";
        return;
    }

    cout << "Tong so tin da dang: " << postedJobIds.size() << "\n\n";

    for(const auto &jobId : postedJobIds) {
        Job* job = findJobById(jobId);
        if(job) {
            cout << "Job ID: " << jobId << " | " << job->getTitle() << "\n";
            cout << "Trang thai: " << (job->getIsApproved() ? "Da duyet" : "Cho duyet")
                 << " | " << job->getStatus() << "\n";
            cout << "Ngay dang: " << job->getPostedDate() << "\n";

            // Đếm số đơn ứng tuyển
            int appCount = 0;
            for(const auto &app : applications) {
                if(app.getJobId() == jobId) {
                    appCount++;
                }
            }
            cout << "So don ung tuyen: " << appCount << "\n";
            cout << "---\n";
        }
    }
}

// ========================================
// JOBSEEKER MENU
// ========================================
void System::jobSeekerMenu(JobSeeker &js) {
    int choice;
    do {

        cout << "              MENU UNG VIEN                        \n";
        cout << "Xin chao, " << js.getUsername() << "!\n\n";

        // Hiển thị số thông báo chưa đọc
        int notifCount = js.getNotifications().size();
        if(notifCount > 0) {
            cout << "🔔 Ban co " << notifCount << " thong bao moi!\n\n";
        }

        cout << "1. Cap nhat CV truc tuyen\n";
        cout << "2. Tai len CV file\n";
        cout << "3. Chinh sua thong tin ca nhan\n";
        cout << "4. Tim kiem viec lam\n";
        cout << "5. Xem chi tiet tin tuyen dung\n";
        cout << "6. Ung tuyen viec lam\n";
        cout << "7. Xem trang thai ho so\n";
        cout << "8. Xem lich su ung tuyen\n";
        cout << "9. Xem thong bao\n";
        cout << "10. Xem thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "\nChon: "; cin >> choice;

        switch(choice) {
            case 1: jobseeker_UpdateCV(js); break;
            case 2: jobseeker_UploadCVFile(js); break;
            case 3: jobseeker_UpdateProfile(js); break;
            case 4: jobseeker_SearchJobs(js); break;
            case 5: jobseeker_ViewJobDetail(js); break;
            case 6: jobseeker_ApplyJob(js); break;
            case 7: jobseeker_ViewApplicationStatus(js); break;
            case 8: jobseeker_ViewHistory(js); break;
            case 9: jobseeker_ViewNotifications(js); break;
            case 10: js.displayInfo(); break;
            case 0: cout << "Dang xuat...\n"; break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while(choice != 0);
}

// 3.1. Cập nhật CV trực tuyến
void System::jobseeker_UpdateCV(JobSeeker &js) {
    cin.ignore();
    cout << "\n=== CAP NHAT CV TRUC TUYEN ===\n";

    string fullName, major;
    int age, yearsOfExp;

    cout << "Ho ten: "; getline(cin, fullName);
    cout << "Tuoi: "; cin >> age;
    cin.ignore();
    cout << "Chuyen nganh: "; getline(cin, major);
    cout << "So nam kinh nghiem: "; cin >> yearsOfExp;

    js.updateCVOnline(fullName, age, major, yearsOfExp);

    // Nhập kỹ năng
    cout << "\nNhap ky nang (nhap '0' de ket thuc):\n";
    js.clearSkills();
    cin.ignore();
    while(true) {
        string skill;
        cout << "Ky nang: ";
        getline(cin, skill);
        if(skill == "0") break;
        js.addSkill(skill);
    }

    cout << "\n✓ Cap nhat CV thanh cong!\n";
    cout << "\n--- CV CUA BAN ---\n";
    cout << js.getCVOnline() << "\n";
    auto skills = js.getSkills();
    if(!skills.empty()) {
        cout << "Ky nang: ";
        for(size_t i = 0; i < skills.size(); i++) {
            cout << skills[i];
            if(i < skills.size() - 1) cout << ", ";
        }
        cout << "\n";
    }
}

// 3.1. Tải lên CV file
void System::jobseeker_UploadCVFile(JobSeeker &js) {
    cin.ignore();
    cout << "\n=== TAI LEN CV FILE ===\n";
    cout << "Nhap duong dan file CV: ";
    string filePath;
    getline(cin, filePath);

    js.setCVFilePath(filePath);
    cout << "✓ Tai len CV thanh cong!\n";
    cout << "Duong dan: " << filePath << "\n";
}

// 3.1. Chỉnh sửa thông tin cá nhân
void System::jobseeker_UpdateProfile(JobSeeker &js) {
    cin.ignore();
    cout << "\n=== CHINH SUA THONG TIN CA NHAN ===\n";
    cout << "1. Email\n";
    cout << "2. So dien thoai\n";
    cout << "3. Doi mat khau\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    string value;
    switch(choice) {
        case 1:
            cout << "Nhap email moi: ";
            getline(cin, value);
            js.setEmail(value);
            cout << "✓ Cap nhat thanh cong!\n";
            break;
        case 2:
            cout << "Nhap so dien thoai moi: ";
            getline(cin, value);
            js.setPhone(value);
            cout << "✓ Cap nhat thanh cong!\n";
            break;
        case 3:
            cout << "Nhap mat khau moi: ";
            getline(cin, value);
            js.setPassword(value);
            cout << "✓ Doi mat khau thanh cong!\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
    }
}

// 3.2. Tìm kiếm việc làm
void System::jobseeker_SearchJobs(JobSeeker &js) {
    cout << "\n=== TIM KIEM VIEC LAM ===\n";
    cout << "1. Xem tat ca cong viec\n";
    cout << "2. Tim theo nganh nghe\n";
    cout << "3. Tim theo dia diem\n";
    cout << "4. Tim theo muc luong\n";
    cout << "5. Loc theo yeu cau ca nhan\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    switch(choice) {
        case 1: {
            cout << "\n--- TAT CA CONG VIEC ---\n";
            for(const auto &job : jobs) {
                if(job.getIsApproved() && job.getStatus() == "active") {
                    job.display();
                }
            }
            break;
        }
        case 2: {
            cout << "\n--- CHON NGANH NGHE ---\n";
            for(size_t i = 0; i < categories.size(); i++) {
                cout << i+1 << ". " << categories[i] << "\n";
            }
            cout << "Chon: ";
            int catChoice;
            cin >> catChoice;

            if(catChoice > 0 && catChoice <= (int)categories.size()) {
                string category = categories[catChoice - 1];
                cout << "\n--- KET QUA TIM KIEM ---\n";
                for(const auto &job : jobs) {
                    if(job.getIsApproved() && job.getStatus() == "active" &&
                       job.getCategory() == category) {
                        job.display();
                    }
                }
            }
            break;
        }
        case 3: {
            cout << "\n--- CHON DIA DIEM ---\n";
            for(size_t i = 0; i < locations.size(); i++) {
                cout << i+1 << ". " << locations[i] << "\n";
            }
            cout << "Chon: ";
            int locChoice;
            cin >> locChoice;

            if(locChoice > 0 && locChoice <= (int)locations.size()) {
                string location = locations[locChoice - 1];
                cout << "\n--- KET QUA TIM KIEM ---\n";
                for(const auto &job : jobs) {
                    if(job.getIsApproved() && job.getStatus() == "active" &&
                       job.getLocation() == location) {
                        job.display();
                    }
                }
            }
            break;
        }
        case 4: {
            double minSalary;
            cout << "Nhap muc luong toi thieu: ";
            cin >> minSalary;

            cout << "\n--- KET QUA TIM KIEM ---\n";
            for(const auto &job : jobs) {
                if(job.getIsApproved() && job.getStatus() == "active" &&
                   job.getMaxSalary() >= minSalary) {
                    job.display();
                }
            }
            break;
        }
        case 5: {
            // Lọc theo kinh nghiệm, mức lương
            int myExp = js.getYearsOfExperience();

            cout << "\n--- LOC THEO YEU CAU CA NHAN ---\n";
            cout << "Kinh nghiem cua ban: " << myExp << " nam\n";
            cout << "Nhap muc luong mong muon: ";
            double expectedSalary;
            cin >> expectedSalary;

            cout << "\n--- KET QUA LOC ---\n";
            for(const auto &job : jobs) {
                if(job.getIsApproved() && job.getStatus() == "active" &&
                   job.getMinExperience() <= myExp &&
                   job.getMaxSalary() >= expectedSalary) {
                    job.display();
                }
            }
            break;
        }
        default:
            cout << "Lua chon khong hop le!\n";
    }
}

// 3.2. Xem chi tiết tin tuyển dụng
void System::jobseeker_ViewJobDetail(JobSeeker &js) {
    cin.ignore();
    cout << "\n=== XEM CHI TIET TIN TUYEN DUNG ===\n";
    cout << "Nhap Job ID: ";
    string jobId;
    getline(cin, jobId);

    Job* job = findJobById(jobId);
    if(!job || !job->getIsApproved() || job->getStatus() != "active") {
        cout << "### Khong tim thay tin tuyen dung!\n";
        return;
    }

    job->display();

    // Hiển thị thông tin công ty
    Employer* emp = findEmployerById(job->getEmployerId());
    if(emp) {
        cout << "\n--- THONG TIN CONG TY ---\n";
        cout << "Ten cong ty: " << emp->getCompanyName() << "\n";
        cout << "Dia chi: " << emp->getCompanyAddress() << "\n";
        cout << "Email: " << emp->getEmail() << "\n";
        cout << "Phone: " << emp->getPhone() << "\n";
    }
}

// 3.3. Ứng tuyển việc làm
void System::jobseeker_ApplyJob(JobSeeker &js) {
    cin.ignore();
    cout << "\n=== UNG TUYEN VIEC LAM ===\n";

    // Kiểm tra CV
    if(js.getAge() == 0 && js.getCVFilePath().empty()) {
        cout << "### Ban chua co CV! Vui long cap nhat CV truoc.\n";
        return;
    }

    cout << "Nhap Job ID: ";
    string jobId;
    getline(cin, jobId);

    Job* job = findJobById(jobId);
    if(!job || !job->getIsApproved() || job->getStatus() != "active") {
        cout << "### Khong tim thay tin tuyen dung hoac tin da dong!\n";
        return;
    }

    // Kiểm tra đã ứng tuyển chưa
    auto appliedJobs = js.getAppliedJobs();
    if(find(appliedJobs.begin(), appliedJobs.end(), jobId) != appliedJobs.end()) {
        cout << "### Ban da ung tuyen tin nay roi!\n";
        return;
    }

    // Tạo đơn ứng tuyển
    string appId = generateAppId();
    string cvContent = js.getCVOnline();

    Application newApp(appId, js.getId(), jobId, cvContent);
    applications.push_back(newApp);
    js.applyForJob(jobId);

    cout << "\n✓ Ung tuyen thanh cong!\n";
    cout << "Application ID: " << appId << "\n";
    cout << "Ban se nhan duoc thong bao khi nha tuyen dung xem ho so.\n";
}

// 3.3. Theo dõi trạng thái hồ sơ
void System::jobseeker_ViewApplicationStatus(JobSeeker &js) {
    cout << "\n=== TRANG THAI HO SO UNG TUYEN ===\n";

    bool found = false;
    for(const auto &app : applications) {
        if(app.getJobSeekerId() == js.getId()) {
            found = true;
            app.display();

            Job* job = findJobById(app.getJobId());
            if(job) {
                cout << "Tin tuyen dung: " << job->getTitle() << "\n";
                Employer* emp = findEmployerById(job->getEmployerId());
                if(emp) {
                    cout << "Cong ty: " << emp->getCompanyName() << "\n";
                }
            }
            cout << "===\n";
        }
    }

    if(!found) {
        cout << "Ban chua ung tuyen cong viec nao!\n";
    }
}
// 3.4. Xem lịch sử ứng tuyển (tiếp)
void System::jobseeker_ViewHistory(JobSeeker &js) {
    cout << "\n=== LICH SU UNG TUYEN ===\n";

    auto appliedJobs = js.getAppliedJobs();
    if(appliedJobs.empty()) {
        cout << "Ban chua ung tuyen cong viec nao!\n";
        return;
    }

    cout << "Tong so cong viec da ung tuyen: " << appliedJobs.size() << "\n\n";

    for(const auto &jobId : appliedJobs) {
        Job* job = findJobById(jobId);
        if(job) {
            cout << "Job ID: " << jobId << " | " << job->getTitle() << "\n";
            cout << "Cong ty: ";
            Employer* emp = findEmployerById(job->getEmployerId());
            if(emp) {
                cout << emp->getCompanyName();
            }
            cout << "\n";

            // Tìm trạng thái đơn ứng tuyển
            for(const auto &app : applications) {
                if(app.getJobId() == jobId && app.getJobSeekerId() == js.getId()) {
                    cout << "Trang thai: " << app.getStatusString() << "\n";
                    break;
                }
            }
            cout << "---\n";
        }
    }
}

// 3.4. Xem thông báo
void System::jobseeker_ViewNotifications(JobSeeker &js) {
    cout << "\n=== THONG BAO ===\n";

    auto notifications = js.getNotifications();
    if(notifications.empty()) {
        cout << "Khong co thong bao nao!\n";
        return;
    }

    for(size_t i = 0; i < notifications.size(); i++) {
        cout << (i+1) << ". " << notifications[i] << "\n";
    }

    cout << "\nXoa tat ca thong bao? (1: Co, 0: Khong): ";
    int clear;
    cin >> clear;
    if(clear == 1) {
        js.clearNotifications();
        cout << "✓ Da xoa tat ca thong bao!\n";
    }
}

// ========================================
// MAIN SYSTEM FUNCTIONS
// ========================================

void System::registerUser() {
    int role;
    string id, username, password, email, phone;

    // regular expression
    std::regex passRegex(R"((?=.*[a-z])(?=.*[A-Z])(?=.*\d).{8,})");
    std::regex emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    cin.ignore();
    cout << "             DANG KY TAI KHOAN                   \n";
    cout << "\nChon loai tai khoan:\n";
    cout << "1. Ung vien (JobSeeker)\n";
    cout << "2. Nha tuyen dung (Employer)\n";
    cout << "Chon: ";
    cin >> role;
    cin.ignore();

    if(role != 1 && role != 2) {
        cout << "### Lua chon khong hop le!\n";
        return;
    }

    cout << "\nNhap ID: "; getline(cin, id);
    cout << "Nhap username: "; getline(cin, username);

    // Check username exists
    for(const auto &js : jobSeekers) {
        if(js.getUsername() == username) {
            cout << "### Username da ton tai!\n";
            return;
        }
    }
    for(const auto &emp : employers) {
        if(emp.getUsername() == username) {
            cout << "### Username da ton tai!\n";
            return;
        }
    }

    while (true) {
        cout << "Nhap password ( 8+ ky tu, 1+ chu in hoa, 1+ chu in thuong, 1+ chu so ): "; getline(cin, password);
        if (std::regex_match(password, passRegex)) break;
        cout << "### Password khong hop le! Vui long thu lai.\n";
    }

    while (true) {
        cout << "Nhap email: "; getline(cin, email);
        if (std::regex_match(email, emailRegex)) break;
        cout << "### Email khong hop le! Vui long thu lai.\n";
    }

    cout << "Nhap phone: "; getline(cin, phone);

    if(role == 1) {
        jobSeekers.emplace_back(id, username, password, email, phone);
        cout << "\n✓ Dang ky Ung vien thanh cong!\n";
        cout << "Vui long dang nhap de su dung he thong.\n";
    }
    else if(role == 2) {
        employers.emplace_back(id, username, password, email, phone);

        string companyName;
        cout << "Nhap ten cong ty: ";
        getline(cin, companyName);
        employers.back().setCompanyName(companyName);

        cout << "\n✓ Dang ky Nha tuyen dung thanh cong!\n";
        cout << "Vui long dang nhap de su dung he thong.\n";
    }
}

void System::loginUser() {
    string username, password;
    cin.ignore();


    cout << "                  DANG NHAP                        \n";
    cout << "\nUsername: "; getline(cin, username);
    cout << "Password: "; getline(cin, password);


    string hashedPassword = sha256(password);


    // json user = PBLJson::getUser(username);
    // if (user["hashedPassword"] == hashedPassword) {
    //     if (user["role"] == "admin")
    //     {
    //         cout << "\n✓ Dang nhap thanh cong voi quyen Admin!\n";
    //         adminMenu(defaultAdmin);
    //         return;
    //     }
    //     if (user["role"] == "jobseeker")
    //     {
    //         for (auto &js : jobSeekers)
    //         {
    //             if (js.getUsername() == username)
    //             {
    //                 if (!js.getIsActive())
    //                 {
    //                     cout << "\n### Tai khoan da bi khoa! Vui long lien he Admin.\n";
    //                     return;
    //                 }
    //                 cout << "\n✓ Dang nhap thanh cong!\n";
    //                 jobSeekerMenu(js);
    //                 return;
    //             }
    //         }
    //     }
    //     else if (user["role"] == "employer")
    //     {
    //         for (auto &e : employers)
    //         {
    //             if (e.getUsername() == username)
    //             {
    //                 if (!e.getIsActive())
    //                 {
    //                     cout << "\n### Tai khoan da bi khoa! Vui long lien he Admin.\n";
    //                     return;
    //                 }
    //                 cout << "\n✓ Dang nhap thanh cong!\n";
    //                 employerMenu(e);
    //                 return;
    //             }
    //         }
    //     }
    // }


    // Check Admin
    if(username == defaultAdmin.getUsername() &&
       password == defaultAdmin.getPassword()) {
        cout << "\n✓ Dang nhap thanh cong voi quyen Admin!\n";
        adminMenu(defaultAdmin);
        return;
    }

    // Check JobSeeker
    for(auto &js : jobSeekers) {
        if(js.getUsername() == username && js.getPassword() == password) {
            if(!js.getIsActive()) {
                cout << "\n### Tai khoan da bi khoa! Vui long lien he Admin.\n";
                return;
            }
            cout << "\n✓ Dang nhap thanh cong!\n";
            jobSeekerMenu(js);
            return;
        }
    }

    // Check Employer
    for(auto &e : employers) {
        if(e.getUsername() == username && e.getPassword() == password) {
            if(!e.getIsActive()) {
                cout << "\n### Tai khoan da bi khoa! Vui long lien he Admin.\n";
                return;
            }
            cout << "\n✓ Dang nhap thanh cong!\n";
            employerMenu(e);
            return;
        }
    }


    cout << "\n### Sai username hoac password!\n";
}

void System::run() {
    int choice;
    do {
        cout << "\n";
        cout << "     HE THONG TIM VIEC LAM TRUC TUYEN              \n";
        cout << "1. Dang ky tai khoan\n";
        cout << "2. Dang nhap\n";
        cout << "0. Thoat\n";
        cout << "\nChon: ";
        cin >> choice;

        switch(choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 0:

                cout << "     Cam on da su dung he thong!                  \n";
                cout << "    Hen gap lai!                                  \n";
                break;
            default:
                cout << "\n### Lua chon khong hop le!\n";
        }
    } while(choice != 0);
}