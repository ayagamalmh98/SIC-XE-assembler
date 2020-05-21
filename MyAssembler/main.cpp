#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>
#include "ObjectCode.h"
#include "convert.h"
#include "operations.h"
using namespace std;


struct preobj {
    string Label = "";
    string Operator = "";
    string Operand = "";
    string objectCode = "";
};

struct symbol_info {
    string address;
    vector<string> reff;
};

map<string, symbol_info> SYMTAB;
map<string, preobj> table;
string locctr = "0";
string name = "";

class pass1 :public ObjectCode {
private:
    map<string, int> format;
    map<string, string> opcode;
public:
    pass1() {
        format = getformat();
        opcode = getopcode();
        SYMTAB = getRegisters();
    }
    void handleLine(string line) {
        vector<string> data = noSpace(line);
        struct preobj info = extract(data);
        cout << "loctr =   " << locctr << '\n';
        cout << info.Label << "\n";
        cout << info.Operator << "\n";
        cout << info.Operand << "\n";
        //check if END
        if (info.Operator == "START") {
            locctr = info.Operand;
            if (info.Label != "")
                name = info.Label;
            table.insert(pair<string, preobj>(locctr, info));
            return;
        }
        calcObjectCode(info);
    }

    void calcObjectCode(preobj info) {
        if (info.Operator == "WORD") {
            info.objectCode = toHex(toDec(info.Operand));
            table.insert(pair<string, preobj>(locctr, info));
            StoreLabels(info.Label, info.Operand);
            locctr = toHex(toDec(locctr) + 3);
        }
        else if (info.Operator == "RESW") {
            table.insert(pair<string, preobj>(locctr, info));
            StoreLabels(info.Label, info.Operand);
            locctr = toHex(toDec(locctr) + 3 * toDec(info.Operand));
        }
        else if (info.Operator == "RESB") {
            table.insert(pair<string, preobj>(locctr, info));
            StoreLabels(info.Label, info.Operand);
            locctr = toHex(toDec(locctr) + 3 * toDec(info.Operand));
        }
        else if (info.Operator == "BYTE") {
            int len = info.Operand.size() - 3;
            if (info.Operand[0] == 'X') {
                len /= 2;
                //objectCode
            }
            else if (info.Operand[0] == 'C') {
                for (int i = 0; i < len; i++) {
                    char temp = info.Operand[i + 2];
                    info.objectCode.append(toHex((int)temp));
                }
            }
            table.insert(pair<string, preobj>(locctr, info));
            StoreLabels(info.Label, info.Operand);
            locctr = toHex(toDec(locctr) + len);
        }
        /* else if(info.Operator== "BASE") {
         }
         else if(info.Operator== "NOBASE") {
         }*/
        else {
            string Operator2 = info.Operator;
            if (Operator2[0] == '+')
                Operator2.erase(0, 1);
            if (format.find(Operator2) == format.end())//Wrong Operator
                info.Operator = "";
            // message wrong
            //return
            else {
                int Format = format.at(Operator2);
                if (Format == 3) {
                    if (info.Operator[0] == '+')
                        Format = 4;
                }
                string Opcode = opcode.at(Operator2);
                char type = char(0);
                if (info.Operand[0] == '#' || info.Operand[0] == '@') {
                    type = info.Operand[0];
                    info.Operand.erase(0, 1);
                }
                bool BASE = false;
                string B = "";
                info.objectCode = objectCode(Format, locctr, B, BASE, Opcode, getOperands(info.Operand), type);
                cout << info.objectCode << endl;
                table.insert(pair<string, preobj>(locctr, info));
                locctr = toHex(toDec(locctr) + Format);
            }
        }
        printSymbols();
        cout << "objectCode=" << info.objectCode << '\n' << '\n';
    }

    void StoreLabels(string label, string address) {
        if (label == "")
            return;
        if (SYMTAB.find(label) != SYMTAB.end()) {
            cout << "symbol is Found";
            SYMTAB.at(label).address = address;
            // go to vector
        }
        else {
            symbol_info n;
            n.address = address;
            SYMTAB.insert(pair<string, symbol_info>(label, n));
        }
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
        string Label, Operator, Operand;
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
        info.Label = Label;
        info.Operand = Operand;
        info.Operator = Operator;
        return info;
    }
    void printSymbols() {
        map<string, symbol_info>::iterator itr;
        for (itr = SYMTAB.begin(); itr != SYMTAB.end(); ++itr) {
            cout << '\t' << itr->first
                << '\t' << itr->second.address << '\n';
        }
    }
    map<string, symbol_info> getRegisters() {
        map<string, symbol_info> registers;
        string labels[9] = { "A","x","l","B","S","T","F","PC","SW" };
        string add[9] = { "0","1","2","3","4","5","6","8","9" };
        for (int i = 0; i < 9; i++) {
            symbol_info t;
            string label = labels[i];
            t.address = add[i];
            registers.insert(pair<string, symbol_info>(label, t));
        }
        return registers;
    }
};

int main() {
    pass1 e;
    e.readFile("assembler.txt");
   /* ObjectCode k;
    vector<string> data;
    data.push_back("RDREC");
    data.push_back("1036");
    string o = k.objectCode(4,"","",false,"48",data,' ');
    cout<<o<<endl;*/
    return 0;
}