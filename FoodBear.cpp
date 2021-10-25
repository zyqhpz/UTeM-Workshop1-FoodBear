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
#include "FoodBear.h"

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

    int operation;

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
                    if (loginUser(chooseLogin)) { // Vendor
                        do {
                            mainHeader();
                            cout << "\n---Welcome " << vendor.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            cout << "Enter 1-View products, 2-View sales report, 3-View all order, 4-View profile 0-Logout\n";
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 4) {
                                vendor.viewProfile(mainHeader, conn);
                            }
                            else if (operation == 0) {
                                break;
                            }
                        } while (operation != 0);
                    }
                    else {
                        cout << "failed login\n";
                        system("pause");
                    }
                }
                else if (chooseLogin == 2) { // Customer
                    if (loginUser(chooseLogin)) {
                        do {
                            mainHeader();
                            cout << "\n---Welcome " << cust.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            cout << "Enter 1-Start food order, 2-View cart, 3-View previous order, 4-View profile, 0-Logout\n";
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 4) {
                                cust.viewProfile(mainHeader, conn);
                            }
                            else if (operation == 0) {
                                break;
                            }
                        } while (operation != 0);
                    }
                    else {
                        cout << "failed login\n";
                        system("pause");
                    }
                }
                else if (chooseLogin == 3) { // Rider
                    if (loginUser(chooseLogin)) {
                        do {
                            mainHeader();
                            cout << "\n---Welcome " << rider.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            cout << "Enter 1-Start food delivery, 2-View past delivery, 3-View earning report, 4-View profile, 0-Logout\n";
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 4) {
                                rider.viewProfile(mainHeader, conn);
                            }
                            else if (operation == 0) {
                                break;
                            }
                        } while (operation != 0);
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