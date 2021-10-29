#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

#include <boost/algorithm/string.hpp>

using namespace std;

#pragma once

struct AdminDetails {
	int id;
	string username;
	string password;
	string name;
};

class Admin {
private:
	string adminName, adminUsername, adminPass, adminID;
	AdminDetails data[100];

public:
	Admin();
	~Admin();

	bool login(string, string);

	void registerCustomer(MYSQL*);

	int fetchData(MYSQL_RES*, int);
	string getName();
	string getID();
	string getPassword();
};

#endif
