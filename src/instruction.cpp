#include "../include/instruction.h"

// Constructor
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