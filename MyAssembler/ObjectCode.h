#pragma once
#include "Formats.h"
#include <iostream>
#include "Conversions.h"
using namespace std;

class ObjectCode : public Formats
{
    public:
        string objectCode(int format, string CurrentLOCCTR, string B, bool BASE, string opCode, vector<string> operand, char typeOfOperand);
        vector<string> getOperands(string exp);

};

