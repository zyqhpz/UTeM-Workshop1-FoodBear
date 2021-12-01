#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <iomanip>

#include <windows.h>

#include <sstream>

#include <conio.h>

#include <mysql.h>

#include "Customer.h"
#include "Vendor.h"
#include "Rider.h"
#include "Admin.h"

#include "TextTable.h"

//#include <ftxui/dom/elements.hpp>
//#include <ftxui/screen/screen.hpp>

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

#include "Menu.h"

//#include "tabulate/single_include/tabulate/tabulate.hpp"
#include <tabulate/table.hpp>
//using Row_t = Table::Row_t;

using namespace tabulate;

#pragma once

using namespace std;

//using namespace ftxui;

// Global variable
MYSQL* conn;

MYSQL_ROW row;
MYSQL_RES* res;

// Total rows for each table
int totalCustomer;
int totalRider;
int totalVendor;
int totalAdmin;

int totalProduct; // later make query to count all product from a vendor.

// class object declaration
Customer cust;
Vendor vendor;
Rider rider;
Admin admin;

class db_connection {
public:
    static MYSQL* ConnectionFunction() {
        conn = mysql_init(0);
        if (conn) {
            //cout << "Connected" << endl;
            system("cls");
        }
        else {
            cout << "DB failed" << mysql_errno(conn) << endl;
        }

        conn = mysql_real_connect(conn, "localhost", "root", "", "foodbear", 3306, NULL, 0);

        if (conn) {
            //cout << "Connected to MySQL" << conn << endl;
            //system("pause");
        }
        else {
            cout << "Failed" << mysql_errno(conn) << endl;
        }
    return conn;
    }
};

void centerify_output(std::string str, int num_cols) {
    // Calculate left padding
    int padding_left = (num_cols / 2) - (str.size() / 2);

    // Put padding spaces
    for (int i = 0; i < padding_left; ++i) std::cout << ' ';

    // Print the message
    std::cout << str;
}

// function to display main header for every page
//inline void mainHeader(std::string additional = "") {
inline void mainHeader() {
    system("cls");


    /* HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);

     COORD max_size = GetLargestConsoleWindowSize(screen);

     char s[] = "Hello world!";

     COORD pos;
     pos.X = (max_size.X - sizeof(s)) / 2;
     pos.Y = max_size.Y / 2;
     SetConsoleCursorPosition(screen, pos);

     LPDWORD written{};
     WriteConsole(screen, s, sizeof(s), written, 0);*/

    /*
    
    gotoXY(5, 3);
    //cout << setw(25);
    cout << "###########################################################" << endl;
    gotoXY(5, 4);
    cout << "##                                                       ##" << endl;
    gotoXY(5, 5);
    cout << "##   FoodBear Online Food Ordering and Delivery System   ##" << endl;
    gotoXY(5, 6);
    cout << "##                                                       ##" << endl;
    gotoXY(5, 7);
    cout << "###########################################################" << endl << endl;
    
    */

    vector<string> lines = {
        "###########################################################",
        "##                                                       ##",
        "##   FoodBear Online Food Ordering and Delivery System   ##",
        "##                                                       ##",
        "###########################################################",
    };

    int num_cols = 100;

    // VIRTUAL BORDER
    gotoXY(0, 3);
    std::cout << std::endl;
    for (int i = 0; i < num_cols; ++i) std::cout << ' ';
    //std::cout << ' ' << std::endl;
    std::cout << ' ' << std::endl;

    // OUTPUT
    for (int i = 0; i < lines.size(); ++i) {
        centerify_output(lines[i], num_cols);
        std::cout << std::endl;
    }

    // VIRTUAL BORDER
    std::cout << std::endl;
    for (int i = 0; i < num_cols; ++i) std::cout << ' ';
    //std::cout << '|' << std::endl;
    std::cout << ' ' << std::endl;
}


// Enumerator
enum IN {

    // 13 is ASCII for carriage
    // return
    IN_BACK = 8,
    IN_RET = 13

};

// Function that accepts the password
string inputPassword(char sp = '*') {
    // Stores the password
    string passwd = "";
    char ch_ipt;

    // Until condition is true
    while (true) {

        ch_ipt = _getch();

        // if the ch_ipt
        if (ch_ipt == IN::IN_RET) {
            cout << endl;
            return passwd;
        }
        else if (ch_ipt == IN::IN_BACK && passwd.length() != 0) {
            passwd.pop_back();

            // Cout statement is very
            // important as it will erase
            // previously printed character
            cout << "\b \b";

            continue;
        }

        // Without using this, program
        // will crash as \b can't be
        // print in beginning of line
        else if (ch_ipt == IN::IN_BACK && passwd.length() == 0) {
            continue;
        }

        passwd.push_back(ch_ipt);
        cout << sp;
    }
}

