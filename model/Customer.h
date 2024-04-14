#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

class Customer {
private:
    string customerId;
    string name;
    string birthday;
    string phoneNumber;

public:
    Customer() {};
    Customer(string customerId, string name, string birthday, string phoneNumber) {
        this->customerId = customerId;
        this->name = name;
        this->birthday = birthday;
        this->phoneNumber = phoneNumber;
    }
    
    ~Customer() {};

    string getCustomerId();

    void setCustomerId(string customerId);

    string getName();

    void setName(string name);

    string getBirthday();

    void setBirthday(string birthday);

    string getPhoneNumber();

    void setPhoneNumber(string phoneNumber);

    bool isEmpty();

    void show();

    static void showTableHeader();
    
    static void showAllCustomers();

    static Customer readLine(string line);

    static void save(Customer& customer);

    static Customer findByCustomerId(string customerId);

    static vector<Customer> getAllCustomers();

    static void update(string customerId, Customer& customer);

    static void remove(string customerId);

    friend istream& operator >> (istream& input, Customer& customer);

    friend ostream& operator << (ostream& input, Customer customer);
};

string Customer::getCustomerId() {
    return this->customerId;
}

void Customer::setCustomerId(string customerId) {
    this->customerId = customerId;
}

string Customer::getName() {
    return this->name;
}

void Customer::setName(string name) {
    this->name = name;
}

string Customer::getBirthday() {
    return this->birthday;
}

void Customer::setBirthday(string birthday) {
    this->birthday = birthday;
}

string Customer::getPhoneNumber() {
    return this->phoneNumber;
}

void Customer::setPhoneNumber(string phoneNumber) {
    this->phoneNumber = phoneNumber;
}

bool Customer::isEmpty() {
    return empty(this->customerId);
}

void Customer::show() {
    if (this->isEmpty()) return;

    string customerIdStr = this->customerId;
    customerIdStr.resize(ID_COL_SIZE, ' ');
    string nameStr = this->name;
    nameStr.resize(NAME_COL_SIZE, ' ');
    string birthdayStr = this->birthday;
    birthdayStr.resize(DATE_COL_SIZE, ' ');
    string phoneNumberStr = this->phoneNumber;
    phoneNumberStr.resize(PHONE_COL_SIZE, ' ');

    cout << customerIdStr << nameStr << birthdayStr << phoneNumberStr << '\n';

}

Customer Customer::readLine(string line) {
    int start = 0;
    string customerId = line.substr(start, ID_COL_SIZE);
    customerId = trim(customerId);
    start += ID_COL_SIZE;
    string name = line.substr(start, NAME_COL_SIZE);
    name = trim(name);
    start += NAME_COL_SIZE;
    string birthday = line.substr(start, DATE_COL_SIZE);
    birthday = trim(birthday);
    start += DATE_COL_SIZE;
    string phoneNumber = line.substr(start, PHONE_COL_SIZE);
    phoneNumber = trim(phoneNumber);

    Customer customer(customerId, name, birthday, phoneNumber);
    return customer;
}

Customer Customer::findByCustomerId(string targetId) {
    ifstream file(CUSTOMER_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << CUSTOMER_FILE_NAME << endl;
        return Customer();
    }
    string line;
    // Inorge the first line
    getline(file, line);
    while (getline(file, line)) {
        Customer customer = readLine(line);
        if (customer.getCustomerId() == targetId) {
            file.close();
            return customer;
        }
    }

    file.close();
    cout << "Not found customer with ID = " << targetId << endl;
    return Customer();
}

void Customer::save(Customer &customer) {
    ofstream file(CUSTOMER_FILE_NAME, ios::app); // Opend in append mode
    if (!file.is_open()) {
        cerr << "Error opening file for writting: " << CUSTOMER_FILE_NAME << endl;
        return;
    }

    // Check if file is empty
    if (isFileEmpty(CUSTOMER_FILE_NAME)) {
        string customerIdCol = "Customer_ID";
        customerIdCol.resize(ID_COL_SIZE, ' ');
        string nameCol = "Name";
        nameCol.resize(NAME_COL_SIZE, ' ');
        string birthdayCol = "Birthday";
        birthdayCol.resize(DATE_COL_SIZE, ' ');
        string phoneNumberCol = "Phone Number";
        phoneNumberCol.resize(PHONE_COL_SIZE, ' ');

        file << customerIdCol << nameCol << birthdayCol << phoneNumberCol << '\n';
    }

    
    string customerIdStr = customer.getCustomerId();
    customerIdStr.resize(ID_COL_SIZE, ' ');
    string nameStr = customer.getName();
    nameStr.resize(NAME_COL_SIZE, ' ');
    string birthdayStr = customer.getBirthday();
    birthdayStr.resize(DATE_COL_SIZE, ' ');
    string phoneNumberStr = customer.getPhoneNumber();
    phoneNumberStr.resize(PHONE_COL_SIZE, ' ');
    file << customerIdStr << nameStr << birthdayStr << phoneNumberStr << '\n';

    file.close();
}

