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

#pragma once

using namespace std;

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
    vendor.viewProduct(vendor.getID(), totalProduct, totalVendor);
    vendor.addProduct(conn, vendor.getID());
}

// Customer operation
void viewVendorList() {
    int operation;
    do {
        mainHeader();
        cout << "\n----Vendor List----\n";
        vendor.viewVendor(totalVendor);
        cout << "\n--Operation--\n\t1-Enter ID to choose restaurant\n\t2-View all products\n";
        cout << ">> ";
        cin >> operation;
        vendor.viewProduct(operation, totalProduct, totalVendor);
        system("pause");
    } while (operation != 0);
}

void viewProductList(int vendorID) {
    mainHeader();
    vendor.viewProduct(vendorID, totalProduct, totalVendor);
}
#endif