#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>

#include "Order.h"
using namespace std;

void setStdinEcho(bool enable) {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); //lay handle cua standard input (ban phim)
    DWORD mode; //luu che do hien tai cua console
    GetConsoleMode(hStdin, &mode); // lay che do hien tai cua console

    if (!enable) mode &= ~ENABLE_ECHO_INPUT; // neu enable = false, disable echo vao che do hien tai
    else mode |= ENABLE_ECHO_INPUT; // neu enable = true, enable echo vao che do hien tai

    SetConsoleMode(hStdin, mode); //thiet lap lai che do cho console
}
#endif

string getPassword() {
    string password; //chuoi luu password
    char ch; // bien tam dung de luu tung ki tu

    setStdinEcho(false); // disable echo ra khoi che do hien tai

    cout << "password: ";

#ifdef _WIN32
    // doc tung ki tu ma khong hien thi
    while ((ch = _getch()) != '\r') { // neu khong phai la ki tu enter
        if (ch == '\b') { // neu la ki tu backspace, xoa ki tu truoc do o trong chuoi password
            if (password.length() != 0) {
                cout << "\b \b"; // di chuyen con tro ve ben trai, in ra space de xoa roi di chuyen con tro ve ben phai
                password.resize(password.size() - 1); // giam chuoi password di 1 ki tu
            }
        } else if (ch == '\r') {
            // dieu kien de thoat vong lap
        } else {
            password += ch; // them ki tu vao chuoi password
            cout << '*'; // in ra dau sao moi khi nhan duoc 1 ki tu moi
        }
    }
    #endif
    setStdinEcho(true); // enable lai echo

    cout << endl;
    return password;
}

class Employee {
    private:
        string username;
        string password;
    public:
        bool login(const string& username, const string& password) {
            return username == "admin" && password == "1234";
        }
};

class App {
    private:
        Employee emp;
        Order order;
        int role; // 1: employee, 2: customer

    public:
        App() : role(0) {}
        void run() {
            loginUser();
            displayMenu();
        }

        void loginUser();
        void displayMenu();  
};

void App::loginUser() {
    int choice;
    bool validChoice = false;
    do {
        cout << "Choose Employee (1) or Customer (2) mode: ";
        cin >> choice;
        cin.ignore(); 

        if (choice == 1 || choice == 2) {
            validChoice = true; 
        } else {
            cout << "Invalid selection. Please enter 1 for Employee or 2 for Customer." << endl;
        }
    } while (!validChoice);

    // Employee
    if (choice == 1) {
        string username, password;
        int loginAttempts = 0;
        bool isLoggedIn = false;

        while (loginAttempts < 3 && !isLoggedIn) {
            cout << "username: ";
            getline(cin, username);

            password = getPassword();

            if (emp.login(username, password)) {
                cout << "Login successful!" << endl;
                isLoggedIn = true;
                this->role = 1; // Update role = 1 (employee)
            } else {
                cout << "Login failed! You have " << 2 - loginAttempts << " attempt(s) left." << endl;
                loginAttempts++;
            }
        }

        if (!isLoggedIn) {
            cout << "You have reached the maximum number of login attempts." << endl;
            this->role = 0; // reset role = 0 neu dang nhap that bai 3 lan
        }
    } else if (choice == 2) {
        this->role = 2; // update role = 2 (customer)
        cout << "Customer mode selected." << endl;
    }
}

void App::displayMenu() {
    int choice;

    do {
        cout << "==============================================  CUA HANG KINH THUOC  ===========================================" << endl;
        if (role == 1) {
            // For employee
            cout << "1. Product " << endl;
        }
        // For customer
        cout << "2. Menu \n";
        cout << "3. Search \n";
        cout << "4. Add to Cart\n";
        cout << "5. View Cart\n";
        cout << "6. Print Receipt\n";
        cout << "0. Exit" << endl;
        cout << "================================================================================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (role == 1) {
                    Product::manageProductMenu();
                } else {
                    cout << "You have no permission to access this feature!" << endl;
                }
                break;
            }
            case 2: {
                Product::showAllProducts();
                break;
            }
            case 3: {
                Product::searchProduct();
                break;
            }
            case 4: {
                cin >> order;
                Order::save(order);
                break;
            }
            case 5: {
                Order::manageOrderMenu(order);
                break;
            }
            case 6: {
                string orderId;
                cout << "Enter the ID of the order you want to manage: ";
                cin >> orderId;

                order = Order::findByOrderId(orderId, true);
                if (!order.isEmpty()) {
                    order.showCart();
                    Customer customer = order.getCustomerInfo();
                    cout << "\nOrder's ID: " << order.getOrderId() << endl;
                    cout << "Customer's ID: " << customer.getCustomerId() << endl;
                    cout << "Customer's name: " << customer.getName() << endl;
                    cout << "Customer's birthday: " << customer.getBirthday() << endl;
                    cout << "Customer's phone number: 0" << customer.getPhoneNumber() << endl;
                    cout << "Total price: " << order.getTotalAmount() <<  " VND" << endl;
                    break;
                }
            }
            case 0: {
                cout << "Thanks for your purchases!" << endl;
                break;
            }
            default: {
                cout << "Invalid selection. Please choose again!" << endl;
                break;
            }
        }
    } while (choice != 0);
}