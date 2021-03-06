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


//#include <ftxui/dom/elements.hpp>
//#include <ftxui/screen/screen.hpp>

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

#include "Menu.h"
#include "External.h"

//#include "tabulate/single_include/tabulate/tabulate.hpp"
//#include <tabulate/table.hpp>
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

    int num_cols = 200;

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
    string sqlAdmin = "SELECT * FROM admin";

    string sqlProduct = "SELECT * FROM product";

    const char* qVendor = sqlVendor.c_str();
    const char* qCust = sqlCust.c_str();
    const char* qRider = sqlRider.c_str();
    const char* qAdmin = sqlAdmin.c_str();

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
       // cout << totalCustomer << endl;
        //system("pause");
    }

    int qSRider = mysql_query(conn, qRider);
    if (!qSRider) {
        res = mysql_store_result(conn);
        totalRider = rider.fetchData(res);
       // cout << totalRider << " success" << endl;
        //system("pause");
    }

    int qSAdmin = mysql_query(conn, qAdmin);
    if (!qSAdmin) {
        res = mysql_store_result(conn);
        int count = mysql_num_fields(res);
        totalAdmin = admin.fetchData(res, count);
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
    for (int i = 0; i < 93; ++i) std::cout << ' ';
    cout << "----Login----\n\n";
    for (int i = 0; i < 80; ++i) std::cout << ' ';
    cout << "Username: ";
    cin >> user;
    for (int i = 0; i < 80; ++i) std::cout << ' ';
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

    if (admin.login(user, pass)) {
        return role = 4;
    }
    
    return 0;
}

