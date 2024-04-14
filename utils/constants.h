#include <iostream>

using namespace std;

const string ORDER_STATUS_PAID = "PAID";
const string ORDER_STATUS_UNPAID = "UNPAID";
const string ORDER_STATUS_DELETED = "DELETED";

// database
const string ORDER_FILE_NAME = "data/order_data.txt";
const string PRODUCT_FILE_NAME = "data/product_data.txt";
const string CUSTOMER_FILE_NAME = "data/customer_data.txt";
const string ORDER_ITEM_FILE_NAME = "data/order_item_data.txt";

// Id counter
const string ITEM_COUNTER_FILE = "data/item_counter_data.txt";

const int ID_COL_SIZE = 20;
const int NAME_COL_SIZE = 30;
const int PRICE_COL_SIZE = 20;
const int AMOUNT_COL_SIZE = 10;
const int DATE_COL_SIZE = 30;
const int PHONE_COL_SIZE = 15;
const int STATUS_COL_SIZE = 10;
