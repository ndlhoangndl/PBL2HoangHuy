#include <bits/stdc++.h>
using namespace std;

// ===== ABSTRACT BASE CLASSES =====

// Abstract class cho User
class User {
protected:
    int id;
    string username;
    string email;
    string passwordHash;
    bool isActive;
    string createdDate;

public:
    User(int id = 0, const string& username = "", const string& email = "",
         const string& password = "", bool active = true)
        : id(id), username(username), email(email), isActive(active) {
        setPassword(password);
        setCreatedDate();
    }

    virtual ~User() = default;

    // Pure virtual functions (Abstract methods)
    virtual string getDisplayInfo() const = 0;
    virtual string toCSV() const = 0;
    virtual bool validateData() const = 0;

    // Getters
    int getId() const { return id; }
    string getUsername() const { return username; }
    string getEmail() const { return email; }
    bool getIsActive() const { return isActive; }
    string getCreatedDate() const { return createdDate; }

    // Setters
    void setId(int newId) { id = newId; }
    void setUsername(const string& newUsername) { username = newUsername; }
    void setEmail(const string& newEmail) { email = newEmail; }
    void setIsActive(bool active) { isActive = active; }

    // Password management
    void setPassword(const string& password) {
        passwordHash = hashPassword(password);
    }

    bool verifyPassword(const string& password) const {
        return passwordHash == hashPassword(password);
    }

protected:
    void setCreatedDate() {
        time_t now = time(0);
        char* timeStr = ctime(&now);
        createdDate = string(timeStr);
        if (!createdDate.empty() && createdDate.back() == '\n') {
            createdDate.pop_back();
        }
    }

private:
    string hashPassword(const string& password) const {
        size_t hashValue = std::hash<string>{}(password);
        ostringstream oss;
        oss << hex << hashValue;
        return oss.str();
    }
};

// Abstract class cho Data Access Object
class BaseDAO {
public:
    virtual ~BaseDAO() = default;
    virtual bool loadFromFile() = 0;
    virtual bool saveToFile() = 0;
    virtual void clearData() = 0;
};

// ===== UTILITY CLASSES =====

// Class cho CSV utilities
class CSVHelper {
public:
    static string escape(const string& str) {
        string result;
        for (char c : str) {
            if (c == '"') result += "\"\"";
            else result += c;
        }
        return result;
    }

    static string unescape(const string& str) {
        string result;
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == '"' && i + 1 < str.size() && str[i + 1] == '"') {
                result += '"';
                ++i;
            } else {
                result += str[i];
            }
        }
        return result;
    }

    static vector<string> parseLine(const string& line) {
        vector<string> fields;
        string current;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char ch = line[i];
            if (ch == '"') {
                if (!inQuotes) {
                    inQuotes = true;
                } else {
                    if (i + 1 < line.size() && line[i + 1] == '"') {
                        current += '"';
                        ++i;
                    } else {
                        inQuotes = false;
                    }
                }
            } else if (ch == ',' && !inQuotes) {
                fields.push_back(current);
                current.clear();
            } else {
                current += ch;
            }
        }
        fields.push_back(current);
        return fields;
    }

    static string removeQuotes(const string& str) {
        if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
            return unescape(str.substr(1, str.size() - 2));
        }
        return str;
    }
};

// Class cho Input/Output utilities
class IOHelper {
public:
    static string inputString(const string& prompt) {
        cout << prompt;
        string input;
        getline(cin, input);
        return input;
    }

    static int inputInteger(const string& prompt) {
        while (true) {
            cout << prompt;
            string input;
            getline(cin, input);
            try {
                return stoi(input);
            } catch (...) {
                cout << "Vui long nhap so hop le!\n";
            }
        }
    }

    static bool confirmAction(const string& message) {
        string confirmation = inputString(message + " (YES/NO): ");
        return confirmation == "YES" || confirmation == "yes";
    }

    static void printSeparator(char ch = '=', int length = 50) {
        cout << string(length, ch) << "\n";
    }

