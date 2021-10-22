#include "Customer.h"

Customer::Customer(string custUsername, string custPass)
{
	this->custUsername = custUsername;
	this->custPass = custPass;
}

Customer::~Customer()
{
}

bool Customer::login(string user, string pass)
{
	for (int i = 0; i < 2; i++) {
		if (user == data[i].username && pass == data[i].password) {
			cout << "login succeed" << endl;
			return true;
		}
		else {
			cout << "login failed" << endl;
			return false;
		}
	}
}

void Customer::getData(MYSQL_ROW row, int count) {
	for (int i = 0; i < count; i++) {
		//data[i].id = row[i];
		cout << row[i] << endl;
	}
}

void Customer::fetchData(MYSQL_RES* res, int count) {
	MYSQL_ROW row;
	int i = 0;
	while (row = mysql_fetch_row(res)) {
		
		data[i].id = (int)row[0];
		data[i].username = (string)row[1];
		data[i].password = (string)row[2];
		data[i].name = (string)row[3];
		data[i].phone = (string)row[4];
		data[i].address = (string)row[5];

	/*	cout << (int)row[0] << endl;
		cout << (string)row[1] << endl;
		cout << (string)row[2] << endl;
		cout << (string)row[3] << endl;
		cout << (string)row[4] << endl;
		cout << (string)row[5] << endl;
		cout << endl;*/
		i++;
	}
	cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << "\tphone" << "\taddress" << endl;
	for (int i = 0; i < 2; i++) {
		cout << data[i].id << "\t" << data[i].username << "\t" << data[i].password << "\t" << data[i].name << "\t" << data[i].phone << "\t" << data[i].address << endl;
	}
}

string Customer::getID()
{
	return this->custID;
}

string Customer::getPassword()
{
	return this->custPass;
}
