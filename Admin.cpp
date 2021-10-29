#include "Admin.h"

Admin::Admin()
{
}

Admin::~Admin()
{
}

bool Admin::login(string user, string pass)
{
	boost::to_lower(user);
	for (int i = 0; i < 6; i++) {
		if (user == data[i].username && pass == data[i].password) {
			cout << "login succeed" << endl;
			this->adminID = data[i].id;
			this->adminUsername = data[i].username;
			this->adminPass = data[i].password;
			this->adminName = data[i].name;
			return true;
		}
	}
	return false;
}

void Admin::registerCustomer(MYSQL* conn) {
	string username, password, name, noPhone;
	cout << "\n---Registration For Customer---\n";
	cout << "Enter Username: ";
	cin >> username;
	cout << "Enter Password: ";
	cin >> password;
	cout << "Enter Your Name: ";
	cin.ignore();
	getline(cin, name);

	stringstream ss;

	ss << "INSERT INTO admin (username, password, name) VALUES ('" + username + "', '" + password + "', '" + name + "')";

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

int Admin::fetchData(MYSQL_RES* res, int count) {
	MYSQL_ROW row;
	int i = 0;
	int total = 0;
	while (row = mysql_fetch_row(res)) {
		data[i].id = stoi(row[0]); // string to int
		data[i].username = (string)row[1];
		data[i].password = (string)row[2];
		data[i].name = (string)row[3];

		i++;
		total++;
	}
	cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < total; i++) {
		cout << "\t" << data[i].id << "\t" << data[i].username << "\t" << data[i].password << "\t\t" << data[i].name << "\t" << endl;
	}
	cout << endl;
	return total;
}

string Admin::getName() {
	return this->adminName;
}

string Admin::getID()
{
	return this->adminID;
}

string Admin::getPassword()
{
	return this->adminPass;
}