    static void printHeader(const string& title) {
        printSeparator();
        cout << "    " << title << "\n";
        printSeparator();
    }
};

// ===== CONCRETE CLASSES =====

// Employer class (kế thừa từ User)
class Employer : public User {
private:
    string companyName;
    string businessField;
    string address;
    string phoneNumber;
    bool isBanned;

public:
    // Constructor
    Employer(int id = 0, const string& username = "", const string& companyName = "",
             const string& email = "", const string& password = "",
             const string& businessField = "", const string& address = "",
             const string& phoneNumber = "", bool banned = false, bool active = true)
        : User(id, username, email, password, active),
          companyName(companyName), businessField(businessField),
          address(address), phoneNumber(phoneNumber), isBanned(banned) {}

    // Copy constructor
    Employer(const Employer& other)
        : User(other), companyName(other.companyName), businessField(other.businessField),
          address(other.address), phoneNumber(other.phoneNumber), isBanned(other.isBanned) {}

    // Assignment operator
    Employer& operator=(const Employer& other) {
        if (this != &other) {
            User::operator=(other);
            companyName = other.companyName;
            businessField = other.businessField;
            address = other.address;
            phoneNumber = other.phoneNumber;
            isBanned = other.isBanned;
        }
        return *this;
    }

    // Getters
    string getCompanyName() const { return companyName; }
    string getBusinessField() const { return businessField; }
    string getAddress() const { return address; }
    string getPhoneNumber() const { return phoneNumber; }
    bool getIsBanned() const { return isBanned; }

    // Setters
    void setCompanyName(const string& name) { companyName = name; }
    void setBusinessField(const string& field) { businessField = field; }
    void setAddress(const string& addr) { address = addr; }
    void setPhoneNumber(const string& phone) { phoneNumber = phone; }
    void setIsBanned(bool banned) { isBanned = banned; }

    // Override virtual functions
    string getDisplayInfo() const override {
        ostringstream oss;
        oss << "\n" << string(40, '=') << "\n";
        oss << "ID: " << id << "\n";
        oss << "Ten dang nhap: " << username << "\n";
        oss << "Ten cong ty: " << companyName << "\n";
        oss << "Email: " << email << "\n";
        oss << "Linh vuc: " << (businessField.empty() ? "Chua cap nhat" : businessField) << "\n";
        oss << "Dia chi: " << (address.empty() ? "Chua cap nhat" : address) << "\n";
        oss << "So dien thoai: " << (phoneNumber.empty() ? "Chua cap nhat" : phoneNumber) << "\n";
        oss << "Ngay tao: " << createdDate << "\n";
        oss << "Trang thai: " << (isBanned ? "BI KHOA" : "HOAT DONG") << "\n";
        oss << string(40, '=') << "\n";
        return oss.str();
    }

    string toCSV() const override {
        ostringstream oss;
        oss << id << ","
            << "\"" << CSVHelper::escape(username) << "\","
            << "\"" << CSVHelper::escape(companyName) << "\","
            << "\"" << CSVHelper::escape(email) << "\","
            << "\"" << CSVHelper::escape(passwordHash) << "\","
            << (isBanned ? "1" : "0") << ","
            << "\"" << CSVHelper::escape(createdDate) << "\","
            << "\"" << CSVHelper::escape(businessField) << "\","
            << "\"" << CSVHelper::escape(address) << "\","
            << "\"" << CSVHelper::escape(phoneNumber) << "\"";
        return oss.str();
    }

    bool validateData() const override {
        return !username.empty() && !companyName.empty() && !email.empty();
    }

    // Business methods
    void updateProfile(const string& newCompanyName, const string& newEmail,
                      const string& newBusinessField, const string& newAddress,
                      const string& newPhoneNumber) {
        if (!newCompanyName.empty()) companyName = newCompanyName;
        if (!newEmail.empty()) email = newEmail;
        if (!newBusinessField.empty()) businessField = newBusinessField;
        if (!newAddress.empty()) address = newAddress;
        if (!newPhoneNumber.empty()) phoneNumber = newPhoneNumber;
    }

