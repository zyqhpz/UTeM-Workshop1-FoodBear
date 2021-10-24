#ifndef VENDOR_H
#define VENDOR_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

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

class Vendor {
private:
	//int custNum;
	string vendorName, vendorUsername, vendorPass, vendorID, vendorAddress, vendorPhone;
	VendorDetails data[100];

public:
	Vendor();
	~Vendor();

	bool login(string, string);

	void registerVendor(MYSQL*);

	int fetchData(MYSQL_RES*, int);
	string getName();
	string getID();
	string getPassword();
};

#endif
