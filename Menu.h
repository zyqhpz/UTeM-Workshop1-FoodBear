#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>

using namespace std;

void color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoXY(int x, int y) {
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void select() {
	int Set[] = { 7, 7, 7 };
	int counter = 2;
	char key;

	for (int i = 0;;) {
		key = _getch();

		gotoXY(10, 5);
		color(Set[0]);
		cout << "1. Menu";

		gotoXY(10, 6);
		color(Set[1]);
		cout << "2. Menu 2";

		gotoXY(10, 7);
		color(Set[2]);
		cout << "3. Menu 3";

		if (key == 72 && (counter >= 2 && counter <= 3))
			counter--;
		if (key == 80 && (counter >= 1 && counter <= 2))
			counter++;

		if (key == '\r') {
			if (counter == 1) {

				cout << "menu 1 opened";
				break;
				system("pause");
			}
			if (counter == 2)
				cout << "menu 2 opened";
			if (counter == 3)
				cout << "menu 3 opened";
		}

		Set[0] = 7;
		Set[1] = 7;
		Set[2] = 7;

		if (counter == 1) {
			Set[0] = 12;
		}

		if (counter == 2) {
			Set[1] = 12;
		}

		if (counter == 3) {
			Set[2] = 12; // 12 color red
		}
	}
}

#endif // !MENU_H
