#pragma once
#ifndef EXTERNAL_H
#define EXTERNAL_H

#include <iostream>

#include <tabulate/table.hpp>

using namespace tabulate;
using namespace std;

inline void centerify_output(std::string str, int num_cols) {
    // Calculate left padding
    int padding_left = (num_cols / 2) - (str.size() / 2);

    // Put padding spaces
    for (int i = 0; i < padding_left; ++i) std::cout << ' ';

    // Print the message
    std::cout << str;
}

#endif