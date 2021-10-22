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

class Log {
private:
    string username, password;

public:
    static bool login(string username, string password) {
        /*this->username = username;
        this->password = password;*/

        string t = "customer";

        string s = "SELECT * FROM " + t;

        const char* q = s.c_str();

        int query = mysql_query(conn, q);

        if (!query) {
            cout << "login succeed" << endl;
            cout << username << endl;
        }
        
        else {
            cout << "login error" << endl;
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

void gData(Customer cust) {
    string userid = cust.getID();
    string pass = cust.getPassword();

    int qstate = mysql_query(conn, "SELECT * FROM customer");

    res = mysql_store_result(conn);
    int count = mysql_num_fields(res);

    if (count > 0) {

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

    Customer cust = Customer("customer01", "cust123");

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
                cust.fetchData(res, count);
                cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << "\tphone" << "\taddress" << endl;
                while (row = mysql_fetch_row(res)) {
                    for (int i = 0; i < count; i++) {
                        cout << "\t" << row[i];
                    }
                    cout << (int)row[0] << endl;
                    cout << (string)row[1] << endl;
                    cout << (string)row[2] << endl;
                    cout << (string)row[3] << endl;
                    cout << (string)row[4] << endl;
                    cout << (string)row[5] << endl;
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
    else if (a == 2) {
        string user, pass;
        cout << "----Login----" << endl;
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> pass;

        //Log::login(user, pass);

        //Customer::login(user, pass);
    }

    else if (a == 3) {
        string slt = "SELECT * FROM customer";
        const char* q = slt.c_str();
        int qstate = mysql_query(conn, q);

        cout << "Selected" << endl;

        if (!qstate) {
            res = mysql_store_result(conn);
            int count = mysql_num_fields(res);
            cout << res << endl;
            cout << count << endl;
            cust.fetchData(res, count);
            cout << "fetched" << endl;

            string user, pass;
            cout << "----Login----" << endl;
            cout << "Username: ";
            cin >> user;
            cout << "Password: ";
            cin >> pass;

            if (cust.login(user, pass)) {
            }
        }
        else {
            cout << "failed to fetch";
        }
    }

    else {
        string user, pass;
        cout << "----Login----" << endl;
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> pass;

        if (cust.login(user, pass)) {
            cout << "Login succesfull" << endl;
        }
        else {
            cout << "Login failed" << endl;
        }
    }
}