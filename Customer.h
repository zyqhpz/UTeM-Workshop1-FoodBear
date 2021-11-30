#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

#include <functional>
#include "sha256.h"

#include <boost/algorithm/string.hpp> // toLower string

#include <iomanip>
#include <vector>
#include "Vendor.h"

#include <ctime>
#include <time.h>

#include "TextTable.h"

#include <cmath> // round off number

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

struct CustomerOrders {
	int id;
	int total_quantity;
	double total_price;
};

struct OrderDetails {
	//int id;
	int cust_order_id;
	int product_id;
	int quantity;
};

class Customer {
private:
	//int custNum
	int custID; // before this in string
	string custName, custUsername, custPass, custAddress, custPhone;
	CustomerDetails data[100];

	vector<vector<string>> order; // 2d vector array to store order details when ordering

	CustomerOrders cust_order[100];
	OrderDetails order_detail[100];

	vector<vector<string>> prev_order;

public:
	Customer();
	Customer(string, string);
	~Customer();

	bool login(string, string, int);
	void registerCustomer(MYSQL*);
	string inputNoPhone();

	int fetchData(MYSQL_RES*);
	int fetchOrderData(MYSQL*);
	int fetchOrderDetails(MYSQL*);
	int fetchPreviousOrder(MYSQL*, TextTable&);

	void viewPreviousOrderDetails(MYSQL*, int, int&);

	void viewProfile(function<void()>, MYSQL*);

	void selectProduct(Vendor, int, int, double&);

	void insertOrder(MYSQL*, int);

	vector<vector<string>> getOrder();

	void displayPreviousOrder(int, int, TextTable&);

	string getName();
	int getID();
	string getPassword();

};

#endif
