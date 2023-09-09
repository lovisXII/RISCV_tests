#include"../include/useful.h"
/*------------------------------------------------------------
                    USEFULL FUNCTION
--------------------------------------------------------------*/
bool isNumeric(const std::string& str) {
    std::istringstream iss(str);
    double num;
    iss >> std::noskipws >> num; // Try to read a number, noskipws avoids skipping whitespace

    return iss.eof() && !iss.fail(); // Check if the whole string was consumed and no errors occurred
}

void initialise_registers(vector<int> &rd, vector<int> &rs1, vector<int> &rs2){
    // Fill rd_values, rs1_values, rs2_values with values 0 to 31
    for (int i = 0; i < 32; ++i) {
        rd[i] = i;
        rs1[i] = i;
        rs2[i] = i;
    }
}

void create_directory(string path, string name){
    #ifdef DEBUG
    cout << " Attempt to create : " << path << name << endl;
    #endif
    int check;
    if (path != ".") name = path + name;
    check = mkdir(name.c_str(),0777);
    if(check == -1 && errno != EEXIST) {
        cerr << "Unable to create directory " << name << endl;
        exit(1);
    }
}

string filename(string path, string name, int number){
    return path + name + "/" + name + "_" + to_string(number) + ".S";
}

void open_checked(ofstream& file, const string& file_name) {
    file.open(file_name, ios::out);
    if (!file.is_open()) {
        cerr << "Error: Unable to open or create file " << file_name << endl;
        perror("Error");
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