// Vendor operation
void viewManageProduct() {
    int e;
    char op;

    do {
        fetchAllData();
        mainHeader();
        vendor.viewProduct(vendor.getID(), totalProduct, totalVendor, e);

        cout << "\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "---Operations---\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "1 - Add product\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "2 - Edit product\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "0 - Exit\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << ">> ";
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

    TextTable td;
    int orderID;

    // view active order
    do {
        jump:;
        int exist = 0;
        TableCart tb;
        mainHeader();
        int totalActive = vendor.viewActiveOrder(conn, tb);
        mainHeader();
        gotoXY(10, 13);
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "------Active Order------\n";
        cout << tb << endl;

        cout << "\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "Enter OrderID to view details (0 - back to Main Menu)\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << ">> ";
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
    TableOrder tb;

    int totalOrder = vendor.viewPreviousOrder(conn, tb);

    TextTable td;
    int orderID;
    int exist = 0;

    do {
    jump:;
        mainHeader();
        gotoXY(10, 13);
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "------All Order------\n";
        cout << tb << endl;

        cout << "\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "Enter OrderID to view details (0 - back to Main Menu)\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << ">> ";
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

void orderGetByMonth(vector<vector<string>> data, int year) {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    vector<int> v1;

    v1 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    for (int i = 0; i < data.size(); i++) {
        if (stoi(data[i][4]) == year) {
            if (data[i][3] == "1")
                v1[1] = stoi(data[i][0]);
            if (data[i][3] == "2")
                v1[2] = stoi(data[i][0]);
            if (data[i][3] == "3")
                v1[3] = stoi(data[i][0]);
            if (data[i][3] == "4")
                v1[4] = stoi(data[i][0]);
            if (data[i][3] == "5")
                v1[5] = stoi(data[i][0]);
            if (data[i][3] == "6")
                v1[6] = stoi(data[i][0]);
            if (data[i][3] == "7")
                v1[7] = stoi(data[i][0]);
            if (data[i][3] == "8")
                v1[8] = stoi(data[i][0]);
            if (data[i][3] == "9")
                v1[9] = stoi(data[i][0]);
            if (data[i][3] == "10")
                v1[10] = stoi(data[i][0]);
            if (data[i][3] == "11")
                v1[11] = stoi(data[i][0]);
            if (data[i][3] == "12")
                v1[12] = stoi(data[i][0]);
        }
    }

    gp << "set title 'Total Order Processed By Month In " << year << "' \n";
    gp << "set xlabel 'Month'\n";
    gp << "set ylabel 'Total Order Processed'\n";
    gp << "set xrange [0:12]\n";
    gp << "set xtics ('1' 1, '2' 2, '3' 3, '4' 4, '5' 5, '6' 6, '7' 7, '8' 8, '9' 9, '10' 10, '11' 11, '12' 12)\n";
    gp << "set yrange [0:10]\n";
    gp << "set ytics ('1' 1, '2' 2, '3' 3, '4' 4, '5' 5, '6' 6, '7' 7, '8' 8, '9' 9, '10' 10, '11' 11, '12' 12)\n";
    //gp << "plot '-' with lines title 'Total Order Made by Month'\n";
    gp << "plot '-' with lines\n";

    gp.send(v1);

    std::cin.get();

    system("pause");
}

void viewVendorReport() {

    mainHeader();

    TableOrder tt;

    tt.add("Month");
    tt.add("Total Get Order");
    tt.add("Total Quantity");
    tt.add("Total Earning (RM)");
    tt.endOfRow();

    vector<vector<string>> graph;

    stringstream ss;
    ss << "SELECT COUNT(cust_order.id) AS order_get , SUM(cust_order.total_quantity) AS quantity, SUM(cust_order.total_price) AS price, extract(month FROM cust_order.date) AS month, extract(year FROM cust_order.date) AS year FROM cust_order JOIN delivery ON cust_order.id = delivery.payment_id JOIN payment ON payment.order_id = delivery.payment_id WHERE payment.vendor_id = " << vendor.getID() << " AND delivery.status > 1 GROUP BY month ORDER BY year";

    string qs = ss.str();
    const char* q = qs.c_str();
    int qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);

    while (row = mysql_fetch_row(res)) {
        // order made, quantity, expenses, month
        graph.push_back({ row[0], row[1], row[2], row[3], row[4] });
        stringstream date;
        date << row[3] << "/" << row[4];
        //tt.add(row[3]);
        tt.add(date.str());
        tt.add(row[0]);
        tt.add(row[1]);
        tt.add(row[2]);
        tt.endOfRow();
    }


    int year;
    do {
        mainHeader();
        cout << tt;
        for (int i = 0; i < 75; ++i) std::cout << ' ';
        cout << "View chart total order processed by month. Enter 2021 or 2022\n";
        for (int i = 0; i < 75; ++i) std::cout << ' ';
        cout << ">> ";
        cin >> year;
        //if (year == 2021)
        if (year != 2021 && year != 2022) {
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            cout << "Invalid choice\n";
            system("pause");
        }
        else if (year == 0)
            break;
        else {
            orderGetByMonth(graph, year);
            break;
        }

    } while (year != 0);
}

void autoRejectOrder() {
    time_t t = time(0);
    char* dt = ctime(&t);

    tm* time = localtime(&t);

    int year = 1900 + time->tm_year;
    int month = 1 + time->tm_mon;
    int day = time->tm_mday;
    //string tm = to_string(5 + time->tm_hour) + ":" + to_string(30 + time->tm_min) + ":" + to_string(time->tm_sec);
    string tm = to_string(time->tm_hour) + ":" + to_string(time->tm_min - 10) + ":" + to_string(time->tm_sec);

    string date = to_string(year) + "-" + to_string(month) + "-" + to_string(day);

    string timestamp = date + " " + tm;

   // cout << date;

    //cout << year << "-" << month << "-" << day << "\n";
    
   // cout << "time: " << time;
    //cout << "date: " << dt;
   // string str(dt);

    //cout << endl << timestamp;

    stringstream sql;
   // sql << "SELECT * FROM cust_order WHERE date < '" << timestamp << "'";
    sql << "SELECT cust_order.id FROM cust_order JOIN delivery ON cust_order.id = delivery.payment_id WHERE cust_order.date <= '" << timestamp << "' AND delivery.status = 0";

    string qC = sql.str();
    const char* q = qC.c_str();
    int qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    string address;

    while (row = mysql_fetch_row(res)) {
        int order_id = stoi(row[0]);
        string upsql = "UPDATE delivery SET status = -1 WHERE payment_id = " + to_string(order_id);
        const char* up = upsql.c_str();
        int upstate = mysql_query(conn, up);
    }
}

// Customer operation
void startOrder(int, int);
void getReceipt(double, int);
void viewVendorList();
void searchProduct();

// Select operation in viewing product
void orderBy() {
    char operation;

    do {
        mainHeader();
        gotoXY(90, 14);
        cout << "---Selection---\n\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "1-Select vendor\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "2-Search product\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "0-Back to Main Menu\n";

        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << ">> ";
        cin >> operation;

        if (operation == '1') {
            cust.clearOrder();
            viewVendorList();
            system("pause");
            break;
        }
        else if (operation == '2') {
            cust.clearOrder();
            searchProduct();
            system("pause");
            break;
        }
        else if (operation == '0') {
            break;
        }

    } while (operation == '1' || operation == '2');
}

void searchProduct() {
    mainHeader();
    gotoXY(90, 14);
    cout << "---Search Product Name---\n\n";

    string target;
    for (int i = 0; i < 80; ++i) std::cout << ' ';
    cout << "Enter product name:\n";
    for (int i = 0; i < 80; ++i) std::cout << ' ';
    cout << ">> ";
    cin >> target;
    boost::algorithm::to_lower(target);

    // get all product name here

    vector<string> words{ "Nasi Lemak", "Nasi Goreng", "Iced Lemon Tea", "Roti Canai", "Lamb Chop" };

    bool found = false;

    int vendor_id;
    int exist = 0;
    do {
        vendor.searchProductByName(target, totalProduct, totalVendor, found);
        if (found) {
            cout << "\n\n\n";
            for (int i = 0; i < 80; ++i) std::cout << ' ';
            cout << "-------------------------------------\n";
            for (int i = 0; i < 80; ++i) std::cout << ' ';
            cout << " * Enter vendor ID to start ordering\n";
            for (int i = 0; i < 80; ++i) std::cout << ' ';
            cout << " 0-Back to Main Menu\n";
            for (int i = 0; i < 80; ++i) std::cout << ' ';
            cout << ">> ";
            cin >> vendor_id;

            if (vendor_id == 0)
                goto exit;

            mainHeader();
            //for (int i = 0; i < 85; ++i) std::cout << ' ';
            gotoXY(90, 14);
            cout << "------" << vendor.getVendorName(vendor_id) << "------\n\n\n";
            vendor.viewProduct(vendor_id, totalProduct, totalVendor, exist);
            if (exist == 1) {
                startOrder(vendor_id, exist);
                goto exit;
            }
            system("pause");
        }
        else {
            break;
        }
    } while (vendor_id != 0);
exit:;
}

// View list of vendor
void viewVendorList() {
    int exist = 0;
    int operation;
    do {
        mainHeader();
        gotoXY(90, 14);
        cout << "----Vendor List----\n";
        vendor.viewVendor(totalVendor);
        // cout << "\n--Operation--\n\t1-Enter vendor ID to choose\n\t2-View all products\n\t0-Back to Main Menu\n";
        //cout << "\n--Operation--\n\t--Enter vendor ID to start ordering\n\t0-Back to Main Menu\n";

        cout << "\n\n\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "-------------------------------------\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << " * Enter vendor ID to start ordering\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << " 0-Back to Main Menu\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << ">> ";
        cin >> operation;
        if (operation == 0)
            break;

        // display vendor name here
        mainHeader();
        //for (int i = 0; i < 85; ++i) std::cout << ' ';
        gotoXY(90, 14);
        cout << "\n";
        //cout << "------" << vendor.getVendorName(operation) << "------\n\n\n";
        vendor.viewProduct(operation, totalProduct, totalVendor, exist);
            if (exist == 1) {
                startOrder(operation, exist);
                goto exit;
            }
        for (int i = 0; i < 80; ++i) std::cout << ' ';
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

    cout << "\n\n";
    for (int i = 0; i < 85; ++i) std::cout << ' ';
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
            gotoXY(90, 14);
            cout << "------" << vendor.getVendorName(venID) << "------\n\n\n";
            vendor.viewProduct(venID, totalProduct, totalVendor, exist);

            cout << "\n\n";
            for (int i = 0; i < 80; ++i) std::cout << ' ';
            cout << "Enter food id (Enter 0 to Cancel) >> ";
            cin >> foodID;

            if (foodID == 0) {
                cust.clearOrder();
                break;
            }

            for (int i = 0; i < 80; ++i) std::cout << ' ';
            cout << "Enter quantity >> ";
            cin >> quantity;

            if (cust.selectProduct(vendor, foodID, quantity, total, venID)) {
                do {
                err:;
                    cout << "\n\n";
                    for (int i = 0; i < 80; ++i) std::cout << ' ';
                    cout << "Have additional order? (Y/N) >> ";
                    cin >> proceed;
                    if (proceed == 'y' || proceed == 'Y') {
                        break;
                    }
                    else if (proceed == 'n' || proceed == 'N') {
                        if (cust.checkOrder()) {
                            //cout << "\nthis is payment page\n";
                            getReceipt(total, venID);
                            goto jump;
                            //break; // go to payment page
                        }
                        else {
                            goto jump;
                        }

                    }
                    else {
                        for (int i = 0; i < 80; ++i) std::cout << ' ';
                        cout << "Error input\n";
                    }
                } while (proceed != 'n' || proceed != 'N' && proceed != 'Y' || proceed != 'Y');
            }
            else {
                //for (int i = 0; i < 80; ++i) std::cout << ' ';
                //cout << "Invalid product id selection. Please try again.\n";
                //cout << "hey\n";
                //system("pause");
                goto err;
            }

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

    TableCart tableOrder('-', '|', '+');

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

    tableOrder.setAlignment(2, TableCart::Alignment::RIGHT);
    tableOrder.setAlignment(3, TableCart::Alignment::RIGHT);

   // cout << tableOrder;

   // cout << "\nTotal amount: RM " << fixed << setprecision(2) << total << endl;

    cout << "\n\n";
    for (int i = 0; i < 80; ++i) std::cout << ' ';
    cout << "-----Receipt------\n";
    cout << tableOrder;

    //cout << "\n\tProduct Name\t\tQuantity\n";
    //for (int i = 0; i < order.size(); i++) {
    //    cout << "\t" << order[i][0] << "\t\t" << order[i][2] << endl;
    //}

    char confirm;
    cout << "\n\n";
    do {
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "Confirm order? (Y/N)\n";
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << ">> ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            cust.insertOrder(conn, vendorID);
            order.clear();
            break;
        }
        else if (confirm == 'n' || confirm == 'N') {
            order.clear();
            break;
        }
        else {
            for (int i = 0; i < 80; ++i) std::cout << ' ';
            cout << "Invalid input. Try again.\n\n";
        }
    } while (confirm != 'y' && confirm != 'Y');
}

void viewPreviousOrder() {
    //TextTable tb;
    TableOrder tb;

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
        for (int i = 0; i < 75; ++i) std::cout << ' ';
        cout << "------Previous Order------\n\n";
        cout << tb << endl;

        cout << endl;
        for (int i = 0; i < 70; ++i) std::cout << ' ';
        cout << "Enter OrderID to view details (0 - back to Main Menu)";

        cout << endl;
        for (int i = 0; i < 72; ++i) std::cout << ' ';
        cout << ">> ";
        cin >> orderID;

        if (orderID == 0)
            break;
        else {
            mainHeader();
            cust.viewPreviousOrderDetails(conn, orderID, exist);
            if (exist == 0) {
                goto jump;
            }
            for (int i = 0; i < 67; ++i) std::cout << ' ';
            system("pause");
        }
    } while (orderID != 0);
}
void graphPlot(vector<vector<string>> data ) {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> normdist(0., 1.);

    std::vector<double> v0, v1, v3;

    v3.push_back(0);

    for (int i = 0; i < data.size(); i++) {
        if (data[i][2] == "1")
            v3.push_back(stoi(data[i][0]));
    }

    for (int i = 0; i < 1000; i++) {
        v0.push_back(normdist(mt));
        v1.push_back(normdist(mt));
    }

    std::partial_sum(v0.begin(), v0.end(), v0.begin());
    std::partial_sum(v1.begin(), v1.end(), v1.begin());

    gp << "set title 'Total Order Made per Month'\n";
    gp << "set xlabel 'Month'\n";
    gp << "set ylabel 'Total Order'\n";
    gp << "set xrange [0:12]\n";
    gp << "set xtics ('1' 1, '2' 2, '3' 3, '4' 4, '5' 5, '6' 6, '7' 7, '8' 8, '9' 9, '10' 10, '11' 11, '12' 12)\n";
    gp << "set yrange [0:10]\n";
    gp << "set ytics ('1' 1, '2' 2, '3' 3, '4' 4, '5' 5, '6' 6, '7' 7, '8' 8, '9' 9, '10' 10, '11' 11, '12' 12)\n";
    gp << "set style data histogram\n";
    gp << "plot '-' with lines title 'Total Order Made by Month'\n";

    //gp.send(v0);
    //gp.send(v1);
    gp.send(v3);

    std::cin.get();

    system("pause");
}

void orderMadeByMonth(vector<vector<string>> data, int year) {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    std::vector<int> v1;

    //v1.push_back(0);
    //v1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    v1 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //v1.push_back(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

            //v1.push_back(stoi(data[i][0]));

    for (int i = 0; i < data.size(); i++) {
        if (stoi(data[i][4]) == year) {
            if (data[i][3] == "1")
                v1[1] = stoi(data[i][0]);
            if (data[i][3] == "2")
                v1[2] = stoi(data[i][0]);
            if (data[i][3] == "3")
                v1[3] = stoi(data[i][0]);
            if (data[i][3] == "4")
                v1[4] = stoi(data[i][0]);
            if (data[i][3] == "5")
                v1[5] = stoi(data[i][0]);
            if (data[i][3] == "6")
                v1[6] = stoi(data[i][0]);
            if (data[i][3] == "7")
                v1[7] = stoi(data[i][0]);
            if (data[i][3] == "8")
                v1[8] = stoi(data[i][0]);
            if (data[i][3] == "9")
                v1[9] = stoi(data[i][0]);
            if (data[i][3] == "10")
                v1[10] = stoi(data[i][0]);
            if (data[i][3] == "11")
                v1[11] = stoi(data[i][0]);
            if (data[i][3] == "12")
                v1[12] = stoi(data[i][0]);
        }
    }

   // gp << "set title 'Total Order Made per Month'\n";
    gp << "set title 'Total Order Made By Month In " << year << "' \n";
    gp << "set xlabel 'Month'\n";
    gp << "set ylabel 'Total Order'\n";
    gp << "set xrange [0:12]\n";
    gp << "set xtics ('1' 1, '2' 2, '3' 3, '4' 4, '5' 5, '6' 6, '7' 7, '8' 8, '9' 9, '10' 10, '11' 11, '12' 12)\n";
    gp << "set yrange [0:10]\n";
    gp << "set ytics ('1' 1, '2' 2, '3' 3, '4' 4, '5' 5, '6' 6, '7' 7, '8' 8, '9' 9, '10' 10, '11' 11, '12' 12)\n";
    //gp << "plot '-' with lines title 'Total Order Made by Month'\n";
    gp << "plot '-' with lines title 'Total Order Made by Month in " << year << "'\n";

    gp.send(v1);

    std::cin.get();

    system("pause");
}

void viewCustomerExpenses() {
    mainHeader();

    //vendor.viewProduct(vendorID, totalProduct, totalVendor);
    TableOrder tt;

    tt.add("Month");
    tt.add("Total Order Made");
    tt.add("Total Quantity");
    tt.add("Total Expenses");
    tt.endOfRow();

    vector<string> data;
    vector<vector<string>> graph;

    stringstream ss;
    //ss << "SELECT COUNT(id) AS order_made, SUM(total_quantity) AS quantity, SUM(total_price) AS price FROM cust_order WHERE customer_id = GROUP BY customer_id";
    //ss << "SELECT COUNT(id) AS order_made, SUM(total_quantity) AS quantity, SUM(total_price) AS price FROM cust_order WHERE customer_id = 2 GROUP BY date";
    //ss << "SELECT COUNT(id) AS order_made , SUM(total_quantity) AS quantity, SUM(total_price) AS price, extract(month FROM date) AS month FROM cust_order WHERE customer_id = " << cust.getID() << " GROUP BY month";
    //ss << "SELECT COUNT(cust_order.id) AS order_made , SUM(cust_order.total_quantity) AS quantity, SUM(cust_order.total_price) AS price, extract(month FROM cust_order.date) AS month FROM cust_order JOIN delivery ON cust_order.id = delivery.payment_id WHERE cust_order.customer_id = " << cust.getID() << " AND delivery.status > 1 GROUP BY month";
    ss << "SELECT COUNT(cust_order.id) AS order_made , SUM(cust_order.total_quantity) AS quantity, SUM(payment.total_payment) AS price, extract(month FROM cust_order.date) AS month, extract(year FROM cust_order.date) AS year FROM cust_order JOIN delivery ON cust_order.id = delivery.payment_id JOIN payment ON cust_order.id = payment.order_id WHERE cust_order.customer_id = " << cust.getID() << " AND delivery.status > 1 GROUP BY month ORDER BY year";

    //cust.displayChart(conn);

    string qs = ss.str();
    const char* q = qs.c_str();
    int qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);

    while (row = mysql_fetch_row(res)) {
        // order made, quantity, expenses, month
        graph.push_back({ row[0], row[1], row[2], row[3], row[4] });
        stringstream date;
        date << row[3] << "/" << row[4];
        //tt.add(row[3]);
        tt.add(date.str());
        tt.add(row[0]);
        tt.add(row[1]);
        tt.add(row[2]);
        tt.endOfRow();
    }
    int year;


    do {
        mainHeader();
        cout << tt;
        for (int i = 0; i < 75; ++i) std::cout << ' ';
        cout << "View chart total order made by month. Enter 2021 or 2022\n";
        for (int i = 0; i < 75; ++i) std::cout << ' ';
        cout << ">> ";
        cin >> year;
        //if (year == 2021)
        if (year != 2021 && year != 2022) {
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            cout << "Invalid choice\n";
            system("pause");
        }
        else if (year == 0)
            break;
        else {
            orderMadeByMonth(graph, year);
            break;
        }

    } while (year != 0);
}

void viewBarChart() {
    TableOrder tt;

    tt.add("Month");
    tt.add("Total Order Made");
    tt.add("Total Quantity");
    tt.add("Total Expenses");
    tt.endOfRow();

    vector<string> data;
    vector<vector<string>> graph;

    stringstream ss;
    ss << "SELECT COUNT(cust_order.id) AS order_made , SUM(cust_order.total_quantity) AS quantity, SUM(cust_order.total_price) AS price, extract(month FROM cust_order.date) AS month FROM cust_order JOIN delivery ON cust_order.id = delivery.payment_id WHERE cust_order.customer_id = " << cust.getID() << " AND delivery.status > 1 GROUP BY month";

    string qs = ss.str();
    const char* q = qs.c_str();
    int qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);

    while (row = mysql_fetch_row(res)) {
        // order made, quantity, expenses, month
        graph.push_back({ row[0], row[1], row[2], row[3] });
        tt.add(row[3]);
        tt.add(row[0]);
        tt.add(row[1]);
        tt.add(row[2]);
        tt.endOfRow();
    }

    int array[5] = { 1, 3, 5, 7, 9 };

    int highest = array[0];

    for (int i = 0; i <= 5; i++) {
        if (array[i] > highest)
            highest = array[i];
    }

    for (int row = highest; row >= 1; row--) {
        for (int col = 0; col <= 5; col++) {
            if (array[col] >= row)
                cout << "*  ";
            else
                cout << "   ";
        }

        cout << endl;
    }

    for (int col = 0; col < 5; col++)
        cout << "---";
    cout << endl;
    for (int col = 1; col < 6; col++)
        cout << col << "  ";
    cout << endl;

}