void fetchAllData() {
    string sqlVendor = "SELECT * FROM vendor";
    string sqlCust = "SELECT * FROM customer";
    string sqlRider = "SELECT * FROM rider";

    string sqlProduct = "SELECT * FROM product";

    const char* qVendor = sqlVendor.c_str();
    const char* qCust = sqlCust.c_str();
    const char* qRider = sqlRider.c_str();

    const char* qProduct = sqlProduct.c_str();

    int qSVendor = mysql_query(conn, qVendor);
    if (!qSVendor) {
        res = mysql_store_result(conn);
        int count = mysql_num_fields(res);
        totalVendor = vendor.fetchData(res, count);
    }

    int qSCust = mysql_query(conn, qCust);
    if (!qSCust) {
        res = mysql_store_result(conn);
        totalCustomer = cust.fetchData(res);
        cout << totalCustomer << endl;
        //system("pause");
    }

    int qSRider = mysql_query(conn, qRider);
    if (!qSRider) {
        res = mysql_store_result(conn);
        totalRider = rider.fetchData(res);
        cout << totalRider << " success" << endl;
        //system("pause");
    }

    int qSProduct = mysql_query(conn, qProduct);
    if (!qSProduct) {
        cout << "1\n";
        res = mysql_store_result(conn);
        cout << "2\n";
        totalProduct = vendor.fetchProduct(res);
        cout << "3\n";
        cout << totalProduct << endl;
        //system("pause");
    }

    else {
        cout << "Failed to fetch\n";
        system("pause");
    }
}

bool loginUser(int logType) {
    mainHeader();

    string user, pass;
    cout << "\n----Login----\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";

    pass = inputPassword();

    cout << pass << endl;

    /*
    if (logType == 3) { // Customer
       // totalRider = rider.fetchData(res, count);
        log = cust.login(user, pass, totalRider);
    }*/

    /*
    if (logType == 1 && vendor.login(user, pass, totalVendor)) {
       // cout << "Welcome " << rider.getName() << endl;
        return true;
    }

    if (logType == 2 && cust.login(user, pass, totalCustomer)) {
       // cout << "Welcome " << rider.getName() << endl;
        return true;
    }

    if (logType == 3 && rider.login(user, pass, totalRider)) {
       // cout << "Welcome " << rider.getName() << endl;
        return true;
    }
    */

    if (vendor.login(user, pass, totalVendor) || cust.login(user, pass, totalCustomer) || rider.login(user, pass, totalRider)) {
        // cout << "Welcome " << rider.getName() << endl;
        return true;
    }

    return false;
}

int login() {
    mainHeader();

    string user, pass;
    for (int i = 0; i < 40; ++i) std::cout << ' ';
    cout << "----Login----\n\n";
    for (int i = 0; i < 30; ++i) std::cout << ' ';
    cout << "Username: ";
    cin >> user;
    for (int i = 0; i < 30; ++i) std::cout << ' ';
    cout << "Password: ";
    pass = inputPassword();

    int role;

    //cout << pass << endl;

    /*
    if (logType == 3) { // Customer
       // totalRider = rider.fetchData(res, count);
        log = cust.login(user, pass, totalRider);
    }*/

    if (vendor.login(user, pass, totalVendor)) {
       // cout << "Welcome " << rider.getName() << endl;
        return role = 1;
    }

    if (cust.login(user, pass, totalCustomer)) {
       // cout << "Welcome " << rider.getName() << endl;
        return role = 2;
    }

    if (rider.login(user, pass, totalRider)) {
       // cout << "Welcome " << rider.getName() << endl;
        return role = 3;
    }
    
    return 0;
}

// Vendor operation
void viewManageProduct() {
    int e;
    char op;

    do {
        mainHeader();
        vendor.viewProduct(vendor.getID(), totalProduct, totalVendor, e);

        cout << "\n\t---Operations---\n\t1-Add product\n\t2-Edit product\n\t0-Exit\n\t>> ";
        cin >> op;

        if (op == '1') {
            mainHeader();
            vendor.viewProduct(vendor.getID(), totalProduct, totalVendor, e);
            vendor.addProduct(conn, vendor.getID());
        }

        else if (op == '2') {
            mainHeader();
            vendor.viewProduct(vendor.getID(), totalProduct, totalVendor, e);
            vendor.editProduct(mainHeader, conn, totalProduct);
        }

        else if (op == '0') {
            //break;
        }

        else
            cout << "\nInvalid input. Try again.\n";

    } while (op != '0');
}

