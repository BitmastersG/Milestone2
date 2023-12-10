#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;

//The base struct for instruction ADT
struct instruction{
	int opcode;
	int reg_index(string& reg);
	int index_num(std::string reg);
	int op(string ins);
	void print(){
		std::cout<<"Opcode: "<<opcode<<"\n";
	}
	virtual void output_to_file(ofstream& file) = 0;
};


 int instruction::op(string ins) // it returns the opcode of the instruction. E.g, for the add instruction, it returns 1
 {
 	std::stringstream ss(ins);
 	std::string temp;
 	ss>>temp;
 	if(temp == "add") return 1;
 	else if(temp == "addi") return 2;
 	else if(temp == "mul") return 3;
 	else if(temp == "div") return 4;
 	else if(temp == "jal") return 11;
 	else if(temp == "jr") return 10;
 	else if(temp == "j") return 9;
 	else if(temp == "lw") return 5;
 	else if(temp == "sw") return 6;
 	else if(temp == "beq") return 7;
 	else if(temp == "slt") return 8;
 	else {
 		std::cout<<"Error! Invalid operation detected\n";
 		return -1;
	 }
 }

//returns the number of the reegister. E.g, for register a0, it a0, it returns the number 0
// returns -1 for an invalid register 
int instruction::index_num(string reg)
{
	char* num = &reg[1];
	int palo = std::atoi(num);
	return palo;
}

int instruction::reg_index(string& reg){ //Computes the index of the register
	if(reg.size() > 3) {
		std::cout<<"Error! Invalid register name.\n";
		return -1;
	}
	else{
		const char* ch = reg.c_str();
		if(ch[0] == 't'){
			return index_num(reg) < 4 ? 4+index_num(reg): -1;
		}
		else if(ch[0] == 's'){
			return index_num(reg) < 4 ? 8+index_num(reg): -1;
		}
		else if(ch[0] == 'v'){
			return index_num(reg) < 2 ? 2+index_num(reg): -1;
		}
			
		else if(ch[0] == 'a'){
			
			return index_num(reg) < 2 ? index_num(reg): -1;
		}
		else if(ch[0] == 'f'){
			return index_num(reg) < 13 ? 12+index_num(reg): -1;
		}
		else if(ch[0] == 'r'&&ch[1]=='a'){
			return 15;
		}
		else{
			return -1;
		}
	}
}

//The memory reference instruction ADT
struct mem_type: instruction{
	int immediate;
	int rd;
	int rs;
	mem_type(std::string str);
	string check(string operation);
	void output_to_file(ofstream& file);
};

string mem_type::check(string operation)
{
	if(operation == "lw"|| operation == "sw"){
		return operation;
	}
	else{
		std::cout<<"Error! Invalid operation detected. Terminating program";
		exit(0);
	}
}
mem_type::mem_type(std::string str)
{
	std::string temp, temp1, temp2;
	std::stringstream ss(str);
	ss>>temp>>temp1>>temp2;
	opcode = op(temp);
	if(opcode == -1){
		cout<<"Error! Invalid operation detected. Terminating program\n";
		exit(0);
	}
	rd = reg_index(temp1);
	rs = reg_index(temp2);
	if(rd == -1 || rs == -1) {
		std::cout<<"Error! Invalid register name detected. Terminating program\n";
		exit(0);
	}
		
}

void mem_type::output_to_file(ofstream& file)
{
	file<<opcode<<"\t"<<rd<<"\t"<<rs;
}

//The R-type instruction ADT
struct r_type: instruction{
	int rd;
	int rs;
	int rt;
	r_type(std::string str);
	string check(string operation);
	void output_to_file(ofstream& file);
};

string r_type::check(string operation)
{
	if(operation == "add"|| operation == "addi"|| operation == "div"||operation == "beq"|| operation == "slt"){
		return operation;
	}
	else{
		std::cout<<"Error! Invalid operation detected. Terminating program";
		exit(0);
	}
}

r_type::r_type(std::string str){
	std::string temp, temp1, temp2, temp3;
	std::stringstream ss(str);
	ss>>temp>>temp1>>temp2>>temp3;
	rd = reg_index(temp1);
	rs = reg_index(temp2);
	rt = reg_index(temp3);
	opcode = op(temp);
	if(opcode == -1){
		cout<<"Error! Invalid operation detected. Terminating program\n";
		exit(0);
	}
	if(rs == -1||rd == -1|| rt == -1){
		std::cout<<"Error! Invalid register name detected. Terminating program\n";
		exit(0);
	}
}

void r_type::output_to_file(ofstream& file)
{
	file<<opcode<<"\t"<<rd<<"\t"<<rs<<"\t"<<rt;
}


