#ifndef RIDER_H
#define RIDER_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

using namespace std;

#pragma once

struct RiderDetails {
	int id;
	string username;
	string password;
	string phone;
	string name;
};

class Rider {
private:
	string adminName, adminUsername, adminPass, adminID, adminPhone;
	RiderDetails data[100];

public:
	Rider();
	~Rider();

	bool login(string, string);

	void registerRider(MYSQL*);

	int fetchData(MYSQL_RES*, int);
	string getName();
	string getID();
	string getPassword();
};

#endif
