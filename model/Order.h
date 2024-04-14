#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "../utils/constants.h"
#include "../utils/helper.h"

#include "Product.h"
#include "Item.h"
#include "Customer.h"


using namespace std;

class Order {
private:
    string orderId;
    string customerId;
    int totalAmount;
    string createDate;
    string status;

public:
    Order() {
        this->status = ORDER_STATUS_UNPAID;
        this->totalAmount = 0.0;
        this->createDate = getCurrentDate();
    }

    Order(string orderId, string customerId, int totalAmount, string status) {
        this->orderId = orderId;
        this->customerId = customerId;
        this->totalAmount = totalAmount;
        this->status = ORDER_STATUS_UNPAID;
    }

    Order(string orderId, string customerId, int totalAmount, string createDate, string status) {
        this->orderId = orderId;
        this->customerId = customerId;
        this->totalAmount = totalAmount;
        this->createDate = createDate;
        this->status = status;
    }

    ~Order() {}

    string getOrderId() {
        return this->orderId;
    }

    void setOrderId(string orderId) {
        this->orderId = orderId;
    }

    string getCustomerId() {
        return this->customerId = customerId;
    }

    void setCustomerId(string customerId) {
        this->customerId = customerId;
    }

    string getStatus() {
        return this->status;
    }

    void setStatus(string status) {
        this->status = status;
    }

    string getCreateDate() {
        return this->createDate;
    }

    int getTotalAmount() {
        return this->totalAmount;
    }

    void setTotalAmount(int totalAmount) {
        this -> totalAmount = totalAmount;
    }

    vector<Item> getItems();

    int calcTotalAmount();

    Customer getCustomerInfo();

    bool isEmpty();

    void show();

    void addItem();

    void showCart();

    void pay();

    static void showTableHeader();

    static void showAllOrders(bool payment);

    static void save(Order& order);

    static Order readLine(string line);

    static vector<Order> getAllOrders();

    static Order findByOrderId(string orderId, bool payment);

    static vector<Order> findByCustomerId(string customerIdId);

    static void update(string orderId, Order& order);

    static void remove(string orderId);

    static void manageOrderMenu(Order& order);

    friend istream& operator >>(istream& is, Order& sp);

    friend ostream& operator <<(ostream& os, Order& sp);

};

void Order::manageOrderMenu(Order& order) {
    int orderChoice;
    string orderId;
    cout << "\nEnter the ID of the order you want to manage: ";
    cin >> orderId;
    
    order = Order::findByOrderId(orderId, false);
    if (order.isEmpty()) {
        cout << "Order ID not found or empty" << endl;
        return; 
    }
    order.showCart();
    while (true) {
        cout << "\n================================================== ORDER MENU ==================================================" << endl;
        cout << "1. Add new items to order" << endl;
        cout << "2. Delete items" << endl;
        cout << "3. Update items information" << endl;
        cout << "4. Pay the bill and save order details to Bill.txt" << endl;
        cout << "0. Return to main menu" << endl;
        cout << "================================================================================================================" << endl;
        cout << "Enter your choice: ";
        cin >> orderChoice;

        switch (orderChoice) {
            case 1:{
                Product::showAllProducts();
                
                cout << "Product's ID: ";
                string productId;
                cin >> productId;
                    
                cout << "Amount: ";
                int amount;
                cin >> amount;
                
                if(amount > 0) {
                    Item item(order.getOrderId(), productId, amount);
                    order.setTotalAmount(order.calcTotalAmount());
                    Item::save(item);
                    cout << "Add new items successfully" << endl;
                    break;
                } else {
                    cout << "Invalid quantity entered." << endl;
                }
                break;
            }
            case 2: {
                order.showCart();

                int itemId;
                cout << "Enter Item ID: ";
                cin >> itemId;

                Item itemToDelete = Item::findByItemId(itemId);
                if (!itemToDelete.isEmpty()) {
                    Item::remove(itemId); 
                    order.setTotalAmount(order.calcTotalAmount());
                    Order::save(order);
                    cout << "Item deleted successfully" << endl;
                    break;
                } else {
                    cout << "Item ID not found." << endl;
                }
                break;
            }
            case 3: {
                order.showCart();
                cout << "Enter Item ID: ";
                int itemId;
                cin >> itemId;

                Item itemToUpdate = Item::findByItemId(itemId);
                if (!itemToUpdate.isEmpty()) {
                    Product product = Product::findByProductId(itemToUpdate.getProductId());
                    if (!product.isEmpty()) {

                        int newAmount;
                        cout << "Enter the new quantity for the item: ";
                        cin >> newAmount;
                    
                        int currentAmount = product.getAmount();
                    
                        if (newAmount >= 1 && newAmount <= currentAmount) {
                            itemToUpdate.setAmount(newAmount);
                            Item::update(itemId, itemToUpdate);
                            order.setTotalAmount(order.calcTotalAmount());
                            Order::update(order.getOrderId(), order);
                            cout << "Item updated successfully!" << endl;
                            order.showCart();
                        } else {
                            cout << "Invalid quantity entered. Quantity must be greater than 0 and less or equal to current stock amount of " << currentAmount << "." << endl;
                        }
                    } else {
                        cout << "Product associated with Item ID " << itemToUpdate.getItemId() << " not found." << endl;
                    }

                } else {
                    cout << "Item with ID " << itemId << " not found." << endl;
                }
                break;
            }
            case 4: {
                order.showCart();
                if (order.isEmpty()) {
                    cout << "You don't have any orders to pay?" << endl;
                    break;
                } else {
                    order.pay();
                    Order::update(order.getOrderId(), order);
                    cout << "The bill has been paid successfully." << endl;
                    Order::showTableHeader();
                    order.show();
                    return;
                }
            }
            case 0: {
                return;
            }
            default: {
                cout << "Invalid option. Please try again." << endl;
                break;
            }
        }
    }
}

