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
	noPhone = inputNoPhone();


	stringstream ss;
	ss << "INSERT INTO customer (username, password, name, phone) VALUES ('"+ username +"', '"+ password +"', '"+ name +"', '"+ noPhone +"')";

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

string Customer::inputNoPhone(){
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

int Customer::fetchOrderData(MYSQL* conn) {
	MYSQL_ROW row;
	MYSQL_RES* res;

	int i = 0;

	//string sql = "SELECT * FROM cust_order";

	stringstream sql;
	//sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order.customer_id = 2;";
	sql << "SELECT * FROM cust_order WHERE customer_id = " + to_string(this->custID);
	string s = sql.str();
	const char* qC = s.c_str();
	int q = mysql_query(conn, qC);
	if (!q) {
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			cust_order[i].id = stoi(row[0]); // string to int
			cust_order[i].total_quantity = stoi(row[2]);
			cust_order[i].total_price = stod(row[3]);
			i++;
		}
	}
	else {
		cout << "\nPrevious Order Cannnot Be Fetched\n";
	}

	return i;
}

int Customer::fetchOrderDetails(MYSQL* conn)
{
	// SELECT * FROM order_detail LEFT JOIN cust_order ON order_detail.cust_order_id = cust_order.id WHERE cust_order.customer_id = 2

	//string sqlD = "SELECT * FROM order_detail ";

	MYSQL_ROW row;
	MYSQL_RES* res;

	int i = 0;

	stringstream sqlD;

	sqlD << "SELECT order_detail.cust_order_id, order_detail.product_id, order_detail.quantity FROM order_detail LEFT JOIN cust_order ON order_detail.cust_order_id = cust_order.id WHERE cust_order.customer_id = " + to_string(this->custID);
	string s = sqlD.str();
	const char* qO = s.c_str();
	int qD = mysql_query(conn, qO);
	if (!qD) {
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			//order_detail[i].id = stoi(row[0]); // string to int
			order_detail[i].cust_order_id = stoi(row[0]); // string to int
			order_detail[i].product_id = stoi(row[1]); // string to int
			order_detail[i].quantity = stoi(row[2]); // string to int
			i++;
		}
	}
	else {
		cout << "\nPrevious Order Detail Cannnot Be Fetched\n";
	}

	return i;
}

