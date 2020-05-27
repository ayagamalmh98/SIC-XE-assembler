#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>
#include "ObjectCode.h"
#include "convert.h"
#include "operations.h"
#include "structures.h"
#include <cstring>
#include <cstdlib>
#include <string>
#include "Conversions.h"
#include "WriteFile.h"
using namespace std;

map<string, preobj> table;
string locctr = "0";
string name = "";
class pass1 :public ObjectCode {
    int end = 0;
private:
    map<string, int> format;
    map<string, string> opcode;
    map<string, int> numOPerands;
public:
    pass1() {
        format = getformat();
        opcode = getopcode();
        numOPerands = getNumOperands();
        storeRegisters();
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
        bool base = false;
        string B = "";
        if (info.Operator == "WORD") {
            info.objectCode = toHex(toDec(info.Operand));
            while (size(info.objectCode) < 6) {
                string temp = info.objectCode;
                info.objectCode = "0" + temp;
            }
            table.insert(pair<string, preobj>(locctr, info));
            Label_is_Found(info.Label, locctr);
            locctr = toHex(toDec(locctr) + 3);
        }
        else if (info.Operator == "RESW") {
            table.insert(pair<string, preobj>(locctr, info));
            Label_is_Found(info.Label, locctr);
            locctr = toHex(toDec(locctr) + 3 * StringToInt(info.Operand));
        }
        else if (info.Operator == "RESB") {
            table.insert(pair<string, preobj>(locctr, info));
            Label_is_Found(info.Label, locctr);
            cout << toDec(locctr) << endl;
            cout << StringToInt(info.Operand) << endl;
            locctr = toHex(toDec(locctr) + StringToInt(info.Operand));
            cout << locctr << endl;
        }
        else if (info.Operator == "BYTE") {
            int len = info.Operand.size() - 3;
            if (info.Operand[0] == 'X') {
                len /= 2;
                for (int i = 2; i < info.Operand.size() - 1; i++) {
                    info.objectCode += (info.Operand[i]);
                }
            }
            else if (info.Operand[0] == 'C') {
                for (int i = 0; i < len; i++) {
                    char temp = info.Operand[i + 2];
                    info.objectCode.append(toHex((int)temp));
                }
            }
            table.insert(pair<string, preobj>(locctr, info));
            Label_is_Found(info.Label, locctr);
            locctr = toHex(toDec(locctr) + len);
        }
         else if (info.Operator == "EQU") {
            char type = char(0);
            if (info.Operand[0] == '#' || info.Operand[0] == '@') {
                type = info.Operand[0];
                info.Operand.erase(0, 1);
            }
            vector<string> operand = split(info.Operand, ',');
            if (is_number(operand[0])) {
                locctr = getTargetAddress(operand[0], type, locctr);
                table.insert(pair<string, preobj>(locctr, info));
                Label_is_Found(info.Label, operand[0]);
            }
            else if (operand[0] == "*") {
                table.insert(pair<string, preobj>(locctr, info));
                Label_is_Found(info.Label, locctr);
            }
            else if (getTargetAddress(operand[0], type, locctr) != "NotFound") {
                locctr = getTargetAddress(operand[0], type, locctr);
                cout << "loctr =   " << locctr << '\n';
                table.insert(pair<string, preobj>(locctr, info));
                Label_is_Found(info.Label, locctr);
                //locttr=
            }
        }
         else if (info.Operator == "ORG") {
            char type = char(0);
            if (info.Operand[0] == '#' || info.Operand[0] == '@') {
                type = info.Operand[0];
                info.Operand.erase(0, 1);
            }
            string value = getTargetAddress(info.Operand, type, locctr);
            if (value != "NotFound")
                locctr = value;

        }
        else if(info.Operator== "BASE") {
            char type = char(0);
            if (info.Operand[0] == '#' || info.Operand[0] == '@') {
                type = info.Operand[0];
                info.Operand.erase(0, 1);
            }
              string value = getTargetAddress(info.Operand, type, locctr);
              if (value != "NotFound") {
                  base = true;
                  B = value;
              }
         }
         else if(info.Operator== "NOBASE") {
            base = false;
         }
         else if (info.Operator == "END") {
            end = 1;
            table.insert(pair<string, preobj>(locctr, info));
        }
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
                info.Format = Format;
                info.Opcode = Opcode;
                char type = char(0);
                if (info.Operand[0] == '#' || info.Operand[0] == '@') {
                    type = info.Operand[0];
                    info.Operand.erase(0, 1);
                }
                info.objectCode = objectCode(Format, locctr, B, base, Opcode, split(info.Operand, ','), type);
                Label_is_Found(info.Label, locctr);
                table.insert(pair<string, preobj>(locctr, info));
                locctr = toHex(toDec(locctr) + Format);
            }
        }
        printSymbols();
        cout << "objectCode=" << info.objectCode << '\n' << '\n';
    }

    void readFile(string filename) {
        ifstream myfile;
        myfile.open(filename.c_str());
        string line;
        while ((getline(myfile, line))&& (end==0))
            handleLine(line);
        myfile.close();
        bool e = checkAllLabelsAreFound();
        //if e is true then write to object file
        if (e == false)
            cout << "you shouldnt use any label not declared";
        else
            writefile(table, getSymbolTable());
    }

    struct preobj extract(vector<string> data) {
        struct preobj info;
        string Label, Operator, Operand;

        if (( numOPerands[data.at(1)]==0 &&data.size() != 2 )|| (numOPerands[data.at(1)]==1 &&data.size() != 3 )
            || (numOPerands[data.at(1)]==2 &&data.size() == 3 && data.at(2).find(",") == string::npos    )
            ||  (numOPerands[data.at(1)]==2 &&data.size() != 3 )   )
                 cout << "error in operand";
        //end program ?

        if (data.size() == 3) {
            Label = data.at(0);
            Operator = data.at(1);
            Operand = data.at(2);
        }
        else if (data.size() == 2) {
            Label = "";
            Operator = data.at(0);
            Operand = data.at(1);
        }
    std::for_each(Label.begin(), Label.end(), [](char & c) {
		c = ::toupper(c);
	});
	std::for_each(Operand.begin(), Operand.end(), [](char & c) {
		c = ::toupper(c);
	});
	std::for_each(Operator.begin(), Operator.end(), [](char & c) {
		c = ::toupper(c);
	});
        info.Label = Label;
        info.Operand =Operand;
        info.Operator = Operator;
        return info;
    }

};