// Rider
void viewSelectOrder() {
    TableOrder tb;

    vector<vector<string>> order;

    MYSQL_ROW row;
    MYSQL_RES* res;

    int i = 0;

    tb.add("Order ID");
    tb.add("Quantity");
    tb.add("Price (RM)");
    tb.add("Order Created");
   // tb.add("Status");
    tb.endOfRow();

    stringstream sql;
   // sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order.customer_id = " << this->custID << " GROUP BY cust_order.id";
   // sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE delivery.status >= 1 GROUP BY cust_order.id";
    sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE delivery.status = 1 GROUP BY cust_order.id";
    string s = sql.str();
    const char* qC = s.c_str();
    int q = mysql_query(conn, qC);
    if (!q) {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res)) {
            string orderID = row[1];
            string quantity = row[7];
            string price = row[12];
            string date = row[9];
            string riderID;
            if (row[15] == NULL) {
                riderID = "Not Set";
            }
            else {
                riderID = row[15];
            }
            string status = row[16];
            ///prev_order.push_back({ row[1], row[5], row[7], row[10], row[13], row[14] });
            order.push_back({ orderID, quantity, price, date, riderID, status });
            rider.getOrder();
            tb.add(orderID);
            tb.add(quantity);
            tb.add(price);
            tb.add(date);
            /*
            if (status == "0")
                tb.add("Pending");
            else if (status == "-1")
                tb.add("Rejected"); // after vendor reject the order
            else if (status == "1")
                tb.add("Accepted"); // after vendor accept the order
            else if (status == "2")
                tb.add("In Delivery"); // after rider has pick up the order. rider only can view the list of order with accepted value (3)
            else if (status == "3")
                tb.add("Delivered"); // rider change the status after has delivered the order
            */
            tb.endOfRow();
            //cout << prev_order[i][0] << endl << prev_order[i][1] << endl << prev_order[i][4] << endl;
            i++;
        }
        rider.fetchOrder(order);
    }

    int orderID;
    int exist = 0;

    string checkOrder = "SELECT payment_id FROM delivery WHERE rider_id = " + to_string(rider.getID()) + " AND status = 2";
    const char* co = checkOrder.c_str();
    int qo = mysql_query(conn, co);

    MYSQL_RES* result = mysql_store_result(conn);
    //MYSQL_ROW r = mysql_fetch_row(result);

    int currentOrderID;
    
    do {
    jump:;
        mainHeader();
        gotoXY(10, 13);

        // CONDITION! if rider already select order to delivery, cannot accept any until it has been delivered
        if (MYSQL_ROW r = mysql_fetch_row(result)) {
            currentOrderID = stoi(r[0]);
            cout << endl;
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            cout << "You have an active order for delivery.\n\n";
            // display address sekali
            rider.viewOrderDetails(conn, currentOrderID);
            rider.updateDeliveryStatus(conn, currentOrderID);
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            system("pause");
            break;
        }

        else {
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            cout << "------Current Active Order------\n\n";
            cout << tb << endl;

            cout << endl;
            for (int i = 0; i < 70; ++i) std::cout << ' ';
            cout << "Enter OrderID to view details (0 - back to Main Menu)";

            cout << endl;
            for (int i = 0; i < 72; ++i) std::cout << ' ';
            cout << ">> ";
            cin >> orderID;

            if (orderID == 0)
                break;
            else {
                mainHeader();
                rider.selectOrderDetails(conn, orderID, exist);
                if (exist == 0) {
                    goto jump;
                }
                else {
                    // update order status
                    char selUp;
                    for (int i = 0; i < 67; ++i) std::cout << ' ';
                    cout << "Enter (A) to accept or (R) to reject.\n";
                    for (int i = 0; i < 67; ++i) std::cout << ' ';
                    cout << ">> ";
                    cin >> selUp;
                    if (selUp == 'a' || selUp == 'A') {
                        string update;
                        update = "UPDATE delivery SET status = " + to_string(2) + ", rider_id = " + to_string(rider.getID()) + " WHERE payment_id = " + to_string(orderID);

                        const char* q = update.c_str();
                        int qstate = mysql_query(conn, q);

                        if (!qstate) {
                            for (int i = 0; i < 67; ++i) std::cout << ' ';
                            cout << "Order has been accepted for delivery.\n";
                            for (int i = 0; i < 67; ++i) std::cout << ' ';
                            system("pause");
                            break;
                        }
                        else {
                            for (int i = 0; i < 67; ++i) std::cout << ' ';
                            cout << "Update Failed!\n";
                            system("pause");
                        }
                    }
                }
                for (int i = 0; i < 67; ++i) std::cout << ' ';
                system("pause");
            }
        }
    } while (orderID != 0);
}

