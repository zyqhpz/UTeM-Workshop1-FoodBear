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
	boost::to_lower(user);
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
	boost::to_lower(username);
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
	cout << "\n\tID" << "\tName" << setw(25) << "\t\tPhone No." << "\t\tLocation" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < totalVendor; i++) {
		cout << "\t" << data[i].id << "\t" << data[i].name << setw(20) << data[i].phone << "\t" << data[i].address << endl;
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


int Vendor::fetchProduct(MYSQL_RES* res) {
	MYSQL_ROW row;
	int i = 0;
	int total = 0;
	while (row = mysql_fetch_row(res)) {
		product[i].id = stoi(row[0]); // string to int
		product[i].name = (string)row[1];
		product[i].price = stod(row[2]);
		product[i].vendor_id = stoi(row[3]);
		if (row[4] == NULL) {
			product[i].category_id = 0; // fix this later
		}
		else {
			product[i].category_id = stoi(row[4]);
		}
		i++;
		total++;
	}
	return total;
}

void Vendor::getCategory(int totalProduct) {
	for (int i = 0; i < totalProduct; i++) {

	}
}

void Vendor::viewProduct(int vendorID, int totalProduct, int totalVendor, int& e) {
	TextTable tf('-', '|', '+');
	TextTable tb('-', '|', '+');

	food.clear();
	beverage.clear();

	bool exist = TRUE;
	e = 1;
	for (int i = 0; i < totalVendor; i++) {
		if (vendorID != data[i].id) { // check id entered by user is exist in system or not
			//cout << "\tNo vendor selected\n";
			exist = FALSE;
			e = 0;
		}
		else {
			exist = TRUE;
			e = 1;
			break;
		}
	}

	for (int i = 0; i < totalProduct; i++) {
		if (product[i].category_id == 1) {
			food.push_back({ product[i].id, product[i].name, product[i].price, product[i].vendor_id, product[i].category_id });
		}
		else if (product[i].category_id == 2) {
			beverage.push_back({ product[i].id, product[i].name, product[i].price, product[i].vendor_id, product[i].category_id });
		}
	}
	//cout << "\n-----Food-----\n";
	tf.add("ID");
	tf.add("Product Name");
	tf.add("Price (RM)");
	tf.endOfRow();
	//cout << "\n\tId" << "\tProduct Name" << "\t\tPrice (RM)" << endl;
	for (int i = 0; i < food.size(); i++) {
		//cout << "-----------------------------------------------------------------\n";
		if (food[i].vendor_id == vendorID) {
			stringstream ss;
			ss << fixed << setprecision(2) << food[i].price;
			tf.add(to_string(food[i].id));
			tf.add(food[i].name);
			tf.add(ss.str());
			tf.endOfRow();
		}
			//cout << "\t" << food[i].id << "\t" << food[i].name << "\t\t" << setprecision(2) << food[i].price << endl;
	}
	tf.setAlignment(8, TextTable::Alignment::LEFT);

	//cout << "\n-----Beverage-----\n";
	tb.add("ID");
	tb.add("Product Name");
	tb.add("Price (RM)");
	tb.endOfRow();

	for (int i = 0; i < beverage.size(); i++) {
		//cout << "-----------------------------------------------------------------\n";
		if (beverage[i].vendor_id == vendorID) {
			stringstream ss;
			ss << fixed << setprecision(2) << beverage[i].price;
			tb.add(to_string(beverage[i].id));
			tb.add(beverage[i].name);
			tb.add(ss.str());
			tb.endOfRow();
			//cout << left << setw(10) << beverage[i].id << left << setw(30) << beverage[i].name << left << setw(10) << beverage[i].price << endl;
			//cout << "\t" << beverage[i].id << "\t" << beverage[i].name << setw(20) << beverage[i].price << endl;
		}
	}

	tb.setAlignment(8, TextTable::Alignment::LEFT);
	//cout << t;

	if (exist) {
		cout << "\n----Foods----\n";
		cout << tf;
		cout << "\n----Beverages---\n";
		cout << tb;
	}
	else {
		cout << "\n\tNo vendor selected. Try again.\n";
	}
	cout << endl;
}

// void makeOrder()

string Vendor::getFoodName(int id) {
	// cout << "Enter ID\t";
	// select to start ordering or not,
	// later enter product id option will be shown

	for (int i = 0; i < 10; i++) {
		if (product[i].id == id) {
			return product[i].name;
		}
	}
}

double Vendor::getPrice(int id) {
	for (int i = 0; i < 10; i++) {
		if (product[i].id == id) {
			return product[i].price;
		}
	}
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
