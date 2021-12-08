#include "Rider.h"

Rider::Rider()
{
}

Rider::~Rider()
{
}

bool Rider::login(string user, string pass, int totalRider)
{
	boost::to_lower(user);
	pass = sha256(pass);
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
	boost::to_lower(username);
	cout << "Enter Password: ";
	cin >> password;
	password = sha256(password);
	cout << "Enter Your Name: ";
	cin.ignore();
	getline(cin, name);
	noPhone = inputNoPhone();

	stringstream ss;
	ss << "INSERT INTO rider (username, password, name, phone) VALUES ('" + username + "', '" + password + "', '" + name + "', '" + noPhone + "')";

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

string Rider::inputNoPhone() {
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
	/*
	cout << "\tid" << "\tusername" << "\tpassword" << "\tname" << "\t\tPhone" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < total; i++) {
		cout << "\t" << data[i].id << "\t" << data[i].username << "\t\t" << data[i].password << "\t\t" << data[i].name << "\t" << data[i].phone << endl;
	}
	cout << endl;
	*/
	return total;
}

// Function to view and user profile
void Rider::viewProfile(function<void()> mainHeader, MYSQL* conn) {
	int operation;

	string pass, username, name, phone;

	TextTable tt;

	do {
		mainHeader();
		tt.add("Username");
		tt.add("Name");
		tt.add("No. Phone");
		tt.endOfRow();

		tt.add(this->riderUsername);
		tt.add(this->riderName);
		tt.add(this->riderPhone);
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

void Rider::selectOrderDetails(MYSQL* conn, int orderID, int& exist) {
	TableOrder tb;

	exist = 0;

	for (int i = 0; i < order.size(); i++) {
		if (orderID == stoi(order[i][0])) {
			exist = 1;
		}
	}

	if (exist == 1) {

		MYSQL_ROW row;
		MYSQL_RES* res;

		stringstream sql;
		sql << "SELECT cust_order.date, product.name, order_detail.quantity, order_detail.price, payment.total_payment FROM order_detail JOIN product ON order_detail.product_id = product.id JOIN cust_order ON order_detail.cust_order_id = cust_order.id JOIN payment ON payment.order_id = cust_order.id JOIN delivery ON delivery.payment_id = payment.order_id WHERE cust_order_id = " << orderID;
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

vector<vector<string>> Rider::getOrder()
{
	return this->order;
}

void Rider::fetchOrder(vector<vector<string>> order) {
	this->order = order;
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
