#pragma once
#include "Formats.h"
#include <iostream>
#include "Conversions.h"
#include <vector>
using namespace std;

        string objectCode(int format, string CurrentLOCCTR, string Bcontent, bool BASE, string opCode, vector<string>operand, char typeOfOperand);
        vector<string> getOperands(string exp);


