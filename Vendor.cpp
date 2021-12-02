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
	noPhone = inputNoPhone();
	cout << "Enter Vendor Name: ";
	cin.ignore();
	getline(cin, name);

	stringstream ss;
	ss << "INSERT INTO vendor (username, password, name, phone) VALUES ('" + username + "', '" + password + "', '" + name + "', '" + noPhone + "')";

	stringstream checker;
	checker << "SELECT username FROM customer WHERE username = '" << username << "' UNION SELECT username FROM vendor WHERE username = '" << username << "' UNION SELECT username FROM rider WHERE username = '" << username << "'";

	string queC = checker.str();
	const char* qC = queC.c_str();
	int qCstate = mysql_query(conn, qC);

	MYSQL_ROW row;
	MYSQL_RES* res;

	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);

	if (row > 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout << "\nUsername already taken. Please try again.\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		system("pause");
	}

	else {
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
}

string Vendor::inputNoPhone() {
	string num;
	string numResult;
	bool loopCheck;
	do {
		cout << "Enter No Phone: ";
		cin >> num;
		char check[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		int length = num.size();
		int verify = 0;
		loopCheck = true;
		for (int i = 0; i < length; i++) {
			for (int k = 0; k <= 9; k++) {
				if (num[i] == check[k]) {
					verify++;
				}
			}
		}
		if (verify != length) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			cout << "Error input. Only numeric value will be accepted.\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			loopCheck = false;
		}
	} while (loopCheck != true);
	return num;
}

int Vendor::fetchData(MYSQL_RES* res, int count) {
	//fill(begin(data), end(data), 0);

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

	/*
	
	cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << "\t\tphone" << "\t\taddress" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < total; i++) {
		cout << "\t" << data[i].id << "\t" << data[i].username << "\t" << data[i].password << "\t\t" << data[i].name << "\t" << data[i].phone << "\t" << data[i].address << endl;
	}
	cout << endl;
	*/
	return total;
}

