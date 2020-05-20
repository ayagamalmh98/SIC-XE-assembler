#include "Formats.h"


int x = 0;
int b = 0;
int p = 0;
int e = 0;

string Formats::XBPE()
{
    string xbpe = "";
    xbpe += to_string(x);
    xbpe += to_string(b);
    xbpe += to_string(p);
    xbpe += to_string(e);
	return xbpe;
}


string Formats::Displacement(string TAOpeanrd, bool X, string Bcontent, bool BASE, string CurrentLOCCTR)
{
    int TA = HexToDecimal(TAOpeanrd);
    x = 0;
    if (X) {
        x = 1;
    }
    
    int locctr = HexToDecimal(CurrentLOCCTR);
    int PC = locctr + 3;
    int displ = TA - PC;
    p = 1;
    b = 0;
    if (displ > 2047 || displ < -2048) {
        if (BASE) {
            int B = HexToDecimal(Bcontent);
            displ = TA - B;
            p = 0;
            b = 1;
            if (displ > 4095 || displ < 0) {
                throw "Displacement out of range!!";
            }
        }
        else {
            throw "Displacement out of range!!";
        }
    }
    string displa = decimal_to_binary(displ);
  //  string displac = HexToBin(displa);
  //  string displace = neg(displac);
    string displacement = convertBinToHex(displa, true);
    return displacement;
}


string Formats::Format2()
{
	return string();
}

string Formats::Format3(string TAOpeanrd, bool X, string Bcontent, bool BASE, string CurrentLOCCTR)
{
    string displacement = Displacement(TAOpeanrd, X, Bcontent, BASE, CurrentLOCCTR);
    string xbpe = XBPE();
    string xbpeHex = convertBinToHex(xbpe, false);
    string xbpeDisplacement = xbpeHex + displacement;
    return xbpeDisplacement;
}

string Formats::Format4()
{
	return string();
}


string Formats::setup(string TAOpeanrd, bool X, string Bcontent, bool BASE, int format, string CurrentLOCCTR)
{
    if (format == 2)
    {
        return Format2();
    }
    else if (format == 3) 
    {
        e = 0;
        return Format3(TAOpeanrd, X, Bcontent, BASE, CurrentLOCCTR);
    }
    else if (format == 4) {
        e = 1;
        return Format4();
    }
}