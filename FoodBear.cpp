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
#include "Menu.h"

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/component/component.hpp"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
//#include <jdbc/cppconn/*.h>


using namespace std;

void displayMainMenu() {
    mainHeader();

    //cout << "\nWelcome to FoodBear Delivery\nNow all on fingertips\n\n";
    cout << "\nWelcome to FoodBear Delivery\n";

    cout << "\n--Main--\n";
    cout << "1-Login\n2-Register\n0-Exit\n";
}

int main()
{
    system("Connection To DB");

    db_connection::ConnectionFunction();
    fetchAllData();

    std::string str = "Hello";
    std::wstring str2(str.length(), L' '); // Make room for characters

    // Copy string to wstring.
    std::copy(str.begin(), str.end(), str2.begin());

    //text(str2);

    int chooseMain;
    int chooseLogin;
    int chooseRegister;

    int operation;

    do {
        displayMainMenu();
        //select(); from menu.h
        cout << "\n>> ";
        cin >> chooseMain;

        int role;

        if (chooseMain == 1) {
            do {
                mainHeader();
                //cout << "---Login---\n";
                //cout << "Enter\n\t1-Vendor\n\t2-Customer\n\t3-Rider\n\t0-Back to Main Menu\n";
                //cout << ">> ";
                //cin >> chooseLogin;
                
                role = login();

                if (role == 1) {
                    if (role == 1) { // Vendor
                        do {
                            fetchAllData();
                            mainHeader();
                            cout << "\n---Welcome " << vendor.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            //cout << "Enter 1-View products, 2-View sales report, 3-View all order, 4-View profile 0-Logout\n";
                            cout << "Enter\n\t1-Manage products\n\t2-View sales report\n\t3-View all order\n\t4-View profile\n\t0-Logout\n";
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 1) {
                                //vendor.viewProduct(vendor.)
                                //vendor.addProduct();
                                viewAddProduct();
                                system("pause");
                            }

                            else if (operation == 4) {
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
                else if (role == 2) { // Customer
                    if (role == 2) {
                        do {
                            fetchAllData();
                            mainHeader();
                            cout << "\n---Welcome " << cust.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            cout << "Enter\n\t1-View foods\n\t2-View order\n\t4-View profile\n\t0-Logout\n";
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 1) {
                                viewVendorList();
                                system("pause");
                            }
                            else if (operation == 2) {
                                viewPreviousOrder();
                                system("pause");
                            }
                            else if (operation == 4) {
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
                else if (role == 3) { // Rider
                    if (role == 3) {
                        do {
                            fetchAllData();
                            mainHeader();
                            cout << "\n---Welcome " << rider.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            cout << "Enter 1-Start food delivery, 2-View past delivery, 3-View earning report, 4-View profile, 0-Logout\n";
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 1) {
                            }
                            else if (operation == 4) {
                                rider.viewProfile(mainHeader, conn);
                            }
                            else if (operation == 0) {
                                break;
                            }
                        } while (operation != 0);
                    }
                    else {
                        cout << "Login Failed\n";
                        system("pause");
                    }
                }
                else if (role == 0) {
                    cout << "\nLogin Failed\n";
                    system("pause");
                    break;
                }
            } while (role == 0);
        } 
        else if (chooseMain == 2) {
            do {
                mainHeader();

                cout << "--Registration--\n";
                cout << "Enter: \n\t1-Vendor\n\t2-Customer\n\t3-Rider\n\t0-Back to Main Menu\n";
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