vector<Item> Order::getItems() {
    return Item::findByOrderId(this->orderId);
}
void Order::showCart(){
    vector<Item> item_list = this -> getItems();
    Item::showTableHeader();
    for(int i = 0; i < item_list.size(); i++){
        item_list[i].show();
    }
}

int Order::calcTotalAmount() {
    int total = 0;
    vector<Item> itemList = this->getItems();
    int length = itemList.size();
    for (int i = 0; i < length; i ++) {
        Item item = itemList[i];
        Product product = Product::findByProductId(item.getProductId());

        total += item.getAmount() * product.getPrice();
    }
    this->totalAmount = total;

    return this->totalAmount;
}

Customer Order::getCustomerInfo() {
    return Customer::findByCustomerId(this->customerId);
}

bool Order::isEmpty() {
    return empty(this->orderId);
}

void Order::show() {
    if (this->isEmpty()) return;

    string orderIdStr = this->orderId;
    orderIdStr.resize(ID_COL_SIZE, ' ');
    string customerIdStr = this->customerId;
    customerIdStr.resize(ID_COL_SIZE, ' ');
    string statusStr = this->status;
    statusStr.resize(STATUS_COL_SIZE, ' ');
    string totalAmountStr = to_string(this->totalAmount);
    totalAmountStr.resize(PRICE_COL_SIZE, ' ');
    string createDateStr = this->createDate;
    createDateStr.resize(DATE_COL_SIZE, ' ');

    cout << orderIdStr << customerIdStr << totalAmountStr << createDateStr << statusStr << '\n';

}

void Order::addItem() {
    Item item;
    cin >> item;
    item.setOrderId(this->orderId);
    Item::save(item);
}

void Order::pay() {
    this->status = ORDER_STATUS_PAID;
    vector<Item> items = Item::findByOrderId(this->orderId);

    for (int i = 0; i < items.size(); i ++) {
        Item item = items[i];
        Product product = Product::findByProductId(item.getProductId());
        int newAmount = max(product.getAmount() - item.getAmount(), 0);
        product.setAmount(newAmount);
        Product::update(product.getProductId(), product);
    }
}

Order Order::readLine(string line) {
    int start = 0;
    string orderId = line.substr(start, ID_COL_SIZE);
    orderId = trim(orderId);

    start += ID_COL_SIZE;
    string customerId = line.substr(start, ID_COL_SIZE);
    customerId = trim(customerId);

    start += ID_COL_SIZE;
    int totalAmount = stof(line.substr(start, PRICE_COL_SIZE));

    start += PRICE_COL_SIZE;
    string createDate = line.substr(start, DATE_COL_SIZE);
    createDate = trim(createDate);

    start += DATE_COL_SIZE;
    string status = line.substr(start, STATUS_COL_SIZE);
    status = trim(status);

    Order order(orderId, customerId, totalAmount, createDate, status);
    return order;
}

Order Order::findByOrderId(string targetId, bool payment) {
    ifstream file(ORDER_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << ORDER_FILE_NAME << endl;
        return Order();
    }
    string line;
    // Inorge the first line
    getline(file, line);
    
    if(payment){
        while (getline(file, line)) {
            Order order = readLine(line);
            if (order.getOrderId() == targetId && order.getStatus() == ORDER_STATUS_PAID) {
                file.close();
                return order;
            }
        }
    } else {
        while (getline(file, line)) {
            Order order = readLine(line);
            if (order.getOrderId() == targetId && order.getStatus() != ORDER_STATUS_PAID) {
                file.close();
                return order;
            }
        }
    }

    file.close();
    return Order();
}

