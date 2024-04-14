#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

class Product {
   friend class Calculator;
   private:
    string productId, STT;
    string name;
    int price;
    string createDate;
    string modifiedDate;
    int amount;
    static int cnt;


   public:
    Product() {
        productId , STT, name = "";
        price = 0;
        amount = 0;
        this->createDate = getCurrentDate();
    }


    Product(string ma,string STT, string ten, int gia, int sl) {
        productId = ma;
        STT = STT;
        name = ten;
        price = gia;
        amount = sl;
        
    }

    Product(string productId, string name, int price, int amount, string createDate, string modifiedDate) {
        this->productId = productId;
        this->name = name;
        this->price = price;
        this->amount = amount;
        this->createDate = createDate;
        this->modifiedDate = modifiedDate;
    }

    Product(string productId, string name, int price, int amount) {
        this->productId = productId;
        this->name = name;
        this->price = price;
        this->amount = amount;
        this->createDate = getCurrentDate();
    }

    void xuatfile(ofstream& file);
    
    string getProductId();    
    
    string getName();
    
    int getPrice();
    
    int getAmount();

    void setAmount(int amount);

    void setProductId(const string& productId); 

    void WriteProducts();
    
    void ReadData();  

    int getCount();

    string getCreateDate();

    void setCreateDate(string createDate);

    string getModifiedDate();

    void setModifiedDate(string modifiedDate);

    bool isEmpty();

    void show();

    static Product readLine(string line);

    static Product findByProductId(string targetId);

    static void save(Product& product);

    static void update(string productId, Product& product);

    static void remove(string productId);

    static vector<Product> getAllProducts();

    static void showTableHeader();

    static void showAllProducts();

    static void manageProductMenu();

    static vector<Product> searchByKeyword(const string& keyword);

    static void searchProduct();

    friend istream& operator >>(istream& is, Product& sp);

    friend ostream& operator <<(ostream& os, Product& sp);
};

vector<Product> Product::searchByKeyword(const string& keyword) {
    vector<Product> findProducts; // tao mang chua san pham co tu khoa
    vector<Product> allProducts = getAllProducts(); // tao mang chua full sp

    for (auto& product : allProducts) { // duyet tung thang trong mang full
        if (product.getName().find(keyword) != string::npos) { // neu keyword ko ton tai, se tra ve string::npos
            findProducts.push_back(product); // them sp vao mang chua keyword
        }
    }
    return findProducts;
}

void Product::searchProduct() {
    string keyword;
    cout << "Enter keyword: ";
    cin.ignore(); 
    getline(cin, keyword); 

    vector<Product> searchResults = searchByKeyword(keyword);

    if (!searchResults.empty()) {
        showTableHeader();
        for (auto& product : searchResults) {
            product.show();
        }
    } else {
        cout << "Keyword is not be found '" << keyword << "'." << endl;
    }
}

