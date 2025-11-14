#include "Job.h"
#include <iostream>
#include <iomanip>
using namespace std;

void Job::display() const {
    cout << "\n========================================\n";
    cout << "Job ID: " << jobId << "\n";
    cout << "Title: " << title << "\n";
    cout << "Employer ID: " << employerId << "\n";
    cout << "Category: " << category << "\n";
    cout << "Location: " << location << "\n";
    cout << "Salary: " << fixed << setprecision(2) << salary << " VND\n";
    cout << "Description: " << description << "\n";
    cout << "Status: " << (isApproved ? "Approved" : "Pending") << "\n";
    cout << "========================================\n";
}