    void ban() { isBanned = true; }
    void unban() { isBanned = false; }
    bool canLogin() const { return isActive && !isBanned; }

    // Static factory method
    static Employer fromCSV(const vector<string>& fields) {
        if (fields.size() < 6) return Employer();

        try {
            int id = stoi(fields[0]);
            string username = CSVHelper::removeQuotes(fields[1]);
            string companyName = CSVHelper::removeQuotes(fields[2]);
            string email = CSVHelper::removeQuotes(fields[3]);
            string passwordHash = CSVHelper::removeQuotes(fields[4]);
            bool banned = (fields[5] == "1");

            Employer emp(id, username, companyName, email, "");
            emp.passwordHash = passwordHash;
            emp.isBanned = banned;

            if (fields.size() > 6) emp.createdDate = CSVHelper::removeQuotes(fields[6]);
            if (fields.size() > 7) emp.businessField = CSVHelper::removeQuotes(fields[7]);
            if (fields.size() > 8) emp.address = CSVHelper::removeQuotes(fields[8]);
            if (fields.size() > 9) emp.phoneNumber = CSVHelper::removeQuotes(fields[9]);

            return emp;
        } catch (...) {
            return Employer();
        }
    }
};

// EmployerDAO class (Data Access Object pattern)
class EmployerDAO : public BaseDAO {
private:
    string filename;
    vector<Employer> employers;
    int nextId;

public:
    EmployerDAO(const string& filename = "employers.csv")
        : filename(filename), nextId(1) {
        loadFromFile();
    }

    ~EmployerDAO() {
        saveToFile();
    }

    bool loadFromFile() override {
        employers.clear();
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Khong the mo file " << filename << ". Tao file moi.\n";
            return false;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            vector<string> fields = CSVHelper::parseLine(line);
            Employer emp = Employer::fromCSV(fields);

            if (emp.validateData()) {
                employers.push_back(emp);
                if (emp.getId() >= nextId) {
                    nextId = emp.getId() + 1;
                }
            }
        }
        file.close();
        return true;
    }

    bool saveToFile() override {
        ofstream file(filename, ios::trunc);
        if (!file.is_open()) {
            cerr << "Loi: Khong the ghi file " << filename << "\n";
            return false;
        }

        for (const auto& emp : employers) {
            file << emp.toCSV() << "\n";
        }
        file.close();
        return true;
    }

    void clearData() override {
        employers.clear();
        nextId = 1;
    }

    // CRUD operations
    Employer* create(const string& username, const string& companyName,
                    const string& email, const string& password,
                    const string& businessField = "", const string& address = "",
                    const string& phoneNumber = "") {
        if (findByUsername(username) != nullptr) {
            return nullptr; // Username already exists
        }

        Employer newEmployer(nextId++, username, companyName, email, password,
                           businessField, address, phoneNumber);
        employers.push_back(newEmployer);
        saveToFile();
        return &employers.back();
    }

    Employer* findById(int id) {
        auto it = find_if(employers.begin(), employers.end(),
                         [id](const Employer& emp) { return emp.getId() == id; });
        return (it != employers.end()) ? &(*it) : nullptr;
    }

    Employer* findByUsername(const string& username) {
        auto it = find_if(employers.begin(), employers.end(),
                         [&username](const Employer& emp) {
                             return emp.getUsername() == username;
                         });
        return (it != employers.end()) ? &(*it) : nullptr;
    }

    vector<Employer*> findByCompanyName(const string& companyName) {
        vector<Employer*> result;
        for (auto& emp : employers) {
            if (emp.getCompanyName().find(companyName) != string::npos) {
                result.push_back(&emp);
            }
        }
        return result;
    }

    bool update(int id, const string& companyName, const string& email,
               const string& businessField, const string& address,
               const string& phoneNumber) {
        Employer* emp = findById(id);
        if (emp) {
            emp->updateProfile(companyName, email, businessField, address, phoneNumber);
            saveToFile();
            return true;
        }
        return false;
    }

    bool remove(int id) {
        auto it = remove_if(employers.begin(), employers.end(),
                           [id](const Employer& emp) { return emp.getId() == id; });
        if (it != employers.end()) {
            employers.erase(it, employers.end());
            saveToFile();
            return true;
        }
        return false;
    }

    vector<Employer> getAll() const {
        return employers;
    }

    size_t count() const {
        return employers.size();
    }

    size_t countActive() const {
        return count_if(employers.begin(), employers.end(),
                       [](const Employer& emp) { return !emp.getIsBanned(); });
    }

    size_t countBanned() const {
        return count_if(employers.begin(), employers.end(),
                       [](const Employer& emp) { return emp.getIsBanned(); });
    }
};

