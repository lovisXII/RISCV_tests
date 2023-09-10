#include "../include/test_generator.h"

/*------------------------------------------------------------
                    CONSTRUCTORS
--------------------------------------------------------------*/
test_generator::test_generator(vector<Instruction> instruction, bool batch) :
_batch(batch),
_number_test_per_file(),
_max_test(),
_assembly(),
_is_easy(),
_path(),
_instructions(instruction)
{
}

test_generator::test_generator(Instruction instruction, bool batch, int nbr_test_per_file, int max_test, string dir) :
_batch(batch),
_number_test_per_file(nbr_test_per_file),
_max_test(max_test),
_assembly(),
_is_easy(0)
{
    (dir == ".") ? _path = "" : _path = dir;
    _instructions.push_back(instruction);
}

test_generator::test_generator(vector<Instruction> instruction, bool batch, int nbr_test_per_file, int max_test, string dir) :
_batch(batch),
_number_test_per_file(nbr_test_per_file),
_max_test(max_test),
_assembly(),
_is_easy(0),
_instructions(instruction)
{
    (dir == ".") ? _path = "" : _path = dir;
}
/*------------------------------------------------------------
                    START UP FUNCTION
--------------------------------------------------------------*/
void test_generator::start_logo(){
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

bool test_generator::print_menu(){
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
    cout << R"(
        Where do you want to write the files ?
        )" << endl;
    while(_path == ""){
        cin >> _path;
    }
    return (is_easy == "A") ?  true : false;

}

void test_generator::test_per_file(){
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
    _number_test_per_file = test;
}

void test_generator::start(){
    start_logo();
    _is_easy = print_menu();
    test_per_file();
}

void test_generator::build_tests(){
    if (!_batch) start();
    _assembly  = R"(
.section .text
.global _start

_start :
)";
    // Instruction needed for the generation :
    Instruction li  ("li"    , true   , R_type);
    Instruction bne ("bne"   , false  , B_type);

    vector<int> rd_vec(32);
    vector<int> rs1_vec(32);
    vector<int> rs2_vec(32);
    initialise_registers(rd_vec, rs1_vec, rs2_vec);
    // This allow to randomise the combinaison of registers
    random_device rd_seed;
    random_device rs1_seed;
    random_device rs2_seed;
    mt19937 rd_mt (rd_seed());
    mt19937 rs1_mt(rs1_seed());
    mt19937 rs2_mt(rs2_seed());
    shuffle(rd_vec.begin(),  rd_vec.end(),  rd_mt);
    shuffle(rs1_vec.begin(), rs1_vec.end(), rs1_mt);
    shuffle(rs2_vec.begin(), rs2_vec.end(), rs2_mt);

    for(auto it = _instructions.begin(); it != _instructions.end(); it++){
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
       int number_of_tests = 0;
       int number_of_file = 0;

        switch(it->getType()){
            case R_type :
                if(it->IsImmediat() == false){
                    for (auto rd_it = rd_vec.begin(); rd_it != rd_vec.end(); rd_it++){
                        for(auto rs1_it = rs1_vec.begin(); rs1_it != rs1_vec.end(); rs1_it++){
                            for(auto rs2_it = rs2_vec.begin(); rs2_it != rs2_vec.end(); rs2_it++)
                            {
                                int rd = *rd_it;
                                int rs1 = *rs1_it;
                                int rs2 = *rs2_it;
                                // FILE NAME GESTION
                                if(number_of_tests % _number_test_per_file == 0){
                                    // We put the following instruction
                                    // at the end of each files
                                    if(number_of_file != 0){
                                        file << "   " << "_exit :" << endl;
                                        file << "       " << "j _good"<< endl;
                                        file.close();
                                    }
                                    // This part of code is called every time we create a new file
                                    create_directory(_path, it->getName());
                                    string file_name = filename(_path, it->getName(), number_of_file);
                                    #ifdef DEBUG
                                    cout << "file name is : "<< file_name << endl;
                                    #endif
                                    if (number_of_file == _max_test) goto end_loops;
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
                    for (auto rd_it = rd_vec.begin(); rd_it != rd_vec.end(); rd_it++){
                        for(auto rs1_it = rs1_vec.begin(); rs1_it != rs1_vec.end(); rs1_it++){
                            for(auto rs2_it = rs2_vec.begin(); rs2_it != rs2_vec.end(); rs2_it++)
                            {
                                int rd = *rd_it;
                                int rs1 = *rs1_it;
                                int rs2 = *rs2_it;
                                // FILE NAME GESTION
                                if(number_of_tests % _number_test_per_file == 0){
                                    if(number_of_file != 0){
                                        file << "   " << "_exit :" << endl;
                                        file << "       " << "j _good"<< endl;
                                        file.close();
                                    }
                                    create_directory(_path, it->getName());
                                    string file_name = filename(_path, it->getName(),number_of_file);
                                    if (number_of_file == _max_test) goto end_loops;
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
                    if (number_of_file == _max_test) goto end_loops;
                }
                break;
        }
        end_loops:
        file << "   " << "j _good" ;
        file.close();
        // Label to exit all nested loops
    }

}