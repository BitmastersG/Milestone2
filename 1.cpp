#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<fstream>



//THe data ADT.
//To be used for loading data from memory
template<typename t>
struct data{
	t info;
	int index;
};

//The register ADT
//It represents the register of the CPU
template<typename type>
struct reg{
	type value;
	int data_index;
	reg(data<type>& info);
};
template<typename type>
reg<type>::reg(data<type>& info){
	value = info.info;
	data_index = info.index;
}
	
//The RAM of the computer
//Data shall be stored in vectors that are members of this ADT
class memory{
	std::vector<data<std::string>> string_memory;
	std::vector<data<int>> int_memory;
	std::vector<data<std::string>> instruction_mem;
	public:
		data<int> at(int pos){
			for(auto p: int_memory){
				if(p.index== pos){
					return p;
				}
			}
			std::cout<<"\nError! Access to non-existent object attempted\n";
			exit(0);
		}
};


//THe processor of the computer.
//All processor components are implemented in here
class CPU{
	//static int curr_instruction;
	reg<std::string> CIR;
	std::vector<reg<int>> registers;
	std::vector<reg<int>> instruction_memory;
	public:
		int control_unit();
		void alu();
		reg<int> current_instruction(int offset);
		void Input_file();
		void adder(int rd, int rs, int rt);
		void fetch();
};

void CPU::adder(int rd, int rs, int rt)
{
	
}

void CPU::fetch()

{
	
}
void CPU::Input_file()
{
	std::stringstream ss;
	std::string code, filename;
	std::cout<<"Enter Code(Type exit to exit editor)\n ";
	
	while(true){
		getline(std::cin, code);
		if(code == "exit") break;
	//	if(process_instruction(code) == "Bad input");
		else{
			ss<<code<<"\n";
		} 
	}
	std::cout<<"Enter name of file to save to: ";
	getline(std::cin, filename);
	filename = filename+".asm";
	std::ofstream os(filename , std::ios::app);
	os<<ss.str();
	os.close();
}


void input()
{
	std::string code;
	std::cout<<"\nEnter Code: ";
	getline(std::cin, code);
	std::stringstream in(code);
	std::string temp;
}

int main()
{
	return 0;
}