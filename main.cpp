#include <iostream>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <utility>
#include <ctype.h>
#include <string>
#include <cstdio>
#include <vector>
#include <time.h>
#include <map>
using namespace std;



struct opcode_info {
    string opcode;
    int format;
};


struct reg_info {
    int num;
};
struct symbol_info {
     string addr;
     vector<string> ref;

};
string locctr;
map<string,opcode_info> OPTAB;
map<string,reg_info> REGISTER;
map<string,symbol_info> SYMTAB;

void incrmtTheCntr(string opCode , string opr){
	if(opCode[0] == '+'){
        locctr = toHex(toDec(locctr) + 4);
        return;
	}
		
	if(OPTAB.find(opCode)!= OPTAB.end())
		locctr = toHex(toDec(locctr) + OPTAB[opCode].format);
		

	if(opCode == "WORD") 
        locctr = toHex(toDec(locctr) + 3);
            
	else if(opCode == "RESW")  
        locctr = toHex(toDec(locctr) + 3*(atoi(opr.c_str())));
       
	else if(opCode == "RESB")  
        locctr = toHex(toDec(locctr) + atoi(opr.c_str()));
     
	else if(opCode == "BYTE") {
		int len = opr.size()-3;
		if(opr[0] == 'X') len /= 2;
		locctr = toHex(toDec(locctr) + len);
	}
}

string toHex(int a) {
    string lookup = "0123456789ABCDEF";
    string res = "";
    while(a)
    {
        res = lookup[a%16] + res;
        a /= 16;
    }
    return res;
}

int toDecimal_digit(char a) {
    if(a=='0') return 0;
    if(a=='1') return 1;
    if(a=='2') return 2;
    if(a=='3') return 3;
    if(a=='4') return 4;
    if(a=='5') return 5;
    if(a=='6') return 6;
    if(a=='7') return 7;
    if(a=='8') return 8;
    if(a=='9') return 9;
    if(a=='A') return 10;
    if(a=='B') return 11;
    if(a=='C') return 12;
    if(a=='D') return 13;
    if(a=='E') return 14;
    if(a=='F') return 15;
}


char toHex_digit(int a) {
    if(a==0) return '0';
    if(a==1) return '1';
    if(a==2) return '2';
    if(a==3) return '3';
    if(a==4) return '4';
    if(a==5) return '5';
    if(a==6) return '6';
    if(a==7) return '7';
    if(a==8) return '8';
    if(a==9) return '9';
    if(a==10) return 'A';
    if(a==11) return 'B';
    if(a==12) return 'C';
    if(a==13) return 'D';
    if(a==14) return 'E';
    if(a==15) return 'F';
}


int toDec(string s) {
    int mul = 1, res = 0;
    for(int n=s.size()-1; n>=0; --n) {
        res += toDecimal_digit(s[n])*mul;
        mul *= 16;
    }
    return res;
}




