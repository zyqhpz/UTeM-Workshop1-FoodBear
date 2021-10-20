#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
using namespace std;

#pragma once

class Customer
{
private:
	int custNum, custPhone;
	string custName, custPass, custID, custAddress;

public:
	Customer(string, string);
	~Customer();
	//bool login(string, string);
	bool login();
};

#endif
