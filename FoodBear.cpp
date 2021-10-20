// FoodBear.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Customer.h"

using namespace std;

int main()
{
    cout << "Hello World!\nasdsasdadasdadasdasdadasd\nsdadfadfadadasdasdasdasdsa\n";
    cout << "Welcome to FoodBear Delivery\nNow all on fingertips\n";

    int a;

    cin >> a;

    Customer cust = Customer("Hello1", "hello123");

    string id = "Hello1";
    string pass = "hello123";

    if (a == 1) {

        system("cls");
        cout << "1!\n";
    }
    else {
        if (cust.login()) {
            cout << "Login succesfull";
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