// Authentication Service class
class AuthenticationService {
private:
    EmployerDAO* employerDAO;
    string adminPassword;

public:
    AuthenticationService(EmployerDAO* dao, const string& adminPass = "admin123")
        : employerDAO(dao), adminPassword(adminPass) {}

    bool authenticateAdmin(const string& password) const {
        return password == adminPassword;
    }

    Employer* authenticateEmployer(const string& username, const string& password) {
        Employer* emp = employerDAO->findByUsername(username);
        if (emp && emp->verifyPassword(password) && emp->canLogin()) {
            return emp;
        }
        return nullptr;
    }

    void setAdminPassword(const string& newPassword) {
        adminPassword = newPassword;
    }
};

// Statistics Service class
class StatisticsService {
private:
    EmployerDAO* employerDAO;

public:
    StatisticsService(EmployerDAO* dao) : employerDAO(dao) {}

    void printStatistics() const {
        IOHelper::printHeader("THONG KE HE THONG");
        cout << "Tong so nha tuyen dung: " << employerDAO->count() << "\n";
        cout << "So tai khoan hoat dong: " << employerDAO->countActive() << "\n";
        cout << "So tai khoan bi khoa: " << employerDAO->countBanned() << "\n";
        IOHelper::printSeparator('-');
    }

    void printEmployerList() const {
        auto employers = employerDAO->getAll();
        if (employers.empty()) {
            cout << "Khong co nha tuyen dung nao!\n";
            return;
        }

        IOHelper::printHeader("DANH SACH NHA TUYEN DUNG");
        for (const auto& emp : employers) {
            cout << emp.getDisplayInfo();
        }
    }
};

// ===== SERVICE CLASSES (Business Logic) =====

// Admin Service class
class AdminService {
private:
    EmployerDAO* employerDAO;
    StatisticsService* statsService;

public:
    AdminService(EmployerDAO* dao, StatisticsService* stats)
        : employerDAO(dao), statsService(stats) {}

    void showMenu() {
        while (true) {
            IOHelper::printHeader("MENU QUAN TRI VIEN");
            cout << "1. Tao tai khoan nha tuyen dung\n";
            cout << "2. Danh sach tat ca nha tuyen dung\n";
            cout << "3. Tim nha tuyen dung theo ID\n";
            cout << "4. Tim nha tuyen dung theo ten cong ty\n";
            cout << "5. Cap nhat thong tin nha tuyen dung\n";
            cout << "6. Khoa tai khoan nha tuyen dung\n";
            cout << "7. Mo khoa tai khoan nha tuyen dung\n";
            cout << "8. Xoa tai khoan nha tuyen dung\n";
            cout << "9. Thong ke\n";
            cout << "0. Quay lai menu chinh\n";
            IOHelper::printSeparator();

            int choice = IOHelper::inputInteger("Nhap lua chon: ");
            handleChoice(choice);
        }
    }

private:
    void handleChoice(int choice) {
        switch (choice) {
            case 0: return;
            case 1: createEmployer(); break;
            case 2: statsService->printEmployerList(); break;
            case 3: findEmployerById(); break;
            case 4: findEmployerByCompany(); break;
            case 5: updateEmployer(); break;
            case 6: banEmployer(); break;
            case 7: unbanEmployer(); break;
            case 8: deleteEmployer(); break;
            case 9: statsService->printStatistics(); break;
            default: cout << "Lua chon khong hop le!\n";
        }
    }

