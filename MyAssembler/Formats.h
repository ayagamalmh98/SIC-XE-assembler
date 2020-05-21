#pragma once
#include <iostream>
#include<cstring>
#include <unordered_map>
#include<string>
#include "Conversions.h"
using namespace std;

class Formats : public Conversions
{
    public:
        string XBPE();
        string Displacement(string TAOpeanrd, bool X, string Bcontent, bool BASE, string CurrentLOCCTR);
        string Format2();
        string Format3(string TAOpeanrd, bool X, string Bcontent, bool BASE, string CurrentLOCCTR);
        string Format4(string TAOpeanrd);
        string setup(string TAOpeanrd, bool X, string Bcontent, bool BASE, int format, string CurrentLOCCTR);
    private:
        char x;
        char b = 0;
        char p = 0;
        char e = 0;
        char format;
};

