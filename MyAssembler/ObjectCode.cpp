#include "ObjectCode.h"


string ObjectCode::objectCode(int format, string CurrentLOCCTR, string B, bool BASE, string opCode, vector<string> operand, char typeOfOperand) {
	
	string ObjectCode = "";
	bool X = false;
	if (format == 2) {
		// if (sizeof(operand) == 1)
			  //get value of the operand[1] from symbol table if exists
			  //objectCode=opCode+value in hex+'0'
	   //   else
			  //get value of two operands from symbol table
			  //objectCode=opCode+value1+value2
		  //locctr+2
	}
	else if (format == 3) {
		opCode = HexToBin(opCode);
		opCode.erase(opCode.size() - 1);
		opCode.erase(opCode.size() - 1);
		string ni = generateNI(typeOfOperand);
		string TA = generateTA(operand);
		string xbpeDisplacementHex = setup(TA, X, B, BASE, format, CurrentLOCCTR);
		string opni = opCode + ni;
		string opniHex = convertBinToHex(opni, false);
		ObjectCode = opniHex + xbpeDisplacementHex;
	} else if (format == 4) {
		opCode = HexToBin(opCode);
		opCode.erase(opCode.size() - 1);
		opCode.erase(opCode.size() - 1);
		string ni = generateNI(typeOfOperand);
		string TA = generateTA(operand);
		string xbpeTAHex = setup(TA, X, B, BASE, format, CurrentLOCCTR);
		string opni = opCode + ni;
		string opniHex = convertBinToHex(opni, false);
		ObjectCode = opniHex + xbpeTAHex;
	}
	return ObjectCode;
}


vector<string> ObjectCode::getOperands(string exp) {
	char delimiter = ',';
	vector<string> oper;
	string temp = "";
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i] == delimiter) {
			oper.push_back(temp);
			temp = "";
		}
		else
			temp += exp[i];
	}
	return oper;
}

string ObjectCode::generateNI(char typeOfOperand) {
	string ni;
	if (typeOfOperand == '#')
	{
		ni += "0";
		ni += "1";
	}
	else if (typeOfOperand == ' ')
	{
		ni += "1";
		ni += "1";
	}
	else if (typeOfOperand == '@')
	{
		ni += "1";
		ni += "0";
	}
	return ni;
}

string ObjectCode::generateTA(vector<string> operand) {
	string TA = operand[1];
	if (TA != "0000") {
		int size = operand.size();
		if (size > 2) {
			X = true;
		}
		else {
			X = false;
		}
	}
	else {
		//objectCode=opCode(hex)+'0000'
	}
	return TA;
}