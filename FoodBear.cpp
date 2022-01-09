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
//#include "External.h"

/*
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
*/

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
//#include <tabulate/table.hpp>
//#include <jdbc/cppconn/*.h>

using namespace tabulate;
using namespace std;

//using namespace ftxui;

void displayMainMenu() {
    mainHeader();

    //cout << "\nWelcome to FoodBear Delivery\nNow all on fingertips\n\n";
    //COORD c;
    //SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    //GetStdHandle();
    //GetStdHandle(STD_OUTPUT_HANDLE);
    vector<string> title = {
        "Welcome to FoodBear Delivery",
        " ",
        "----Main----",
        "1-Login",
        "2-Register",
        "0-Exit",
    };

    int num_cols = 200;

    // VIRTUAL BORDER
    //gotoXY(0, 3);
    std::cout << std::endl;
    for (int i = 0; i < num_cols; ++i) std::cout << ' ';
    //std::cout << ' ' << std::endl;
    std::cout << ' ' << std::endl;

    // OUTPUT
    for (int i = 0; i < title.size(); ++i) {
        centerify_output(title[i], num_cols);
        std::cout << std::endl;
    }

    // VIRTUAL BORDER
    std::cout << std::endl;
    for (int i = 0; i < num_cols; ++i) std::cout << ' ';
    //std::cout << '|' << std::endl;
    std::cout << ' ' << std::endl;

    /*
    
    
    Table table;

    table.add_row({ "This paragraph contains a veryveryveryveryveryverylong word. The long word will "
                   "break and word wrap to the next line.", " ",
                   "This paragraph \nhas embedded '\\n' \ncharacters and\n will break\n exactly "
                   "where\n you want it\n to\n break.", " "});
    table.add_row({ "This paragraph contains a veryveryveryveryveryverylong word. The long word will "
                   "break and word wrap to the next line.", " ", " ",
                   "This paragraph \nhas embedded '\\n' \ncharacters and\n will break\n exactly "
                   "where\n you want it\n to\n break." });
    table.add_row({" ", "Column1", "Column2", " "});

    table[0][0].format().width(20);
    table[0][1].format().width(30);
    table[0][2].format().width(30);
    table[0][3].format().width(30);
    */
    //for (int i = 0; i < num_cols; ++i) std::cout << ' ';
    //std::cout << "     "  << table << std::endl;

    //for (int i = 0; i < cols; i++) {
      //  cout << "       " << table;
    //}

   // cout << table << endl;

    //cout << "\n\t\t\tWelcome to FoodBear Delivery\n\n";
    //unsigned int padding = 40 + (sname.size() / 2);
    //cout << setw(padding) << sname;

    //centerify_output("\n--Main--\n", 100);
    //for (int i = 0; i < 50; ++i) std::cout << ' ';
    //cout << "--Main--\n";
    //cout << "1-Login\n2-Register\n0-Exit\n";
}

void userUI(int role) {

    /*
    
    for (int i = 0; i < 75; ++i) std::cout << ' ';
    cout << "---Welcome " << vendor.getName() << "---\n" << endl;

    cout << "---Operation---\n";
    //cout << "Enter 1-View products, 2-View sales report, 3-View all order, 4-View profile 0-Logout\n";
    cout << "Enter\n\t1-Manage products\n\t2-View active order\n\t3-View all order\n\t4-View sales report\n\t5-View profile\n\t0-Logout\n";
    cout << ">> ";
    */

    vector<string> title;

    // Vendor
    if (role == 1) {
        title = {
           "--- Welcome " + vendor.getName() + " ---",
           " ",
           "1-Manage products",
           "2-View active order",
           "3-View all order",
           "4-View sales report",
           "5-View profile",
           "0-Logout",
        };
    }

    // Customer
    else if (role == 2) {
        title = {
          "--- Welcome " + cust.getName() + " ---",
          " ",
          "1-View foods",
          "2-View order",
          "3-View total expenses",
          "4-View profile",
          "0-Logout",
        };
    }

    // Rider
    else if (role == 3) {
        title = {
          "--- Welcome " + rider.getName() + " ---",
          " ",
          "1-Start food delivery",
          "2-View past delivery",
          "3-View total earning",
          "4-View profile",
          "0-Logout",
        };
    }
    
    // Admin
    else if (role == 4) {
        title = {
         "--- Welcome " + admin.getName() + " ---",
         " ",
         "1-Manage Vendor",
         "2-Manage Customer",
         "3-Manage Rider",
         "0-Logout",
        };
    }

    int num_cols = 200;

    // VIRTUAL BORDER
    //gotoXY(0, 3);
    std::cout << std::endl;
    for (int i = 0; i < num_cols; ++i) std::cout << ' ';
    //std::cout << ' ' << std::endl;
    std::cout << ' ' << std::endl;

    // OUTPUT
    for (int i = 0; i < title.size(); ++i) {
        centerify_output(title[i], num_cols);
        std::cout << std::endl;
    }

    // VIRTUAL BORDER
    std::cout << std::endl;
    for (int i = 0; i < num_cols; ++i) std::cout << ' ';
    //std::cout << '|' << std::endl;
    std::cout << ' ' << std::endl;
}