int main() {
     map<string, preobj> Map;
    struct preobj ob = { "copy","START","1000","141033","43",3 };
    Map.insert(std::pair<string, preobj>("", ob));
    ob = { "FIRST","STL","retadd","141033","43",3 };
    Map.insert(std::pair<string, preobj>("1000", ob));
    ob = { "CLOOP","JSUB","retadd","482039","43",3 };
    Map.insert(std::pair<string, preobj>("1003", ob));
    ob = { "","LDA","retadd","001036","43",3 };
    Map.insert(std::pair<string, preobj>("1006", ob));
    ob = { "","COMP","zero","281030","43",3 };
    Map.insert(std::pair<string, preobj>("1009", ob));
    ob = { "","JEQ","endfile","301015","43",3 };
    Map.insert(std::pair<string, preobj>("100C", ob));
    ob = { "","JSUB","retadd","482036","43",3 };

    Map.insert(std::pair<string, preobj>("100F", ob));
    ob = { "Length","WORD","3","4820361","43",3 };

    Map.insert(std::pair<string, preobj>("1012", ob));
    ob = { "","J","cloop","3C1003","43",3 };
    Map.insert(std::pair<string, preobj>("1015", ob));
    ob = { "endfile","LDA","retadd","00102A","43",3 };
    Map.insert(std::pair<string, preobj>("1018", ob));
    ob = { "","STA","buffer","0C1039","43",3 };
    Map.insert(std::pair<string, preobj>("101B", ob));
    ob = { "","LDA","three","00102D","43",3 };
    Map.insert(std::pair<string, preobj>("101E", ob));
    ob = { "","STA","length","0C1036","43",3 };
    Map.insert(std::pair<string, preobj>("1021", ob));
    ob = { "","JSUB","length","482061","43",3 };
    Map.insert(std::pair<string, preobj>("1024", ob));
    ob = { "","END","1000","","43",3 };
    Map.insert(std::pair<string, preobj>("1027", ob));

    map<string, symbol_info> m;
    vector <string> v;
    v.push_back("1233"); v.push_back("9823"); v.push_back("4567");
    struct symbol_info ob1 = { "0101",v };
    m.insert(std::pair<string,symbol_info>("Length", ob1));
    writefile(Map,m);
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
