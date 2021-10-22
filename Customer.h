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
	int custNum;
	string custName, custUsername, custPass, custID, custAddress, custPhone;
	Details data[100];

public:
	Customer(string, string);
	~Customer();

	bool login(string, string);

	void fetchData(MYSQL_RES*, int);
	string getName();
	string getID();
	string getPassword();
};

#endif
