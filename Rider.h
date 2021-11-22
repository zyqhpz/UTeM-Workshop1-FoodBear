#ifndef RIDER_H
#define RIDER_H

#include <string>
#include <iostream>

#include <sstream>

#include <mysql.h>

#include <functional>
#include "sha256.h"

#include "TextTable.h"

#include <boost/algorithm/string.hpp>

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
	int riderID;
	string riderName, riderUsername, riderPass, riderPhone;
	RiderDetails data[100];

public:
	Rider();
	~Rider();

	bool login(string, string, int);
	void registerRider(MYSQL*);
	string inputNoPhone();

	int fetchData(MYSQL_RES*);
	void viewProfile(function<void()>, MYSQL*);

	string getName();
	int getID();
	string getPassword();
};

#endif