void viewPastDelivery() {
    TableOrder tb;

    vector<vector<string>> order;

    MYSQL_ROW row;
    MYSQL_RES* res;

    int i = 0;

    tb.add("Order ID");
    tb.add("Quantity");
    tb.add("Price (RM)");
    tb.add("Order Created");
    // tb.add("Status");
    tb.endOfRow();

    stringstream sql;
    // sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order.customer_id = " << this->custID << " GROUP BY cust_order.id";
    // sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE delivery.status >= 1 GROUP BY cust_order.id";
    sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE delivery.status = 3 AND delivery.rider_id = " + to_string(rider.getID()) + " GROUP BY cust_order.id";
    string s = sql.str();
    const char* qC = s.c_str();
    int q = mysql_query(conn, qC);
    if (!q) {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res)) {
            string orderID = row[1];
            string quantity = row[7];
            string price = row[12];
            string date = row[9];
            string riderID;
            if (row[15] == NULL) {
                riderID = "Not Set";
            }
            else {
                riderID = row[15];
            }
            string status = row[16];
            ///prev_order.push_back({ row[1], row[5], row[7], row[10], row[13], row[14] });
            order.push_back({ orderID, quantity, price, date, riderID, status });
            rider.getOrder();
            tb.add(orderID);
            tb.add(quantity);
            tb.add(price);
            tb.add(date);
            /*
            if (status == "0")
                tb.add("Pending");
            else if (status == "-1")
                tb.add("Rejected"); // after vendor reject the order
            else if (status == "1")
                tb.add("Accepted"); // after vendor accept the order
            else if (status == "2")
                tb.add("In Delivery"); // after rider has pick up the order. rider only can view the list of order with accepted value (3)
            else if (status == "3")
                tb.add("Delivered"); // rider change the status after has delivered the order
            */
            tb.endOfRow();
            //cout << prev_order[i][0] << endl << prev_order[i][1] << endl << prev_order[i][4] << endl;
            i++;
        }
        rider.fetchOrder(order);
    }

    int orderID;
    int exist = 0;

    do {
    jump:;
        mainHeader();
        gotoXY(10, 13);

            for (int i = 0; i < 75; ++i) std::cout << ' ';
            cout << "------Past Delivery History------\n\n";
            cout << tb << endl;
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            //system("pause");

            cout << endl;
            for (int i = 0; i < 70; ++i) std::cout << ' ';
            cout << "Enter OrderID to view details (0 - back to Main Menu)";

            cout << endl;
            for (int i = 0; i < 72; ++i) std::cout << ' ';
            cout << ">> ";
            cin >> orderID;

            if (orderID == 0)
                break;
            else {
                mainHeader();
                rider.selectOrderDetails(conn, orderID, exist);
                if (exist == 0) {
                    goto jump;
                }
                for (int i = 0; i < 67; ++i) std::cout << ' ';
                system("pause");
            }

    } while (orderID != 0);
}

