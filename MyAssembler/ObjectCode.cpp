#include "ObjectCode.h"
#include "operations.h"
#include "structures.h"
#include "convert.h"


 map<string, symbol_info> SYMTAB;
 void ObjectCode:: storeRegisters() {
	 string labels[9] = { "A","X","L","B","S","T","F","PC","SW" };
	 string add[9] = { "0","1","2","3","4","5","6","8","9" };
	 for (int i = 0; i < 9; i++) {
		 symbol_info t;
		 string label = labels[i];
		 t.address = add[i];
		 SYMTAB.insert(pair<string, symbol_info>(label, t));
	 }
	 printSymbols();
 }

 void ObjectCode::Label_is_Found(string label, string address) {
	 if (label == "")
		 return;
	 if (SYMTAB.find(label) != SYMTAB.end()) {
		 cout << "varriable is Found";
		 SYMTAB.at(label).address = address;
		 // go to vector
	 }
	 else {
		 symbol_info n;
		 n.address = address;
		 SYMTAB.insert(pair<string, symbol_info>(label, n));
	 }
 }

 void ObjectCode::printSymbols() {
	 map<string, symbol_info>::iterator itr;
	 for (itr = SYMTAB.begin(); itr != SYMTAB.end(); ++itr) {
		 cout << '\t' << itr->first
			 << '\t' << itr->second.address << '\n';
	 }
 }
 
 
string ObjectCode::objectCode(int format, string CurrentLOCCTR, string B, bool BASE, string opCode, vector<string> operand, char typeOfOperand) {
	string ObjectCode = "";
	bool X = false;
	if (format == 2) {
		// if operand is empty return error
		ObjectCode = opCode;
		string TA = getTargetAddress(operand[0], typeOfOperand, CurrentLOCCTR);
		if (TA == "NotFound")
			ObjectCode.append("0");
		else
			ObjectCode.append(TA);
		if (operand.size() > 1) {
			TA = getTargetAddress(operand[1], typeOfOperand, CurrentLOCCTR);
			if (TA == "NotFound")
				ObjectCode.append("0");
			else
				ObjectCode.append(TA);
		}
		else
			ObjectCode.append("0");
	}
	else if (format == 3) {
		opCode = HexToBin(opCode);
		opCode.erase(opCode.size() - 1);
		opCode.erase(opCode.size() - 1);
		string ni = generateNI(typeOfOperand);
		string opni = opCode + ni;
		string opniHex = convertBinToHex(opni, false);
		string TA = generateTA(operand, typeOfOperand, CurrentLOCCTR);
		if (TA != "NotFound"){
			string xbpeDisplacementHex = setup(TA, X, B, BASE, format, CurrentLOCCTR);
			ObjectCode = opniHex + xbpeDisplacementHex;
		}
		else
			ObjectCode = opniHex.append("0000");
	} else if (format == 4) {
		opCode = HexToBin(opCode);
		opCode.erase(opCode.size() - 1);
		opCode.erase(opCode.size() - 1);
		string ni = generateNI(typeOfOperand);
		string opni = opCode + ni;
		string opniHex = convertBinToHex(opni, false);
		string TA = generateTA(operand, typeOfOperand, CurrentLOCCTR);
		if (TA != "NotFound") {
		string xbpeTAHex = setup(TA, X, B, BASE, format, CurrentLOCCTR);
		ObjectCode = opniHex + xbpeTAHex;
		}
		else
			// check numbers of zeros if format 4
			ObjectCode = opniHex.append("0000");
	}
	return ObjectCode;
}


vector<string> ObjectCode::split(string exp,char delimiter) {
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
	if (temp != "")
		oper.push_back(temp);
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

string ObjectCode::generateTA(vector<string> operand,char type,string loc) {
	string TA = getTargetAddress(operand.at(0), type, loc);;
		int size = operand.size();
		if (size > 2) {
			X = true;
		}
		else {
			X = false;
		}
	
	return TA;
}

string ObjectCode::getTargetAddress(string var, char type, string loc) {
	if (type == '#' && is_number(var))//immediate number
		return var;
	if (var.find("+") != std::string::npos) {
		vector<string> temp = split(var, '+');
		string var1 = getValue(temp.at(0), loc);
		string var2 = getValue(temp.at(1), loc);
		if (var1 == "NotFound" || var2 == "NotFound")
			return "NotFound";
		return toHex(toDec(var1) + toDec(var2));
	}
	else if (var.find("-") != std::string::npos) {
		vector<string> temp = split(var, '-');
		string var1 = getValue(temp.at(0), loc);
		string var2 = getValue(temp.at(1), loc);
		if (var1 == "NotFound" || var2 == "NotFound")
			return "NotFound";
		return toHex(toDec(var1) -toDec(var2));
	}
	else if (var.find("*") != std::string::npos) {
		vector<string> temp = split(var, '*');
		string var1 = getValue(temp.at(0), loc);
		string var2 = getValue(temp.at(1), loc);
		if (var1 == "NotFound" || var2 == "NotFound")
			return "NotFound";
		return toHex(toDec(var1) * toDec(var2));
	}
	else if (var.find("/") != std::string::npos) {
		vector<string> temp = split(var, '/');
		string var1 = getValue(temp.at(0), loc);
		string var2 = getValue(temp.at(1), loc);
		if (var1 == "NotFound" || var2 == "NotFound")
			return "NotFound";
		return toHex(toDec(var1)/ toDec(var2));
	}
	else
		return getValue(var, loc);
}
string ObjectCode::getValue(string var, string loc) {
	if (SYMTAB.find(var) != SYMTAB.end()) {// varriable exists
		if (SYMTAB.at(var).address != "*") {// varriable has an address
			cout << "here is the varriable" << '\n';
			return SYMTAB.at(var).address;
		}
		else { // varriable hasn't an address
			SYMTAB.at(var).reff.push_back(loc);
			return "NotFound";
		}
	}
	else {//varriable doesnt exist
		symbol_info n;
		n.address = "*";
		n.reff.push_back(loc);
		SYMTAB.insert(pair<string, symbol_info>(var, n));
		return "NotFound";
	}
}
bool ObjectCode::is_number(const std::string& s)
{
	return(strspn(s.c_str(), "-.0123456789") == s.size());
}