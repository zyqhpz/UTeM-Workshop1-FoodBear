#ifndef VENDOR_H
#define VENDOR_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

#include <functional>
#include "sha256.h"

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
};

class Vendor {
private:
	//int custNum;
	//string vendorID;
	int vendorID;
	string vendorName, vendorUsername, vendorPass, vendorAddress, vendorPhone;
	VendorDetails data[100];
	VendorProducts product[100];

public:
	Vendor();
	~Vendor();

	bool login(string, string, int);

	void registerVendor(MYSQL*);

	int fetchData(MYSQL_RES*, int);
	void viewProfile(function<void()>, MYSQL*);

	void viewVendor(int);

	void fetchProduct(MYSQL_RES*, int);
	void viewProduct(int);

	void addProduct(MYSQL*, int);


	void selectProduct();

	string getName();
	int getID();
	string getPassword();
};

#endif
