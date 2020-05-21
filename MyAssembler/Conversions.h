#pragma once
#include <map>
#include <iostream>
using namespace std;

class Conversions
{
    public:
        void createMap(map<string, char>* um);
        string convertBinToHex(string bin, bool add);
        string DecToHex(int n);
        int HexToDecimal(string hexVal);
        char* StringToChar(string s);
        string CharToString(char* a);
        string neg(string bin);
        string HexToBin(string hexdec);
        string decimal_to_binary(int n);
};