void Order::save(Order &order) {
    ofstream file(ORDER_FILE_NAME, ios::app); // Open in append mode
    if (!file.is_open()) {
        cerr << "Error opening file for writting: " << ORDER_FILE_NAME << endl;
        return;
    }

    // Check if file is empty
    if (isFileEmpty(ORDER_FILE_NAME)) {
        string orderIdCol = "Order_ID";
        orderIdCol.resize(ID_COL_SIZE, ' ');

        string customerIdCol = "Customer ID";
        customerIdCol.resize(ID_COL_SIZE, ' ');

        string totalAmountCol = "Total Amount";
        totalAmountCol.resize(PRICE_COL_SIZE, ' ');

        string createDateCol = "Create Date";
        createDateCol.resize(DATE_COL_SIZE, ' ');

        string statusCol = "Status";
        statusCol.resize(STATUS_COL_SIZE, ' ');

        file << orderIdCol << customerIdCol << totalAmountCol << createDateCol << statusCol << '\n';
    }

    
    string orderIdStr = order.getOrderId();
    orderIdStr.resize(ID_COL_SIZE, ' ');

    string customerIdStr = order.getCustomerId();
    customerIdStr.resize(ID_COL_SIZE, ' ');

    string totalAmountStr = to_string(order.calcTotalAmount());
    totalAmountStr.resize(PRICE_COL_SIZE, ' ');

    string createDateStr = empty(order.getCreateDate()) ? getCurrentDate() : order.getCreateDate();
    createDateStr.resize(DATE_COL_SIZE, ' ');

    string statusStr = order.getStatus();
    statusStr.resize(STATUS_COL_SIZE, ' ');

    file << orderIdStr << customerIdStr << totalAmountStr << createDateStr << statusStr << '\n';

    file.close();
}

void Order::update(string orderId, Order& order) {
    Order originOrder = findByOrderId(orderId, false);
    if (originOrder.isEmpty()) return;

    vector<Order> orders = getAllOrders();
    int index = -1;
    for (int i = 0; i < orders.size(); i ++) {
        if (orders[i].getOrderId() == orderId) {
            index = i;
            break;
        }
    }

    orders[index] = order;

    ofstream file(ORDER_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for writting; " << ORDER_FILE_NAME << endl;
        return;
    }

    string orderIdCol = "Order_ID";
    orderIdCol.resize(ID_COL_SIZE, ' ');

    string customerIdCol = "Customer ID";
    customerIdCol.resize(ID_COL_SIZE, ' ');

    string totalAmountCol = "Total Amount";
    totalAmountCol.resize(PRICE_COL_SIZE, ' ');

    string createDateCol = "Create Date";
    createDateCol.resize(DATE_COL_SIZE, ' ');

    string statusCol = "Status";
    statusCol.resize(STATUS_COL_SIZE, ' ');

    file << orderIdCol << customerIdCol << totalAmountCol << createDateCol << statusCol << '\n';

    for (int i = 0; i < orders.size(); i ++) {

        string orderIdStr = orders[i].getOrderId();
        orderIdStr.resize(ID_COL_SIZE, ' ');

        string customerIdStr = orders[i].getCustomerId();
        customerIdStr.resize(ID_COL_SIZE, ' ');

        string totalAmountStr = to_string(orders[i].getTotalAmount());
        totalAmountStr.resize(PRICE_COL_SIZE, ' ');

        string createDateStr = orders[i].getCreateDate();
        createDateStr.resize(DATE_COL_SIZE, ' ');

        string statusStr = orders[i].getStatus();
        statusStr.resize(STATUS_COL_SIZE, ' ');

        file << orderIdStr << customerIdStr << totalAmountStr << createDateStr << statusStr << '\n';
    }
    

    file.close();
}

