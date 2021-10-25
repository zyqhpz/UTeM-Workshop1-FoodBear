#include "Rider.h"

Rider::Rider()
{
}

Rider::~Rider()
{
}


bool Rider::login(string user, string pass, int totalRider)
{
	for (int i = 0; i < totalRider; i++) {
		if (user == data[i].username && pass == data[i].password) {
			cout << "login succeed" << endl;
			this->riderID = data[i].id;
			this->riderUsername = data[i].username;
			this->riderPass = data[i].password;
			this->riderName = data[i].name;
			this->riderPhone = data[i].phone;
			return true;
		}
	}
	return false;
}

void Rider::registerRider(MYSQL* conn) {
	string username, password, name, noPhone;
	cout << "\n---Registration For Rider---\n";
	cout << "Enter Username: ";
	cin >> username;
	cout << "Enter Password: ";
	cin >> password;
	cout << "Enter Your Name: ";
	cin.ignore();
	getline(cin, name);
	cout << "Enter Phone Number: ";
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

int Rider::fetchData(MYSQL_RES* res) {
	MYSQL_ROW row;
	int i = 0;
	int total = 0;
	while (row = mysql_fetch_row(res)) {
		data[i].id = stoi(row[0]); // string to int
		data[i].username = (string)row[1];
		data[i].password = (string)row[2];
		data[i].name = (string)row[3];
		data[i].phone = (string)row[4];

		i++;
		total++;
	}
	cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << "\t\tPhone" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < total; i++) {
		cout << "\t" << data[i].id << "\t" << data[i].username << "\t\t" << data[i].password << "\t\t" << data[i].name << "\t" << data[i].phone << endl;
	}
	cout << endl;
	return total;
}

// Function to view and user profile
void Rider::viewProfile(function<void()> mainHeader, MYSQL* conn) {
	int operation;

	string pass, username, name, phone;
	//string id = (string)this->custID;

	do {
		mainHeader();
		cout << "\tusername" << "\tname" << "\t\tphone" << endl;
		cout << "---------------------------------------------------------------------------------------------------------------" << endl;
		cout << "\t" << this->riderUsername << "\t" << this->riderName << "\t" << this->riderPhone << endl;
		cout << endl;

		cout << "\n---Edit User Profile---\n";
		cout << "\nEnter number to edit respective data:\n 1-Username\n 2-Password\n 3-Name\n 4-Phone\n 5-Address\n 0-Back to Home\n";
		cout << ">> ";
		cin >> operation;

		stringstream update;

		if (operation == 1) { // Username
			mainHeader();
			cout << "Enter new username: ";
			cin >> username;

			this->riderUsername = username;

			update << "UPDATE rider SET username = '" + username + "' WHERE id = " + to_string(this->riderID);
		}
		else if (operation == 2) { // Password
			mainHeader();
			cout << "Enter new password: ";
			cin >> pass;

			pass = sha256(pass);

			this->riderPass = pass;

			update << "UPDATE rider SET password = '" + pass + "' WHERE id = " + to_string(this->riderID);
		}
		else if (operation == 3) { // Name
			mainHeader();
			cout << "Enter new name: ";
			cin.ignore();
			getline(cin, name);

			this->riderName = name;

			update << "UPDATE rider SET name = '" + name + "' WHERE id = " + to_string(this->riderID);
		}
		else if (operation == 4) { // Phone
			mainHeader();
			cout << "Update phone number: ";
			cin >> phone;

			this->riderPhone = phone;

			update << "UPDATE rider SET phone = '" + phone + "' WHERE id = " + to_string(this->riderID);
		}
		else if (operation == 0) {
			break;
		}
		else {
			cout << "Invalid input\n";
			system("pause");
		}

		string query = update.str();
		const char* q = query.c_str();
		int qstate = mysql_query(conn, q);

		if (!qstate) {
			cout << "\nUpdate Successful!\n";
			break;
		}
		else {
			cout << "\nUpdate Failed!\n";
			system("pause");
		}
	} while (operation != -1);
}

string Rider::getName() {
	return this->riderName;
}

int Rider::getID()
{
	return this->riderID;
}

string Rider::getPassword()
{
	return this->riderPass;
}
