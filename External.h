#pragma once
#ifndef EXTERNAL_H
#define EXTERNAL_H

#include <iostream>

#include "TextTable.h"

#include <tabulate/table.hpp>

using namespace tabulate;
using namespace std;
using Row_t = Table::Row_t;

inline void centerify_output(std::string str, int num_cols) {
    // Calculate left padding
    int padding_left = (num_cols / 2) - (str.size() / 2);

    // Put padding spaces
    for (int i = 0; i < padding_left; ++i) std::cout << ' ';

    // Print the message
    std::cout << str;
}

inline void gotoXY(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

#endif