//The j-type instruction ADT
struct j_type: instruction{
	int rd;
	j_type(std::string str);
	string check(string instruction);
	void output_to_file(ofstream& file);
};

j_type::j_type(std::string str)
{
	string instruction = check(str);
	std::string temp, temp1;
	std::stringstream ss(instruction);
	ss>>temp>>temp1;
	rd = reg_index(temp1);
	opcode = op(temp);
	if(opcode == -1){
		cout<<"Error! Invalid operation detected. Terminating program\n";
		exit(0);
	}
	if(rd == -1){
		std::cout<<"Error! Invalid register name detected. Terminating program\n";
		exit(0);
	}
}


string j_type::check(string instruction){ //Checks if the j_type instruction has the correct operation
	stringstream ss(instruction);
	string temp, temp1;
	ss>>temp>>temp1;
	if(temp == "j"||temp == "jr"){
		return instruction;
	}
	else{
		cout<<"Corrupt instruction detected. Terminating program\n";
		exit(0);
	}
}
void j_type::output_to_file(ofstream& file)
{
	file<<opcode<<"\t"<<rd;
}

class compiler{
	std::vector<instruction*> instruction_store;
	public:
		void compile(ifstream& in);
		void input_and_compile();
		void compile(stringstream& str);
		string process_instruction(std::string& instruction);
		void open_file();
};


//Function: Takes user input, compiles it, then saves output to named file
void compiler::input_and_compile()
{
	string temp;
	stringstream ss;
	std::cout<<"Enter assembly code: ";
	while(true){
		getline(cin, temp);
		ss<<temp;
	}
	compile(ss);
}
void compiler::compile(stringstream& str)
{
	string temp, filename;
	while(getline(str,temp )){
		string temp1 = process_instruction(temp);
		if(temp1 == "Bad input\n"){
			cout<<"Bad input\n";
			exit(0);
		}
		else if(temp1 == "r_type"){
			r_type* ins = new r_type(temp);
			instruction_store.push_back(ins);
		}
		else if(temp1 == "mem_ref"){
			mem_type* ins = new mem_type(temp);
			instruction_store.push_back(ins);
		}
		else if(temp1 == "j_type"){
			j_type* ins = new j_type(temp);
			instruction_store.push_back(ins);
		}
	}
	std::cout<<"Enter file name to save to: ";
	std::cin>>filename;
	ofstream out(filename+".machine");
	for(auto p: instruction_store){
		p->output_to_file(out);
		out<<"\n";
	}
	out.close();
	out.open(filename+".asm");
	out<<str.str();
	out.close();
}
void compiler::compile(ifstream& in)
{
	
	string temp, bad_input = "Bad input", instruction;
	stringstream ss;
	
	while(getline(in, temp)){
		string temp1 = process_instruction(temp);
		if(temp1 == "Bad input\n"){
			cout<<"Bad input\n";
			exit(0);
		}
		else if(temp1 == "r_type"){
			r_type* ins = new r_type(temp);
			instruction_store.push_back(ins);
		}
		else if(temp1 == "mem_ref"){
			mem_type* ins = new mem_type(temp);
			instruction_store.push_back(ins);
		}
		else if(temp1 == "j_type"){
			j_type* ins = new j_type(temp);
			instruction_store.push_back(ins);
		}
	}
}

//A function to test if an instruction has the relavant fields
string compiler::process_instruction(std::string& instruction)  
{
	std::string temp, bad_input = "Bad input";
	std::stringstream ss(instruction);
	int count = 0;
	while(ss>>temp){
		count+=1;   //counts the number of whitespaces encountered in input
	}
	switch(count){
		case(1):
		//	std::cout<<"Bad input\n";
			return bad_input;
			break;
		case(2):
		//	std::cout<<"Instruction is J-type\n";
			return "j_type";
			break;
		case(3):
			return "mem_ref";
			break;
		case(4):
		//	std::cout<<"Instruction is R-type\n";
			return "r_type";
			break;
		default:
		//	std::cout<<"Bad input\n";
			return bad_input;
	}
}

void compiler::open_file()
{
	string filename;
	cout<<"Enter file name: ";
	cin>>filename;
	filename = filename+".asm";
	ifstream in(filename);
	compile(in);
}
int main()
{
	stringstream ss;
	string temp;
	compiler c;
	int selection;
	std::cout<<"Select action\n1. Open a file\n2. Write new code: ";
	std::cin>>selection;
	switch(selection){
		case(1):
			
			break;
		case(2):
			std::cout<<"\nEnter code:\n";
			while(true){
				getline(cin, temp);
				if(temp == "exit") break;
				ss<<temp<<"\n";
			}
			c.compile(ss);
			break;
	}
	
}