    void createEmployer() {
        IOHelper::printHeader("TAO TAI KHOAN NHA TUYEN DUNG");

        string username = IOHelper::inputString("Ten dang nhap: ");
        if (username.empty()) {
            cout << "Ten dang nhap khong duoc de trong!\n";
            return;
        }

        if (employerDAO->findByUsername(username) != nullptr) {
            cout << "Ten dang nhap da ton tai!\n";
            return;
        }

        string companyName = IOHelper::inputString("Ten cong ty: ");
        string email = IOHelper::inputString("Email: ");
        string password = IOHelper::inputString("Mat khau: ");
        string businessField = IOHelper::inputString("Linh vuc (co the de trong): ");
        string address = IOHelper::inputString("Dia chi (co the de trong): ");
        string phoneNumber = IOHelper::inputString("So dien thoai (co the de trong): ");

        if (companyName.empty() || email.empty() || password.empty()) {
            cout << "Ten cong ty, email va mat khau khong duoc de trong!\n";
            return;
        }

        Employer* newEmp = employerDAO->create(username, companyName, email, password,
                                             businessField, address, phoneNumber);
        if (newEmp) {
            cout << "Tao tai khoan thanh cong! ID: " << newEmp->getId() << "\n";
        } else {
            cout << "Loi khi tao tai khoan!\n";
        }
    }

    void findEmployerById() {
        int id = IOHelper::inputInteger("Nhap ID can tim: ");
        Employer* emp = employerDAO->findById(id);
        if (emp) {
            cout << emp->getDisplayInfo();
        } else {
            cout << "Khong tim thay nha tuyen dung voi ID: " << id << "\n";
        }
    }

    void findEmployerByCompany() {
        string companyName = IOHelper::inputString("Nhap ten cong ty (hoac phan ten): ");
        auto results = employerDAO->findByCompanyName(companyName);

        if (results.empty()) {
            cout << "Khong tim thay cong ty nao phu hop!\n";
        } else {
            IOHelper::printHeader("KET QUA TIM KIEM");
            for (const auto& emp : results) {
                cout << emp->getDisplayInfo();
            }
        }
    }

    void updateEmployer() {
        int id = IOHelper::inputInteger("Nhap ID can cap nhat: ");
        Employer* emp = employerDAO->findById(id);
        if (!emp) {
            cout << "Khong tim thay nha tuyen dung voi ID: " << id << "\n";
            return;
        }

        IOHelper::printHeader("CAP NHAT THONG TIN");
        cout << "Thong tin hien tai:" << emp->getDisplayInfo();

        string companyName = IOHelper::inputString("Ten cong ty moi (Enter de giu nguyen): ");
        string email = IOHelper::inputString("Email moi (Enter de giu nguyen): ");
        string businessField = IOHelper::inputString("Linh vuc moi (Enter de giu nguyen): ");
        string address = IOHelper::inputString("Dia chi moi (Enter de giu nguyen): ");
        string phoneNumber = IOHelper::inputString("So dien thoai moi (Enter de giu nguyen): ");

        if (companyName.empty()) companyName = emp->getCompanyName();
        if (email.empty()) email = emp->getEmail();
        if (businessField.empty()) businessField = emp->getBusinessField();
        if (address.empty()) address = emp->getAddress();
        if (phoneNumber.empty()) phoneNumber = emp->getPhoneNumber();

        if (employerDAO->update(id, companyName, email, businessField, address, phoneNumber)) {
            cout << "Cap nhat thong tin thanh cong!\n";
        } else {
            cout << "Loi khi cap nhat thong tin!\n";
        }
    }

    void banEmployer() {
        int id = IOHelper::inputInteger("Nhap ID can khoa: ");
        Employer* emp = employerDAO->findById(id);
        if (emp) {
            emp->ban();
            employerDAO->saveToFile();
            cout << "Da khoa tai khoan ID: " << id << "\n";
        } else {
            cout << "Khong tim thay tai khoan!\n";
        }
    }

