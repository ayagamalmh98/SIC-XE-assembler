#pragma once
#include "Formats.h"
#include <iostream>
#include "Conversions.h"
using namespace std;

class ObjectCode : public Formats
{
    public:
        string objectCode(int format, string CurrentLOCCTR, string Bcontent, bool BASE, string opCode, string operand[], char typeOfOperand);

};

