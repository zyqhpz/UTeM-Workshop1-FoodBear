#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

using namespace std;

#pragma once

struct CustomerDetails {
	int id;
	string username;
	string password;
	string name;
	string phone;
	string address;
};

class Customer {
private:
	//int custNum
	string custName, custUsername, custPass, custID, custAddress, custPhone;
	CustomerDetails data[100];

public:
	Customer();
	Customer(string, string);
	~Customer();

	bool login(string, string);

	void registerCustomer(MYSQL*);

	int fetchData(MYSQL_RES*, int);
	string getName();
	string getID();
	string getPassword();
};

#endif
