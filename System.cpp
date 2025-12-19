#include "System.h"
#include <algorithm>
#include <iostream>
#include <regex>
#include "hash.h"
#include "json.h"
using namespace std;

// ========== CONSTRUCTOR ==========

System::System() :
    latestJobId(Job::getLatestJobId()), nextAppId(1),
    admin(Admin("0", "admin", "", "", "", "", "", "", "", "")) // temporary, will be overwritten
{}

System::System(const Admin &admin) : admin(admin), latestJobId(Job::getLatestJobId()), nextAppId(1) {}

void System::initializeDefaultAdmin() { admin.setFullName("Quan Tri Vien"); }

// ========== HELPER FUNCTIONS ==========
string System::generateJobId() { return to_string(Job::getLatestJobId() + 1); }

string System::generateAppId() { return "APP" + to_string(nextAppId++); }

JobSeeker *System::findJobSeekerById(const string &id) {
    for (auto &js: jobSeekers) {
        if (js.getId() == id) {
            return &js;
        }
    }
    return nullptr;
}

Employer *System::findEmployerById(const string &id) {
    for (auto &emp: employers) {
        if (emp.getId() == id) {
            return &emp;
        }
    }
    return nullptr;
}

void Job::display() const {
    cout << "\n========================================\n";
    cout << "Job id: " << id << "\n";
    cout << "Tieu de: " << title << "\n";
    cout << "Loai hinh: " << getJobTypeString() << "\n";
    cout << "Muc luong: " << std::fixed << std::setprecision(2) << minSalary << " - " << maxSalary << " VND\n";
    cout << "Kinh nghiem: " << minExperience << "+ nam\n";
    cout << "Do tuoi: " << minAge << " - " << maxAge << "\n";
    cout << "So nguoi ung tuyen: " << applicants.size() << "\n";
    cout << "Mo ta: " << description << "\n";
    cout << "Ngay dang: " << postedDate << "\n";
    cout << "Cong ty: " << Employer::getEmployerById(id).getCompanyName() << "\n";
    cout << "========================================\n";
}

void Job::acceptedView() const {
    cout << "\n========================================\n";
    cout << "Tieu de: " << title << "\n";
    cout << "Loai hinh: " << getJobTypeString() << "\n";
    cout << "Muc luong: " << std::fixed << std::setprecision(2) << minSalary << " - " << maxSalary << " VND\n";
    cout << "So nguoi ung tuyen: " << applicants.size() << "\n";
    cout << "Mo ta: " << description << "\n";
    cout << "Ngay dang: " << postedDate << "\n";
    cout << "Email nguoi dang: " << Employer::getEmployerById(id).getEmail() << "\n";
    cout << "Cong ty: " << Employer::getEmployerById(id).getCompanyName() << "\n";
    cout << "========================================\n";
}

void Job::notAcceptedView() const {
    cout << "\n========================================\n";
    cout << "Tieu de: " << title << "\n";
    cout << "Loai hinh: " << getJobTypeString() << "\n";
    cout << "Muc luong: " << std::fixed << std::setprecision(2) << minSalary << " - " << maxSalary << " VND\n";
    cout << "So nguoi ung tuyen: " << applicants.size() << "\n";
    cout << "Cong ty: " << Employer::getEmployerById(id).getCompanyName() << "\n";
    cout << "========================================\n";
}

