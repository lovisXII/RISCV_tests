#include "include/instruction.h"
#include "include/test_generator.h"

/*------------------------------------------------------------
                    MAIN, GENERATES TEST FILES
--------------------------------------------------------------*/

int main(int argc, char** argv){
    // Check if at least one argument is provided
    bool batch = false;
    int nbr_test_per_file = 1;
    int max_test;
    string dir = "";

    if (argc > 1) {
        // Loop through the arguments to find if "batch" is one of them
        for (int i = 1; i < argc; ++i) {
            if (string(argv[i]) == "--batch") {
                batch = true;
            }
            if (string(argv[i]) == "--test-per-file") {
                if (argv[i+1] == NULL || !isNumeric(argv[i+1])){
                    cerr << "Please enter a valid number of test per file, minimum is 1" << endl;
                    exit(1);
                }
                nbr_test_per_file = stoi(argv[i+1]);
            }
            if (string(argv[i]) == "--max-test") {
                if (argv[i+1] == NULL || !isNumeric(argv[i+1])) {
                    cerr << "Please enter a valid number of maximum test" << endl;
                    exit(1);
                }
                max_test = stoi(argv[i+1]);
            }
            if (string(argv[i]) == "--dir") {
                if (argv[i+1] == NULL){
                    cerr << "Please enter valid directory" << endl;
                    exit(1);
                }
                dir = argv[i+1];
            }
        }
    }
    #ifdef DEBUG
    cout << "Generating " << max_test << " files per instruction with " << nbr_test_per_file << " tests per file in the directory " << dir << endl;
    #endif
    Instruction sub("sub"       , false, R_type);
    Instruction sll("sll"       , false, R_type);
    Instruction slt("slt"       , false, R_type);
    Instruction add("add"       , false, R_type);
    Instruction sltu("sltu"     , false, R_type);
    Instruction xor_("xor"      , false, R_type);
    Instruction srl("srl"       , false, R_type);
    Instruction sra("sra"       , false, R_type);
    Instruction or_("or"        , false, R_type);
    Instruction and_("and"      , false, R_type);
    Instruction addi("addi"     , true, R_type);
    Instruction slli("slli"     , true, R_type);
    Instruction slti("slti"     , true, R_type);
    Instruction sltiu("sltiu"   , true, R_type);
    Instruction xori("xori"     , true, R_type);
    Instruction srli("srli"     , true, R_type);
    Instruction srai("srai"     , true, R_type);
    Instruction ori("ori"       , true, R_type);
    Instruction andi("andi"     , true, R_type);
    Instruction lb("lb"         , true, MEM_type);
    Instruction lh("lh"         , true, MEM_type);
    Instruction lw("lw"         , true, MEM_type);
    Instruction lbu("lbu"       , true, MEM_type);
    Instruction lhu("lhu"       , true, MEM_type);
    Instruction sb("sb"         , true, MEM_type);
    Instruction sh("sh"         , true, MEM_type);
    Instruction sw("sw"         , true, MEM_type);
    Instruction beq("beq"       , true, B_type);
    Instruction bne("bne"       , true, B_type);
    Instruction blt("blt"       , true, B_type);
    Instruction bge("bge"       , true, B_type);
    Instruction bltu("bltu"     , true, B_type);
    Instruction bgeu("bgeu"     , true, B_type);
    Instruction jal("jal"       , true, J_type);
    Instruction jalr("jalr"     , true, J_type);

    vector<Instruction> v_Instructions = {
    add, sub, sll, slt, sltu, xor_, srl, sra, or_, and_,
    addi, slli, slti, sltiu, xori, srli, srai, ori, andi,
    lb, lh, lw, lbu, lhu, sb, sh, sw,
    beq, bne, blt, bge, bltu, bgeu,
    jal, jalr
    };
    // vector<Instruction> v_Instructions = {addi};
    test_generator gen(v_Instructions, batch, nbr_test_per_file, max_test, dir);
    gen.build_tests();
}