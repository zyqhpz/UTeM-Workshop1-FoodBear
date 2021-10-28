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
	pass = sha256(pass);
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
	password = sha256(password);
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
			data[i].address = "Not Set";
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

// Function to view and user profile
void Vendor::viewProfile(function<void()> mainHeader, MYSQL* conn) {
	int operation;

	string pass, username, name, phone, address;
	//string id = (string)this->custID;

	do {
		mainHeader();
		cout << "\tusername" << "\tname" << "\t\tphone" << "\t\taddress" << endl;
		cout << "---------------------------------------------------------------------------------------------------------------" << endl;
		cout << "\t" << this->vendorUsername << "\t" << this->vendorName << "\t" << this->vendorPhone << "\t" << this->vendorAddress << endl;
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

			this->vendorUsername = username;

			update << "UPDATE customer SET username = '" + username + "' WHERE id = " + to_string(this->vendorID);
		}
		else if (operation == 2) { // Password
			mainHeader();
			cout << "Enter new password: ";
			cin >> pass;

			pass = sha256(pass);

			this->vendorPass = pass;

			update << "UPDATE customer SET password = '" + pass + "' WHERE id = " + to_string(this->vendorID);
		}
		else if (operation == 3) { // Name
			mainHeader();
			cout << "Enter new name: ";
			cin.ignore();
			getline(cin, name);

			this->vendorName = name;

			update << "UPDATE customer SET name = '" + name + "' WHERE id = " + to_string(this->vendorID);
		}
		else if (operation == 4) { // Phone
			mainHeader();
			cout << "Update phone number: ";
			cin >> phone;

			this->vendorPhone = phone;

			update << "UPDATE customer SET phone = '" + phone + "' WHERE id = " + to_string(this->vendorID);
		}
		else if (operation == 5) { // Address
			mainHeader();
			cout << "Update address: ";
			cin.ignore();
			getline(cin, address);

			this->vendorAddress = address;

			update << "UPDATE customer SET address = '" + address + "' WHERE id = " + to_string(this->vendorID);
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

void Vendor::viewVendor(int totalVendor) {
	cout << "\n\tname" << "\t\tphone" << "\t\taddress" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < totalVendor; i++) {
		cout << "\t" << data[i].name << "\t" << data[i].phone << "\t" << data[i].address << endl;
	}
	cout << endl;
}

int Vendor::fetchProduct(MYSQL_RES* res) {
	MYSQL_ROW row;
	int i = 0;
	int total = 0;
	while (row = mysql_fetch_row(res)) {
		product[i].id = stoi(row[0]); // string to int
		product[i].name = (string)row[1];
		product[i].price = stod(row[2]);
		product[i].vendor_id = stoi(row[3]);
		product[i].category_id = stoi(row[4]);
		if (row[5] == NULL) {
			product[i].category_id = 0; // fix this later
		}
		else {
			product[i].category_id = 1;
		}
		i++;
		total++;
	}
}


void Vendor::viewProduct(int vendorID, int totalProduct) {
	cout << "\n\tId" << "\tProduct Name" << "\t\tPrice" << endl;
	cout << "-----------------------------------------------------------------\n";
	for (int i = 0; i < totalProduct; i++) {
		if (product[i].vendor_id == vendorID)
			cout << "\t" << product[i].id << "\t" << product[i].name << "\t\t" << product[i].price << endl;
	}
	cout << endl;
}

void Vendor::addProduct(MYSQL* conn, int vendorID) {
	string id, name;
	double price;

	cout << "\n---Add Product---\n";
	cout << "Enter product ID: ";
	cin >> id; // Check data in table, ada similar with other vendor tak. Or predefined. V01F01 -> Vendor 01, Food 01
	cout << "Enter product name: ";
	cin.ignore();
	getline(cin, name);
	cout << "Enter product price: ";
	cin >> price;

	stringstream ss;

	ss << "INSERT INTO product (id, name, price, vendor_id) VALUES ('" + id + "', '" + name + "', " + to_string(price) + ", " + to_string(vendorID) + ")";

	string query = ss.str();
	const char* q = query.c_str();
	int qstate = mysql_query(conn, q);

	if (!qstate) {
		cout << "\nProduct has been added!\n";
		system("pause");
	}
	else {
		cout << "\nProduct cannot be added. Please try again.\n"; // output error? why cant insert?
		system("pause");
	}
}


// void makeOrder()

void Vendor::selectProduct() {
	cout << "Enter ID\t";
	// select to start ordering or not,
	// later enter product id option will be shown
}

string Vendor::getName() {
	return this->vendorName;
}

int Vendor::getID()
{
	return this->vendorID;
}

string Vendor::getPassword()
{
	return this->vendorPass;
}
