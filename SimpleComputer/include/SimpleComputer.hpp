#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <signal.h>
#include <sys/time.h>

#include "myBigChars.hpp"
#include "myReadkey.hpp"

using namespace std;

#define SIZE 100

#define OVERFLOW 1   //П ---> 00001
#define DIVISION_ZERO 2   //0 ---> 00010
#define MEMORY_ACCESS 3   //М ---> 00100
#define CLOCK_IGNORE 4   //Т ---> 01000
#define UNKNOWN_COMMAND 5   //Е ---> 10000


int sc_memoryInit(int *ram);
int sc_memorySet(int *ram, int *registr, int address, int value);
int sc_memoryGet(int *ram, int *registr, int address, int *value);
int sc_memorySave(int *ram, int *registr, char *filename);
int sc_memoryLoad(int *ram, int *registr, char *filename);
void sc_regInit(int *registr);
int sc_regSet(int *registr, int flag, int value);
int sc_regGet(int *registr, int flag, int *value);
int sc_commandEncode(int *registr, int command, int operand, int *value);
int sc_commandDecode(int *registr, int *value, int *command, int *operand);


int sc_interface();
void memory_output(int *ram, int instruction);
void accumulator_output(int *accumulator);
void big_char_output(int *ram, int instruction);
void instruction_output(int *ram, int instruction);
void operation_output(int command, int operand);
void flags_output(int *registr);
int instruction_check(int *ram, int *registr, int instruction);
int value_input();
void file_input(char *filename);
void run_timer(int *ram, int instruction);


int ALU(int *ram, int *accumulator, int *registr, int *user_value, int command, int operand);
int CU(int *ram, int *accumulator, int *registr, int *instruction);
int asmb(int *ram, int *accumulator, int *registr, int *instruction, char *filename);
int command_to_code(string command);
