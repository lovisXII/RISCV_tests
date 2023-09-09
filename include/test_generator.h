#pragma once
#include "useful.h"
#include "instruction.h"
/*------------------------------------------------------------
                    TEST_GENERATOR CLASS
--------------------------------------------------------------*/

class test_generator
{
    public:
        test_generator(vector<Instruction> instruction, bool batch);
        test_generator(Instruction instruction, bool batch, int nbr_test_per_file, int max_test, string dir);
        test_generator(vector<Instruction> instruction, bool batch, int nbr_test_per_file, int max_test, string dir);
        void build_tests();
        vector<string>  getInstructions();

        void start_logo();
        bool print_menu();
        void test_per_file();
        void start();
    private:
        bool                _batch;
        int                 _number_test_per_file;
        int                 _max_test;
        string              _assembly;
        bool                _is_easy;
        string              _path;
        vector<Instruction> _instructions;
};