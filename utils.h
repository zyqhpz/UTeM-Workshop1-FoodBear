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

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#pragma once

using namespace std;

using namespace ftxui;

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
            cout << "Connected" << endl;
            system("cls");
        }
        else {
            cout << "DB failed" << mysql_errno(conn) << endl;
        }

        conn = mysql_real_connect(conn, "localhost", "root", "", "foodbear", 3306, NULL, 0);

        if (conn) {
            cout << "Connected to MySQL" << conn << endl;
            //system("pause");
        }
        else {
            cout << "Failed" << mysql_errno(conn) << endl;
        }
    return conn;
    }
};

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

    cout << setw(25);
    cout << "###########################################################" << endl;
    cout << "##                                                       ##" << endl;
    cout << "##   FoodBear Online Food Ordering and Delivery System   ##" << endl;
    cout << "##                                                       ##" << endl;
    cout << "###########################################################" << endl << endl;

 /*   if (additional != "") {
        cout << additional << endl;
    }*/
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

    return false;
}

// Vendor operation

void viewAddProduct() {
    mainHeader();
    //vendor.viewProduct(vendor.getID(), totalProduct, totalVendor);
    vendor.addProduct(conn, vendor.getID());
}

// Customer operation
void startOrder(int, int);
void getReceipt(double);

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

    cout << "Start ordering (Y/N) >> ";
    cin >> selection;

    if (selection == 'Y' || selection == 'y') {
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
                    getReceipt(total);
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

void getReceipt(double total) { // confirmed order?? //kat sini jugak assign all the data to database
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

    cust.insertOrder(conn);
    order.clear();
}

void viewProductList(int vendorID) {
    mainHeader();
    //vendor.viewProduct(vendorID, totalProduct, totalVendor);
}
#endif