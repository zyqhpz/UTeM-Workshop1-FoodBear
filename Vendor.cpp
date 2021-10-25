#include "Vendor.h"

Vendor::Vendor()
{
	//this->vendorUsername = "";
}

Vendor::~Vendor()
{
}

bool Vendor::login(string user, string pass, int totalVendor)
{
	for (int i = 0; i < totalVendor; i++) {
		if (user == data[i].username && pass == data[i].password) {
			cout << "login succeed" << endl;
			this->vendorID = data[i].id;
			this->vendorUsername = data[i].username;
			this->vendorPass = data[i].password;
			this->vendorName = data[i].name;
			this->vendorPhone = data[i].phone;
			this->vendorAddress = data[i].address;
			return true;
		}
	}
	return false;
}

void Vendor::registerVendor(MYSQL* conn) {
	string username, password, name, noPhone;
	cout << "\n---Registration For Vendor---\n";
	cout << "Enter Username: ";
	cin >> username;
	cout << "Enter Password: ";
	cin >> password;
	cout << "Enter Vendor Name: ";
	cin.ignore();
	getline(cin, name);
	cout << "Enter Phone Number: ";
	cin >> noPhone;

	stringstream ss;

	ss << "INSERT INTO vendor (username, password, name, phone) VALUES ('" + username + "', '" + password + "', '" + name + "', '" + noPhone + "')";

	string query = ss.str();
	const char* q = query.c_str();
	int qstate = mysql_query(conn, q);

	if (!qstate) {
		cout << "\nRegistration Successful!\n";
		system("pause");
	}
	else {
		cout << "\nRegistration Failed!\n";
		system("pause");
	}
}

int Vendor::fetchData(MYSQL_RES* res, int count) {
	MYSQL_ROW row;
	int i = 0;
	int total = 0;
	while (row = mysql_fetch_row(res)) {
		data[i].id = stoi(row[0]); // string to int
		data[i].username = (string)row[1];
		data[i].password = (string)row[2];
		data[i].name = (string)row[3];
		data[i].phone = (string)row[4];
		if (row[5] == NULL) {
			data[i].address = "0";
		}
		else {
			data[i].address = (string)row[5];
		}

		/*	cout << (int)row[0] << endl;
			cout << (string)row[1] << endl;
			cout << (string)row[2] << endl;
			cout << (string)row[3] << endl;
			cout << (string)row[4] << endl;
			cout << (string)row[5] << endl;
			cout << endl;*/
		i++;
		total++;
	}
	cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << "\t\tphone" << "\t\taddress" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < total; i++) {
		cout << "\t" << data[i].id << "\t" << data[i].username << "\t" << data[i].password << "\t\t" << data[i].name << "\t" << data[i].phone << "\t" << data[i].address << endl;
	}
	cout << endl;
	return total;
}

string Vendor::getName() {
	return this->vendorName;
}

string Vendor::getID()
{
	return this->vendorID;
}

string Vendor::getPassword()
{
	return this->vendorPass;
}
