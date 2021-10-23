#include <iostream>
#include <iomanip>
#include <windows.h>

#include <sstream>

#include <conio.h>

#include <mysql.h>

#include "Customer.h"

using namespace std;

// Global variable
MYSQL* conn;

MYSQL_ROW row;
MYSQL_RES* res;

inline void mainHeader(std::string additional = "") {
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

    if (additional != "") {
        cout << additional << endl;
    }
}

class db_response {
public:
    static void ConnectionFunction() {
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
    }
};

// Enumerator
enum IN {

    // 13 is ASCII for carriage
    // return
    IN_BACK = 8,
    IN_RET = 13

};

// Function that accepts the password
std::string takePasswdFromUser(
    char sp = '*')
{
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
        else if (ch_ipt == IN::IN_BACK
            && passwd.length() != 0) {
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
        else if (ch_ipt == IN::IN_BACK
            && passwd.length() == 0) {
            continue;
        }

        passwd.push_back(ch_ipt);
        cout << sp;
    }
}

void displayMainMenu() {
    mainHeader();

    cout << "\nWelcome to FoodBear Delivery\nNow all on fingertips\n\n";

    cout << "\n--Main--\n";
    cout << "1-Login\n2-Register\n0-Exit\n";
}

int main()
{
    system("Connection To DB");

    db_response::ConnectionFunction();


    int chooseMain;
    int chooseLogin;
    int chooseRegister;

    Customer cust = Customer("customer01", "cust123");


    do {
        displayMainMenu();
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
                    string slt = "SELECT * FROM customer";
                    const char* q = slt.c_str();
                    int qstate = mysql_query(conn, q);

                    if (!qstate) {
                        res = mysql_store_result(conn);
                        int count = mysql_num_fields(res);
                        cust.fetchData(res, count);
                        cout << "fetched" << endl;

                        string user, pass;
                        cout << "----Login----" << endl;
                        cout << "Username: ";
                        cin >> user;
                        cout << "Password: ";
                        //cin >> pass;

                        pass = takePasswdFromUser();

                        cout << pass << endl;

                        if (cust.login(user, pass)) {
                            cout << "Welcome " << cust.getName() << endl;
                            system("pause");
                        }
                    }
                    else {
                        cout << "failed to fetch";
                    }
                }
                else if (chooseLogin == 2) {

                }
                else if (chooseLogin == 3) {

                }
                else if (chooseLogin == 0) {
                }
            } while (chooseLogin != 0);
        } 
        else if (chooseMain == 2) {
            cout << "Register\n";
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