void viewActiveOrder() {
    TextTable tb;

    int totalActive = vendor.viewActiveOrder(conn, tb);

    TextTable td;
    int orderID;
    int exist = 0;

    // view active order
    do {
        jump:;
        mainHeader();
        gotoXY(10, 13);
        cout << "\n\t------Active Order------\n";
        cout << tb << endl;

        cout << "\n   Enter OrderID to view details (0 - back to Main Menu)\n\t>> ";
        cin >> orderID;
        
        if (orderID == 0)
            break;
        else {
            mainHeader();
            vendor.viewActiveOrderDetail(conn, orderID, exist);
            if (exist == 0) {
                goto jump;
            }
            //system("pause");
        }
    } while (orderID != 0);

    /*
        1. enter orderId to view details of the order - done
        2. ada option untuk accept or reject
    */

}
// View all orders for vendor
void viewAllOrder() {
    TextTable tb;

    int totalOrder = vendor.viewPreviousOrder(conn, tb);

    TextTable td;
    int orderID;
    int exist = 0;

    do {
    jump:;
        mainHeader();
        gotoXY(10, 13);
        cout << "\n\t------All Order------\n";
        cout << tb << endl;

        cout << "\n   Enter OrderID to view details (0 - back to Main Menu)\n\t>> ";
        cin >> orderID;

        if (orderID == 0)
            break;
        else {
            mainHeader();
            vendor.viewPreviousOrderDetail(conn, orderID, exist);
            if (exist == 0) {
                goto jump;
            }
            system("pause");
        }
    } while (orderID != 0);
}

// Customer operation
void startOrder(int, int);
void getReceipt(double, int);

void viewVendorList() {
    int exist = 0;
    int operation;
    do {
        mainHeader();
        cout << "\n----Vendor List----\n";
        vendor.viewVendor(totalVendor);
        // cout << "\n--Operation--\n\t1-Enter vendor ID to choose\n\t2-View all products\n\t0-Back to Main Menu\n";
        cout << "\n--Operation--\n\t--Enter vendor ID to start ordering\n\t0-Back to Main Menu\n";
        cout << ">> ";
        cin >> operation;
        if (operation == 0)
            break;

        // display vendor name here
        mainHeader();
        cout << "\n------" << vendor.getVendorName(operation) << "------\n";
        vendor.viewProduct(operation, totalProduct, totalVendor, exist);
            if (exist == 1) {
                startOrder(operation, exist);
                goto exit;
            }
        system("pause");
    } while (operation != 0);
exit:;
}

void startOrder(int venID, int exist) {
    char selection;
    int foodID;
    int quantity;
    char proceed;
    double total = 0;
    //do {
        // 1. select id to add to cart -> enter quantity -> display eg. "2x Nasi Lemak has been added to cart." 
        // 2. display add more item? OR cancel order -> customer choose YES or NO
        // 3. if yes -> back to no 1. else no -> display all added products name and quantity -> "confirm order?" -> YES (goto receipt page) or NO (cancel order -> home) 

        // cout << "\n1-Start ordering\n0-Back to Main Menu\n";
        //cout << "\n1-Enter product id\n0-Cancel\n";
       // cout << ">> ";
    
    //mainHeader();
    //vendor.viewProduct(venID, totalProduct, totalVendor, exist);

    stringstream checker;
    checker << "SELECT address FROM customer WHERE id = " << cust.getID();

    string qC = checker.str();
    const char* q = qC.c_str();
    int qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    string address;

    row = mysql_fetch_row(res);

    cout << "Start ordering (Y/N) >> ";
    cin >> selection;

    if (selection == 'N' || selection == 'n') {}

    else if (row[0] == NULL) {
        color(12);
        cout << "\nAddress not set. Update your address to start ordering\n\n";
        color(7);
    }

    else if (selection == 'Y' || selection == 'y') {
        // start select product id
        //cout << "start order\n";
        //cout << "\n1-Enter product id\n0-Cancel\n";
        do {
            mainHeader();
            cout << "\n------" << vendor.getVendorName(venID) << "------\n";
            vendor.viewProduct(venID, totalProduct, totalVendor, exist);

            cout << "\nEnter food id (Enter 0 to Cancel) >> ";
            cin >> foodID;

            if (foodID == 0) {
                break;
            }
                
            cout << "Enter quantity >> ";
            cin >> quantity;

            cust.selectProduct(vendor, foodID, quantity, total);

            do {
                cout << "\nHave additional order? (Y/N) >> ";
                cin >> proceed;
                if (proceed == 'y' || proceed == 'Y') {
                    break;
                }
                else if (proceed == 'n' || proceed == 'N') {
                    //cout << "\nthis is payment page\n";
                    getReceipt(total, venID);
                    goto jump;
                    //break; // go to payment page
                }
                else
                    cout << "Error input";
            } while (proceed != 'n' || proceed != 'N' && proceed != 'Y' || proceed != 'Y');
        } while (foodID != 0); // check only food id >= 0 entered accepted, else -> product not found
    }
    jump:;
        /*
        else if (selection == 'n' || selection == 'N') {
            break;
        }
        else {
            cout << "\nInvalid selection. Try again.\n";
        }*/

    //} while (selection != 0);
}

