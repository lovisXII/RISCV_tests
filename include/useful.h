#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>

#ifndef CONSTANTS_H
#define CONSTANTS_H

// #define DEBUG
#define USER        0b00
#define SUPERVISOR  0b01
#define MACHINE     0b11

#define NONE        0b00 << 2
#define READ_ONLY   0b01 << 2
#define WRITE_ONLY  0b01 << 2
#define R_W         0b11 << 2

enum TYPE { R_type, B_type, J_type, MEM_type };
#endif // CONSTANTS_H

using namespace std;

bool isNumeric(const std::string& str);
void initialise_registers(vector<int> &rd, vector<int> &rs1, vector<int> &rs2);
void create_directory(string path, string name);
string filename(string path, string name, int number);
void open_checked(ofstream& file, const string& file_name);
int operation(string instruction, int op1, int op2);
void priviledge_test();
void test_instruction(std::string instruction_name);