void orderDeliveredByMonth(vector<vector<string>> data, int year) {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    vector<int> v1;

    v1 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    for (int i = 0; i < data.size(); i++) {
        if (stoi(data[i][4]) == year) {
            if (data[i][3] == "1")
                v1[1] = stoi(data[i][0]);
            if (data[i][3] == "2")
                v1[2] = stoi(data[i][0]);
            if (data[i][3] == "3")
                v1[3] = stoi(data[i][0]);
            if (data[i][3] == "4")
                v1[4] = stoi(data[i][0]);
            if (data[i][3] == "5")
                v1[5] = stoi(data[i][0]);
            if (data[i][3] == "6")
                v1[6] = stoi(data[i][0]);
            if (data[i][3] == "7")
                v1[7] = stoi(data[i][0]);
            if (data[i][3] == "8")
                v1[8] = stoi(data[i][0]);
            if (data[i][3] == "9")
                v1[9] = stoi(data[i][0]);
            if (data[i][3] == "10")
                v1[10] = stoi(data[i][0]);
            if (data[i][3] == "11")
                v1[11] = stoi(data[i][0]);
            if (data[i][3] == "12")
                v1[12] = stoi(data[i][0]);
        }
    }

    gp << "set title 'Total Order Delivered By Month In " << year << "' \n";
    gp << "set xlabel 'Month'\n";
    gp << "set ylabel 'Total Order Delivered'\n";
    gp << "set xrange [0:12]\n";
    gp << "set xtics ('1' 1, '2' 2, '3' 3, '4' 4, '5' 5, '6' 6, '7' 7, '8' 8, '9' 9, '10' 10, '11' 11, '12' 12)\n";
    gp << "set yrange [0:10]\n";
    gp << "set ytics ('1' 1, '2' 2, '3' 3, '4' 4, '5' 5, '6' 6, '7' 7, '8' 8, '9' 9, '10' 10, '11' 11, '12' 12)\n";
    gp << "plot '-' with lines\n";

    gp.send(v1);

    std::cin.get();

    system("pause");
}

