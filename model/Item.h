#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

class Item {
private:
    int itemId;
    string orderId;
    string productId;
    int amount;

public:
    Item() {
        this->amount = 0;
    };
    Item(int itemId, string orderId, string productId, int amount) {
        this->itemId = itemId;
        this->orderId = orderId;
        this->productId = productId;
        this->amount = amount;
    }

    Item(string orderId, string productId, int amount) {
        this->orderId = orderId;
        this->productId = productId;
        this->amount = amount;
    }

    ~Item() {};

    int getItemId();

    void setItemId(int itemId);

    string getOrderId();

    void setOrderId(string orderId);

    string getProductId();

    void setProductId(string productId);

    int getAmount();

    void setAmount(int amount);

    bool isEmpty();

    void show();

    static Item readLine(string line);

    static void showTableHeader();

    static vector<Item> getAllItems();

    static void showAllItems(const string& orderId);

    static Item findByItemId(int itemId);

    static vector<Item> findByOrderId(string orderId);

    static void save(Item& item);

    static void update(int itemId, Item& item);

    static void remove(int itemId);

    friend istream& operator >> (istream& input, Item& item);

    friend ostream& operator << (ostream& output, Item item);

};

int Item::getItemId() {
    return this->itemId;
}

void Item::setItemId(int itemId) {
    this->itemId = itemId;
}

string Item::getOrderId() {
    return this->orderId;
}

void Item::setOrderId(string orderId) {
    this->orderId = orderId;
}

string Item::getProductId() {
    return this->productId;
}

void Item::setProductId(string productId) {
    this->productId = productId;
}

int Item::getAmount() {
    return this->amount;
}

void Item::setAmount(int amount) {
    this->amount = amount;
}

bool Item::isEmpty() {
    return this->itemId == 0;
}

void Item::show() {
    if (this->isEmpty()) {
        return ;
    } 
    
    string itemIdStr = to_string(this->itemId);
    itemIdStr.resize(ID_COL_SIZE, ' ');
    string orderIdStr = this->orderId;
    orderIdStr.resize(ID_COL_SIZE, ' ');
    string productIdStr = this->productId;
    productIdStr.resize(ID_COL_SIZE, ' ');
    string amountStr = to_string(this->amount);
    amountStr.resize(AMOUNT_COL_SIZE, ' ');

    cout << itemIdStr << orderIdStr << productIdStr << amountStr << '\n';
}

Item Item::readLine(string line) {
    int start = 0;
    string itemIdStr = line.substr(start, ID_COL_SIZE);
    itemIdStr = trim(itemIdStr);
    int itemId = stoi(itemIdStr);

    start += ID_COL_SIZE;
    string orderId = line.substr(start, ID_COL_SIZE);
    orderId = trim(orderId);

    start += ID_COL_SIZE;
    string productIdStr = line.substr(start, ID_COL_SIZE);
    string productId = trim(productIdStr);

    start += ID_COL_SIZE;
    string amountStr = line.substr(start, AMOUNT_COL_SIZE);
    int amount = stoi(amountStr);

    Item item(itemId, orderId, productId, amount);
    return item;
}

Item Item::findByItemId(int targetId) {
    ifstream file(ORDER_ITEM_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << ORDER_ITEM_FILE_NAME << endl;
        return Item();
    }
    string line;
    // Inorge the first line
    getline(file, line);
    while (getline(file, line)) {
        Item item = readLine(line);
        if (item.getItemId() == targetId) {
            file.close();
            return item;
        }
    }

    file.close();
    cout << "Not found item with ID = " << targetId << endl;
    return Item();
}

void Item::save(Item &item) {
    ofstream file(ORDER_ITEM_FILE_NAME, ios::app); // Opend in append mode
    if (!file.is_open()) {
        cerr << "Error opening file for writting: " << ORDER_ITEM_FILE_NAME << endl;
        return;
    }

    // Check if file is empty
    if (isFileEmpty(ORDER_ITEM_FILE_NAME)) {
        string itemIdCol = "Item_ID";
        itemIdCol.resize(ID_COL_SIZE, ' ');
        string orderIdCol = "Order ID";
        orderIdCol.resize(ID_COL_SIZE, ' ');
        string productIdCol = "Product ID";
        productIdCol.resize(ID_COL_SIZE, ' ');
        string amountCol = "Amount";
        amountCol.resize(AMOUNT_COL_SIZE, ' ');

        file << itemIdCol << orderIdCol << productIdCol << amountCol << '\n';
    }

    item.setItemId(time(0));
    string itemIdStr = to_string(item.getItemId());
    itemIdStr.resize(ID_COL_SIZE, ' ');
    string orderIdStr = item.getOrderId();
    orderIdStr.resize(ID_COL_SIZE, ' ');
    string productIdStr = item.getProductId();
    productIdStr.resize(ID_COL_SIZE, ' ');
    string amountStr = to_string(item.getAmount());
    amountStr.resize(AMOUNT_COL_SIZE, ' ');
    file << itemIdStr << orderIdStr << productIdStr << amountStr << '\n';

    file.close();
}

