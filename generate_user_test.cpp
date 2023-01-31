#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define USER        0b00
#define SUPERVISOR  0b01 
#define MACHINE     0b11

#define NONE        0b00 << 2
#define READ_ONLY   0b01 << 2
#define WRITE_ONLY  0b01 << 2
#define R_W         0b11 << 2

using namespace std;

int number_test_per_file = 10;
enum TYPE { R_type, B_type, J_type, MEM_type };


/*------------------------------------------------------------
                    START UP FUNCTION
--------------------------------------------------------------*/
void start_logo(){
    cout << R"(
 ________  ___  ___      ___ _______   ________         
|\   __  \|\  \|\  \    /  /|\  ___ \ |\   __  \        
\ \  \|\  \ \  \ \  \  /  / | \   __/|\ \  \|\  \       
 \ \   _  _\ \  \ \  \/  / / \ \  \_|/_\ \   _  _\      
  \ \  \\  \\ \  \ \    / /   \ \  \_|\ \ \  \\  \|     
   \ \__\\ _\\ \__\ \__/ /     \ \_______\ \__\\ _\     
    \|__|\|__|\|__|\|__|/       \|_______|\|__|\|__|    
                                                        
                                                        
                                                        
 _________  _______   ________  _________               
|\___   ___\\  ___ \ |\   ____\|\___   ___\             
\|___ \  \_\ \   __/|\ \  \___|\|___ \  \_|             
     \ \  \ \ \  \_|/_\ \_____  \   \ \  \              
      \ \  \ \ \  \_|\ \|____|\  \   \ \  \             
       \ \__\ \ \_______\____\_\  \   \ \__\            
        \|__|  \|_______|\_________\   \|__|            
                        \|_________|                                                          
                                                    )"<< endl;
}
bool print_menu(){
    string is_easy;
    cout << R"(
        Welcome to RiVer Test !!
        This framework allows you to generate assembly test for your architecture.
        If you're using a testbench to test your architecture it will need to parse .s files 
        and to detect _bad and _good symbols !
    )" << endl;
    cout << R"(
        Please enter the following commands :

        A : Just a few tests
        B : Maximum number of test per instruction (32768 per instruction)
        )" << endl;
    while(is_easy != "A" && is_easy != "B"){
        cin >> is_easy;
        transform(is_easy.begin(), is_easy.end(),is_easy.begin(), ::toupper);
    }
    return (is_easy == "A") ?  true : false;

}

void test_per_file(){
    cout << R"(
        Enter how much test you want per file, minimum is 1 :
    )" << endl;
    int test = 0;
    while(test == 0){
        cin >> test;
        if(test <= 0)
        {
            test == 0;
        }
    }
    number_test_per_file = test;
}

bool start(){
    start_logo();
    bool is_easy = print_menu();
    test_per_file();
    return is_easy;
}

/*------------------------------------------------------------
                    USEFULL FUNCTION
--------------------------------------------------------------*/

void create_directory(string name){
    int check;
    check = mkdir(name.c_str(),0777);
    if(check == -1 && errno != EEXIST) {
        cerr << "Unable to create directory " << name << endl;
        exit(1);
    }
}
string filename(string name, int number){
    return name + "/" + name + "_" + to_string(number) + ".S";
}

void open_checked(ofstream& file, string file_name){
    file.open(file_name);
    if(!file.is_open())
    {
        cerr << "Error file " << file_name << " impossible to open" << endl;
        exit(1);
    }
}

int operation(string instruction, int op1, int op2) {
  int result = 0;
  if (instruction == "add" || instruction == "addi") {
    result = op1 + op2;
  } else if (instruction == "sub") {
    result = op1 - op2;
  } else if (instruction == "sll" || instruction == "slli") {
    result = op1 << op2;
  } else if (instruction == "slt" || instruction == "slti") {
    result = (op1 < op2) ? 1 : 0;
  } else if (instruction == "sltu") {
    result = (unsigned int)op1 < (unsigned int)op2 ? 1 : 0;
  } else if (instruction == "xor" || instruction == "xori") {
    result = op1 ^ op2;
  } else if (instruction == "srl" || instruction == "srli") {
    result = op1 >> op2;
  } else if (instruction == "sra") {
    result = op1 >> op2;
  } else if (instruction == "or" || instruction == "ori") {
    result = op1 | op2;
  } else if (instruction == "and" || instruction == "andi") {
    result = op1 & op2;
  } else {
    cout << "Invalid instruction" << endl;
    return -1;
  }
  return result;
}