// ========================================
// ADMIN MENU
// ========================================
void System::adminMenu(const Admin &a) {
    int choice;
    do {
        cout << "             MENU QUAN TRI VIEN                   \n";
        cout << "Xin chao, " << a.getFullName() << "!\n\n";
        cout << "1. Quan ly danh sach nguoi dung\n";
        cout << "2. Xoa tai khoan nguoi dung\n";
        cout << "3. Quan ly tin tuyen dung\n";
        cout << "0. Dang xuat\n";
        cout << "\nChon: ";
        cin >> choice;

        switch (choice) {
            case 1:
                admin_ViewAllUsers();
                break;
            case 2:
                admin_removeUser();
                break;
            case 3:
                admin_ManageJobs();
                break;
            case 0:
                cout << "Dang xuat...\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}

// 1.1. Quản lý danh sách người dùng
void System::admin_ViewAllUsers() {


    json data = PBLJson::loadList("users.json");

    for (auto &user: data) {
        if (user.value("role", "") == "jobSeeker") {
            string companyName;
            if (user.contains("companyName") && user["companyName"].is_string())
                companyName = user["companyName"].get<std::string>();

            jobSeekers.emplace_back(User(user.value("id", "0"), user.value("username", ""), user.value("password", ""),
                                         user.value("email", ""), user.value("phone", ""), user.value("fullName", ""),
                                         user.value("dateOfBirth", "0000-00-00"), user.value("isActive", true),
                                         user.value("createdAt", ""), companyName));
        } else if (user.value("role", "") == "employer") {
            string companyName;
            if (user.contains("companyName") && user["companyName"].is_string())
                companyName = user["companyName"].get<std::string>();

            employers.emplace_back(User(user.value("id", "0"), user.value("username", ""), user.value("password", ""),
                                        user.value("email", ""), user.value("phone", ""), user.value("fullName", ""),
                                        user.value("dateOfBirth", "0000-00-00"), user.value("isActive", true),
                                        user.value("createdAt", ""), companyName));
        }
    }


    cout << "        DANH SACH NGUOI DUNG                      \n";
    cout << "\n--- UNG VIEN (JOBSEEKER) ---\n";

    if (jobSeekers.empty()) {
        cout << "Khong co ung vien nao.\n";
    } else {
        for (const auto &js: jobSeekers) {
            cout << "\nID: " << js.getId() << " | Ho ten: " << js.getUsername() << " | Email: " << js.getEmail()
                 << " | Status: " << (js.getStatus() ? "Active" : "Inactive") << "\n";
            cout << "Ho ten: " << js.getFullName() << " | Tuoi: " << js.getAge() << " | Chuyen nganh: " << js.getMajor()
                 << " | KN: " << js.getYearsOfExperience() << " nam\n";
        }
        cout << "\nTong: " << jobSeekers.size() << " ung vien\n";
    }

    cout << "\n--- NHA TUYEN DUNG (EMPLOYER) ---\n";
    if (employers.empty()) {
        cout << "Khong co nha tuyen dung nao.\n";
    } else {
        for (const auto &emp: employers) {
            cout << "\nID: " << emp.getId() << " | Username: " << emp.getUsername() << " | Email: " << emp.getEmail()
                 << " | Status: " << (emp.getStatus() ? "Active" : "Inactive") << "\n";
            cout << "Cong ty: " << emp.getCompanyName() << " | Nganh: " << emp.getIndustry()
                 << " | So tin: " << emp.getPostedJobIds().size() << "\n";
        }
        cout << "\nTong: " << employers.size() << " nha tuyen dung\n";
    }
}

// 1.3. xóa người dùng
void System::admin_removeUser() {

    std::regex emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    string removeData;

    cout << "\n=== XOA NGUOI DUNG ===\n";

    cin.ignore();
    cout << "\nVui long nhap username/email/so dien thoai cua nguoi dung can xoa: ";
    getline(cin, removeData);

    User target;

    if (removeData.empty())
        return;

    if (removeData.substr(0, 3) == "+84") {
        target = User::findPhone(removeData);
    } else if (std::regex_match(removeData, emailRegex)) {
        target = User::findEmail(removeData);
    } else {
        target = User::findUser(removeData);
    }

    if (target.username.empty()) {
        cout << "#### Khong the tim thay nguoi dung voi thong tin " << removeData << "\n";
        return;
    }
    User::removeUser(target);
    cout << "#### Xoa thanh cong nguoi dung voi thong tin " << removeData << "\n";
}

// 1.4. Quản lý tin tuyển dụng
void System::admin_ManageJobs() {
    int choice;
    do {
        cout << "\n=== QUAN LY TIN TUYEN DUNG ===\n";
        cout << "1. Tim kiem tin tuyen dung\n";
        cout << "2. Xoa tin tuyen dung\n";
        cout << "3. Xem tat ca tin tuyen dung\n";
        cout << "0. Quay lai\n";
        cout << "Chon: ";
        cin >> choice;

        switch (choice) {
            case 1:
                admin_FindJobs();
                break;
            case 2:
                admin_removeJob();
                break;
            case 3: {
                cout << "\n--- TAT CA TIN TUYEN DUNG ---\n";
                json data = PBLJson::loadList("jobs.json");
                vector<Job> vJobs = Job::getAllJob(data);
                if (vJobs.size() < 3) {
                    for (auto &j: vJobs) {
                        j.display();
                    }
                    break;
                }
                int p = 1;
                while (p > 0) {
                    const int maxJobsPerPage = 3;
                    int totalPages = static_cast<int>(ceil(static_cast<double>(vJobs.size()) / maxJobsPerPage));
                    int start = (p - 1) * maxJobsPerPage;
                    int end = min(start + maxJobsPerPage, static_cast<int>(vJobs.size()));

                    for (int i = start; i < end; i++) {
                        cout << (i + 1) << ".\n";
                        vJobs[i].display();
                    }

                    cout << " ---- Trang " << p << "/" << ceil(vJobs.size() / maxJobsPerPage) << " ---\n";
                    cout << " ---- ---- Vui long nhap so trang can den (0 de thoat): ";

                    cin >> p;

                    if (p < 0 || p > totalPages) {
                        cout << "SO TRANG KHONG HOP LE!\n";
                        p = 1;
                    }
                }
                [[fallthrough]];
            }
            case 0:
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}

void System::admin_FindJobs() {
    int choice;
    do {
        cout << "\n=== TIM KIEM TIN TUYEN DUNG ===\n";
        cout << "1. Tim kiem theo id\n";
        cout << "2. Tim kiem theo nguoi dang\n";
        cout << "3. Tim kiem theo ten cong ty\n";
        cout << "0. Quay lai\n";
        cout << "Chon: ";
        cin >> choice;
        string removeData;
        std::regex emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
        User target;
        vector<Job> result;


        switch (choice) {
            case 1:

                cin.ignore();
                cout << "\n ---- Nhap id tin tuyen dung: ";
                getline(cin, removeData);

                Job::getJobById(removeData).display();
                break;

            case 2:

                cin.ignore();
                cout << "\n ---- Nhap username/email/so dien thoai cua nguoi dang: ";
                getline(cin, removeData);

                if (removeData.empty())
                    break;

                if (removeData.substr(0, 3) == "+84") {
                    target = User::findPhone(removeData);
                } else if (std::regex_match(removeData, emailRegex)) {
                    target = User::findEmail(removeData);
                } else {
                    target = User::findUser(removeData);
                }

                if (target.username.empty()) {
                    cout << "#### Khong the tim thay nguoi dung voi thong tin " << removeData << "\n";
                    break;
                }

                if (target.getRole() != "employer") {
                    cout << "#### Nguoi dung khong phai nha tuyen dung.\n";
                    break;
                }

                result = Job::getJobsByEmployerId(target.getId());

                for (auto &j: result) {
                    j.display();
                }
                break;

            case 3:

                cin.ignore();
                cout << "\n ---- Nhap ten cong ty tuyen dung: ";
                getline(cin, removeData);

                result = Job::getJobByEmployerCompanyName(removeData);

                for (auto &j: result) {
                    j.display();
                }

                break;

            case 0:
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}

void System::admin_removeJob() {
    int choice;
    do {
        cout << "\n=== XOA TIN TUYEN DUNG ===\n";
        cout << "1. Xoa theo id\n";
        cout << "2. Xoa toan bo theo nguoi dang\n";
        cout << "3. Xoa toan bo theo ten cong ty\n";
        cout << "0. Quay lai\n";
        cout << "Chon: ";
        cin >> choice;
        string removeData;
        std::regex emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
        User target;
        vector<Job> result;


        switch (choice) {
            case 1:

                cin.ignore();
                cout << "\n ---- Nhap id tin tuyen dung: ";
                getline(cin, removeData);

                Job::getJobById(removeData).remove();
                break;

            case 2:

                cin.ignore();
                cout << "\n ---- Nhap username/email/so dien thoai cua nguoi dang: ";
                getline(cin, removeData);

                if (removeData.empty())
                    break;

                if (removeData.substr(0, 3) == "+84") {
                    target = User::findPhone(removeData);
                } else if (std::regex_match(removeData, emailRegex)) {
                    target = User::findEmail(removeData);
                } else {
                    target = User::findUser(removeData);
                }

                if (target.username.empty()) {
                    cout << "#### Khong the tim thay nguoi dung voi thong tin " << removeData << "\n";
                    break;
                }

                if (target.getRole() != "employer") {
                    cout << "#### Nguoi dung khong phai nha tuyen dung.\n";
                    break;
                }

                result = Job::getJobsByEmployerId(target.getId());

                for (auto &j: result) {
                    j.remove();
                }
                break;

            case 3:

                cin.ignore();
                cout << "\n ---- Nhap ten cong ty tuyen dung: ";
                getline(cin, removeData);

                result = Job::getJobByEmployerCompanyName(removeData);

                for (auto &j: result) {
                    j.remove();
                }

                break;

            case 0:
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}

// ========================================
// EMPLOYER MENU
// ========================================
void System::employerMenu(Employer &e) {
    int choice;
    do {

        cout << "            MENU NHA TUYEN DUNG                    \n";

        cout << "Xin chao, " << e.getUsername();
        if (!e.getCompanyName().empty()) {
            cout << " (" << e.getCompanyName() << ")";
        }
        cout << "!\n\n";
        cout << "1. Dang tin tuyen dung\n";
        cout << "2. Xem tin da dang\n";
        cout << "3. Chinh sua tin tuyen dung\n";
        cout << "4. Xoa tin tuyen dung\n";
        cout << "5. Xem danh sach ung vien ung tuyen\n";
        // cout << "6. Loc ho so ung vien\n";
        cout << "6. Dong tin tuyen dung\n";
        cout << "7. Cap nhat thong tin cong ty\n";
        // cout << "9. Xem lich su tin da dang\n";
        cout << "8. Xem thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "\nChon: ";
        cin >> choice;

        switch (choice) {
            case 1:
                employer_PostJob(e);
                break;
            case 2:
                employer_ViewMyJobs(e);
                break;
            case 3:
                employer_EditJob(e);
                break;
            case 4:
                employer_DeleteJob(e);
                break;
            case 5:
                employer_ViewApplications(e);
                break;
            // case 6:
            //     employer_FilterApplications(e);
            //     break;
            case 6:
                employer_CloseJob(e);
                break;
            case 7:
                employer_UpdateProfile(e);
                break;
            // case 9:
            //     employer_ViewHistory(e);
            //     break;
            case 8:
                e.displayInfo();
                break;
            case 0:
                cout << "Dang xuat...\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}

// 2.1. Đăng tin tuyển dụng
void System::employer_PostJob(Employer &e) {
    cin.ignore();
    cout << "\n=== DANG TIN TUYEN DUNG ===\n";

    string title, description;
    int jobType, minExp, minAge, maxAge;
    double minSalary, maxSalary;
    vector<int> placeOfWork;

    cout << "Tieu de: ";
    getline(cin, title);

    cout << "\nLoai hinh (1: Full-time, 2: Part-time): ";
    cin >> jobType;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nDia diem lam viec (1: On-site, 2: Hybrid, 3: Remote) < co the chon nhieu hon 1 - vd: 1, 2, 3>: ";
    string _;
    getline(cin, _);

    stringstream ss(_);
    int x;
    char comma;

    while (ss >> x) {
        placeOfWork.push_back(x);
        if (!(ss >> comma))
            break;
    }


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
    newJob.setPlaceOfWork(placeOfWork);
    newJob.setJobType(jobType);
    newJob.setSalaryRange(minSalary, maxSalary);
    newJob.setMinExperience(minExp);
    newJob.setAgeRange(minAge, maxAge);
    newJob.setApplicants({});

    jobs.push_back(newJob);
    e.addPostedJob(jobId);

    newJob.postJob();

    cout << "\n✓ Dang tin thanh cong! Job ID: " << jobId << "\n";
    cout << "ℹ️  Tin tuyen dung dang cho Admin duyet.\n";
}

// 2.1. Xem tin đã đăng
void System::employer_ViewMyJobs(Employer &e) {
    cout << "\n=== TIN TUYEN DUNG DA DANG ===\n";
    const auto postedJobs = Job::getJobsByEmployerId(e.getId());

    if (postedJobs.empty()) {
        cout << "Ban chua dang tin tuyen dung nao!\n";
        return;
    }

    for (const auto &job: postedJobs) {
        job.display();
    }
}
// 2.1. Chỉnh sửa tin tuyển dụng
void System::employer_EditJob(Employer &e) {
    cin.ignore();
    cout << "\n=== CHINH SUA TIN TUYEN DUNG ===\n";
    cout << "Nhap Job ID: ";
    string jobId;
    getline(cin, jobId);

    Job job = Job::getJobById(jobId);
    if (job.getJobId().empty() || job.getEmployerId() != e.getId()) {
        cout << "### Khong tim thay hoac ban khong co quyen chinh sua!\n";
        return;
    }

    job.display();

    cout << "\n--- CHINH SUA ---\n";
    cout << "1. Mo ta\n";
    cout << "2. Muc luong\n";
    cout << "3. Kinh nghiem yeu cau\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1: {
            string desc;
            cout << "Nhap mo ta moi: ";
            getline(cin, desc);
            job.setDescription(desc);
            cout << "✓ Cap nhat thanh cong!\n";
            Job::editJob(job);
            break;
        }
        case 2: {
            double minSalary, maxSalary;
            cout << "Muc luong toi thieu: ";
            cin >> minSalary;
            cout << "Muc luong toi da: ";
            cin >> maxSalary;
            job.setSalaryRange(minSalary, maxSalary);
            cout << "✓ Cap nhat thanh cong!\n";
            Job::editJob(job);
            break;
        }
        case 3: {
            int minExp;
            cout << "Kinh nghiem toi thieu (nam): ";
            cin >> minExp;
            job.setMinExperience(minExp);
            cout << "✓ Cap nhat thanh cong!\n";
            Job::editJob(job);
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

    Job toBeDeleted = Job::getJobById(jobId);

    if (!toBeDeleted.getJobId().empty() && toBeDeleted.getEmployerId() != e.getId()) {
        cout << "Khong tim thay bai tuyen dung hoac ban khong co quyen xoa bai. Vui long kiem tra lai job id!\n";
        return;
    }

    toBeDeleted.display();

    char decision;
    cout << "\n#### Xoa bai tuyen dung nay? (y/n) ";
    cin >> decision;
    if (tolower(decision) == 'y') {
        toBeDeleted.remove();
        cout << "#### XOA BAI THANH CONG!\n";
    }
}

void filteredDisplay(const vector<JobSeeker> &vJ) {
    int count = 0;
    for (auto &i: vJ) {
        ++count;
        cout << "\n ########## \n" << count << ".\\ \n";
        i.displayInfo();
    }
}

void filterByExperience(const Job &j) {
    int choice;

    do {
        cout << "\n### LOC THEO KINH NGHIEM ###\n";
        cout << "1. Tang dan\n";
        cout << "2. Giam dan\n";
        cout << "3. Kinh nghiem toi thieu\n";
        cout << "0. Ve lai\n";
        cout << "Chon: ";
        cin >> choice;

        vector<JobSeeker> result;

        switch (choice) {
            case 1:
                result = j.getApplicationsFiltered(Job::FilterType::Experience, Job::SortType::ASC);
                filteredDisplay(result);
                break;

            case 2:
                result = j.getApplicationsFiltered(Job::FilterType::Experience, Job::SortType::DESC);
                filteredDisplay(result);
                break;

            case 3: {
                int min;
                cout << "Min: ";
                cin >> min;
                result = j.getApplicationsFiltered(Job::FilterType::Experience, Job::SortType::RANGE, min);
                filteredDisplay(result);
                break;
            }

            case 0:
                return;

            default:
                cout << "Lua chon khong hop le!\n";
        }

    } while (true);
}

void filterByAge(const Job &j) {
    int choice;

    do {
        cout << "\n### LOC THEO TUOI ###\n";
        cout << "1. Tang dan\n";
        cout << "2. Giam dan\n";
        cout << "3. Nam trong khoang\n";
        cout << "0. Ve lai\n";
        cout << "Chon: ";
        cin >> choice;

        vector<JobSeeker> result;

        switch (choice) {
            case 1:
                result = j.getApplicationsFiltered(Job::FilterType::Age, Job::SortType::ASC);
                filteredDisplay(result);
                break;

            case 2:
                result = j.getApplicationsFiltered(Job::FilterType::Age, Job::SortType::DESC);
                filteredDisplay(result);
                break;

            case 3: {
                int min, max;
                cout << "--- Toi thieu: ";
                cin >> min;
                cout << "--- Toi da: ";
                cin >> max;
                result = j.getApplicationsFiltered(Job::FilterType::Age, Job::SortType::RANGE, -1, min, max);
                filteredDisplay(result);
                break;
            }

            case 0:
                return;

            default:
                cout << "Lua chon khong hop le!\n";
        }

    } while (true);
}

void FilterApplications(const Job &j) {
    int choice;
    do {
        cout << "\n=== LOC HO SO UNG VIEN ===\n";
        cout << "1. Loc theo kinh nghiem\n";
        cout << "2. Loc theo tuoi\n";
        cout << "0. Ve lai menu\n";
        cout << "Chon: ";
        cin >> choice;

        switch (choice) {
            case 1:
                filterByExperience(j);
                break;

            case 2:
                filterByAge(j);
                break;

            case 0:
                return;

            default:
                cout << "Lua chon khong hop le!\n";
        }

    } while (true);
}

// 2.2. Xem danh sách ứng viên
void System::employer_ViewApplications(Employer &e) {
    cout << "\n=== DANH SACH UNG VIEN UNG TUYEN ===\n";

    string jobId;

    auto postedJobIds = Job::getJobsByEmployerId(e.getId());
    if (postedJobIds.empty()) {
        cout << "Ban chua co tin tuyen dung nao!\n";
        return;
    }

    cin.ignore();
    cout << "Nhap job id: ";
    getline(cin, jobId);

    Job job = Job::getJobById(jobId);
    if (!job.getJobId().empty() && job.getEmployerId() != e.getId()) {
        cout << "Khong tim thay thong tin tuyen dung hoac ban khong co quyen. Vui long kiem tra lai job id!";
        return;
    }
    if (job.getApplicants().empty()) {
        cout << "Khong tim thay ung vien. Neu khong dung hay lien he admin!\n";
        return;
    }

    const vector<JobSeeker> vApplications = job.getApplications();

    if (vApplications.size() < 3) {
        for (const auto &app: vApplications) {
            app.displayInfo();
        }
        return;
    }

    int p = 1;
    constexpr int maxApplicationsPerPage = 3;

    int totalPages = static_cast<int>(ceil(static_cast<double>(vApplications.size()) / maxApplicationsPerPage));

    while (p > 0) {
        int start = (p - 1) * maxApplicationsPerPage;
        int end = min(start + maxApplicationsPerPage, (int) vApplications.size());

        for (int i = start; i < end; i++) {
            cout << "\n##########\n" << (i + 1) << ".\n";
            vApplications[i].displayInfo();
        }

        cout << " ---- Trang " << p << "/" << totalPages << " ---\n";
        cout << " ---- ---- Vui long nhap so trang can den (-1 de thoat | 0 de loc ung vien): ";

        cin >> p;

        if (p < -1 || p > totalPages) {
            cout << "SO TRANG KHONG HOP LE!\n";
            p = 1;
        }

        if (p == 0) {
            FilterApplications(job);
            return;
        }
    }
}

// 2.2. Chọn ứng viên trúng tuyển
// void System::employer_SelectCandidate(Employer &e) {
//     cin.ignore();
//     cout << "\n=== CHON UNG VIEN TRUNG TUYEN ===\n";
//     cout << "Nhap Id bai dang tuyen dung: ";
//     string jobId;
//     getline(cin, jobId);
//
//     Job job = Job::getJobById(jobId);
//
//     if (job.getJobId().empty() || job.getEmployerId() != e.getId()) {
//         cout << "Khong tim thay bai dang hoac ban khong co quyen. Vui long kiem tra lai id bai dang!\n";
//         return;
//     }
//
//     employer_ViewApplications(e);
//
//     cout << "Nhap id cua nhung nguoi"
// }

void System::employer_CloseJob(Employer &e) {
    cin.ignore();
    cout << "\n=== DONG BAI TUYEN DUNG ===\n";
    cout << "Nhap Id bai dang tuyen dung: ";
    string jobId;
    getline(cin, jobId);

    Job job = Job::getJobById(jobId);

    if (job.getJobId().empty() || job.getEmployerId() != e.getId()) {
        cout << "Khong tim thay bai dang hoac ban khong co quyen. Vui long kiem tra lai id bai dang!\n";
        return;
    }

    job.setStatus(false);
    Job::editJob(job);
    cout << "#### Dong bai tuyen dung thanh cong.\n";
}

// 2.3. Cập nhật thông tin công ty
void System::employer_UpdateProfile(Employer &e) {
    cin.ignore();
    cout << "\n=== CAP NHAT THONG TIN CONG TY ===\n";
    cout << "1. Ten cong ty\n";
    // cout << "2. Dia chi\n";
    // cout << "3. Nganh nghe\n";
    // cout << "4. Mo ta\n";
    cout << "2. Email\n";
    cout << "3. So dien thoai\n";
    cout << "0. Ve lai menu\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    string value;
    switch (choice) {
        case 1:
            cout << "Nhap ten cong ty: ";
            getline(cin, value);
            e.setCompanyName(value);
            e.updateProfile();
            break;
        // case 2:
        //     cout << "Nhap dia chi: ";
        //     getline(cin, value);
        //     e.setCompanyAddress(value);
        //     break;
        // case 3:
        //     cout << "Nhap nganh nghe: ";
        //     getline(cin, value);
        //     e.setIndustry(value);
        //     break;
        // case 4:
        //     cout << "Nhap mo ta: ";
        //     getline(cin, value);
        //     e.setCompanyDescription(value);
        //     break;
        case 2:
            cout << "Nhap email: ";
            getline(cin, value);
            e.setEmail(value);
            e.updateProfile();
            break;
        case 3:
            cout << "Nhap so dien thoai: ";
            getline(cin, value);
            e.setPhone(value);
            e.updateProfile();
            break;
        case 0:
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

    if (postedJobIds.empty()) {
        cout << "Ban chua dang tin tuyen dung nao!\n";
        return;
    }

    cout << "Tong so tin da dang: " << postedJobIds.size() << "\n\n";

    for (const auto &jobId: postedJobIds) {
        if (const Job job = Job::getJobById(jobId); !job.getJobId().empty()) {
            cout << "Job ID: " << jobId << " | " << job.getTitle() << "\n";
            cout << "Trang thai: " << (job.getStatus() ? "Active" : "Closed") << "\n";
            cout << "Ngay dang: " << job.getPostedDate() << "\n";

            // Đếm số đơn ứng tuyển
            int appCount = 0;
            for (const auto &app: applications) {
                if (app.getJobId() == jobId) {
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


        cout << "1. Cap nhat CV truc tuyen\n";
        cout << "2. Chinh sua thong tin ca nhan\n";
        cout << "3. Tim kiem viec lam\n";
        // cout << "4. Xem chi tiet tin tuyen dung\n";
        cout << "4. Ung tuyen viec lam\n";
        cout << "5. Xem trang thai ung tuyen\n";
        cout << "6. Xem thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "\nChon: ";
        cin >> choice;

        switch (choice) {
            case 1:
                jobseeker_UpdateCV(js);
                break;
            case 2:
                jobseeker_UpdateProfile(js);
                break;
            case 3:
                jobseeker_SearchJobs(js);
                break;
            // case 4:
            //     jobseeker_ViewJobDetail(js);
            //     break;
            case 4:
                jobseeker_ApplyJob(js);
                break;
            case 5:
                jobseeker_ViewAppliesStatus(js);
                break;
            case 6:
                js.displayInfo();
                break;
            case 0:
                cout << "Dang xuat...\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}

// 3.1. Cập nhật CV trực tuyến
void System::jobseeker_UpdateCV(JobSeeker &js) {
    cout << "\n=== CAP NHAT CV TRUC TUYEN ===\n";

    string major;
    float yearsOfExp;

    cin.ignore();
    cout << "Chuyen nganh: ";
    getline(cin, major);
    cout << "So nam kinh nghiem: ";
    cin >> yearsOfExp;

    js.updateCVOnline(major, yearsOfExp);

    // Nhập kỹ năng
    cout << "\nNhap ky nang (nhap '0' de ket thuc):\n";
    js.clearSkills();
    cin.ignore();
    while (true) {
        string skill;
        cout << "Ky nang: ";
        getline(cin, skill);
        if (skill == "0")
            break;
        js.addSkill(skill);
    }

    js.updateCV();

    cout << "\n✓ Cap nhat CV thanh cong!\n";
    cout << "\n--- CV CUA BAN ---\n";
    cout << js.getCVOnline() << "\n";
    auto skills = js.getSkills();
    if (!skills.empty()) {
        cout << "Ky nang: ";
        for (size_t i = 0; i < skills.size(); i++) {
            cout << skills[i];
            if (i < skills.size() - 1)
                cout << ", ";
        }
        cout << "\n";
    }
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

    const std::regex emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    string value;
    bool cancel = false;
    switch (choice) {
        case 1:
            cout << "Nhap email moi: ";
            getline(cin, value);
            while (!regex_match(value, emailRegex) && !cancel) {
                cout << "Email khong hop le. Vui long nhap lai ( 0  de huy ): ";
                getline(cin, value);
                if (value == "0")
                    cancel = true;
            }
            if (cancel)
                break;
            js.setEmail(value);
            cout << "✓ Cap nhat thanh cong!\n";
            break;
        case 2:
            cout << "Nhap so dien thoai moi: +84";
            getline(cin, value);
            value = "+84" + value;
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
    // cout << "2. Tim theo nganh nghe\n";
    // cout << "3. Tim theo dia diem\n";
    // cout << "4. Tim theo muc luong\n";
    // cout << "5. Loc theo yeu cau ca nhan\n";
    cout << "0. Quay lai\n";
    cout << "Chon: ";
    int choice;
    cin >> choice;
    cin.ignore();

    vector<Job> vJobs = Job::getAllActiveJob();

    switch (choice) {
        case 1: {
            cout << "\n--- TAT CA CONG VIEC ---\n";
            if (vJobs.size() < 3) {
                for (auto &j : vJobs) {
                    j.display();
                }
                break;
            }

            int p = 1;
            const int maxJobsPerPage = 3;
            int totalPages = static_cast<int>(
                ceil(static_cast<double>(vJobs.size()) / maxJobsPerPage)
            );

            while (p > 0) {
                int start = (p - 1) * maxJobsPerPage;
                int end = min(start + maxJobsPerPage, (int)vJobs.size());

                for (int i = start; i < end; i++) {
                    cout << (i + 1) << ".\n";
                    vJobs[i].display();
                }

                cout << " ---- Trang " << p << "/" << totalPages << " ---\n";
                cout << " ---- ---- Vui long nhap so trang can den (0 de thoat): ";

                cin >> p;

                if (p < 0 || p > totalPages) {
                    cout << "SO TRANG KHONG HOP LE!\n";
                    p = 1;
                }
            }
            break;
        }
        // case 2: {
        //     cout << "\n--- CHON NGANH NGHE ---\n";
        //     for (size_t i = 0; i < categories.size(); i++) {
        //         cout << i + 1 << ". " << categories[i] << "\n";
        //     }
        //     cout << "Chon: ";
        //     int catChoice;
        //     cin >> catChoice;
        //
        //     if (catChoice > 0 && catChoice <= (int) categories.size()) {
        //         string category = categories[catChoice - 1];
        //         cout << "\n--- KET QUA TIM KIEM ---\n";
        //         for (const auto &job: jobs) {
        //             if (job.getIsApproved() && job.getStatus() == "active" && job.getCategory() == category) {
        //                 job.display();
        //             }
        //         }
        //     }
        //     break;
        // }
        // case 3: {
        //     cout << "\n--- CHON DIA DIEM ---\n";
        //     for (size_t i = 0; i < locations.size(); i++) {
        //         cout << i + 1 << ". " << locations[i] << "\n";
        //     }
        //     cout << "Chon: ";
        //     int locChoice;
        //     cin >> locChoice;
        //
        //     if (locChoice > 0 && locChoice <= (int) locations.size()) {
        //         string location = locations[locChoice - 1];
        //         cout << "\n--- KET QUA TIM KIEM ---\n";
        //         for (const auto &job: jobs) {
        //             if (job.getIsApproved() && job.getStatus() == "active" && job.getLocation() == location) {
        //                 job.display();
        //             }
        //         }
        //     }
        //     break;
        // }
        // case 4: {
        //     double minSalary;
        //     cout << "Nhap muc luong toi thieu: ";
        //     cin >> minSalary;
        //
        //     cout << "\n--- KET QUA TIM KIEM ---\n";
        //     for (const auto &job: jobs) {
        //         if (job.getIsApproved() && job.getStatus() == "active" && job.getMaxSalary() >= minSalary) {
        //             job.display();
        //         }
        //     }
        //     break;
        // }
        // case 5: {
        //     // Lọc theo kinh nghiệm, mức lương
        //     int myExp = js.getYearsOfExperience();
        //
        //     cout << "\n--- LOC THEO YEU CAU CA NHAN ---\n";
        //     cout << "Kinh nghiem cua ban: " << myExp << " nam\n";
        //     cout << "Nhap muc luong mong muon: ";
        //     double expectedSalary;
        //     cin >> expectedSalary;
        //
        //     cout << "\n--- KET QUA LOC ---\n";
        //     for (const auto &job: jobs) {
        //         if (job.getIsApproved() && job.getStatus() == "active" && job.getMinExperience() <= myExp &&
        //             job.getMaxSalary() >= expectedSalary) {
        //             job.display();
        //         }
        //     }
        //     break;
        // }
        case 0:
            break;
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

    Job job = Job::getJobById(jobId);
    if (job.getJobId().empty() || job.getStatus()) {
        cout << "### Khong tim thay tin tuyen dung!\n";
        return;
    }

    job.display();

    // Hiển thị thông tin công ty
    Employer *emp = findEmployerById(job.getEmployerId());
    if (emp) {
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

    cout << "Nhap Job ID: ";
    string jobId;
    getline(cin, jobId);

    if (!js.getUpdated()) {
        cout << "### Ban chua cap nhat CV cua minh. Vui long cap nhat CV cua minh o menu de tiep tuc!\n";
        return;
    }

    Job job = Job::getJobById(jobId);
    if (job.getJobId().empty() || !job.getStatus()) {
        cout << "### Khong tim thay tin tuyen dung hoac tin da dong!\n";
        return;
    }

    // Kiểm tra đã ứng tuyển chưa
    auto appliedJobs = js.getAppliedJobs();
    if (find(appliedJobs.begin(), appliedJobs.end(), jobId) != appliedJobs.end()) {
        cout << "### Ban da ung tuyen tin nay roi!\n";
        return;
    }

    if (js.getYearsOfExperience() < job.getMinExperience() || js.getAge() < job.getMinAge() ||
        js.getAge() > job.getMaxAge()) {
        cout << "### Ban khong du dieu kien ung tuyen. Vui long kiem tra lai yeu cau!\n";
        return;
    }

    js.addAppliedJob(jobId);
    job.addApplicant(js.getId());

    cout << "\n✓ Ung tuyen thanh cong!\n";
    cout << "Ban se nhan duoc email tu nha tuyen dung khi da duoc chap nhan vao vong trong.\n";
}

// 3.3. Theo dõi trạng thái hồ sơ
void System::jobseeker_ViewAppliesStatus(JobSeeker &js) {
    cout << "\n=== TRANG THAI UNG TUYEN ===\n";

    vector<string> appliedJobs = js.getAppliedJobs();
    if (appliedJobs.empty()) {
        cout << "#### Ban chua ung tuyen cong viec nao.\n";
        return;
    }
    cout << "Tong so cong viec da ung tuyen: " << appliedJobs.size() << "\n\n";

    vector<string> accepted, notAccepted, waitingForReviews;

    for (auto &jobId: appliedJobs) {
        const int status = Job::getJobById(jobId).checkApplyStatus(js.getId());
        if (status == 0)
            accepted.push_back(jobId);
        else if (status == -1)
            notAccepted.push_back(jobId);
        else
            waitingForReviews.push_back(jobId);
    }

    cout << "--- " << accepted.size() << " cong ty da chap nhan.\n";
    cout << "--- " << notAccepted.size() << " cong ty da tu choi.\n";
    cout << "--- " << waitingForReviews.size() << " cong ty dang trong qua trinh xem xet.\n";

    cout << "\n### Xem chi tiet:\n";
    cout << "1. Da chap nhan.\n";
    cout << "2. Da tu choi.\n";
    cout << "3. Dang trong qua trinh xem xet.\n";
    cout << "0. Quay lai.\n";

    int decision;
    cin >> decision;

    switch (decision) {
        case 1:
            for (auto &i: accepted) {
                Job::getJobById(i).acceptedView();
            }
            break;
        case 2:
            for (auto &i: notAccepted) {
                Job::getJobById(i).notAcceptedView();
            }
            break;
        case 3:
            for (auto &i: waitingForReviews) {
                Job::getJobById(i).display();
            }
            break;
        case 0:
            break;
        default:
            cout << "Lua chon khong hop le.\n";
    }
}
// 3.4. Xem lịch sử ứng tuyển (tiếp)
void System::jobseeker_ViewHistory(JobSeeker &js) {
    cout << "\n=== LICH SU UNG TUYEN ===\n";

    auto appliedJobs = js.getAppliedJobs();
    if (appliedJobs.empty()) {
        cout << "Ban chua ung tuyen cong viec nao!\n";
        return;
    }

    cout << "Tong so cong viec da ung tuyen: " << appliedJobs.size() << "\n\n";

    for (const auto &jobId: appliedJobs) {
        if (Job job = Job::getJobById(jobId); !job.getJobId().empty()) {
            cout << "Job ID: " << jobId << " | " << job.getTitle() << "\n";
            cout << "Cong ty: ";
            Employer *emp = findEmployerById(job.getEmployerId());
            if (emp) {
                cout << emp->getCompanyName();
            }
            cout << "\n";

            // Tìm trạng thái đơn ứng tuyển
            for (const auto &app: applications) {
                if (app.getJobId() == jobId && app.getJobSeekerId() == js.getId()) {
                    cout << "Trang thai: " << app.getStatusString() << "\n";
                    break;
                }
            }
            cout << "---\n";
        }
    }
}

// ========================================
// MAIN SYSTEM FUNCTIONS
// ========================================

void System::registerUser() {
    int role;
    string id, username, password, email, phone, fullName, dateOfBirth;

    // regular expression
    const std::regex passRegex(R"((?=.*[a-z])(?=.*[A-Z])(?=.*\d).{8,})");
    const std::regex emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    cin.ignore();
    cout << "             DANG KY TAI KHOAN                   \n";
    cout << "\nChon loai tai khoan:\n";
    cout << "1. Ung vien (JobSeeker)\n";
    cout << "2. Nha tuyen dung (Employer)\n";
    cout << "Chon: ";
    cin >> role;
    cin.ignore();

    if (role != 1 && role != 2) {
        cout << "### Lua chon khong hop le, vui long chon lai!\n";
        cout << "1. Ung vien (JobSeeker)\n";
        cout << "2. Nha tuyen dung (Employer)\n";
        cout << "Chon: ";
        cin >> role;
        cin.ignore();
    }

    cout << "Nhap username: ";
    getline(cin, username);

    User u = User::findUser(username);
    while (!u.username.empty()) {
        cout << "### Username da ton tai, vui long nhap lai!\n";
        cout << "Nhap username: ";
        getline(cin, username);
        u = User::findUser(username);
    }

    while (true) {
        cout << "Nhap password ( 8+ ky tu, 1+ chu in hoa, 1+ chu in thuong, 1+ chu so ): ";
        getline(cin, password);
        if (std::regex_match(password, passRegex)) {
            password = sha256(password);
            break;
        }
        cout << "### Password khong hop le! Vui long thu lai.\n";
    }

    cout << "Nhap ten day du: ";
    getline(cin, fullName);

    cout << "Nhap ngay sinh ( yyyy-mm-dd ): ";
    getline(cin, dateOfBirth);

    while (true) {
        cout << "Nhap email: ";
        getline(cin, email);
        if (!User::findEmail(email).getEmail().empty()) {
            cout << "Email da co tren he thong, vui long nhap lai!\n";
            continue;
        }
        if (std::regex_match(email, emailRegex))
            break;
        cout << "### Email khong hop le! Vui long thu lai.\n";
    }

    cout << "Nhap so dien thoai: +84 ";
    getline(cin, phone);

    phone.insert(0, "+84");

    while (true) {
        if (User::findPhone(phone).getPhone().empty())
            break;
        cout << "So dien thoai da co tren he thong, vui long nhap lai! +84 ";
        getline(cin, phone);
        phone.insert(0, "+84");
    }

    string companyName;

    if (role == 2) {
        cout << "Nhap ten cong ty: ";
        getline(cin, companyName);
    }

    User::addUser(username, password, fullName, dateOfBirth, phone, email, role == 1 ? "jobSeeker" : "employer",
                  companyName);

    if (role == 1) {
        cout << "\n#### Dang ky Ung vien thanh cong!\n";
    } else if (role == 2) {
        cout << "\n✓ Dang ky Nha tuyen dung thanh cong!\n";
    }
    cout << "Vui long dang nhap de su dung he thong.\n";
}

void System::loginUser() {
    string username, password;
    cin.ignore();


    cout << "                  DANG NHAP                        \n";
    cout << "\nUsername: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    const string hashedPassword = sha256(password);

    const User user = User::findUser(username);

    if (user.getUsername().empty()) {
        cout << "\n### Sai username hoac password!\n";
        return;
    }

    // password is stored in User::password as hashedPassword from JSON
    if (!user.getPassword().empty() && user.getPassword() == hashedPassword) {
        const string &role = user.role;

        if (role == "admin") {
            cout << "\n✓ Dang nhap thanh cong voi quyen Admin!\n";
            adminMenu(admin);
            return;
        }
        if (role == "jobSeeker") {
            cout << "\n✓ Dang nhap thanh cong!\n";
            JobSeeker _(user);
            jobSeekerMenu(_);
            return;
        }
        if (role == "employer") {
            cout << "\n✓ Dang nhap thanh cong!\n";
            Employer _(user);
            employerMenu(_);
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

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 0:

                cout << "     Cam on da su dung he thong!                  \n";
                cout << "    Hen gap lai!                                  \n";
                break;
            default:
                cout << "\n### Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}