int Customer::fetchPreviousOrder(MYSQL* conn, TableOrder& tb)
{
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
	//sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order.customer_id = " + this->custID;
	sql << "SELECT * FROM order_detail JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order.customer_id = " << this->custID << " GROUP BY cust_order.id";
	string s = sql.str();
	const char* qC = s.c_str();
	int q = mysql_query(conn, qC);
	if (!q) {
		cout << "success";
		res = mysql_store_result(conn);
		cout << "success";
		while (row = mysql_fetch_row(res)) {
		cout << "success";
			string orderID = row[1];
		cout << "success";
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
			prev_order.push_back({ orderID, quantity, price, date, riderID, status });
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
		cout << "\nPrevious Order New Cannnot Be Fetched\n";
	}

	return i;
}

void Customer::viewPreviousOrderDetails(MYSQL* conn, int orderID, int& exist) {
	TableOrder tb;

	exist = 0;

	for (int i = 0; i < prev_order.size(); i++) {
		if (orderID == stoi(prev_order[i][0])) {
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

			tb.setAlignment(2, TableOrder::Alignment::RIGHT);
			tb.setAlignment(3, TableOrder::Alignment::RIGHT);
			tb.setAlignment(4, TableOrder::Alignment::RIGHT);

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
	}
	else {
		for (int i = 0; i < 67; ++i) std::cout << ' ';
		cout << "Invalid choice. Try again.\n";
		for (int i = 0; i < 67; ++i) std::cout << ' ';
		system("pause");
	}
}

// Function to view and user profile
void Customer::viewProfile(function<void()> mainHeader, MYSQL* conn) {
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

		tt.add(this->custUsername);
		tt.add(this->custName);
		tt.add(this->custPhone);
		tt.add(this->custAddress);
		tt.endOfRow();

		cout << tt << endl;

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

	//TextTable tableOrder('-', '|', '+');

	TableCart tableOrder;

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

	tableOrder.setAlignment(2, TableCart::Alignment::RIGHT);
	tableOrder.setAlignment(3, TableCart::Alignment::RIGHT);

	cout << "\n\n";
	for (int i = 0; i < 80; ++i) std::cout << ' ';
	cout << "---Cart---\n";
	cout << tableOrder;

	cout << "\n\n";
	for (int i = 0; i < 80; ++i) std::cout << ' ';
	cout << "Subtotal: RM " << fixed << setprecision(2) << total << endl;

	//order.clear();
}

void Customer::insertOrder(MYSQL* conn, int venID) {
	// 1. create cust_order table first
	// 2. insert details to order_detail

	int totalQuantity = 0;
	double totalPrice = 0;

	time_t t = time(0);
	//char* dt = ctime(&t);
	tm* now = localtime(&t);
	stringstream date;
	//date << to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday);
	date << to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min) + ":" + to_string(now->tm_sec);

	for (int i = 0; i < order.size(); i++) {
		totalQuantity += stoi(order[i][2]);
		totalPrice += stod(order[i][1]);
	}

	stringstream ss;

	// ss << "INSERT INTO order_detail SET username = '" + username + "' WHERE id = " + to_string(this->custID);

	// ss << "INSERT INTO customer (username, password, name, phone) VALUES ('" + username + "', '" + password + "', '" + name + "', '" + noPhone + "')";

	ss << "INSERT INTO cust_order (customer_id, total_quantity, total_price, date) VALUES ('" + to_string(this->custID) + "', '" + to_string(totalQuantity) + "', '" + to_string(totalPrice) + "', '" + date.str() + "')";

	string query = ss.str();
	const char* q = query.c_str();
	int qstate = mysql_query(conn, q);
	//mysql_insert_id(conn);

	int orderID = mysql_insert_id(conn);

	// insert to payment table here
	stringstream sp;

	totalPrice += 4;

	sp << "INSERT INTO payment (order_id, vendor_id, total_payment) VALUES (" + to_string(orderID) + ", " + to_string(venID) + ", " + to_string(totalPrice) + ")";

	string queryP = sp.str();
	const char* qP = queryP.c_str();
	int qPState = mysql_query(conn, qP);

	// insert to delivery table
	stringstream sd;

	sd << "INSERT INTO delivery (payment_id) VALUES (" + to_string(orderID) + ")";

	string queryD = sd.str();
	const char* qD = queryD.c_str();
	int qDState = mysql_query(conn, qD);

	if (!qstate) {

		cout << "\n\n";
		for (int i = 0; i < 80; ++i) std::cout << ' ';
		cout << "Order has been created successfully!\n";
		for (int i = 0; i < order.size(); i++) {
			stringstream ssOrder;
			//ssOrder << "INSERT INTO order_detail (product_id, quantity) VALUES ('" + order[i][3] + "', '" + order[i][2]  + "')";
			ssOrder << "INSERT INTO order_detail (cust_order_id, product_id, quantity, price) VALUES ('" + to_string(orderID) + "', '" + order[i][3] + "', '" + order[i][2] + "', '" + order[i][1] + "')";
		
			string queryDetail = ssOrder.str();
			const char* qD = queryDetail.c_str();
			int qDstate = mysql_query(conn, qD);

			if (!qDstate) {
				//cout << "\nProduct " << i + 1 << " created.\n";
			}

			else {
				//cout << "\nOrder cannot be created!\n";
				//system("pause");
				//break;
			}
		}
		//system("pause");
	}
	else {
		cout << "\n\n";
		for (int i = 0; i < 80; ++i) std::cout << ' ';
		cout << "Order cannot be created!\n";
		system("pause");
	}

}

vector<vector<string>> Customer::getOrder() {
	return this->order;
}
// display previous order that being made by the customer
// get all data from table product, cust_order, payment, delivery.
void Customer::displayPreviousOrder(int i, int totalDetail, TextTable& tt) {
	int id = getID();

	string username, password;

	stringstream s, ss;

	ss << "SELECT * FROM user WHERE username LIKE '" << username << "' AND password LIKE '" << password << "'";

	ss << "SELECT cust_order.id, cust_order.customer_id, cust_order.date, order_detail.product_id, order_detail.quantity FROM `cust_order` INNER JOIN order_detail ON cust_order.id = order_detail.cust_order_id";
	ss << "SELECT cust_order.id FROM cust_order WHERE customer_id = " << id;

	ss << "SELECT * FROM cust_order INNER JOIN order_detail ON cust_order.id = order_detail.cust_order_id INNER JOIN product ON order_detail.product_id = product.id";

	ss << "SELECT * FROM payment";
	s << "SELECT * FROM delivery";

	//TextTable tt;

	stringstream tp;
	tp << fixed << setprecision(2) << cust_order[i].total_price;

	tt.add(to_string(cust_order[i].id));
	tt.add(tp.str());
	tt.add(to_string(cust_order[i].total_quantity));
	tt.endOfRow();

	//cout << cust_order[i].id << " " << fixed << setprecision(2) << cust_order[i].total_price << " " << cust_order[i].total_quantity << endl;

	/*
	for (int k = 0; k < totalDetail; k++) {
		if (order_detail[k].cust_order_id == cust_order[i].id) {
			cout << order_detail[k].product_id << " " << order_detail[k].quantity << endl;
		}
	}
	*/
}

void Customer::displayChart(MYSQL* conn) {

	MYSQL_ROW row;
	MYSQL_RES* res;

	stringstream ss;
	//ss << "SELECT COUNT(id) AS order_made, SUM(total_quantity) AS quantity, SUM(total_price) AS price FROM cust_order WHERE customer_id = GROUP BY customer_id";
	//ss << "SELECT COUNT(id) AS order_made, SUM(total_quantity) AS quantity, SUM(total_price) AS price FROM cust_order WHERE customer_id = 2 GROUP BY date";
	ss << "SELECT COUNT(id) AS order_made, SUM(total_quantity) AS quantity, SUM(total_price) AS price, extract(month FROM date) AS month FROM cust_order WHERE customer_id = 2 GROUP BY month";

	string qs = ss.str();
	const char* q = qs.c_str();
	int qstate = mysql_query(conn, q);

	res = mysql_store_result(conn);

	TableOrder tt;

	tt.add("Month");
	tt.add("Total Order Made");
	tt.add("Total Quantity");
	tt.add("Total Expenses");
	tt.endOfRow();

	while (row = mysql_fetch_row(res)) {
		graph.push_back({ row[0], row[1], row[2], row[3] });
		tt.add(row[3]);
		tt.add(row[0]);
		tt.add(row[1]);
		tt.add(row[2]);
		tt.endOfRow();
	}

	for (int i = 0; i < graph.size(); i++) {
		cout << graph[i][0] << endl;
		cout << graph[i][1] << endl;
		cout << graph[i][2] << endl;
		cout << graph[i][3] << endl;
	}

	cout << tt;

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