    void unbanEmployer() {
        int id = IOHelper::inputInteger("Nhap ID can mo khoa: ");
        Employer* emp = employerDAO->findById(id);
        if (emp) {
            emp->unban();
            employerDAO->saveToFile();
            cout << "Da mo khoa tai khoan ID: " << id << "\n";
        } else {
            cout << "Khong tim thay tai khoan!\n";
        }
    }

    void deleteEmployer() {
        int id = IOHelper::inputInteger("Nhap ID can xoa: ");
        Employer* emp = employerDAO->findById(id);
        if (!emp) {
            cout << "Khong tim thay tai khoan voi ID: " << id << "\n";
            return;
        }

        cout << "Ban co chac chan muon xoa tai khoan sau?" << emp->getDisplayInfo();

        if (IOHelper::confirmAction("Nhap 'YES' de xac nhan xoa")) {
            if (employerDAO->remove(id)) {
                cout << "Da xoa tai khoan thanh cong!\n";
            } else {
                cout << "Loi khi xoa tai khoan!\n";
            }
        } else {
            cout << "Huy bo thao tac xoa!\n";
        }
    }
};

// Employer Service class
class EmployerService {
private:
    EmployerDAO* employerDAO;
    Employer* currentEmployer;

public:
    EmployerService(EmployerDAO* dao, Employer* emp)
        : employerDAO(dao), currentEmployer(emp) {}

    void showMenu() {
        while (true) {
            IOHelper::printHeader("MENU NHA TUYEN DUNG");
            cout << "Xin chao: " << currentEmployer->getUsername()
                 << " (" << currentEmployer->getCompanyName() << ")\n";
            cout << "1. Xem thong tin ca nhan\n";
            cout << "2. Cap nhat thong tin ca nhan\n";
            cout << "3. Doi mat khau\n";
            cout << "0. Dang xuat\n";
            IOHelper::printSeparator();

            int choice = IOHelper::inputInteger("Nhap lua chon: ");
            if (choice == 0) {
                cout << "Da dang xuat thanh cong!\n";
                return;
            }
            handleChoice(choice);
        }
    }

private:
    void handleChoice(int choice) {
        switch (choice) {
            case 1: viewProfile(); break;
            case 2: updateProfile(); break;
            case 3: changePassword(); break;
            default: cout << "Lua chon khong hop le!\n";
        }
    }

    void viewProfile() {
        cout << currentEmployer->getDisplayInfo();
    }

    void updateProfile() {
        IOHelper::printHeader("CAP NHAT THONG TIN CA NHAN");

        string companyName = IOHelper::inputString("Ten cong ty moi (Enter de giu nguyen): ");
        string email = IOHelper::inputString("Email moi (Enter de giu nguyen): ");
        string businessField = IOHelper::inputString("Linh vuc moi (Enter de giu nguyen): ");
        string address = IOHelper::inputString("Dia chi moi (Enter de giu nguyen): ");
        string phoneNumber = IOHelper::inputString("So dien thoai moi (Enter de giu nguyen): ");

        if (companyName.empty()) companyName = currentEmployer->getCompanyName();
        if (email.empty()) email = currentEmployer->getEmail();
        if (businessField.empty()) businessField = currentEmployer->getBusinessField();
        if (address.empty()) address = currentEmployer->getAddress();
        if (phoneNumber.empty()) phoneNumber = currentEmployer->getPhoneNumber();

        if (employerDAO->update(currentEmployer->getId(), companyName, email,
                               businessField, address, phoneNumber)) {
            cout << "Cap nhat thong tin thanh cong!\n";
            // Refresh current employer data
            currentEmployer = employerDAO->findById(currentEmployer->getId());
        } else {
            cout << "Loi khi cap nhat thong tin!\n";
        }
    }

    void changePassword() {
        string newPassword = IOHelper::inputString("Nhap mat khau moi: ");
        if (newPassword.empty()) {
            cout << "Mat khau khong duoc de trong!\n";
            return;
        }

        currentEmployer->setPassword(newPassword);
        employerDAO->saveToFile();
        cout << "Doi mat khau thanh cong!\n";
    }
};

