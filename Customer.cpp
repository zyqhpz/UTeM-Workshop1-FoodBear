#include "Customer.h"

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
	boost::to_lower(user);
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
	boost::to_lower(username);
	cout << "Enter Password: ";
	cin >> password;
	password = sha256(password);
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
void Customer::viewProfile(function<void()> mainHeader, MYSQL* conn) {
	int operation;

	string pass, username, name, phone, address;
	//string id = (string)this->custID;

	do {
		mainHeader();
		cout << "\tusername" << "\tname" << "\t\tphone" << "\t\taddress" << endl;
		cout << "---------------------------------------------------------------------------------------------------------------" << endl;
		cout << "\t" << this->custUsername << "\t" << this->custName << "\t" << this->custPhone << "\t" << this->custAddress << endl;
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

			this->custUsername = username;

			update << "UPDATE customer SET username = '" + username + "' WHERE id = " + to_string(this->custID);
		}
		else if (operation == 2) { // Password
			mainHeader();
			cout << "Enter new password: ";
			cin >> pass;

			pass = sha256(pass);

			this->custPass = pass;

			update << "UPDATE customer SET password = '" + pass + "' WHERE id = " + to_string(this->custID);
		}
		else if (operation == 3) { // Name
			mainHeader();
			cout << "Enter new name: ";
			cin.ignore();
			getline(cin, name);

			this->custName = name;

			update << "UPDATE customer SET name = '" + name + "' WHERE id = " + to_string(this->custID);
		}
		else if (operation == 4) { // Phone
			mainHeader();
			cout << "Update phone number: ";
			cin >> phone;

			this->custPhone = phone;

			update << "UPDATE customer SET phone = '" + phone + "' WHERE id = " + to_string(this->custID);
		}
		else if (operation == 5) { // Address
			mainHeader();
			cout << "Update address: ";
			cin.ignore();
			getline(cin, address);

			this->custAddress = address;

			update << "UPDATE customer SET address = '" + address + "' WHERE id = " + to_string(this->custID);
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

void Customer::selectProduct(Vendor vendor, int id, int quantity, double& total) {
	string name;
	//order.push_back({ id + 1, quantity + 1});

	double t;
	name = vendor.getFoodName(id);
	double price = vendor.getPrice(id);
	price = price * quantity;
	total = total + price;
	total = floor((total * 100) + .5) / 100;
	price = floor((price * 100) + .5) / 100;

	stringstream streamPrice;
	streamPrice << fixed << setprecision(2) << price;

	order.push_back({ name, streamPrice.str() , to_string(quantity), to_string(id) });

	TextTable tableOrder('-', '|', '+');

	tableOrder.add("No.");
	tableOrder.add("Product Name");
	tableOrder.add("Quantity");
	tableOrder.add("Total Price (RM)");
	tableOrder.endOfRow();

	//for (int i = 0; i < order.size(); i++) {
	//	//cout << "Name: " << order[i][0] << " Quantity: " << order[i][1] << endl;
	//	cout << order[i][0] << " ---Quantity: " << order[i][2] << "\tPrice: RM " << order[i][1] << endl;
	//}	
	
	for (int i = 0; i < order.size(); i++) {
		tableOrder.add(to_string(i+1));
		tableOrder.add(order[i][0]);
		tableOrder.add(order[i][2]);
		tableOrder.add(order[i][1]);
		tableOrder.endOfRow();
		//cout << order[i][0] << " ---Quantity: " << order[i][2] << "\tPrice: RM " << order[i][1] << endl;
	}

	/*tableOrder.add("Total amount: RM");
	tableOrder.add(to_string(total));
	tableOrder.endOfRow();*/

	tableOrder.setAlignment(2, TextTable::Alignment::RIGHT);
	tableOrder.setAlignment(3, TextTable::Alignment::RIGHT);

	cout << "\n---Cart---\n";
	cout << tableOrder;

	cout << "\nSubtotal amount: RM " << fixed << setprecision(2) << total << endl;

	//order.clear();
}

void Customer::insertOrder(MYSQL* conn) {
	// 1. create cust_order table first
	// 2. insert details to order_detail

	int totalQuantity = 0;
	double totalPrice = 0;

	time_t t = time(0);
	//char* dt = ctime(&t);
	tm* now = localtime(&t);
	stringstream date;
	date << to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday);

	for (int i = 0; i < order.size(); i++) {
		totalQuantity += stoi(order[i][2]);
		totalPrice += stod(order[i][1]);
	}

	stringstream ss;

	// ss << "INSERT INTO order_detail SET username = '" + username + "' WHERE id = " + to_string(this->custID);

	// ss << "INSERT INTO customer (username, password, name, phone) VALUES ('" + username + "', '" + password + "', '" + name + "', '" + noPhone + "')";

	ss << "INSERT INTO cust_order (customer_id, total_quantity, total_price, date) VALUES ('" + to_string(this->custID) + "', '" + to_string(totalQuantity) + "', '" + to_string(totalPrice) + "', '" + date.str() + "')";

	string slt = "SELECT id FROM cust_order";

	string query = ss.str();
	const char* q = query.c_str();
	int qstate = mysql_query(conn, q);
	//mysql_insert_id(conn);

	int orderID = mysql_insert_id(conn);

	if (!qstate) {

		cout << "\nOrder has been created successfully!\n";
		for (int i = 0; i < order.size(); i++) {
			stringstream ssOrder;
			//ssOrder << "INSERT INTO order_detail (product_id, quantity) VALUES ('" + order[i][3] + "', '" + order[i][2]  + "')";
			ssOrder << "INSERT INTO order_detail (cust_order_id, product_id, quantity) VALUES ('" + to_string(orderID) + "', '" + order[i][3] + "', '" + order[i][2]  + "')";
		
			// insert to payment table here

			string queryDetail = ssOrder.str();
			const char* qD = queryDetail.c_str();
			int qDstate = mysql_query(conn, qD);

			if (!qDstate) {
				cout << "\nProduct " << i + 1 << " created.\n";
			}

			else {
				cout << "\nOrder cannot be created!\n";
				//system("pause");
				//break;
			}
		}
		system("pause");
	}
	else {
		cout << "\nOrder cannot be created!\n";
		system("pause");
	}

}

vector<vector<string>> Customer::getOrder() {
	return this->order;
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