/*------------------------------------------------------------
                    INSTRUCTION CLASS
--------------------------------------------------------------*/

class Instruction{
    public:
        Instruction(string name, int is_immediat, int type);
        inline int getType()    {return _type ;                          };
        inline int IsImmediat() {return (_is_immediat == 1) ? 1 : 0 ;    };
        inline string getName() {return _name ;                          };
        string toString(int rd, int rs1, int rs2, string label);
    private:
    int     _is_immediat;
    int     _type;
    string  _name;
};
Instruction::Instruction(string name, int is_immediat, int type) :
_is_immediat(is_immediat),
_type(type),
_name(name){

}
string Instruction::toString(int rd, int rs1, int rs2, string label)
{
    if(_type == R_type && _is_immediat == false){
        return _name + " x" + to_string(rd)
        +", x" + to_string(rs1) 
        + ", x" + to_string(rs2); // operation rd, rs1, rs2*

    }
    else if(_type == R_type && _is_immediat == true && _name != "li"){
        return _name + " x" + to_string(rd)
        +", x" + to_string(rs1) 
        + ", " + to_string(rs2) ; // operation rd, rs1, rs2*
    }
    else if(_name == "li"){
        return "li x" + to_string(rd) + ", " + to_string(rs1); // li rs1, random_value

    }
    else if(_type == B_type){
            return "bne x" + to_string(rs1) + ", x" + to_string(rs2) + ", " + label ;
    }

}

/*------------------------------------------------------------
                    TEST_GENERATOR CLASS
--------------------------------------------------------------*/

class test_generator 
{
    public:
        test_generator(Instruction instruction, bool is_easy);
        test_generator(vector<Instruction> instruction, bool is_easy);
        void build_tests();
        vector<string>  getInstructions();
    private:
        string              _assembly;
        vector<Instruction> _instructions; 
        bool                _is_easy;
};

// Constructor
test_generator::test_generator(Instruction instruction, bool is_easy) :
_assembly(),
_is_easy(is_easy)
{
    _instructions.push_back(instruction);
}

test_generator::test_generator(vector<Instruction> instruction, bool is_easy) :
_assembly(),
_is_easy(is_easy),
_instructions(instruction)
{
    
}

