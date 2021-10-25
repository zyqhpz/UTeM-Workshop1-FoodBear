#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

#include <functional>
#include "sha256.h"

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
	int custID; // before this in string
	string custName, custUsername, custPass, custAddress, custPhone;
	CustomerDetails data[100];

public:
	Customer();
	Customer(string, string);
	~Customer();

	bool login(string, string, int);

	void registerCustomer(MYSQL*);

	int fetchData(MYSQL_RES*);
	void viewProfile(function<void()>, MYSQL*);

	string getName();
	int getID();
	string getPassword();

};

#endif
