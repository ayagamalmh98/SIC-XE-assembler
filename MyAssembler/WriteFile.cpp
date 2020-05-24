#include "WriteFile.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include<sstream>
#include "convert.h"

void  writefile(map<string, preobj> Map, map<string, symbol_info>modification) {    //object file
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
        if (Map[it->first].Operator == "START") {
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
    while (Map[it->first].Operator != "END") {      //Text records
        if (Map[it->first].Operator != "START") {
            int counter = 0; int length = 0;
            map<string, preobj>::iterator save = it;
            while (save != Map.end() && Map[save->first].Operator != "STARt" && Map[save->first].Operator != "END" && (counter + Map[save->first].Format) <= 30) {
                counter += Map[it->first].Format;
                std::advance(save, 1);
            }
            cout << counter << endl;
            length = counter; counter = 0;
            if (it != Map.end() && Map[it->first].Operator != "START" && Map[it->first].Operator != "END" && Map[it->first].Operator != "BYTE" && Map[it->first].Operator != "WORD" && Map[it->first].Operator != "RESB" && Map[it->first].Operator != "RESW" && (counter + Map[it->first].Format) <= 30) {
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
            while (it != Map.end() && Map[it->first].Operator != "START" && Map[it->first].Operator != "END" && (counter + Map[it->first].Format) <= 30) {
                if (Map[it->first].Operator != "BYTE" || Map[it->first].Operator == "WORD" || Map[it->first].Operator == "RESB" || Map[it->first].Operator == "RESW") {
                    vector<string>modify = modification[Map[it->first].Label].reff;
                    if (modify.size() == 0 && Map[it->first].Operator != "BYTE") {
                        file_ << " ";
                        file_ << Map[it->first].Opcode;
                        counter += Map[it->first].Format;
                        std::advance(it, 1);
                    }
                    else if (modify.size() == 0 && Map[it->first].Operator != "WORD") {
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
                            file_ << modify.at(j);
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

        if (Map[it->first].Operator == "END") {
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