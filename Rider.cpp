#include "Rider.h"

Rider::Rider()
{
}

Rider::~Rider()
{
}


bool Rider::login(string user, string pass)
{
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

void Rider::registerRider(MYSQL* conn) {
	string username, password, name, noPhone;
	cout << "\n---Registration For Customer---\n";
	cout << "Enter Username: ";
	cin >> username;
	cout << "Enter Password: ";
	cin >> password;
	cout << "Enter Your Name: ";
	cin.ignore();
	getline(cin, name);
	cout << "Enter Your Name: ";
	cin >> noPhone;

	stringstream ss;

	ss << "INSERT INTO rider (username, password, name, phone) VALUES ('" + username + "', '" + password + "', '" + name + "', '" + noPhone + "')";

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

int Rider::fetchData(MYSQL_RES* res, int count) {
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

string Rider::getName() {
	return this->adminName;
}

string Rider::getID()
{
	return this->adminID;
}

string Rider::getPassword()
{
	return this->adminPass;
}
