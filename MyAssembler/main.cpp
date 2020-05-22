#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <D:\Assemblerproject\Conversions.h>
#include<sstream>
#include "ObjectCode.h"
#include "convert.h"
#include "operations.h"

using namespace std;

struct preobj
{
    string Label;
    string Operator;
    string Operand;
    string Opcode;
    int Format;
    string objectCode;
};

string decTohexa(int n)
{
    char hexaDeciNum[100];
    string s = "";
    int i = 0;
    while (n != 0)
    {
        int temp = 0;
        temp = n % 16;
        if (temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        n = n / 16;
    }

    for (int j = i - 1; j >= 0; j--) {
        s=s+ hexaDeciNum[j];
    }
    return s;
}

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

void writefile(map<string, preobj> Map,map<string,symbol_info>modification) {    //object file

//(Map) is the map where we store all instructions
    
    ofstream file_;
    file_.open("myfile.txt");
    map<string, preobj>::iterator it;
    vector<string> arr; int i = 0;
    for (it = Map.begin(); it != Map.end(); ++it) {
        if (i == 0) {
            i++;
        }
        else {
            arr.push_back(it->first);
            i++;
        }
    }

    for (it = Map.begin(); it != Map.end(); ++it) {     //Header record
        if (Map[it->first].Operator == "start") {
            file_ << "H ";        //printing location counter
            file_ << Map[it->first].Label;
            file_ << " 00";
            file_ << Map[it->first].Operand;
            file_ << " 00";
            int num1, num2;
            std::stringstream str_strm1(arr.at(0));
            std::stringstream str_strm2(arr.at(Map.size() - 2));
            std::stringstream str_hex1;
            std::stringstream str_hex2;
            str_strm1 >> std::hex >> num2;
            str_strm2 >> std::hex >> num1;
            int num = num1 - num2 + 1;
            string s = decTohexa(num);
            for (i = 0; i < 4 - s.size(); i++) {
                file_ << "0";
            }
            file_ << s;
            file_ << "\n";
        }
    }



    it = Map.begin();
    while (Map[it->first].Operator != "end") {      //Text records
        if (Map[it->first].Operator != "start") {
            int counter = 0; int length = 0;
            map<string, preobj>::iterator save = it;
            while (save != Map.end() && Map[save->first].Operator != "start" && Map[save->first].Operator != "end" && (counter + Map[save->first].Format) <= 30) {
                counter += Map[it->first].Format;
                std::advance(save, 1);
            }
            cout << counter << endl;
            length = counter; counter = 0;
            if (it != Map.end() && Map[it->first].Operator != "start" && Map[it->first].Operator != "end" && Map[it->first].Operator != "byte" && Map[it->first].Operator != "word" && Map[it->first].Operator != "resb" && Map[it->first].Operator != "resw" && (counter + Map[it->first].Format) <= 30) {
                file_ << "T 00";
                file_ << it->first;
                file_ << " ";
                if (decTohexa(length).size() < 2) {
                    file_ << "0";
                }
                file_ << decTohexa(length);
                file_ << " ";
                file_ << Map[it->first].Opcode;
                counter += Map[it->first].Format;
                std::advance(it, 1);
            }
            while (it != Map.end() && Map[it->first].Operator != "start" && Map[it->first].Operator != "end" && (counter + Map[it->first].Format) <= 30) {
                if (Map[it->first].Operator != "byte" || Map[it->first].Operator == "word" || Map[it->first].Operator == "resb" || Map[it->first].Operator == "resw") {
                   vector<string>modify=modification[ Map[it->first].Label].reff;
                   if (modify.size() == 0 && Map[it->first].Operator != "byte") {
                       file_ << " ";
                       file_ << Map[it->first].Opcode;
                       counter += Map[it->first].Format;
                       std::advance(it, 1);
                   }
                   else if (modify.size() == 0 && Map[it->first].Operator != "word") {
                       file_ << " ";
                       file_ << Map[it->first].Opcode;
                       counter += Map[it->first].Format;
                       std::advance(it, 1);
                   }
                   else if (modify.size() == 0) {
                       std::advance(it, 1);
                   }
                   else {
                       int j;
                       for (j = 0; j < modify.size(); j++) {
                           file_ << "\nT 00";
                           file_<<modify.at(j);
                           file_ << " 02 ";
                           file_ << modification[Map[it->first].Label].address;
                       }

                       counter = 40;
                       std::advance(it, 1);
                   }
                }
                else {
                    file_ << " ";
                    file_ << Map[it->first].Opcode;
                    counter += Map[it->first].Format;
                    std::advance(it, 1);
                }
            }
            file_ << "\n";
        }
        else {
            std::advance(it, 1);
        }

    }

    it = Map.begin();
    string raddr = it->first;
    while (it != Map.end()) {        //end record

        if (Map[it->first].Operator == "end") {
            file_ << "E 00";
            if (Map[it->first].Operand == "") {
                file_ << raddr;
            }
            else {
                file_ << Map[it->first].Operand;
            }
            break;
        }
        else {
            it++;
        }

    }

    file_.close();
    std::cin.get();
}

int main() {
    map<string, preobj> Map;
    struct preobj ob = { "copy","start","1000","141033",3 };
    Map.insert(std::pair<string, preobj>("", ob));
    ob = { "first","stl","retadd","141033",3 };
    Map.insert(std::pair<string, preobj>("1000", ob));
    ob = { "cloop","jsub","retadd","482039",3 };
    Map.insert(std::pair<string, preobj>("1003", ob));
    ob = { "","lda","retadd","001036",3 };
    Map.insert(std::pair<string, preobj>("1006", ob));
    ob = { "","comp","zero","281030",3 };
    Map.insert(std::pair<string, preobj>("1009", ob));
    ob = { "","jeq","endfile","301015",3 };
    Map.insert(std::pair<string, preobj>("100C", ob));
    ob = { "","jsub","retadd","4820361",3 };

    Map.insert(std::pair<string, preobj>("100F", ob));
    ob = { "Length","word","3","4820361",3 };

    Map.insert(std::pair<string, preobj>("1012", ob));
    ob = { "","j","cloop","3C1003",3 };
    Map.insert(std::pair<string, preobj>("1015", ob));
    ob = { "endfile","lda","retadd","00102A",3 };
    Map.insert(std::pair<string, preobj>("1018", ob));
    ob = { "","sta","buffer","0C1039",3 };
    Map.insert(std::pair<string, preobj>("101B", ob));
    ob = { "","lda","three","00102D",3 };
    Map.insert(std::pair<string, preobj>("101E", ob));
    ob = { "","sta","length","0C1036",3 };
    Map.insert(std::pair<string, preobj>("1021", ob));
    ob = { "","jsub","length","482061",3 };
    Map.insert(std::pair<string, preobj>("1024", ob));
    ob = { "","end","1000","",3 };
    Map.insert(std::pair<string, preobj>("1027", ob));

    map<string, symbol_info> m;
    vector <string> v;
    v.push_back("ABCD");
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
