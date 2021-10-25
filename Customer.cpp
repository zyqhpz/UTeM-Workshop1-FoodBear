#include "Customer.h"

#include "sha256.h"

Customer::Customer()
{
}

Customer::Customer(string custUsername, string custPass)
{
	this->custUsername = custUsername;
	this->custPass = custPass;
}

Customer::~Customer()
{
}

bool Customer::login(string user, string pass, int totalCustomer)
{
	pass = sha256(pass);
	for (int i = 0; i < totalCustomer; i++) {
		if (user == data[i].username && pass == data[i].password) {
			cout << "login succeed" << endl;
			this->custID = data[i].id;
			this->custUsername = data[i].username;
			this->custPass = data[i].password;
			this->custName = data[i].name;
			this->custPhone = data[i].phone;
			this->custAddress = data[i].address;
			return true;
		}
	}
	return false;
}

void Customer::registerCustomer(MYSQL* conn) {
	string username, password, name, noPhone;
	cout << "\n---Registration For Customer---\n";
	cout << "Enter Username: ";
	cin >> username;
	cout << "Enter Password: ";
	cin >> password;
	cout << "Enter Your Name: ";
	//cin >> name;
	cin.ignore();
	getline(cin, name);
	cout << "Enter Phone Number: ";
	cin >> noPhone;

	stringstream ss;

	ss << "INSERT INTO customer (username, password, name, phone) VALUES ('"+ username +"', '"+ password +"', '"+ name +"', '"+ noPhone +"')";

	string slt = "SELECT * FROM customer";

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

int Customer::fetchData(MYSQL_RES* res) {
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

// Function to view profile of user
void Customer::editProfile() {

	cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << "\t\tphone" << "\t\taddress" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
		
	cout << "\t" << this->custID << "\t" << this->custUsername << "\t" << this->custPass << "\t\t" << this->custName << "\t" << this->custPhone << "\t" << this->custAddress << endl;
	
	cout << endl;

	// operation to edit profile
	int operation;

	do {
		cout << "---View User Profile---\n";
		cout << "\nEnter 1-Edit, 0-Back to Home\n";
		cout << ">> ";
		cin >> operation;

		if (operation == 1) {
			editProfile();
		}
		else if (operation == 0) {
			break;
		}
		else {
			cout << "Invalid input\n";
			system("pause");
		}
	} while (operation != 0);
}

// Function to edit user profile
void Customer::viewProfile(function<void()> mainHeader, MYSQL* conn) {
	int operation;

	string pass;
	//string id = (string)this->custID;

	do {
		mainHeader();
		cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << "\t\tphone" << "\t\taddress" << endl;
		cout << "---------------------------------------------------------------------------------------------------------------" << endl;
		cout << "\t" << this->custID << "\t" << this->custUsername << "\t" << this->custPass << "\t\t" << this->custName << "\t" << this->custPhone << "\t" << this->custAddress << endl;
		cout << endl;

		cout << "\n---Edit User Profile---\n";
		cout << "\nEnter number to edit respective data:\n 1-Username\n 2-Password\n 3-Name\n 4-Phone\n 5-Address\n 0-Back to Home\n";
		cout << ">> ";
		cin >> operation;

		if (operation == 1) {
			cout << "Edit\n";
			system("pause");
		}
		else if (operation == 2) {
			mainHeader();
			cout << "Enter new password: ";
			cin >> pass;

			pass = sha256(pass);

			this->custPass = pass;
			stringstream updatePass;

			updatePass << "UPDATE customer SET password = '" + pass + "' WHERE id = " + to_string(this->custID);

			string query = updatePass.str();
			const char* q = query.c_str();
			int qstate = mysql_query(conn, q);

			if (!qstate) {
				cout << "\nUpdate Successful!\n";
				system("pause");
			}
			else {
				cout << "\nRegistration Failed!\n";
				system("pause");
			}
		}
		else if (operation == 0) {
			break;
		}
		else {
			cout << "Invalid input\n";
			system("pause");
		}
	} while (operation != -1);
}

string Customer::getName() {
	return this->custName;
}

int Customer::getID()
{
	return this->custID;
}

string Customer::getPassword()
{
	return this->custPass;
}