void getReceipt(double total, int vendorID) { // confirmed order?? //kat sini jugak assign all the data to database
    vector<vector<string>> order = cust.getOrder();

    TextTable tableOrder('-', '|', '+');

    tableOrder.add("No.");
    tableOrder.add("Product Name");
    tableOrder.add("Quantity");
    tableOrder.add("Total Price (RM)");
    tableOrder.endOfRow();

    for (int i = 0; i < order.size(); i++) {
        tableOrder.add(to_string(i + 1));
        tableOrder.add(order[i][0]);
        tableOrder.add(order[i][2]);
        tableOrder.add(order[i][1]);
        tableOrder.endOfRow();
    }

    total += 4;

    stringstream t;
    t << fixed << setprecision(2) << total;

    tableOrder.add("");
    tableOrder.add("Delivery charge");
    tableOrder.add("");
    tableOrder.add("4.00");
    tableOrder.endOfRow();

    tableOrder.add("");
    tableOrder.add("Total");
    tableOrder.add("");
    tableOrder.add(t.str());
    tableOrder.endOfRow();

    tableOrder.setAlignment(2, TextTable::Alignment::RIGHT);
    tableOrder.setAlignment(3, TextTable::Alignment::RIGHT);

   // cout << tableOrder;

   // cout << "\nTotal amount: RM " << fixed << setprecision(2) << total << endl;

    cout << "\n-----Receipt------\n";
    cout << tableOrder;

    //cout << "\n\tProduct Name\t\tQuantity\n";
    //for (int i = 0; i < order.size(); i++) {
    //    cout << "\t" << order[i][0] << "\t\t" << order[i][2] << endl;
    //}

    cust.insertOrder(conn, vendorID);
    order.clear();
}

void viewPreviousOrder() {
    TextTable tb;

    int totalOrder = cust.fetchOrderData(conn);
    cout << "1\n";
    int totalOrderDetail = cust.fetchOrderDetails(conn);
    cout << "1\n";
    int totalPrev = cust.fetchPreviousOrder(conn, tb);
    cout << "1\n";

    vector<vector<int>> detail;

    TextTable tt;

    tt.add("Order ID");
    tt.add("Total Price (RM)");
    tt.add("Quantity");
    tt.endOfRow();

    for (int i = 0; i < totalOrder; i++) {
        cust.displayPreviousOrder(i, totalOrderDetail, tt);
    }

    mainHeader();

    //cout << tt;

    /*
    
    // latest prevOrder display
    gotoXY(10, 13);
    cout << "-----Previous Order------\n";
    cout << tb;

    */

    // view previous order

    int orderID;
    int exist = 0;

    do {
    jump:;
        mainHeader();
        gotoXY(10, 13);
        cout << "\n\t------Previous Order------\n";
        cout << tb << endl;

        cout << "\n   Enter OrderID to view details (0 - back to Main Menu)\n\t>> ";
        cin >> orderID;

        if (orderID == 0)
            break;
        else {
            mainHeader();
            cust.viewPreviousOrderDetails(conn, orderID, exist);
            if (exist == 0) {
                goto jump;
            }
            system("pause");
        }
    } while (orderID != 0);
}

void viewProductList(int vendorID) {
    mainHeader();
    //vendor.viewProduct(vendorID, totalProduct, totalVendor);
}
#endif