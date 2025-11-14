#include "System.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

// ========== HELPER FUNCTIONS ==========

string System::generateJobId() {
    return "JOB" + to_string(nextJobId++);
}

Job* System::findJobById(string jobId) {
    for(auto &job : jobs) {
        if(job.getJobId() == jobId) {
            return &job;
        }
    }
    return nullptr;
}

// ========== JOBSEEKER MENU ==========

void System::jobSeekerMenu(JobSeeker &js) {
    int choice;
    do {

        cout << "1. Tim kiem viec lam\n";
        cout << "2. Ung tuyen viec lam\n";
        cout << "3. Xem cong viec da ung tuyen\n";
        cout << "4. Cap nhat thong tin ca nhan\n";
        cout << "5. Xem thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "Chon: "; cin >> choice;

        switch(choice){
            case 1: searchJobs(js); break;
            case 2: applyForJob(js); break;
            case 3: viewAppliedJobs(js); break;
            case 4: updateProfile(js); break;
            case 5: js.displayInfo(); break;
            case 0: cout << "Dang xuat...\n"; break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while(choice != 0);
}

void System::searchJobs(JobSeeker &js) {
    cout << "\n=== TIM KIEM VIEC LAM ===\n";
    cout << "1. Xem tat ca cong viec\n";
    cout << "2. Tim theo nganh nghe\n";
    cout << "3. Tim theo dia diem\n";
    cout << "4. Tim theo muc luong\n";
    cout << "Chon: ";

    int choice;
    cin >> choice;
    cin.ignore();

    switch(choice) {
        case 1: {
            cout << "\n--- DANH SACH CONG VIEC ---\n";
            for(const auto &job : jobs) {
                if(job.getIsApproved()) {
                    job.display();
                }
            }
            break;
        }
        case 2: {
            string category;
            cout << "Nhap nganh nghe: ";
            getline(cin, category);

            cout << "\n--- KET QUA TIM KIEM ---\n";
            bool found = false;
            for(const auto &job : jobs) {
                if(job.getIsApproved() && job.getCategory() == category) {
                    job.display();
                    found = true;
                }
            }
            if(!found) cout << "Khong tim thay cong viec phu hop!\n";
            break;
        }
        case 3: {
            string location;
            cout << "Nhap dia diem: ";
            getline(cin, location);

            cout << "\n--- KET QUA TIM KIEM ---\n";
            bool found = false;
            for(const auto &job : jobs) {
                if(job.getIsApproved() && job.getLocation() == location) {
                    job.display();
                    found = true;
                }
            }
            if(!found) cout << "Khong tim thay cong viec phu hop!\n";
            break;
        }
        case 4: {
            double minSalary;
            cout << "Nhap muc luong toi thieu: ";
            cin >> minSalary;

            cout << "\n--- KET QUA TIM KIEM ---\n";
            bool found = false;
            for(const auto &job : jobs) {
                if(job.getIsApproved() && job.getSalary() >= minSalary) {
                    job.display();
                    found = true;
                }
            }
            if(!found) cout << "Khong tim thay cong viec phu hop!\n";
            break;
        }
        default:
            cout << "Lua chon khong hop le!\n";
    }
}

void System::applyForJob(JobSeeker &js) {
    cout << "\n=== UNG TUYEN VIEC LAM ===\n";

    if(js.getCV().empty()) {
        cout << "Ban chua co CV! Vui long cap nhat CV truoc.\n";
        return;
    }

    cout << "Nhap Job ID de ung tuyen: ";
    string jobId;
    cin >> jobId;

    Job* job = findJobById(jobId);
    if(job && job->getIsApproved()) {
        js.applyForJob(jobId);
        cout << "Ung tuyen thanh cong!\n";
    } else {
        cout << "Khong tim thay cong viec hoac cong viec chua duoc duyet!\n";
    }
}

void System::viewAppliedJobs(JobSeeker &js) {
    cout << "\n=== CONG VIEC DA UNG TUYEN ===\n";
    auto appliedJobs = js.getAppliedJobs();

    if(appliedJobs.empty()) {
        cout << "Ban chua ung tuyen cong viec nao!\n";
        return;
    }

    for(const auto &jobId : appliedJobs) {
        Job* job = findJobById(jobId);
        if(job) {
            job->display();
        }
    }
}

void System::updateProfile(JobSeeker &js) {
    cout << "\n=== CAP NHAT THONG TIN ===\n";
    cout << "1. Cap nhat CV\n";
    cout << "2. Cap nhat email\n";
    cout << "3. Cap nhat so dien thoai\n";
    cout << "4. Doi mat khau\n";
    cout << "Chon: ";

    int choice;
    cin >> choice;
    cin.ignore();

    switch(choice) {
        case 1: {
            string cv;
            cout << "Nhap CV (kinh nghiem, ky nang...): ";
            getline(cin, cv);
            js.updateCV(cv);
            cout << "Cap nhat CV thanh cong!\n";
            break;
        }
        case 2: {
            string email;
            cout << "Nhap email moi: ";
            cin >> email;
            js.setEmail(email);
            cout << "Cap nhat email thanh cong!\n";
            break;
        }
        case 3: {
            string phone;
            cout << "Nhap so dien thoai moi: ";
            cin >> phone;
            js.setPhone(phone);
            cout << "Cap nhat so dien thoai thanh cong!\n";
            break;
        }
        case 4: {
            string newPassword;
            cout << "Nhap mat khau moi: ";
            cin >> newPassword;
            js.setPassword(newPassword);
            cout << "Doi mat khau thanh cong!\n";
            break;
        }
        default:
            cout << "Lua chon khong hop le!\n";
    }
}

// ========== EMPLOYER MENU ==========

void System::employerMenu(Employer &e) {
    int choice;
    do {

        cout << "1. Dang tin tuyen dung\n";
        cout << "2. Xem tin da dang\n";
        cout << "3. Chinh sua tin tuyen dung\n";
        cout << "4. Xoa tin tuyen dung\n";
        cout << "5. Xem thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "Chon: "; cin >> choice;

        switch(choice){
            case 1: createJob(e); break;
            case 2: viewMyJobs(e); break;
            case 3: editJob(e); break;
            case 4: deleteJob(e); break;
            case 5: e.displayInfo(); break;
            case 0: cout << "Dang xuat...\n"; break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while(choice != 0);
}

void System::createJob(Employer &e) {
    cin.ignore();

    string title, description, location, category;
    double salary;

    cout << "\n=== DANG TIN TUYEN DUNG ===\n";
    cout << "Tieu de: "; getline(cin, title);
    cout << "Nganh nghe: "; getline(cin, category);
    cout << "Dia diem: "; getline(cin, location);
    cout << "Luong: "; cin >> salary;
    cin.ignore();
    cout << "Mo ta cong viec: "; getline(cin, description);

    string jobId = generateJobId();
    Job newJob(jobId, title, e.getId(), description, location, salary, category);

    jobs.push_back(newJob);
    e.addPostedJob(jobId);

    cout << "Dang tin thanh cong! Job ID: " << jobId << "\n";
    cout << "Tin tuyen dung dang cho duyet tu Admin.\n";
}

void System::viewMyJobs(Employer &e) {
    cout << "\n=== TIN DA DANG ===\n";
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

void System::editJob(Employer &e) {
    cout << "\n=== CHINH SUA TIN TUYEN DUNG ===\n";
    cout << "Nhap Job ID can chinh sua: ";
    string jobId;
    cin >> jobId;
    cin.ignore();

    Job* job = findJobById(jobId);

    if(!job || job->getEmployerId() != e.getId()) {
        cout << "Khong tim thay cong viec hoac ban khong co quyen chinh sua!\n";
        return;
    }

    cout << "1. Chinh sua mo ta\n";
    cout << "2. Chinh sua dia diem\n";
    cout << "3. Chinh sua luong\n";
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
            cout << "Cap nhat thanh cong!\n";
            break;
        }
        case 2: {
            string loc;
            cout << "Nhap dia diem moi: ";
            getline(cin, loc);
            job->setLocation(loc);
            cout << "Cap nhat thanh cong!\n";
            break;
        }
        case 3: {
            double sal;
            cout << "Nhap muc luong moi: ";
            cin >> sal;
            job->setSalary(sal);
            cout << "Cap nhat thanh cong!\n";
            break;
        }
        default:
            cout << "Lua chon khong hop le!\n";
    }
}

void System::deleteJob(Employer &e) {
    cout << "\n=== XOA TIN TUYEN DUNG ===\n";
    cout << "Nhap Job ID can xoa: ";
    string jobId;
    cin >> jobId;

    auto it = find_if(jobs.begin(), jobs.end(),
                      [&](const Job &job) {
                          return job.getJobId() == jobId &&
                                 job.getEmployerId() == e.getId();
                      });

    if(it != jobs.end()) {
        jobs.erase(it);
        cout << "Xoa tin tuyen dung thanh cong!\n";
    } else {
        cout << "Khong tim thay cong viec hoac ban khong co quyen xoa!\n";
    }
}

// ========== ADMIN MENU ==========

void System::adminMenu(Admin &a) {
    int choice;
    do {

        cout << "1. Quan ly tai khoan\n";
        cout << "2. Duyet tin tuyen dung\n";
        cout << "3. Xem tat ca tin tuyen dung\n";
        cout << "4. Thong ke he thong\n";
        cout << "0. Dang xuat\n";
        cout << "Chon: "; cin >> choice;

        switch(choice){
            case 1: {
                cout << "\n--- DANH SACH JOBSEEKER ---\n";
                for(auto &js: jobSeekers) {
                    js.displayInfo();
                    cout << "---\n";
                }

                cout << "\n--- DANH SACH EMPLOYER ---\n";
                for(auto &emp: employers) {
                    emp.displayInfo();
                    cout << "---\n";
                }

                cout << "\nNhap ID de xoa tai khoan (hoac '0' de thoat): ";
                string delId; cin >> delId;

                if(delId != "0") {
                    auto itJs = remove_if(jobSeekers.begin(), jobSeekers.end(),
                                          [&](JobSeeker &js){ return js.getId() == delId; });
                    jobSeekers.erase(itJs, jobSeekers.end());

                    auto itE = remove_if(employers.begin(), employers.end(),
                                         [&](Employer &e){ return e.getId() == delId; });
                    employers.erase(itE, employers.end());

                    cout << "Xoa thanh cong!\n";
                }
                break;
            }
            case 2: approveJobs(); break;
            case 3: viewAllJobs(); break;
            case 4: viewStatistics(); break;
            case 0: cout << "Dang xuat...\n"; break;
            default: cout << "Lua chon khong hop le!\n";
        }

    } while(choice != 0);
}

void System::approveJobs() {
    cout << "\n=== DUYET TIN TUYEN DUNG ===\n";

    bool hasPending = false;
    for(auto &job : jobs) {
        if(!job.getIsApproved()) {
            hasPending = true;
            job.display();

            cout << "Duyet tin nay? (1: Co, 0: Khong): ";
            int approve;
            cin >> approve;

            if(approve == 1) {
                job.setApproved(true);
                cout << "Da duyet tin tuyen dung!\n";
            }
        }
    }

    if(!hasPending) {
        cout << "Khong co tin tuyen dung nao can duyet!\n";
    }
}

void System::viewAllJobs() {
    cout << "\n=== TAT CA TIN TUYEN DUNG ===\n";

    if(jobs.empty()) {
        cout << "Chua co tin tuyen dung nao!\n";
        return;
    }

    for(const auto &job : jobs) {
        job.display();
    }
}

void System::viewStatistics() {


    cout << "Tong so JobSeeker: " << jobSeekers.size() << "\n";
    cout << "Tong so Employer: " << employers.size() << "\n";
    cout << "Tong so Admin: " << admins.size() << "\n";
    cout << "Tong so tin tuyen dung: " << jobs.size() << "\n";

    int approvedJobs = 0;
    for(const auto &job : jobs) {
        if(job.getIsApproved()) approvedJobs++;
    }
    cout << "Tin da duyet: " << approvedJobs << "\n";
    cout << "Tin cho duyet: " << (jobs.size() - approvedJobs) << "\n";
}

// ========== MAIN SYSTEM FUNCTIONS ==========

void System::registerUser() {
    int role;
    string id, username, password, email, phone;

    cin.ignore();
    cout << "\n=== DANG KY TAI KHOAN ===\n";
    cout << "Nhap ID: "; getline(cin, id);
    cout << "Nhap username: "; getline(cin, username);
    cout << "Nhap password: "; getline(cin, password);
    cout << "Nhap email: "; getline(cin, email);
    cout << "Nhap phone: "; getline(cin, phone);

    cout << "Chon role:\n";
    cout << "1. JobSeeker (Nguoi tim viec)\n";
    cout << "2. Employer (Nha tuyen dung)\n";
    cout << "3. Admin (Quan tri vien)\n";
    cout << "Chon: "; cin >> role;

    if(role == 1) {
        jobSeekers.emplace_back(id, username, password, email, phone);
        cout << "Dang ky JobSeeker thanh cong!\n";
    }
    else if(role == 2) {
        employers.emplace_back(id, username, password, email, phone);
        cout << "Dang ky Employer thanh cong!\n";

        cin.ignore();
        string companyName;
        cout << "Nhap ten cong ty: ";
        getline(cin, companyName);
        employers.back().setCompanyName(companyName);
    }
    else if(role == 3) {
        admins.emplace_back(id, username, password, email, phone);
        cout << "Dang ky Admin thanh cong!\n";
    }
    else {
        cout << "Lua chon khong hop le!\n";
    }
}

void System::loginUser() {
    string username, password;
    cin.ignore();

    cout << "\n=== DANG NHAP ===\n";
    cout << "Username: "; getline(cin, username);
    cout << "Password: "; getline(cin, password);

    for(auto &js: jobSeekers){
        if(js.getUsername() == username && js.getPassword() == password){
            cout << "\nDang nhap thanh cong! Chao mung " << username << "!\n";
            jobSeekerMenu(js);
            return;
        }
    }

    for(auto &e: employers){
        if(e.getUsername() == username && e.getPassword() == password){
            cout << "\nDang nhap thanh cong! Chao mung " << username << "!\n";
            employerMenu(e);
            return;
        }
    }

    for(auto &a: admins){
        if(a.getUsername() == username && a.getPassword() == password){
            cout << "\nDang nhap thanh cong! Chao mung Admin " << username << "!\n";
            adminMenu(a);
            return;
        }
    }

    cout << "Sai tai khoan hoac mat khau!\n";
}

void System::run() {
    int choice;
    do{
        cout << "\n";

        cout << "  HE THONG TIM VIEC LAM TRUC TUYEN  \n";

        cout << "1. Dang ky tai khoan\n";
        cout << "2. Dang nhap\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        cin >> choice;

        switch(choice){
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 0:
                cout << "\n========================================\n";
                cout << "Cam on ban da su dung he thong!\n";
                cout << "========================================\n";
                break;
            default: cout << "Lua chon khong hop le!\n";
        }
    }while(choice != 0);
}