void viewRiderReport() {

    mainHeader();

    TableCart tt;

    tt.add("Month");
    tt.add("Total Order Delivered");
    tt.add("Total Earning (RM)");
    tt.endOfRow();

    vector<vector<string>> graph;

    stringstream ss;
    ss << "SELECT COUNT(cust_order.id) AS order_get, COUNT(cust_order.id) * 4, extract(month FROM cust_order.date) AS month, extract(year FROM cust_order.date) AS year FROM cust_order JOIN delivery ON cust_order.id = delivery.payment_id JOIN payment ON payment.order_id = delivery.payment_id WHERE delivery.rider_id = " << rider.getID() << " AND delivery.status = 3 GROUP BY month ORDER BY year; ";

    string qs = ss.str();
    const char* q = qs.c_str();
    int qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);

    while (row = mysql_fetch_row(res)) {
        // order made, quantity, expenses, month
        graph.push_back({ row[0], row[1], " ", row[2], row[3]});
        stringstream date;
        date << row[2] << "/" << row[3];
        //tt.add(row[3]);
        tt.add(date.str());
        tt.add(row[0]);
        tt.add(row[1]);
       // tt.add(row[2]);
        tt.endOfRow();
    }


    int year;
    do {
        mainHeader();
        cout << tt;
        for (int i = 0; i < 75; ++i) std::cout << ' ';
        cout << "View chart total order processed by month. Enter 2021 or 2022\n";
        for (int i = 0; i < 75; ++i) std::cout << ' ';
        cout << ">> ";
        cin >> year;
        //if (year == 2021)
        if (year != 2021 && year != 2022) {
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            cout << "Invalid choice\n";
            system("pause");
        }
        else if (year == 0)
            break;
        else {
            orderDeliveredByMonth(graph, year);
            break;
        }

    } while (year != 0);
}

