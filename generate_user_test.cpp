#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <fstream>
#define USER        0b00
// #define SUPERVISOR  0b01 // not implemented in our architecture
#define MACHINE     0b11

#define NONE        0b00 << 2
#define READ_ONLY   0b01 << 2
#define WRITE_ONLY  0b01 << 2
#define R_W         0b11 << 2
/*
    This is a test generator for the CSR
    priviledge.
    The map will store every csr implemented in the architecture and
    the key associated will be the priviledge needed to access it.

    These programm are supposed to be launched after the reset, so cpu 
    should be in user mode
*/
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
int main(){
    // priviledge_test();
    test_instruction("sub");
}