void Customer::update(string customerId, Customer& customer) {
    Customer originCustomer = findByCustomerId(customerId);
    if (originCustomer.isEmpty()) return;

    vector<Customer> customers = getAllCustomers();
    int index = -1;
    for (int i = 0; i < customers.size(); i ++) {
        if (customers[i].getCustomerId() == customerId) {
            index = i;
            break;
        }
    }

    customers[index] = customer;

    ofstream file(CUSTOMER_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for writting; " << CUSTOMER_FILE_NAME << endl;
        return;
    }

    string customerIdCol = "Customer_ID";
    customerIdCol.resize(ID_COL_SIZE, ' ');
    string nameCol = "Name";
    nameCol.resize(NAME_COL_SIZE, ' ');
    string birthdayCol = "Birthday";
    birthdayCol.resize(DATE_COL_SIZE, ' ');
    string phoneNumberCol = "Phone Number";
    phoneNumberCol.resize(PHONE_COL_SIZE, ' ');

    file << customerIdCol << nameCol << birthdayCol << phoneNumberCol << '\n';

    for (int i = 0; i < customers.size(); i ++) {

        string customerIdStr = customers[i].getCustomerId();
        customerIdStr.resize(ID_COL_SIZE, ' ');
        string nameStr = customers[i].getName();
        nameStr.resize(NAME_COL_SIZE, ' ');
        string birthdayStr = customers[i].getBirthday();
        birthdayStr.resize(DATE_COL_SIZE, ' ');
        string phoneNumberStr = customers[i].getPhoneNumber();
        phoneNumberStr.resize(PHONE_COL_SIZE, ' ');

        file << customerIdStr << nameStr << birthdayStr << phoneNumberStr << '\n';
    }
    

    file.close();
}

void Customer::remove(string customerId) {
    Customer customer = findByCustomerId(customerId);
    if (customer.isEmpty()) {
        return;
    }

    vector<Customer> customers = getAllCustomers();
    int index = -1;
    for (int i = 0; i < customers.size(); i ++) {
        if (customers[i].getCustomerId() == customerId) {
            index = i;
            break;
        }
    }

    customers.erase(customers.begin() + index);

    ofstream file(CUSTOMER_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for writting; " << CUSTOMER_FILE_NAME << endl;
        return;
    }

    string customerIdCol = "Customer_ID";
    customerIdCol.resize(ID_COL_SIZE, ' ');
    string nameCol = "Name";
    nameCol.resize(NAME_COL_SIZE, ' ');
    string birthdayCol = "Birthday";
    birthdayCol.resize(DATE_COL_SIZE, ' ');
    string phoneNumberCol = "Phone Number";
    phoneNumberCol.resize(PHONE_COL_SIZE, ' ');

    file << customerIdCol << nameCol << birthdayCol << phoneNumberCol << '\n';

    for (int i = 0; i < customers.size(); i ++) {

        string customerIdStr = customers[i].getCustomerId();
        customerIdStr.resize(ID_COL_SIZE, ' ');
        string nameStr = customers[i].getName();
        nameStr.resize(NAME_COL_SIZE, ' ');
        string birthdayStr = customers[i].getBirthday();
        birthdayStr.resize(DATE_COL_SIZE, ' ');
        string phoneNumberStr = customers[i].getPhoneNumber();
        phoneNumberStr.resize(PHONE_COL_SIZE, ' ');

        file << customerIdStr << nameStr << birthdayStr << phoneNumberStr << '\n';
    }
    

    file.close();
}

vector<Customer> Customer::getAllCustomers() {
    vector<Customer> customers;
    ifstream file(CUSTOMER_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << CUSTOMER_FILE_NAME << endl;
        return customers;
    }

    string line;
    // Skip the header line
    getline(file, line);
    while (getline(file, line)) {
        Customer customer = readLine(line);
        if (!customer.isEmpty()) customers.push_back(customer);
        
    }

    file.close();
    return customers;
}

void Customer::showTableHeader() {
    string customerIdCol = "Customer_ID";
    customerIdCol.resize(ID_COL_SIZE, ' ');
    string nameCol = "Name";
    nameCol.resize(NAME_COL_SIZE, ' ');
    string birthdayCol = "Birthday";
    birthdayCol.resize(DATE_COL_SIZE, ' ');
    string phoneNumberCol = "Phone Number";
    phoneNumberCol.resize(PHONE_COL_SIZE, ' ');

    cout << customerIdCol << nameCol << birthdayCol << phoneNumberCol << '\n';
}

void Customer::showAllCustomers() {
    vector<Customer> customers = getAllCustomers();
    Customer::showTableHeader();

    for (int i = 0; i < customers.size(); i ++) {
        customers[i].show();
    }
    
}

istream& operator >> (istream& input, Customer& customer) {
    cout << "Enter customer ID: ";
    input >> customer.customerId;
    cout << "Enter name: ";
    input.ignore();
    getline(input, customer.name);
    cout << "Enter your birthday: ";
    input.ignore();
    getline(input, customer.birthday);
    cout << "Enter your phone number: ";
    input.ignore();
    getline(input, customer.phoneNumber);

    return input;
}

ostream& operator << (ostream& output, Customer customer) {
    Customer::showTableHeader();
    customer.show();
    return output;
}