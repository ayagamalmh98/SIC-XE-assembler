#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


#include<string>
using namespace std;
struct symbol_info {
    string address;
    vector<std::string> reff;
};
struct preobj {
    string Label = "";
    string Operator = "";
    string Operand = "";
    string objectCode = "";
    string Opcode = "";
    int Format = 0;
    string locctr = "";
};