int main()
{
   // text("I am a piece of text");
    //border(gauge(0.5));
    system("Connection To DB");

    db_connection::ConnectionFunction();

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
        fetchAllData();
        displayMainMenu();

    viewBarChart();
        //select(); from menu.h
        for (int i = 0; i < 75; ++i) std::cout << ' ';
        cout << ">> ";
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

                            /*
                            
                            for (int i = 0; i < 75; ++i) std::cout << ' ';
                            cout << "---Welcome " << vendor.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            //cout << "Enter 1-View products, 2-View sales report, 3-View all order, 4-View profile 0-Logout\n";
                            cout << "Enter\n\t1-Manage products\n\t2-View active order\n\t3-View all order\n\t4-View sales report\n\t5-View profile\n\t0-Logout\n";
                            */
                            userUI(role);
                            for (int i = 0; i < 75; ++i) std::cout << ' ';
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 1) {
                                //vendor.viewProduct(vendor.)
                                //vendor.addProduct();
                                viewManageProduct();
                                system("pause");
                            }
                            else if (operation == 2) {
                                viewActiveOrder();
                                //system("pause");
                            }
                            else if (operation == 3) {
                                viewAllOrder();

                            }
                            else if (operation == 4) {

                            }
                            else if (operation == 5) {
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
                            /*
                            for (int i = 0; i < 75; ++i) std::cout << ' ';
                            cout << "---Welcome " << cust.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            cout << "Enter\n\t1-View foods\n\t2-View order\n\t3-View total expenses\n\t4-View profile\n\t0-Logout\n";
                            */
                            userUI(role);
                            for (int i = 0; i < 75; ++i) std::cout << ' ';
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 1) {
                                //viewVendorList();
                                orderBy();
                                //system("pause");
                            }
                            else if (operation == 2) {
                                viewPreviousOrder();
                                //system("pause");
                            }
                            else if (operation == 3) {
                               // viewBarChart();
                                viewCustomerExpenses();
                               // plotGraph();
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
                        for (int i = 0; i < 75; ++i) std::cout << ' ';
                        cout << "failed login\n";
                        system("pause");
                    }
                }
                else if (role == 3) { // Rider
                    if (role == 3) {
                        do {
                            fetchAllData();
                            mainHeader();
                            /*
                            cout << "\n---Welcome " << rider.getName() << "---\n" << endl;

                            cout << "---Operation---\n";
                            cout << "Enter 1-Start food delivery, 2-View past delivery, 3-View total earning, 4-View profile, 0-Logout\n";
                            */
                            userUI(role);
                            for (int i = 0; i < 75; ++i) std::cout << ' ';
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 1) {
                                viewSelectOrder();
                            }
                            else if (operation == 2) {
                                viewPastDelivery();
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
                else if (role == 4) { // Admin
                    if (role == 4) {
                        do {
                            fetchAllData();
                            mainHeader();
                            userUI(role);
                            for (int i = 0; i < 75; ++i) std::cout << ' ';
                            cout << ">> ";
                            cin >> operation;

                            if (operation == 1) {
                                viewManage(1);
                            }
                            else if (operation == 2) {
                                viewManage(2);
                            }
                            else if (operation == 3) {
                                viewManage(3);
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
                    cout << endl;
                    for (int i = 0; i < 67; ++i) std::cout << ' ';
                    cout << "Login Failed\n";
                    for (int i = 0; i < 67; ++i) std::cout << ' ';
                    system("pause");
                    break;
                }
            } while (role == 0);
        } 
        else if (chooseMain == 2) {
            do {
                mainHeader();

                vector<string> reg = {
                    "----Registration----",
                    " ",
                    "1-Vendor",
                    "2-Customer",
                    "3-Rider",
                    "0-Back to Main Menu",
                };

                int num_cols = 200;

                // VIRTUAL BORDER
                //gotoXY(0, 3);
                std::cout << std::endl;
                for (int i = 0; i < num_cols; ++i) std::cout << ' ';
                //std::cout << ' ' << std::endl;
                std::cout << ' ' << std::endl;

                // OUTPUT
                for (int i = 0; i < reg.size(); ++i) {
                    centerify_output(reg[i], num_cols);
                    std::cout << std::endl;
                }

                // VIRTUAL BORDER
                std::cout << std::endl;
                for (int i = 0; i < num_cols; ++i) std::cout << ' ';
                //std::cout << '|' << std::endl;
                std::cout << ' ' << std::endl;

               // cout << "--Registration--\n";
               // cout << "Enter: \n\t1-Vendor\n\t2-Customer\n\t3-Rider\n\t0-Back to Main Menu\n";
                for (int i = 0; i < 75; ++i) std::cout << ' ';
                cout << ">> ";
                cin >> chooseRegister;

                if (chooseRegister == 1) {
                    mainHeader();
                    vendor.registerVendor(conn);
                    break;
                }
                else if (chooseRegister == 2) {
                    mainHeader();
                    cust.registerCustomer(conn);
                    break;
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
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            cout << "Exiting..\n";
            break;
        }
        else if (chooseMain > 2 || chooseMain < 0) {
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            cout << "Error! Wrong choice\n";
            for (int i = 0; i < 75; ++i) std::cout << ' ';
            system("pause");
        }
        else {}
    } while (chooseMain != 0);
}