void Product::manageProductMenu() {
    int productChoice;
    while (true) {
        cout << "================================================= PRODUCT MENU =================================================" << endl;
        cout << "1. Add new product" << endl;
        cout << "2. Update product information" << endl;
        cout << "3. Delete product" << endl;
        cout << "0. Return to main menu" << endl;
        cout << "================================================================================================================" << endl;
        cout << "Enter your choice: ";
        cin >> productChoice;

        switch (productChoice) {
            case 1: {
                Product::showAllProducts();
                Product newProduct;
                cin >> newProduct;
                Product::save(newProduct);
                cout << "Product added successfully!" << endl;
                break;
            }
            case 2: {  
                vector<Product> list = getAllProducts();
                Product::showAllProducts();
                
                string productId;
                cout << "Enter Product ID: ";
                cin >> productId;

                bool valid_id = false;
                int size = list.size();
                for(int i = 0; i < size; i++) {
                    if(list[i].getProductId() == productId) {
                        valid_id = true;
                        break;
                    }
                }
                if(valid_id) {
                    Product productToUpdate;
                    cin >> productToUpdate;
                    Product::update(productId, productToUpdate); 
                    cout << "Product updated successfully!" << endl;
                } else {
                    cout << "Product ID not found." << endl;
                }
                break;
            }
            case 3: {
                vector<Product> list = getAllProducts();
                Product::showAllProducts();
                
                string productId;
                cout << "Enter Product ID: ";
                cin >> productId;

                bool valid_id = false;
                int size = list.size();
                for(int i = 0; i < size; i++) {
                    if(list[i].getProductId() == productId) {
                        valid_id = true;
                        break;
                    }
                }
                if(valid_id) {
                    Product::remove(productId); 
                    cout << "Product removed successfully!" << endl;
                } else {
                    cout << "Product ID not found." << endl;
                }
                break;
            }
            case 0: {
                return;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    }
}

void Product::xuatfile(ofstream& file) {
        file << productId << "\n";
        file << name << "\n";
        cin.ignore(1, '\n');     // Xóa bộ đệm đầu vào của cin, bỏ qua ký tự newline
        file << price << "\n";
        file << amount << "\n";
}

string Product::getProductId() {
        return productId;
}

string Product::getName() {
        return name;
}

int Product::getPrice() {
        return price;
}

int Product::getAmount() {
        return amount;
}

void Product::setAmount(int amount) {
    this->amount = amount;
}

void Product::setProductId(const string& productId) {
        this->productId = productId;
}

void Product::WriteProducts() {
    cnt++;
    STT = string(3 - to_string(cnt).length(), '0') + to_string(cnt);
    cout << "Enter product's ID: "; 
    cin >> productId;
    cout << "Enter product's name: ";
    cin.ignore();
    getline(cin,name);
    cout << "Enter price: ";
    cin >> price;
    cout << "Enter amout: ";
    cin>> amount;

    createDate = getCurrentDate();
}

void Product::ReadData() {
    cout<< setw(5)<<"STT " << setw(20) << "Ma hang: " << setw(20) << "Ten hang: " <<setw(25) << "Gia tien: " << setw(25)   << "So luong: "  << endl;

    cout << setw(5) << STT << setw(20) << productId << setw(20) << name  << setw(25) << price << setw(25) << amount << endl;
}

int Product::getCount(){
    return cnt;
}

string Product::getCreateDate() {
    return this->createDate;
}

void Product::setCreateDate(string createDate) {
    this->createDate = createDate;
}

string Product::getModifiedDate() {
    return this->modifiedDate;
}

void Product::setModifiedDate(string modifiedDate) {
    this->modifiedDate = modifiedDate;
}

bool Product::isEmpty() {
    return empty(this->productId);
}

void Product::show() {
    if (this->isEmpty()) {
        return ;
    } 
    
    string productIdStr = this->productId;
    productIdStr.resize(ID_COL_SIZE, ' ');
    string nameStr = this->name;
    nameStr.resize(NAME_COL_SIZE, ' ');
    string priceStr = to_string(this->price);
    priceStr.resize(PRICE_COL_SIZE, ' ');
    string amountStr = to_string(this->amount);
    amountStr.resize(AMOUNT_COL_SIZE, ' ');
    string createDateStr = this->createDate;
    createDateStr.resize(DATE_COL_SIZE, ' ');
    string modifiedDateStr = this->modifiedDate;
    modifiedDateStr.resize(DATE_COL_SIZE, ' ');

    cout << productIdStr << nameStr << priceStr << amountStr << createDateStr << modifiedDateStr << '\n';
}

istream& operator >>(istream& is, Product& sp) {
    sp.WriteProducts();
    return is;
}

ostream& operator <<(ostream& os, Product& sp) {
    sp.ReadData(); 
    return os;
}
int Product::cnt = 0;

Product Product::readLine(string line) {
        int start = 0;
        string productId = line.substr(start, ID_COL_SIZE);
        productId = trim(productId);
        start += ID_COL_SIZE;
        string name = line.substr(start, NAME_COL_SIZE);
        name = trim(name);
        start += NAME_COL_SIZE;
        string priceStr = line.substr(start, PRICE_COL_SIZE);
        int price = stof(priceStr);
        start += PRICE_COL_SIZE;
        string amountStr = line.substr(start, AMOUNT_COL_SIZE);
        int amount = stoi(amountStr);
        start += AMOUNT_COL_SIZE;
        string createDate = line.substr(start, DATE_COL_SIZE);
        createDate = trim(createDate);
        start += DATE_COL_SIZE;
        string modifiedDate = line.substr(start, DATE_COL_SIZE);
        modifiedDate = trim(modifiedDate);

        Product product(productId, name, price, amount, createDate, modifiedDate);
        return product;
}

Product Product::findByProductId(string targetId) {
    ifstream file(PRODUCT_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << PRODUCT_FILE_NAME << endl;
        return Product();
    }
    string line;
    // Inorge the first line
    getline(file, line);
    while (getline(file, line)) {
        Product product = readLine(line);
        if (product.getProductId() == targetId) {
            file.close();
            return product;
        }
    }

    file.close();
    cout << "Not found product with ID = " << targetId << endl;
    return Product();
}

void Product::save(Product &product) {
    ofstream file(PRODUCT_FILE_NAME, ios::app); // Opend in append mode
    if (!file.is_open()) {
        cerr << "Error opening file for writting: " << PRODUCT_FILE_NAME << endl;
        return;
    }

    // Check if file is empty
    if (isFileEmpty(PRODUCT_FILE_NAME)) {
        string productIdCol = "Product_ID";
        productIdCol.resize(ID_COL_SIZE, ' ');
        string nameCol = "Name";
        nameCol.resize(NAME_COL_SIZE, ' ');
        string priceCol = "Price";
        priceCol.resize(PRICE_COL_SIZE, ' ');
        string amountCol = "Amount";
        amountCol.resize(AMOUNT_COL_SIZE, ' ');
        string createDateCol = "Create Date";
        createDateCol.resize(DATE_COL_SIZE, ' ');
        string modifiedDateCol = "Modified Date";
        modifiedDateCol.resize(DATE_COL_SIZE, ' ');

        file << productIdCol << nameCol << priceCol << amountCol << createDateCol << modifiedDateCol << '\n';
    }

    
    string productIdStr = product.getProductId();
    productIdStr.resize(ID_COL_SIZE, ' ');
    string nameStr = product.getName();
    nameStr.resize(NAME_COL_SIZE, ' ');
    string priceStr = to_string(product.getPrice());
    priceStr.resize(PRICE_COL_SIZE, ' ');
    string amountStr = to_string(product.getAmount());
    amountStr.resize(AMOUNT_COL_SIZE, ' ');
    string createDateStr = product.getCreateDate();
    createDateStr.resize(DATE_COL_SIZE, ' ');
    string modifiedDateStr = product.getModifiedDate();
    modifiedDateStr.resize(DATE_COL_SIZE, ' ');
    file << productIdStr << nameStr << priceStr << amountStr << createDateStr << modifiedDateStr << '\n';

    file.close();
}

void Product::update(string productId, Product& product) {
    Product originProduct = findByProductId(productId);
    if (originProduct.isEmpty()) return;

    vector<Product> products = getAllProducts();
    int index = -1;
    for (int i = 0; i < products.size(); i ++) {
        if (products[i].getProductId() == productId) {
            index = i;
            break;
        }
    }

    product.setModifiedDate(getCurrentDate());
    products[index] = product;

    ofstream file(PRODUCT_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for writting; " << PRODUCT_FILE_NAME << endl;
        return;
    }

    string productIdCol = "Product_ID";
    productIdCol.resize(ID_COL_SIZE, ' ');
    string nameCol = "Name";
    nameCol.resize(NAME_COL_SIZE, ' ');
    string priceCol = "Price";
    priceCol.resize(PRICE_COL_SIZE, ' ');
    string amountCol = "Amount";
    amountCol.resize(AMOUNT_COL_SIZE, ' ');
    string createDateCol = "Create Date";
    createDateCol.resize(DATE_COL_SIZE, ' ');

    file << productIdCol << nameCol << priceCol << amountCol << createDateCol << '\n';

    for (int i = 0; i < products.size(); i ++) {

        string productIdStr = products[i].getProductId();
        productIdStr.resize(ID_COL_SIZE, ' ');
        string nameStr = products[i].getName();
        nameStr.resize(NAME_COL_SIZE, ' ');
        string priceStr = to_string(products[i].getPrice());
        priceStr.resize(PRICE_COL_SIZE, ' ');
        string amountStr = to_string(products[i].getAmount());
        amountStr.resize(AMOUNT_COL_SIZE, ' ');
        string createDateStr = products[i].getCreateDate();
        createDateStr.resize(DATE_COL_SIZE, ' ');

        file << productIdStr << nameStr << priceStr << amountStr << createDateStr << '\n';
    }
    

    file.close();
}

void Product::remove(string productId) {
    Product product = findByProductId(productId);
    if (product.isEmpty()) {
        return;
    }

    vector<Product> products = getAllProducts();
    int index = -1;
    for (int i = 0; i < products.size(); i ++) {
        if (products[i].getProductId() == productId) {
            index = i;
            break;
        }
    }

    products.erase(products.begin() + index);

    ofstream file(PRODUCT_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for writting; " << PRODUCT_FILE_NAME << endl;
        return;
    }

    string productIdCol = "Product_ID";
    productIdCol.resize(ID_COL_SIZE, ' ');
    string nameCol = "Name";
    nameCol.resize(NAME_COL_SIZE, ' ');
    string priceCol = "Price";
    priceCol.resize(PRICE_COL_SIZE, ' ');
    string amountCol = "Amount";
    amountCol.resize(AMOUNT_COL_SIZE, ' ');
    string createDateCol = "Create Date";
    createDateCol.resize(DATE_COL_SIZE, ' ');

    file << productIdCol << nameCol << priceCol << amountCol << createDateCol << '\n';

    for (int i = 0; i < products.size(); i ++) {

        string productIdStr = products[i].getProductId();
        productIdStr.resize(ID_COL_SIZE, ' ');
        string nameStr = products[i].getName();
        nameStr.resize(NAME_COL_SIZE, ' ');
        string priceStr = to_string(products[i].getPrice());
        priceStr.resize(PRICE_COL_SIZE, ' ');
        string amountStr = to_string(products[i].getAmount());
        amountStr.resize(AMOUNT_COL_SIZE, ' ');
        string createDateStr = products[i].getCreateDate();
        createDateStr.resize(DATE_COL_SIZE, ' ');
        string modifiedDateStr = products[i].getModifiedDate();
        modifiedDateStr.resize(DATE_COL_SIZE, ' ');

        file << productIdStr << nameStr << priceStr << amountStr << createDateStr << modifiedDateStr << '\n';
    }
    

    file.close();
}

vector<Product> Product::getAllProducts() {
    vector<Product> products;
    ifstream file(PRODUCT_FILE_NAME);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << PRODUCT_FILE_NAME << endl;
        return products;
    }

    string line;
    // Skip the header line
    getline(file, line);
    while (getline(file, line)) {
        Product product = readLine(line);
        if (!product.isEmpty()) products.push_back(product);
        
    }

    file.close();
    return products;
}

void Product::showTableHeader() {
    string productIdCol = "Product_ID";
    productIdCol.resize(ID_COL_SIZE, ' ');
    string nameCol = "Name";
    nameCol.resize(NAME_COL_SIZE, ' ');
    string priceCol = "Price";
    priceCol.resize(PRICE_COL_SIZE, ' ');
    string amountCol = "Amount";
    amountCol.resize(AMOUNT_COL_SIZE, ' ');
    string createDateCol = "Create Date";
    createDateCol.resize(DATE_COL_SIZE, ' ');
    string modifiedDateCol = "Modified Date";
    modifiedDateCol.resize(DATE_COL_SIZE, ' ');

    cout << productIdCol << nameCol << priceCol << amountCol << createDateCol << modifiedDateCol << '\n';
}

void Product::showAllProducts() {
    vector<Product> products = getAllProducts();
    Product::showTableHeader();

    for (int i = 0; i < products.size(); i ++) {
        products[i].show();
    }
    
}