void Order::remove(string orderId) {
    Order order = findByOrderId(orderId, false);
    if (order.isEmpty()) {
        return;
    }

    vector<Order> orders = getAllOrders();
    int index = -1;
    for (int i = 0; i < orders.size(); i ++) {
        if (orders[i].getOrderId() == orderId) {
            index = i;
            break;
        }
    }

    orders.erase(orders.begin() + index);

    ofstream file(ORDER_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for writting; " << ORDER_FILE_NAME << endl;
        return;
    }

    string orderIdCol = "Order_ID";
    orderIdCol.resize(ID_COL_SIZE, ' ');

    string customerIdCol = "Customer ID";
    customerIdCol.resize(ID_COL_SIZE, ' ');

    string totalAmountCol = "Total Amount";
    totalAmountCol.resize(PRICE_COL_SIZE, ' ');

    string createDateCol = "Create Date";
    createDateCol.resize(DATE_COL_SIZE, ' ');

    string statusCol = "Status";
    statusCol.resize(STATUS_COL_SIZE, ' ');

    file << orderIdCol << customerIdCol << totalAmountCol << createDateCol << statusCol << '\n';

    for (int i = 0; i < orders.size(); i ++) {

        string orderIdStr = orders[i].getOrderId();
        orderIdStr.resize(ID_COL_SIZE, ' ');

        string customerIdStr = orders[i].getCustomerId();
        customerIdStr.resize(ID_COL_SIZE, ' ');

        string totalAmountStr = to_string(orders[i].getTotalAmount());
        totalAmountStr.resize(PRICE_COL_SIZE, ' ');

        string createDateStr = orders[i].getCreateDate();
        createDateStr.resize(DATE_COL_SIZE, ' ');

        string statusStr = orders[i].getStatus();
        statusStr.resize(STATUS_COL_SIZE, ' ');

        file << orderIdStr << customerIdStr << totalAmountStr << createDateStr << statusStr << '\n';
    }
    
    // Delete all items belong to this order
    vector<Item> items = Item::findByOrderId(orderId);
    for (int i = 0; i < items.size(); i ++) {
        Item::remove(items[i].getItemId());
    }
    file.close();
}

vector<Order> Order::getAllOrders() {
    vector<Order> orders;
    ifstream file(ORDER_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << ORDER_FILE_NAME << endl;
        return orders;
    }

    string line;
    // Skip the header line
    getline(file, line);
    while (getline(file, line)) {
        Order order = readLine(line);
        if (!order.isEmpty()) orders.push_back(order);
        
    }

    file.close();
    return orders;
}

void Order::showTableHeader() {
    string orderIdCol = "Order_ID";
    orderIdCol.resize(ID_COL_SIZE, ' ');

    string customerIdCol = "Customer ID";
    customerIdCol.resize(ID_COL_SIZE, ' ');

    string totalAmountCol = "Total Amount";
    totalAmountCol.resize(PRICE_COL_SIZE, ' ');

    string createDateCol = "Create Date";
    createDateCol.resize(DATE_COL_SIZE, ' ');

    string statusCol = "Status";
    statusCol.resize(STATUS_COL_SIZE, ' ');

    cout << orderIdCol << customerIdCol << totalAmountCol << createDateCol << statusCol << '\n';
}

void Order::showAllOrders(bool payment) {
    vector<Order> orders = getAllOrders();
    Order::showTableHeader();

    if(payment){
        for (int i = 0; i < orders.size(); i ++) {
            if (orders[i].getStatus() == ORDER_STATUS_PAID) {
                orders[i].show();
            }
        }
    } else {
        for (int i = 0; i < orders.size(); i ++) {
            if (orders[i].getStatus() == ORDER_STATUS_UNPAID) {
                orders[i].show();
            }
        }
    }
}

istream &operator>>(istream& input, Order &order)
{
    string orderId;
    do {
        cout << "Enter the order's ID: ";
        cin >> orderId;

        if (!Order::findByOrderId(orderId, false).isEmpty()) {
            cout << "Error: Order ID already exists. Please enter a different Order ID." << endl;
        } else {
            order.setOrderId(orderId);
            break;
        }
    } while(true);

    Customer customer;
    cin >> customer;
    Customer::save(customer);
    order.customerId = customer.getCustomerId();
    order.status = ORDER_STATUS_UNPAID;

    Product::showAllProducts();
    string productId;
    bool addMore = true; // This variable to control whether to add more products

    while (addMore) {
        cout << "Product's ID: ";
        cin >> productId;

        Product product = Product::findByProductId(productId);
        if (product.isEmpty()) {
            cout << "Product with ID " << productId << " not found." << endl;
            continue;  // Skip the rest of this iteration and continue with the next
        }

        int amount;
        bool validAmount;
        do {
            cout << "Amount: ";
            cin >> amount;
            validAmount = amount >= 1 && amount <= product.getAmount();
            if (!validAmount) {
                cout << "Invalid amount. Please enter a number between 1 and " << product.getAmount() << ".\n";
            }
        } while (!validAmount);  // Only exit loop when amount is valid

        Item item(order.getOrderId(), productId, amount);
        Item::save(item);

        cout << "Do you want to add more product? (Y/N) ";
        string isExit;
        cin.ignore();
        getline(cin, isExit);
        if (isExit == "N" || isExit == "n") {
            addMore = false;  // Exit the while loop
        }
    }

    return input;
}

ostream &operator<<(ostream &output, Order order)
{
    Order::showTableHeader();
    order.show();
    return output;
}