void test_generator::build_tests(){

    _assembly  = R"(
.section .text
.global _start

_start :
)";
    // Instruction needed for the generation :
    Instruction li  ("li"    , true   , R_type);
    Instruction bne ("bne"   , false  , B_type);

    for(auto it = _instructions.begin(); it != _instructions.end(); it++){
    
        int number_of_tests = 0;
        /*
            * Each test will be made like this :
                li rs1, random_value
                li rs2, random value
                rd = operation(rs1, rs2) // for example add rd, rs1, rs2
                li test_register, operation(rs1,rs2)
                bne test_register, rd, _bad     

            * The register used to test the value of the 
                operation will always be x23, if x23 is used as rd, 
                it will be 24.

            * Each test will be created in a directory named according to the instruction.
            * Each time a new file is created, the old one must be closed and a j_good must cloture the file.
        */
       ofstream file;
       int number_of_file = 0;
        switch(it->getType()){
            case R_type :
                if(it->IsImmediat() == false){
                    for (int rd = 0; rd < 32; rd++){
                        for(int rs1 = 0; rs1 < 32; rs1 ++){
                            for(int rs2 = 0; rs2 < 32; rs2 ++)
                            {
                                // FILE NAME GESTION
                                if(number_of_tests % number_test_per_file == 0){
                                    if(number_of_file != 0){
                                        file << "   " << "_exit :" << endl;
                                        file << "       " << "j _good"<< endl;
                                        file.close();
                                    }

                                    create_directory(it->getName());
                                    string file_name = filename(it->getName(),number_of_file);
                                    open_checked(file, file_name);
                                    file << _assembly; 
                                    number_of_file++;
                                }
                                // END OF FILE NAME GESTION

                                file << "   " << "test_" << number_of_tests << " :"<< endl;

                                int value_rs1 = rand() % 4096; // 2^12, should be enough
                                int value_rs2 = rand() % 4096;

                                int result;
                                if(rs1 == 0 && rs2 != 0)
                                    result = operation(it->getName(), 0, value_rs2);
                                else if(rs1 != 0 && rs2 == 0)
                                    result = operation(it->getName(), value_rs1, 0);
                                else if(rs1 == 0 && rs2 == 0)
                                    result = operation(it->getName(), 0, 0);
                                else if(rs1 == rs2)
                                    result = operation(it->getName(), value_rs2, value_rs2);
                                else
                                    result = operation(it->getName(), value_rs1, value_rs2);

                                file << "       " << li.toString(rs1,value_rs1,0,"") << endl; 
                                file << "       " << li.toString(rs2,value_rs2,0,"") << endl; 
                                file << "       " << it->toString(rd,rs1,rs2,"") << endl;
                    
                                int test_register = 23;
                                if(rd == 23)
                                    test_register = 24;
                                if(rd == 0)
                                    file << "       " << li.toString(test_register, 0, 0, "") << endl;
                                else
                                    file << "       " << li.toString(test_register, result, 0, "") << endl;
                                
                                file << "       " << bne.toString(0,rd,test_register,"_bad") << endl;
                                number_of_tests++;
                            }
                        }
                    }
                       
                }
                else{
                    for (int rd = 0; rd < 32; rd++){
                        for(int rs1 = 0; rs1 < 32; rs1 ++){
                            for(int rs2 = 0; rs2 < 32; rs2 ++)
                            {
                                // FILE NAME GESTION
                                if(number_of_tests % number_test_per_file == 0){
                                    if(number_of_file != 0){
                                        file << "   " << "_exit :" << endl;
                                        file << "       " << "j _good"<< endl;
                                        file.close();
                                    }

                                    create_directory(it->getName());
                                    string file_name = filename(it->getName(),number_of_file);
                                    open_checked(file, file_name);
                                    file << _assembly; 
                                    number_of_file++;
                                }
                                // END OF FILE NAME GESTION

                                file << "   " << "test_" << number_of_tests << " :"<< endl;

                                int value_rs1 = rand() % 4096; // 2^12, should be enough
                                int immediat_value = rand() % 4096;
                                string instruction1_s = "li x" + to_string(rs1) + ", " 
                                + to_string(value_rs1); // li rs1, random_value
                                
                                string instruction3_s = it->getName() + " x" + to_string(rd)
                                +", x" + to_string(rs1) 
                                + ", " + to_string(immediat_value) ; // operation rd, rs1, rs2*

                                int result = operation(it->getName(), value_rs1, immediat_value);
                                file << "       " << instruction1_s << endl; 
                                file << "       " << instruction3_s << endl;
                    
                                int test_register;
                                if(rd != 0){
                                    if(rd != 23){
                                        file << "       " << "li x23," << result << endl;
                                        test_register = 23;
                                    }
                                    else {   
                                        file << "       " << "li x24," << result << endl;
                                        test_register = 24;
                                    }
                                    file << "       " << "bne x" << to_string(rd) 
                                    << ", x" << to_string(test_register) << ", _bad" << endl;
                                }
                                else{
                                    file << "       " << "li x23, 0" << endl;
                                    file << "       " << "bne x" << to_string(rd) 
                                    << ", x23" << ", _bad" << endl;        
                                }
                                number_of_tests++;
                            }
                        }
                    }
                }
                break;
            case B_type :
                break;
            case J_type :
                break;
            case MEM_type :
                for (int rd = 0; rd < 32; rd++){
                    for (int rs1 = 0; rs1 < 32; rs1++){
                            // access with rand number multiple of 4
                            string instruction_s = it->getName() + " x" + to_string(rd)
                            +", " + to_string(((rand()%4096)/4)*4) + "(x" + to_string(rs1) +")" ;

                            file << "   " << instruction_s << endl;
                    }
                }
                break;
        }
        file << "   " << "j _good" ;
        file.close();
    }

}