// ===== MAIN APPLICATION CLASS =====

class EmployerManagementApp {
private:
    EmployerDAO employerDAO;
    AuthenticationService authService;
    StatisticsService statsService;
    AdminService adminService;

public:
    EmployerManagementApp()
        : authService(&employerDAO), statsService(&employerDAO),
          adminService(&employerDAO, &statsService) {}

    void run() {
        IOHelper::printHeader("UNG DUNG QUAN LY NHA TUYEN DUNG");

        while (true) {
            showMainMenu();
            int choice = IOHelper::inputInteger("Nhap lua chon: ");
            if (choice == 0) {
                cout << "Cam on ban da su dung ung dung!\n";
                cout << "Dang luu du lieu...\n";
                break;
            }
            handleMainChoice(choice);
        }
    }

private:
    void showMainMenu() {
        IOHelper::printHeader("MENU CHINH");
        cout << "1. Dang nhap quan tri vien\n";
        cout << "2. Dang nhap nha tuyen dung\n";
        cout << "3. Dang ky tai khoan nha tuyen dung\n";
        cout << "0. Thoat chuong trinh\n";
        IOHelper::printSeparator();
    }

    void handleMainChoice(int choice) {
        switch (choice) {
            case 1: adminLogin(); break;
            case 2: employerLogin(); break;
            case 3: employerRegister(); break;
            default: cout << "Lua chon khong hop le!\n";
        }
    }

    void adminLogin() {
        string password = IOHelper::inputString("Nhap mat khau quan tri vien: ");
        if (authService.authenticateAdmin(password)) {
            cout << "Dang nhap quan tri vien thanh cong!\n";
            adminService.showMenu();
        } else {
            cout << "Mat khau quan tri vien khong dung!\n";
        }
    }

    void employerLogin() {
        string username = IOHelper::inputString("Ten dang nhap: ");
        string password = IOHelper::inputString("Mat khau: ");

        Employer* emp = authService.authenticateEmployer(username, password);
        if (emp) {
            cout << "Dang nhap thanh cong!\n";
            EmployerService empService(&employerDAO, emp);
            empService.showMenu();
        } else {
            cout << "Dang nhap that bai! Kiem tra lai ten dang nhap/mat khau hoac tai khoan co bi khoa.\n";
        }
    }

    void employerRegister() {
        IOHelper::printHeader("DANG KY TAI KHOAN NHA TUYEN DUNG");

        string username = IOHelper::inputString("Ten dang nhap: ");
        if (username.empty()) {
            cout << "Ten dang nhap khong duoc de trong!\n";
            return;
        }

        if (employerDAO.findByUsername(username) != nullptr) {
            cout << "Ten dang nhap da ton tai!\n";
            return;
        }

        string companyName = IOHelper::inputString("Ten cong ty: ");
        string email = IOHelper::inputString("Email: ");
        string password = IOHelper::inputString("Mat khau: ");
        string businessField = IOHelper::inputString("Linh vuc (co the de trong): ");
        string address = IOHelper::inputString("Dia chi (co the de trong): ");
        string phoneNumber = IOHelper::inputString("So dien thoai (co the de trong): ");

        if (companyName.empty() || email.empty() || password.empty()) {
            cout << "Ten cong ty, email va mat khau khong duoc de trong!\n";
            return;
        }

        Employer* newEmp = employerDAO.create(username, companyName, email, password,
                                            businessField, address, phoneNumber);
        if (newEmp) {
            cout << "Dang ky tai khoan thanh cong!\n";
            cout << "ID tai khoan cua ban la: " << newEmp->getId() << "\n";
            cout << "Ban co the dang nhap ngay bay gio!\n";
        } else {
            cout << "Loi khi dang ky tai khoan!\n";
        }
    }
};

// ===== MAIN FUNCTION =====

int main() {
    try {
        EmployerManagementApp app;
        app.run();
    } catch (const exception& e) {
        cerr << "Loi: " << e.what() << "\n";
        return 1;
    }

    return 0;
}