#include <iostream>
#include <iomanip>
#include <windows.h>

#include <sstream>

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

void display(MYSQL* conn) {

    conn = mysql_real_connect(conn, "localhost", "root", "", "foodbear", 3306, NULL, 0);

    string slt = "SELECT * FROM customer";
    const char* q = slt.c_str();

    int qstate = mysql_query(conn, "SELECT * FROM custom");

    cout << "Selected" << endl;

    if (qstate == 0) {
        res = mysql_store_result(conn);
        int count = mysql_num_fields(res);
        cout << count << endl;
        while (row = mysql_fetch_row(res)) {
            for (int i = 0; i < count; i++) {
                cout << "\t" << row[i];
            }
            cout << endl;
        }
    }
    else {
        cout << "failed to fetch";
    }
}

int main()
{
    system("Connection To DB");

    db_response::ConnectionFunction();
    mainHeader();

    cout << "Hello World!\nasdsasdadasdadasdasdadasd\nsdadfadfadadasdasdasdasdsa\n";
    cout << "Welcome to FoodBear Delivery\nNow all on fingertips\n";

    int a;

    cin >> a;

    Customer cust = Customer("Hello1", "hello123");

    //stringstream ss;

    //ss << "SELECT * FROM customer";


    //sql::Statement* stmt;

    //const char* q = " ";

    //strcpy(q, slt.c_str());

    string id = "Hello1";
    string pass = "hello123";

    if (a == 1) {
        mainHeader();
        cout << "1!\n";

        //int qstate = 0;

        //string query = mysql_query(conn, q);


        //if (qstate == 0) {
        if (conn) {
            string slt = "SELECT * FROM customer";
            const char* q = slt.c_str();

            int qstate = mysql_query(conn, "SELECT * FROM customer");

            cout << "Selected" << endl;

            if (!qstate) {
                res = mysql_store_result(conn);
                int count = mysql_num_fields(res);
                cout << count << endl;
                while (row = mysql_fetch_row(res)) {
                    for (int i = 0; i < count; i++) {
                        cout << "\t" << row[i];
                    }
                    cout << endl;
                }
            }
            else {
                cout << "failed to fetch";
            }
        }
        else {
            cout << "failed";
        }

        //system("pause");
    }
    else {
        if (cust.login()) {
            cout << "Login succesfull";
        }
    }
}