void priviledge_test(){
    std::map<std::string, int> csr_registers;

    // Implemented CSR :
    
    csr_registers.insert({"mvendor_id"  ,MACHINE || READ_ONLY});  // 0xF11, R
    csr_registers.insert({"marchid"     ,MACHINE || READ_ONLY});  // 0xF12, R
    csr_registers.insert({"mimpid"      ,MACHINE || READ_ONLY});  // 0xF13, R
    csr_registers.insert({"mstatus"     ,MACHINE || R_W      });  // 0x300, R/W
    csr_registers.insert({"misa"        ,MACHINE || R_W      });  // 0x301, R/W
    csr_registers.insert({"mie"         ,MACHINE || R_W      });  // 0x304, R/W
    csr_registers.insert({"mtvec"       ,MACHINE || R_W      });  // 0x305, R/W
    csr_registers.insert({"mstatush"    ,MACHINE || R_W      });  // 0x310, R/W
    csr_registers.insert({"mepc"        ,MACHINE || R_W      });  // 0x341, R/W
    csr_registers.insert({"mcause"      ,MACHINE || R_W      });  // 0x342, R/W
    csr_registers.insert({"mtval"       ,MACHINE || R_W      });  // 0x343, R/W
    csr_registers.insert({"mip"         ,MACHINE || R_W      });  // 0x344, R/W
    csr_registers.insert({"mscratch"    ,MACHINE || R_W      });  // 0x340, R/W
    
    int i = 0;
    for (auto it = csr_registers.begin(); it != csr_registers.end(); ++it)
    {
         std::string file_name = "../csr_priviledge_access_" + std::to_string(i) + ".S"; 
         std::ofstream file(file_name);

        // Generation of the text for the file
        
         std::string assembly = R"(
.section .text
.global _start

_start :
         )";

         std::string instruction = R"(
    csrrwi x0,)" + it->first + ",0x1200" ;
        
        if(it-> second && 0b11 == 0b11) // if in machine mode, instruction should not execute
        {
            instruction += R"(
    j _bad
         )";
         }
        else // if in user mode, instruction should execute
        {
            instruction += R"(
    j _good
         )";

        }

         assembly += instruction;

        // Writing the file :
        
        file << assembly;
        file.close();
        i++;
    }
}

void test_instruction(std::string instruction_name){
    std::string file_name = instruction_name + ".S"; 
    std::ofstream file(file_name);
           
    // Generation of the text for the file
        
    std::string assembly = R"(
.section .text
.global _start

_start :
)";

    file << assembly; 

    for (int rd = 0; rd < 32; rd++){
        for (int rs1 = 0; rs1 < 32; rs1++){
            for (int rs2 = 0; rs2 < 32; rs2++){
                std::string instruction = instruction_name + " x" + std::to_string(rd)
                +", x" + std::to_string(rs1) + ", x" + std::to_string(rs2);
                file << "   " << instruction << std::endl;       
            }
        }
    }
    file << "   " << "j _good" ;
    file.close();
}


/*------------------------------------------------------------
                    MAIN, GENERATES TEST FILES
--------------------------------------------------------------*/

int main(){
    bool is_easy = start();
    Instruction sub("sub", false, R_type);
    Instruction sll("sll", false, R_type);
    Instruction slt("slt", false, R_type);
    Instruction add("add", false, R_type);
    Instruction sltu("sltu", false, R_type);
    Instruction xor_("xor", false, R_type);
    Instruction srl("srl", false, R_type);
    Instruction sra("sra", false, R_type);
    Instruction or_("or", false, R_type);
    Instruction and_("and", false, R_type);
    Instruction addi("addi", true, R_type);
    Instruction slli("slli", true, R_type);
    Instruction slti("slti", true, R_type);
    Instruction sltiu("sltiu", true, R_type);
    Instruction xori("xori", true, R_type);
    Instruction srli("srli", true, R_type);
    Instruction srai("srai", true, R_type);
    Instruction ori("ori", true, R_type);
    Instruction andi("andi", true, R_type);
    Instruction lb("lb", true, MEM_type);
    Instruction lh("lh", true, MEM_type);
    Instruction lw("lw", true, MEM_type);
    Instruction lbu("lbu", true, MEM_type);
    Instruction lhu("lhu", true, MEM_type);
    Instruction sb("sb", true, MEM_type);
    Instruction sh("sh", true, MEM_type);
    Instruction sw("sw", true, MEM_type);
    Instruction beq("beq", true, B_type);
    Instruction bne("bne", true, B_type);
    Instruction blt("blt", true, B_type);
    Instruction bge("bge", true, B_type);
    Instruction bltu("bltu", true, B_type);
    Instruction bgeu("bgeu", true, B_type);
    Instruction jal("jal", true, J_type);
    Instruction jalr("jalr", true, J_type);

    vector<Instruction> v_Instructions = {
    add, sub, sll, slt, sltu, xor_, srl, sra, or_, and_,
    addi, slli, slti, sltiu, xori, srli, srai, ori, andi,
    lb, lh, lw, lbu, lhu, sb, sh, sw,
    beq, bne, blt, bge, bltu, bgeu,
    jal, jalr
    };
    // vector<Instruction> v_Instructions = {addi};
    test_generator gen(v_Instructions, is_easy);
    gen.build_tests();
}