void generateTables() {
OPTAB["ADD"].opcode="18";
OPTAB["ADD"].format=3;

OPTAB["ADDF"].opcode="58";
OPTAB["ADDF"].format=3;

OPTAB["ADDR"].opcode="90";
OPTAB["ADDR"].format=2;

OPTAB["AND"].opcode="40";
OPTAB["AND"].format=3;

OPTAB["CLEAR"].opcode="B4";
OPTAB["CLEAR"].format=2;

OPTAB["COMP"].opcode="28";
OPTAB["COMP"].format=3;

OPTAB["COMPF"].opcode="88";
OPTAB["COMPF"].format=3;

OPTAB["COMPR"].opcode="A0";
OPTAB["COMPR"].format=2;

OPTAB["DIV"].opcode="24";
OPTAB["DIV"].format=3;

OPTAB["DIVF"].opcode="64";
OPTAB["DIVF"].format=3;

OPTAB["DIVR"].opcode="9C";
OPTAB["DIVR"].format=2;

OPTAB["FIX"].opcode="C4";
OPTAB["FIX"].format=1;

OPTAB["FLOAT"].opcode="C0";
OPTAB["FLOAT"].format=1;

OPTAB["HIO"].opcode="F4";
OPTAB["HIO"].format=1;

OPTAB["J"].opcode="3C";
OPTAB["J"].format=3;

OPTAB["JEQ"].opcode="30";
OPTAB["JEQ"].format=3;

OPTAB["JGT"].opcode="34";
OPTAB["JGT"].format=3;

OPTAB["JLT"].opcode="38";
OPTAB["JLT"].format=3;

OPTAB["JSUB"].opcode="48";
OPTAB["JSUB"].format=3;

OPTAB["LDA"].opcode="00";
OPTAB["LDA"].format=3;

OPTAB["LDB"].opcode="68";
OPTAB["LDB"].format=3;

OPTAB["LDCH"].opcode="50";
OPTAB["LDCH"].format=3;

OPTAB["LDF"].opcode="70";
OPTAB["LDF"].format=3;

OPTAB["LDL"].opcode="08";
OPTAB["LDL"].format=3;

OPTAB["LDS"].opcode="6C";
OPTAB["LDS"].format=3;

OPTAB["LDT"].opcode="74";
OPTAB["LDT"].format=3;

OPTAB["LDX"].opcode="04";
OPTAB["LDX"].format=3;

OPTAB["LPS"].opcode="D0";
OPTAB["LPS"].format=3;

OPTAB["MUL"].opcode="20";
OPTAB["MUL"].format=3;

OPTAB["MULF"].opcode="60";
OPTAB["MULF"].format=3;

OPTAB["MULR"].opcode="98";
OPTAB["MULR"].format=2;

OPTAB["NORM"].opcode="C8";
OPTAB["NORM"].format=1;

OPTAB["OR"].opcode="44";
OPTAB["OR"].format=3;

OPTAB["RD"].opcode="D8";
OPTAB["RD"].format=3;

OPTAB["RMO"].opcode="AC";
OPTAB["RMO"].format=2;

OPTAB["RSUB"].opcode="4C";
OPTAB["RSUB"].format=3;

OPTAB["SHIFTL"].opcode="A4";
OPTAB["SHIFTL"].format=2;

OPTAB["SHIFTR"].opcode="A8";
OPTAB["SHIFTR"].format=2;

OPTAB["SIO"].opcode="F0";
OPTAB["SIO"].format=1;

OPTAB["SSK"].opcode="EC";
OPTAB["SSK"].format=3;

OPTAB["STA"].opcode="0C";
OPTAB["STA"].format=3;

OPTAB["STB"].opcode="78";
OPTAB["STB"].format=3;

OPTAB["STCH"].opcode="54";
OPTAB["STCH"].format=3;

OPTAB["STF"].opcode="80";
OPTAB["STF"].format=3;

OPTAB["STI"].opcode="D4";
OPTAB["STI"].format=3;

OPTAB["STL"].opcode="14";
OPTAB["STL"].format=3;

OPTAB["STS"].opcode="7C";
OPTAB["STS"].format=3;

OPTAB["STSW"].opcode="E8";
OPTAB["STSW"].format=3;

OPTAB["STT"].opcode="84";
OPTAB["STT"].format=3;

OPTAB["STX"].opcode="10";
OPTAB["STX"].format=3;

OPTAB["SUB"].opcode="1C";
OPTAB["SUB"].format=3;

OPTAB["SUBF"].opcode="5C";
OPTAB["SUBF"].format=3;

OPTAB["SUBR"].opcode="94";
OPTAB["SUBR"].format=2;

OPTAB["SVC"].opcode="B0";
OPTAB["SVC"].format=2;

OPTAB["TD"].opcode="E0";
OPTAB["TD"].format=3;

OPTAB["TIO"].opcode="F8";
OPTAB["TIO"].format=1;

OPTAB["TIX"].opcode="2C";
OPTAB["TIX"].format=3;

OPTAB["TIXR"].opcode="B8";
OPTAB["TIXR"].format=2;

OPTAB["WD"].opcode="DC";
OPTAB["WD"].format=3;

REGISTER["A"].num=0;
REGISTER["X"].num=1;
REGISTER["L"].num=2;
REGISTER["B"].num=3;
REGISTER["S"].num=4;
REGISTER["T"].num=5;
REGISTER["F"].num=6;
REGISTER["PC"].num=8;
REGISTER["SW"].num=9;

}

int regNum(char x) {
    switch(x){
        case 'A':
            return 0;
            break;
        case 'X':
            return 1;
            break;
        case 'L':
            return 2;
            break;
        case 'B':
            return 3;
            break;
        case 'S':
            return 4;
            break;
        case 'T':
            return 5;
            break;
        case 'F':
            return 6;
            break;
        default:
            return -1;
    }
}



int main()
{
    cout << toHex(17) << endl;
    return 0;
}
