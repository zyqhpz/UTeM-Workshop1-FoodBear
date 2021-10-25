#include <iostream>
#include <iomanip>
#include <windows.h>

#include <sstream>

#include <conio.h>

#include <mysql.h>

#include <winsqlite/winsqlite3.h>

#include "Customer.h"
#include "Vendor.h"
#include "Rider.h"
#include "Admin.h"

#include "utils.h"

using namespace std;

// Global variable
MYSQL* conn;

MYSQL_ROW row;
MYSQL_RES* res;

void displayMainMenu() {
    mainHeader();

    cout << "\nWelcome to FoodBear Delivery\nNow all on fingertips\n\n";

    cout << "\n--Main--\n";
    cout << "1-Login\n2-Register\n0-Exit\n";
}

int main()
{
    system("Connection To DB");

    conn = db_connection::ConnectionFunction();

    int chooseMain;
    int chooseLogin;
    int chooseRegister;

    int totalCustomer = 0;
    int totalRider = 0;
    int* t = &totalCustomer;
    int totalVendor = 0;

    Customer cust;
    Vendor vendor;
    Rider rider;
    Admin admin;

    do {
        displayMainMenu();
        cout << "\n>> ";
        cin >> chooseMain;

        if (chooseMain == 1) {
            do {
                mainHeader();
                cout << "---Login---\n";
                cout << "Enter 1-Vendor, 2-Customer, 3-Rider, 0-Back to Main Menu\n";
                cout << ">> ";
                cin >> chooseLogin;

                if (chooseLogin == 1) {
                    mainHeader();
                    string sql = "SELECT * FROM vendor";
                    string r = "SELECT COUNT(*) FROM vendor";
                    const char* q = sql.c_str();
                    int qstate = mysql_query(conn, q);

                    if (!qstate) {
                        res = mysql_store_result(conn);
                        int count = mysql_num_fields(res);
                        totalVendor = vendor.fetchData(res, count);
                        cout << "fetched" << endl;

                        string user, pass;
                        cout << "\n----Login----\n";
                        cout << "Username: ";
                        cin >> user;
                        cout << "Password: ";

                        pass = inputPassword();

                        cout << pass << endl;

                        if (vendor.login(user, pass)) {
                            cout << "Welcome " << vendor.getName() << endl;
                            system("pause");
                        }
                        else {
                            cout << "Failed login" << endl;
                            system("pause");
                        }
                    }
                    else {
                        cout << "failed to fetch";
                    }
                }
                else if (chooseLogin == 2) {
                    mainHeader();
                    string sql = "SELECT * FROM customer";
                    const char* q = sql.c_str();
                    int qstate = mysql_query(conn, q);

                    if (!qstate) {
                        res = mysql_store_result(conn);
                        int count = mysql_num_fields(res);
                        totalCustomer = cust.fetchData(res, count);
                        cout << "fetched " << totalCustomer << endl;

                        string user, pass;
                        cout << "\n----Login----\n";
                        cout << "Username: ";
                        cin >> user;
                        cout << "Password: ";

                        pass = inputPassword();

                        cout << pass << endl;

                        if (cust.login(user, pass, totalCustomer)) {
                            cout << "Welcome " << cust.getName() << endl;
                            system("pause");
                        }
                        else {
                            cout << "Failed login" << endl;
                            system("pause");
                        }
                    }
                    else {
                        cout << "failed to fetch";
                    }
                }
                else if (chooseLogin == 3) {
                    mainHeader();
                    string sql = "SELECT * FROM rider";
                    const char* q = sql.c_str();
                    int qstate = mysql_query(conn, q);

                    if (!qstate) {
                        res = mysql_store_result(conn);
                        int count = mysql_num_fields(res);
                        totalRider = rider.fetchData(res, count);
                        cout << "fetched " << totalRider << " data " << endl;

                        string user, pass;
                        cout << "\n----Login----\n";
                        cout << "Username: ";
                        cin >> user;
                        cout << "Password: ";

                        pass = inputPassword();

                        cout << pass << endl;

                        if (rider.login(user, pass, totalRider)) {
                            cout << "Welcome " << rider.getName() << endl;
                            system("pause");
                        }
                        else {
                            cout << "Failed login" << endl;
                            system("pause");
                        }
                    }
                    else {
                        cout << "failed to fetch";
                    }
                }
                else if (chooseLogin == 0) {
                }
            } while (chooseLogin != 0);
        } 
        else if (chooseMain == 2) {
            do {
                mainHeader();

                cout << "--Registration--\n";
                cout << "Enter 1-Vendor, 2-Customer, 3-Rider, 0-Back to Main Menu\n";
                cout << ">> ";
                cin >> chooseRegister;

                if (chooseRegister == 1) {
                    mainHeader();
                    vendor.registerVendor(conn);
                }
                else if (chooseRegister == 2) {
                    mainHeader();
                    cust.registerCustomer(conn);
                }
                else if (chooseRegister == 3) {
                    mainHeader();
                    rider.registerRider(conn);
                    break;
                }
                else if (chooseRegister == 0) {

                }
            } while (chooseRegister != 0);
        }
        else if (chooseMain == 0) {
            cout << "Exiting..\n";
            break;
        }
        else if (chooseMain > 2 || chooseMain < 0) {
            cout << "Error! Wrong choice\n";
        }
        else {}
    } while (chooseMain != 0);
}