#include "Customer.h"

Customer::Customer(string custId, string custPass)
{
	this->custID = custId;
	this->custPass = custPass;
}

Customer::~Customer()
{
}

bool Customer::login()
{
	if (this->custID == "Hello1" && this->custPass == "hello123")
		return true;
	else
		return false;
}