// Function to view and user profile
void Vendor::viewProfile(function<void()> mainHeader, MYSQL* conn) {
	int operation;

	string pass, username, name, phone, address;
	//string id = (string)this->custID;

	TextTable tt;

	do {
		mainHeader();
		tt.add("Username");
		tt.add("Name");
		tt.add("No. Phone");
		tt.add("Address");
		tt.endOfRow();

		tt.add(this->vendorUsername);
		tt.add(this->vendorName);
		tt.add(this->vendorPhone);
		tt.add(this->vendorAddress);
		tt.endOfRow();

		cout << tt << endl;

		/*
		cout << "\tusername" << "\tname" << "\t\tphone" << "\t\taddress" << endl;
		cout << "---------------------------------------------------------------------------------------------------------------" << endl;
		cout << "\t" << this->vendorUsername << "\t" << this->vendorName << "\t" << this->vendorPhone << "\t" << this->vendorAddress << endl;
		cout << endl;
		*/

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

	TextTable vv;

	vv.add("ID");
	vv.add("Name");
	vv.add("Phone No.");
	vv.add("Address");
	vv.endOfRow();

	//cout << "\n\tID" << "\tName" << setw(25) << "\t\tPhone No." << "\t\tLocation" << endl;
	//cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < totalVendor; i++) {
		vv.add(to_string(data[i].id));
		vv.add(data[i].name);
		vv.add(data[i].phone);
		vv.add(data[i].address);
		vv.endOfRow();
		//cout << "\t" << data[i].id << "\t" << data[i].name << setw(20) << data[i].phone << "\t" << data[i].address << endl;
	}

	vv.setAlignment(3, TextTable::Alignment::LEFT);

	cout << endl;
	cout << vv;
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

void Vendor::viewProduct(int vendorID, int totalProduct, int totalVendor, int& e) {
	//TextTable tf('-', '|', '+');
	//TextTable tb('-', '|', '+');

	Table tf;
	Table tb;

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

	/*
	
	
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
	*/

	tf.add_row({ "ID", "Product Name", "Price (RM)"}); // NEW TABLE

	for (int i = 0; i < food.size(); i++) {
		//cout << "-----------------------------------------------------------------\n";
		if (food[i].vendor_id == vendorID) {
			stringstream ss;
			ss << fixed << setprecision(2) << food[i].price;
			//tf.add(to_string(food[i].id));
			//tf.add(food[i].name);
			//tf.add(ss.str());
			//tf.endOfRow();
			tf.add_row({ to_string(food[i].id), food[i].name, ss.str() });
		}
		//cout << "\t" << food[i].id << "\t" << food[i].name << "\t\t" << setprecision(2) << food[i].price << endl;
	}

	//cout << "\n-----Beverage-----\n";
	//tb.add("ID");
	//tb.add("Product Name");
	//tb.add("Price (RM)");
	//tb.endOfRow();

	tb.add_row({ "ID", "Product Name", "Price (RM)" }); // NEW TABLE

	for (int i = 0; i < beverage.size(); i++) {
		//cout << "-----------------------------------------------------------------\n";
		if (beverage[i].vendor_id == vendorID) {
			stringstream ss;
			ss << fixed << setprecision(2) << beverage[i].price;
			//tb.add(to_string(beverage[i].id));
			//tb.add(beverage[i].name);
			//tb.add(ss.str());
			//tb.endOfRow();
			tb.add_row({ to_string(beverage[i].id), beverage[i].name, ss.str() });
			//cout << left << setw(10) << beverage[i].id << left << setw(30) << beverage[i].name << left << setw(10) << beverage[i].price << endl;
			//cout << "\t" << beverage[i].id << "\t" << beverage[i].name << setw(20) << beverage[i].price << endl;
		}
	}

	//tb.setAlignment(8, TextTable::Alignment::LEFT);
	//cout << t;

	tf[0][0].format().width(10);
	tf[0][1].format().width(20);
	tf[0][2].format().width(20);
	
	tb[0][0].format().width(10);
	tb[0][1].format().width(20);
	tb[0][2].format().width(20);

	if (exist) {
		cout << "\n\t\t\t----Foods----\n";
		//cout << tf;
		cout << tf;
		cout << "\n\n\t\t\t----Beverages---\n";
		cout << tb;
	}
	else {
		cout << "\n\tNo vendor selected. Try again.\n";
	}
	cout << endl;
}

void Vendor::addProduct(MYSQL* conn, int vendorID) {
	string id, name, category;
	double price;

	cout << "\n---Add Product---\n";
	// cout << "Enter product ID: ";
	// cin >> id; // Check data in table, ada similar with other vendor tak. Or predefined. V01F01 -> Vendor 01, Food 01
	cout << "Enter product name: ";
	cin.ignore();
	getline(cin, name);
	
	do {
		cout << "Product category (1-Food, 2-Beverage): ";
		cin >> category;
		if (category == "1" || category == "2")
			break;
		else
			cout << "\nInvalid input.Try again.\n";
	} while (category != "1" || category != "2");

	cout << "Enter product price: ";
	cin >> price;

	stringstream ss;

	ss << "INSERT INTO product (name, price, vendor_id, category_id) VALUES ('" + name + "', " + to_string(price) + ", " + to_string(vendorID) + ", " + category + ")";

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

void Vendor::editProduct(function<void()> mainHeader, MYSQL* conn, int totalProduct) {
	char operation, edit;
	int id;
	bool exist = true;
	TextTable tt;

	string name;
	char category;
	double price;

	do {
		cout << "\nEnter product ID that want to be edit\n\t>> ";
		cin >> operation;
		id = operation - '0'; // convert char to int

		tt.add("ID");
		tt.add("Name");
		tt.add("Price (RM)");
		tt.add("Category");
		tt.endOfRow();

		int i;

		for (i = 0; i < totalProduct; i++) {
			if (id == product[i].id && this->vendorID == product[i].vendor_id) {
				exist = true;
				tt.add(to_string(product[i].id));
				tt.add(product[i].name);
				stringstream p;
				p << fixed << setprecision(2) << product[i].price;
				tt.add(p.str());
				cout << "exist";
				if (product[i].category_id == 1)
					tt.add("Food");
				else
					tt.add("Beverage");
				tt.endOfRow();
				break;
			}
			else {
				exist = false;
			}
		}

		if (!exist) {
			cout << "\nInvalid input. Item not found. Please try again.\n";
			system("pause");
			break;
		}

		else {
			mainHeader();
			cout << tt;

			cout << "\n----Manage product----\n";
			cout << "\nEnter number to edit respective data:\n\t1-Name\n\t2-Price\n\t3-Category\n\t0-Cancel\n\t>> ";
			cin >> edit;

			stringstream update;

			if (edit == '0')
				break;

			else if (edit == '1') { // Name
				//mainHeader();
				cout << "Edit name: ";
				cin.ignore();
				getline(cin, name);
				product[i].name = name;
				update << "UPDATE product SET name = '" + name + "' WHERE id = " + to_string(id);
			}

			else if (edit == '2') { // Price
				//mainHeader();
				cout << "Edit price: ";
				cin >> price;
				product[i].price = price;
				update << "UPDATE product SET price = '" + to_string(price) + "' WHERE id = " + to_string(id);
			}

			else if (edit == '3') { // Category
				//mainHeader();
				do {
					cout << "Edit category (1-Food, 2-Beverage): ";
					cin >> category;
					if (category != '1' || category != '2')
						cout << "Invalid input. Try again.\n";
				} while (category != '1' || category != '2');
				product[i].category_id = category;
				update << "UPDATE product SET category_id = '" + to_string(category) + "' WHERE id = " + to_string(id);
			}

			string query = update.str();
			const char* q = query.c_str();
			int qstate = mysql_query(conn, q);

			if (!qstate) {
				cout << "\nUpdate Successful!\n";
				system("pause");
				break;
			}
			else {
				cout << "\nUpdate Failed!\n";
				system("pause");
			}
		}
	} while (operation != '0');	
}

int Vendor::viewActiveOrder(MYSQL* conn, TextTable &tb) {
	MYSQL_ROW row;
	MYSQL_RES* res;

	int i = 0;

	tb.add("Order ID");
	tb.add("Quantity");
	tb.add("Price (RM)");
	tb.add("Order Created");
	tb.add("Status");
	tb.endOfRow();

	stringstream sql;
	sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE payment.vendor_id = " << to_string(this->vendorID) << " AND delivery.status = 0 GROUP BY cust_order.id";
	string s = sql.str();
	const char* qC = s.c_str();
	int q = mysql_query(conn, qC);
	if (!q) {
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			string orderID = row[1];
			string quantity = row[7];
			string price = row[12];
			string date = row[9];
			string riderID;
			if (row[15] == NULL) {
				riderID = "Not Set";
			}
			else {
				riderID = row[15];
			}
			string status = row[16];
			///prev_order.push_back({ row[1], row[5], row[7], row[10], row[13], row[14] });
			active_order.push_back({ orderID, quantity, price, date, riderID, status });
			tb.add(orderID);
			tb.add(quantity);
			tb.add(price);
			tb.add(date);
			if (status == "0")
				tb.add("Pending");
			else if (status == "-1")
				tb.add("Rejected"); // after vendor reject the order
			else if (status == "1")
				tb.add("Accepted"); // after vendor accept the order
			else if (status == "2")
				tb.add("In Delivery"); // after rider has pick up the order. rider only can view the list of order with accepted value (3)
			else if (status == "3")
				tb.add("Delivered"); // rider change the status after has delivered the order
			tb.endOfRow();
			//cout << prev_order[i][0] << endl << prev_order[i][1] << endl << prev_order[i][4] << endl;
			i++;
		}
	}
	else {
		cout << "\nActive Order Cannnot Be Fetched\n";
		system("pause");
	}

	return i;
}

void Vendor::viewActiveOrderDetail(MYSQL* conn, int orderID, int& exist) {
	TextTable tb;

	exist = 0;

	for (int i = 0; i < active_order.size(); i++) {
		if (orderID == stoi(active_order[i][0])) {
			exist = 1;
		}
	}

	if (exist == 1) {

		MYSQL_ROW row;
		MYSQL_RES* res;

		stringstream sql;
		sql << "SELECT cust_order.date, product.name, order_detail.quantity, order_detail.price, payment.total_payment FROM order_detail JOIN product ON order_detail.product_id = product.id JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order_id = " << orderID;
		//sql << "SELECT * FROM order_detail JOIN product ON order_detail.product_id = product.id JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order_id = " << orderID;
		//sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE payment.vendor_id = " << to_string(this->vendorID);
		string s = sql.str();
		const char* qC = s.c_str();
		int q = mysql_query(conn, qC);

		//cout << "exist orderID\n";
		stringstream m;
		m << "  OrderID: " << orderID << "  ";
		tb.add("");
		tb.add(m.str());
		tb.add("");
		tb.add("");
		tb.add("");
		tb.endOfRow();

		tb.add("No.");
		tb.add("Name");
		tb.add("Quantity");
		tb.add("Price per unit(RM)");
		tb.add("Total Price (RM)");
		//tb.addRow("1");
		tb.endOfRow();

		if (!q) {
			int i = 0;
			string date;
			double pxq;
			string totalP;
			double subTotal = 0;
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res)) {
				++i;
				string date = row[0];
				string name = row[1];
				int quantity = stoi(row[2]);
				pxq = stod(row[3]);
				//pxq = quantity * price;
				double price = pxq / quantity;
				subTotal += pxq;
				totalP = row[4];

				stringstream p;
				p << fixed << setprecision(2) << price;

				stringstream pq;
				pq << fixed << setprecision(2) << pxq;

				tb.add(to_string(i));
				tb.add(name);
				tb.add(to_string(quantity));
				tb.add(p.str());
				tb.add(pq.str());
				tb.endOfRow();
			}

			tb.setAlignment(2, TextTable::Alignment::RIGHT);
			tb.setAlignment(3, TextTable::Alignment::RIGHT);
			tb.setAlignment(4, TextTable::Alignment::RIGHT);

			tb.add("");
			tb.add("");
			tb.add("");
			tb.add("Subtotal ");
			stringstream st;
			st << fixed << setprecision(2) << subTotal;
			tb.add(st.str());
			tb.endOfRow();

			tb.add("");
			tb.add("");
			tb.add("");
			tb.add("Delivery Charge ");
			tb.add("4.00");
			tb.endOfRow();

			tb.add("");
			tb.add("");
			tb.add("");
			tb.add("Total Payment ");
			tb.add(totalP);
			tb.endOfRow();

		}
		cout << tb;

		int accept;
		stringstream ss;
		do {
			cout << "\n    1-Accept OR 2-Reject\n\t>> ";
			cin >> accept;
			if (accept == 1) {
				ss << "UPDATE delivery SET status = 1 WHERE payment_id = " << orderID;
				string s = ss.str();
				const char* qC = s.c_str();
				int q = mysql_query(conn, qC);
				cout << "\tOrder has been accepted\n";
				system("pause");
				break;
			}
			else if (accept == 2) {
				ss << "UPDATE delivery SET status = -1 WHERE payment_id = " << orderID;
				string s = ss.str();
				const char* qC = s.c_str();
				int q = mysql_query(conn, qC);
				cout << "\tOrder has been rejected\n";
				system("pause");
				break;
			}
		} while (1 <= accept <= 2);
	}
	else {
		cout << "\nInvalid choice. Try again.\n";
		system("pause");
	}
}

