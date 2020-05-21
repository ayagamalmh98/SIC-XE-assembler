#include "object_code.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include "object_code.h"
#include "convert.h"
#include "operations.h"
#include <vector>
using namespace std;
string objectCode(string locctr,int format,string opCode,vector <string> operand,char typeOfOperand){
    cout<<format;
    string objectCode;
    if(format==1)
        objectCode=opCode;
    else if(format==2){
        if(sizeof(operand)==1)
            //get value of the operand[1] from symbol table if exists
            //objectCode=opCode+value in hex+'0'
            cout<<"hello";
        else
            cout<<"hello";
            //get value of two operands from symbol table
            //objectCode=opCode+value1+value2
    }
    else if(format==3){
        int TA;
        opCode=hexToBin(opCode);
        //change opcode from 8 bits to 6 bits
        opCode.erase(opCode.size()-1);
        opCode.erase(opCode.size()-1);
        char nixbpe[6];
        nixbpe[5]='0';
        if(typeOfOperand=='#'){
            //immediate n=0 i=1
            nixbpe[0]='0';
            nixbpe[1]='1';
            //TA=value of operand[1]
        }
        else if(typeOfOperand==char(0)){
             //direct i=1 n=1
            nixbpe[0]='1';
            nixbpe[1]='1';
            //TA=value of operand[1] if exists in symbol table else TA=0 else store it
            if(TA!=0){
                   // disp=TA;
                if(sizeof(operand)>1){
                    //indexed disp=disp-X
                    //nixbpe[2]='1'
                }
            else
                //nixbpe[2]='0'

                //locctr+3
                //disp=disp-locctr
                //change to hex if negative get 2nd complement
                //if in range of PC put nixbpe[3]='0'  nixbpe[4]='1'
                //Base
                //change disp to binary 12 bit
                //objectCode=opcode+nixbpe+disp  then change to hex
                cout<<"hello";
            }
            else
                 cout<<"hello";
                //objectCode=opCode(hex)+'0000'
        }
    }
}
 vector<string> getOperands(string exp){
         char delimiter = ',';
         vector<string> oper;
         string temp = "";
         for(int i = 0; i < exp.size(); i++){
            if(exp[i] == delimiter){
                    oper.push_back(temp);
                    temp = "";
            }
            else
                    temp += exp[i];
        }
    return oper;
    }