// Admin
void viewManage(int role) {
    // view in table
    // - name, phone num, address


    MYSQL_RES* result;
    MYSQL_ROW r;
    TableOrder tt;

    string sql, title;

    if (role == 1) {
        sql = "SELECT name, phone, address FROM vendor";
        title = "Vendor";
    }
    else if (role == 2) {
        sql = "SELECT name, phone, address FROM customer";
        title = "Customr";
    }
    else if (role == 3) {
        sql = "SELECT name, phone FROM rider";
        title = "Rider";
    }

    /*
    string customer = "SELECT name, phone, address FROM customer";
    string vendor = "SELECT name, phone, address FROM vendor";
    string rider = "SELECT name, phone, address FROM rider";
    */

    tt.add("No.");
    tt.add("Name");
    tt.add("No. Phone");
    if (role != 3)
        tt.add("Address");
    tt.endOfRow();

    int count = 0;

    const char* qS = sql.c_str();
    int q = mysql_query(conn, qS);
    if (!q) {
        result = mysql_store_result(conn);
        while (r = mysql_fetch_row(result)) {
            tt.add(to_string(++count));
            tt.add(r[0]);
            tt.add(r[1]);
            if (role != 3) {
                if (r[2] == NULL)
                    tt.add("Not Available");
                else
                    tt.add(r[2]);
            }
            tt.endOfRow();
        }

        mainHeader();
        for (int i = 0; i < 80; ++i) std::cout << ' ';
        cout << "Manage " << title << endl << endl;
        cout << tt;
        for (int i = 0; i < 67; ++i) std::cout << ' ';
        system("pause");
    }

    else {
        cout << "failed";
    }
}

void plotGraph() {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> normdist(0., 1.);

    std::vector<double> v0, v1;

    for (int i = 0; i < 1000; i++) {
        v0.push_back(normdist(mt));
        v1.push_back(normdist(mt));
    }

    std::partial_sum(v0.begin(), v0.end(), v0.begin());
    std::partial_sum(v1.begin(), v1.end(), v1.begin());

    gp << "set title 'Graph of two random lines'\n";
    gp << "plot '-' with lines title 'v0'," << "'-' with lines title 'v1'\n";

    gp.send(v0);
    gp.send(v1);

    std::cin.get();

    system("pause");
}
#endif