#ifndef VENDOR_H
#define VENDOR_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

#include <functional>
#include "sha256.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

#include <iomanip> // for decimal places // setW

#include <boost/algorithm/string.hpp> // toLower string


//#include "tabulate/single_include/tabulate/tabulate.hpp"

#include "External.h"

using namespace std;

#pragma once

struct VendorDetails {
	int id;
	string username;
	string password;
	string name;
	string phone;
	string address;
};

struct VendorProducts {
	int id;
	string name;
	double price;
	int vendor_id;
	int category_id;
};

class Vendor {
private:
	//int custNum;
	//string vendorID;
	int vendorID;
	string vendorName, vendorUsername, vendorPass, vendorAddress, vendorPhone;
	VendorDetails data[100];
	VendorProducts product[100];
	vector<VendorProducts> food;
	vector<VendorProducts> beverage;

	vector<vector<string>> active_order;
	vector<vector<string>> all_order;

public:
	Vendor();
	~Vendor();

	bool login(string, string, int);
	void registerVendor(MYSQL*);
	string inputNoPhone();

	int fetchData(MYSQL_RES*, int);
	void viewProfile(function<void()>, MYSQL*);

	void viewVendor(int);

	int fetchProduct(MYSQL_RES*);
	void viewProduct(int, int, int, int&);

	void addProduct(MYSQL*, int);
	void editProduct(function<void()>, MYSQL*, int);

	int viewActiveOrder(MYSQL*, TableCart&);
	void viewActiveOrderDetail(MYSQL*, int, int&);

	int viewPreviousOrder(MYSQL*, TableOrder&);
	void viewPreviousOrderDetail(MYSQL*, int, int&);

	void searchProductByName(string, int, int);

	void getCategory(int);
	string getFoodName(int);
	double getPrice(int id);
	string getVendorName(int);
	string getName();
	int getID();
	string getPassword();
};

#endif
