#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include "operations.h"
#include "ObjectCode.h"
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;

struct preobj
{
    string Label;
    string Operator;
    string Operand;
    string Opcode;
    int Format;
};


class pass1 {
private:
    map<string, int> format;
    map<string, string> opcode;
    string locctr;
public:
    pass1() {
        format = getformat();
        opcode = getopcode();
    }
    string incrmtTheCntr(string op, string opr) {
        if (op[0] == '+') {
            locctr = toHex(toDec(locctr) + 4);
            return locctr;
        }

        if (opcode.find(op) != opcode.end())
            locctr = toHex(toDec(locctr) + format[op]);


        if (op == "WORD")
            locctr = toHex(toDec(locctr) + 3);

        else if (op == "RESW")
            locctr = toHex(toDec(locctr) + 3 * (atoi(opr.c_str())));

        else if (op == "RESB")
            locctr = toHex(toDec(locctr) + atoi(opr.c_str()));

        else if (op == "BYTE") {
            int len = opr.size() - 3;
            if (opr[0] == 'X') len /= 2;
            locctr = toHex(toDec(locctr) + len);
        }
        return locctr;
    }
    void handleLine(string line) {
        vector<string> data = noSpace(line);
        struct preobj info = extract(data);

    }

    void readFile(string filename) {
        ifstream myfile;
        myfile.open(filename.c_str());
        string line;
        while (getline(myfile, line))
            handleLine(line);
        myfile.close();
    }

    struct preobj extract(vector<string> data) {
        struct preobj info;
        string Label, Operator, Operand, Opcode;
        int Format;
        if (data.size() == 3) {
            Label = data.at(0);
            Operator = data.at(1);
            Operand = data.at(2);
        }
        else if (data.size() == 2) {
            Label = "";
            Operator = data.at(0);
            Operand = data.at(2);
        }
        string Operator2 = Operator;
        if (Operator2[0] == '+')
            Operator2.erase(0, 1);
        if (format.find(Operator2) == format.end())//Wrong Operator
            Operator = "";
        else {
            Format = format.at(Operator2);
            if (Format == 3) {
                if (Operator[0] == '+')
                    Format = 4;
            }
            Opcode = opcode.at(Operator2);
        }
        info.Format = Format;
        info.Label = Label;
        info.Opcode = Opcode;
        info.Operand = Operand;
        info.Operator = Operator;
        return info;
    }



    string toHex(int a) {
        string lookup = "0123456789ABCDEF";
        string res = "";
        while (a)
        {
            res = lookup[a % 16] + res;
            a /= 16;
        }
        return res;
    }

    int toDecimal_digit(char a) {
        if (a == '0') return 0;
        if (a == '1') return 1;
        if (a == '2') return 2;
        if (a == '3') return 3;
        if (a == '4') return 4;
        if (a == '5') return 5;
        if (a == '6') return 6;
        if (a == '7') return 7;
        if (a == '8') return 8;
        if (a == '9') return 9;
        if (a == 'A') return 10;
        if (a == 'B') return 11;
        if (a == 'C') return 12;
        if (a == 'D') return 13;
        if (a == 'E') return 14;
        if (a == 'F') return 15;
    }


    char toHex_digit(int a) {
        if (a == 0) return '0';
        if (a == 1) return '1';
        if (a == 2) return '2';
        if (a == 3) return '3';
        if (a == 4) return '4';
        if (a == 5) return '5';
        if (a == 6) return '6';
        if (a == 7) return '7';
        if (a == 8) return '8';
        if (a == 9) return '9';
        if (a == 10) return 'A';
        if (a == 11) return 'B';
        if (a == 12) return 'C';
        if (a == 13) return 'D';
        if (a == 14) return 'E';
        if (a == 15) return 'F';
    }


    int toDec(string s) {
        int mul = 1, res = 0;
        for (int n = s.size() - 1; n >= 0; --n) {
            res += toDecimal_digit(s[n]) * mul;
            mul *= 16;
        }
        return res;
    }
};


int main()
{
   /* pass1 e;
    cout << e.incrmtTheCntr("ADD", "a") << "\n";
    cout << e.incrmtTheCntr("+ADD", "a") << "\n";*/

    string h[2] = {"RETADR", "0030"};
    ObjectCode CC;
    string out = CC.objectCode(3, "0", "", false, "00010100", h, ' ');
    cout<<out<<endl;

    return 0;
}