int Vendor::viewPreviousOrder(MYSQL* conn, TextTable& tb) {
	MYSQL_ROW row;
	MYSQL_RES* res;

	int i = 0;

	tb.add("Order ID");
	tb.add("Quantity");
	tb.add("Price (RM)");
	tb.add("Order Created");
	tb.add("Status");
	tb.endOfRow();

	stringstream sql;
	sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE payment.vendor_id = " << to_string(this->vendorID) << " AND delivery.status >= 1 GROUP BY cust_order.id";
	string s = sql.str();
	const char* qC = s.c_str();
	int q = mysql_query(conn, qC);
	if (!q) {
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			string orderID = row[1];
			string quantity = row[7];
			string price = row[12];
			string date = row[9];
			string riderID;
			if (row[15] == NULL) {
				riderID = "Not Set";
			}
			else {
				riderID = row[15];
			}
			string status = row[16];
			///prev_order.push_back({ row[1], row[5], row[7], row[10], row[13], row[14] });
			all_order.push_back({ orderID, quantity, price, date, riderID, status });
			tb.add(orderID);
			tb.add(quantity);
			tb.add(price);
			tb.add(date);
			if (status == "0")
				tb.add("Pending");
			else if (status == "-1")
				tb.add("Rejected"); // after vendor reject the order
			else if (status == "1")
				tb.add("Accepted"); // after vendor accept the order
			else if (status == "2")
				tb.add("In Delivery"); // after rider has pick up the order. rider only can view the list of order with accepted value (3)
			else if (status == "3")
				tb.add("Delivered"); // rider change the status after has delivered the order
			tb.endOfRow();
			//cout << prev_order[i][0] << endl << prev_order[i][1] << endl << prev_order[i][4] << endl;
			i++;
		}
	}
	else {
		cout << "\nActive Order Cannnot Be Fetched\n";
		system("pause");
	}

	return i;
}

