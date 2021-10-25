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
	string riderName, riderUsername, riderPass, riderID, riderPhone;
	RiderDetails data[100];

public:
	Rider();
	~Rider();

	bool login(string, string, int);

	void registerRider(MYSQL*);

	int fetchData(MYSQL_RES*);
	string getName();
	string getID();
	string getPassword();
};

#endif
