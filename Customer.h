#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>

#include <mysql.h>

using namespace std;

#pragma once

struct Details {
	int id;
	string username;
	string password;
	string name;
	string phone;
	string address;
};

class Customer {
private:
	int custNum, custPhone;
	string custName, custUsername, custPass, custID, custAddress;
	Details data[100];

public:
	Customer(string, string);
	~Customer();
	//bool login(string, string);
	//bool login();

	bool login(string, string);



	void fetchData(MYSQL_RES*, int);
	void getData(MYSQL_ROW, int);
	string getID();
	string getPassword();
};

#endif
