/***
* Brian Tran
* btran54
* 2024 Winter CSE101 PA7
* Order.cpp
* Order CPP file containing the main execution of Dictionary.cpp functions
***/

#include <iostream>
#include <fstream>
#include <string>
#include "Dictionary.h"

int main (int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Invalid Arguments.\nUsage: " << argv[0] << "<input file> <output file>" << std::endl;
        return 1;
    }

    std::ifstream in(argv[1]);
    std::ofstream out(argv[2]);

    if (!in) {
        std::cerr << "Failed to open file " << argv[1] << " for reading.\n" << std::endl;
        return 1;
    }

    if (!out) {
        std::cerr << "Failed to open file " << argv[2] << " for writing.\n" << std::endl;
        return 1;
    }

    Dictionary dict;
    std::string line;
    int lineNum = 1;

    while (std::getline(in, line)) {
        dict.setValue(line, lineNum);
        lineNum++;
    }

    out << dict.to_string() << std::endl;
    out << dict.pre_string() << std::endl;

    in.close();
    out.close();

    return 0;
}