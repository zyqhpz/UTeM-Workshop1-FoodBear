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

void displayMainMenu() {
    mainHeader();

    cout << "\nWelcome to FoodBear Delivery\nNow all on fingertips\n\n";

    cout << "\n--Main--\n";
    cout << "1-Login\n2-Register\n0-Exit\n";
}

int main()
{
    system("Connection To DB");

    db_connection::ConnectionFunction();
    fetchAllData();

    int chooseMain;
    int chooseLogin;
    int chooseRegister;

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
                    if (loginUser(chooseLogin)) {
                        cout << "Welcome " << vendor.getName() << endl;
                        system("pause");
                    }
                    else {
                        cout << "failed login\n";
                        system("pause");
                    }
                }
                else if (chooseLogin == 2) {
                    if (loginUser(chooseLogin)) {
                        cout << "Welcome " << cust.getName() << endl;
                        system("pause");
                    }
                    else {
                        cout << "failed login\n";
                        system("pause");
                    }
                }
                else if (chooseLogin == 3) {
                    if (loginUser(chooseLogin)) {
                        cout << "Welcome " << rider.getName() << endl;
                        system("pause");
                    }
                    else {
                        cout << "failed login\n";
                        system("pause");
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