#include "ObjectCode.h"
#include <vector>
#include "Conversions.h"
string objectCode(int format, string CurrentLOCCTR, string B,bool BASE, string opCode, vector<string>operand, char typeOfOperand)
{
	string ObjectCode = "";
	bool X = false;
	if (format == 2)
	{
		// if (sizeof(operand) == 1)
		      //get value of the operand[1] from symbol table if exists
			  //objectCode=opCode+value in hex+'0'
	   //   else
			  //get value of two operands from symbol table
			  //objectCode=opCode+value1+value2
		  //locctr+2
	}
	else if (format == 3)
	{
		string TA;
		opCode.erase(opCode.size() - 1);
		opCode.erase(opCode.size() - 1);
		string ni;
		if (typeOfOperand == '#')
		{
			//immediate n=0 i=1
			ni+= "0";
			ni+= "1";
			TA = operand[1];
		}
		else if (typeOfOperand == ' ')
		{
			//direct i=1 n=1
			ni += "1";
			ni += "1";
			size_t size = operand->size();
			TA = operand[1]; // if exists in symbol table else TA = 0 else store it
			if (TA != "0000")
			{
				// disp=TA;
				int size = operand->length();
				if ( size > 2)
				{
				//	X = true;
				}
				else
				{
					X = false;
				}
			}
			else
			{
				//objectCode=opCode(hex)+'0000'
			}
		}

		string xbpeDisplacementHex = setup(TA, X, B, BASE, format, CurrentLOCCTR);
		string opni = opCode + ni;
		string opniHex = convertBinToHex(opni,false);
		ObjectCode = opniHex + xbpeDisplacementHex;
	}
	else if (format == 4)
	{
	}
	return ObjectCode;
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