void Vendor::viewPreviousOrderDetail(MYSQL* conn, int orderID, int& exist) {
	TextTable tb;

	exist = 0;

	for (int i = 0; i < all_order.size(); i++) {
		if (orderID == stoi(all_order[i][0])) {
			exist = 1;
		}
	}

	if (exist == 1) {

		MYSQL_ROW row;
		MYSQL_RES* res;

		stringstream sql;
		sql << "SELECT cust_order.date, product.name, order_detail.quantity, order_detail.price, payment.total_payment FROM order_detail JOIN product ON order_detail.product_id = product.id JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order_id = " << orderID;
		//sql << "SELECT * FROM order_detail JOIN product ON order_detail.product_id = product.id JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order_id = " << orderID;
		//sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE payment.vendor_id = " << to_string(this->vendorID);
		string s = sql.str();
		const char* qC = s.c_str();
		int q = mysql_query(conn, qC);

		//cout << "exist orderID\n";
		stringstream m;
		m << "  OrderID: " << orderID << "  ";
		tb.add("");
		tb.add(m.str());
		tb.add("");
		tb.add("");
		tb.add("");
		tb.endOfRow();

		tb.add("No.");
		tb.add("Name");
		tb.add("Quantity");
		tb.add("Price per unit(RM)");
		tb.add("Total Price (RM)");
		//tb.addRow("1");
		tb.endOfRow();

		if (!q) {
			int i = 0;
			string date;
			double pxq;
			string totalP;
			double subTotal = 0;
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res)) {
				++i;
				string date = row[0];
				string name = row[1];
				int quantity = stoi(row[2]);
				pxq = stod(row[3]);
				//pxq = quantity * price;
				double price = pxq / quantity;
				subTotal += pxq;
				totalP = row[4];

				stringstream p;
				p << fixed << setprecision(2) << price;

				stringstream pq;
				pq << fixed << setprecision(2) << pxq;

				tb.add(to_string(i));
				tb.add(name);
				tb.add(to_string(quantity));
				tb.add(p.str());
				tb.add(pq.str());
				tb.endOfRow();
			}

			tb.setAlignment(2, TextTable::Alignment::RIGHT);
			tb.setAlignment(3, TextTable::Alignment::RIGHT);
			tb.setAlignment(4, TextTable::Alignment::RIGHT);

			tb.add("");
			tb.add("");
			tb.add("");
			tb.add("Subtotal ");
			stringstream st;
			st << fixed << setprecision(2) << subTotal;
			tb.add(st.str());
			tb.endOfRow();

			tb.add("");
			tb.add("");
			tb.add("");
			tb.add("Delivery Charge ");
			tb.add("4.00");
			tb.endOfRow();

			tb.add("");
			tb.add("");
			tb.add("");
			tb.add("Total Payment ");
			tb.add(totalP);
			tb.endOfRow();

		}
		cout << tb;

		/*
		
		int accept;
		stringstream ss;
		do {
			cout << "\n    1-Accept OR 2-Reject\n\t>> ";
			cin >> accept;
			if (accept == 1) {
				ss << "UPDATE delivery SET status = 1 WHERE payment_id = " << orderID;
				string s = ss.str();
				const char* qC = s.c_str();
				int q = mysql_query(conn, qC);
				cout << "\tOrder has been accepted\n";
				system("pause");
				break;
			}
			else if (accept == 2) {
				ss << "UPDATE delivery SET status = -1 WHERE payment_id = " << orderID;
				string s = ss.str();
				const char* qC = s.c_str();
				int q = mysql_query(conn, qC);
				cout << "\tOrder has been rejected\n";
				system("pause");
				break;
			}
		} while (1 <= accept <= 2);
		*/
	}
	else {
		cout << "\nInvalid choice. Try again.\n";
		system("pause");
	}
}

void Vendor::getCategory(int totalProduct) {
	for (int i = 0; i < totalProduct; i++) {

	}
}

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

string Vendor::getVendorName(int vendorID)
{
	for (int i = 0; i < sizeof(data); i++) {
		if (vendorID == data[i].id) { // check id entered by user is exist in system or not
			//cout << "\tNo vendor selected\n";
			return data[i].name;
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