void Item::update(int itemId, Item& item) {
    Item originItem = findByItemId(itemId);
    if (originItem.isEmpty()) return;

    vector<Item> items = getAllItems();
    int index = -1;
    for (int i = 0; i < items.size(); i ++) {
        if (items[i].getItemId() == itemId) {
            index = i;
            break;
        }
    }

    items[index] = item;

    ofstream file(ORDER_ITEM_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for writting; " << ORDER_ITEM_FILE_NAME << endl;
        return;
    }

    string itemIdCol = "Item_ID";
    itemIdCol.resize(ID_COL_SIZE, ' ');
    string orderIdCol = "Order ID";
    orderIdCol.resize(ID_COL_SIZE, ' ');
    string productIdCol = "Product ID";
    productIdCol.resize(ID_COL_SIZE, ' ');
    string amountCol = "Amount";
    amountCol.resize(AMOUNT_COL_SIZE, ' ');

    file << itemIdCol << orderIdCol << productIdCol << amountCol << '\n';

    for (int i = 0; i < items.size(); i ++) {
        string itemIdStr = to_string(items[i].getItemId());
        itemIdStr.resize(ID_COL_SIZE, ' ');
        string orderIdStr = items[i].getOrderId();
        orderIdStr.resize(ID_COL_SIZE, ' ');
        string productIdStr = items[i].getProductId();
        productIdStr.resize(ID_COL_SIZE, ' ');
        string amountStr = to_string(items[i].getAmount());
        amountStr.resize(AMOUNT_COL_SIZE, ' ');

        file << itemIdStr << orderIdStr << productIdStr << amountStr << '\n';
    }
    

    file.close();
}

void Item::remove(int itemId) {
    Item item = findByItemId(itemId);
    if (item.isEmpty()) {
        return;
    }

    vector<Item> items = getAllItems();
    int index = -1;
    for (int i = 0; i < items.size(); i ++) {
        if (items[i].getItemId() == itemId) {
            index = i;
            break;
        }
    }

    items.erase(items.begin() + index);

    ofstream file(ORDER_ITEM_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for writting; " << ORDER_ITEM_FILE_NAME << endl;
        return;
    }

    string itemIdCol = "Item_ID";
    itemIdCol.resize(ID_COL_SIZE, ' ');
    string orderIdCol = "Order ID";
    orderIdCol.resize(ID_COL_SIZE, ' ');
    string productIdCol = "Product ID";
    productIdCol.resize(ID_COL_SIZE, ' ');
    string amountCol = "Amount";
    amountCol.resize(AMOUNT_COL_SIZE, ' ');

    file << itemIdCol << orderIdCol << productIdCol << amountCol << '\n';

    for (int i = 0; i < items.size(); i ++) {

        string itemIdStr = to_string(items[i].getItemId());
        itemIdStr.resize(ID_COL_SIZE, ' ');
        string orderIdStr = items[i].getOrderId();
        orderIdStr.resize(ID_COL_SIZE, ' ');
        string productIdStr = items[i].getProductId();
        productIdStr.resize(ID_COL_SIZE, ' ');
        string amountStr = to_string(items[i].getAmount());
        amountStr.resize(AMOUNT_COL_SIZE, ' ');

        file << itemIdStr << orderIdStr << productIdStr << amountStr << '\n';
    }
    

    file.close();
}

void Item::showTableHeader() {
    string itemIdCol = "Item_ID";
    itemIdCol.resize(ID_COL_SIZE, ' ');
    string orderIdCol = "Order ID";
    orderIdCol.resize(ID_COL_SIZE, ' ');
    string productIdCol = "Product ID";
    productIdCol.resize(ID_COL_SIZE, ' ');
    string amountCol = "Amount";
    amountCol.resize(AMOUNT_COL_SIZE, ' ');

    cout << itemIdCol << orderIdCol << productIdCol << amountCol << '\n';
}

vector<Item> Item::findByOrderId(string orderId) {
    vector<Item> items;
    ifstream file(ORDER_ITEM_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << ORDER_ITEM_FILE_NAME << endl;
        return items;
    }
    string line;
    // Inorge the first line
    getline(file, line);

    while (getline(file, line)) {
        Item item = readLine(line);
        if (item.getOrderId() == orderId) {
            items.push_back(item);
        }
    }

    file.close();
    return items;
}

vector<Item> Item::getAllItems() {
    vector<Item> items;
    ifstream file(ORDER_ITEM_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << ORDER_ITEM_FILE_NAME << endl;
        return items;
    }

    string line;
    // Skip the header line
    getline(file, line);
    while (getline(file, line)) {
        Item item = readLine(line);
        if (!item.isEmpty()) items.push_back(item);
        
    }

    file.close();
    return items;
}

void Item::showAllItems(const string& orderId) {
    vector<Item> items = Item::findByOrderId(orderId);
    Item::showTableHeader();

    for (int i = 0; i < items.size(); i ++) {
        items[i].show();
    }
}

istream& operator >> (istream& input, Item& item) {
    cout << "Enter product's ID: ";
    cin.ignore();
    getline(cin, item.productId);
    Product product = Product::findByProductId(item.productId);

    if (!product.isEmpty()) {
        do {
            cout << "Enter the amount of product: ";
            input >> item.amount;
            if (item.amount <= product.getAmount()) {
                break;
            }
            cout << "Our store don't have enough product that have ID is " << product.getProductId() << endl;
            cout << "Please try again!" << endl;

        } while (1);
        
    }
    

    return input;
}

ostream& operator << (ostream& output, Item item) {
    Item::showTableHeader();
    item.show();
    return output;
}