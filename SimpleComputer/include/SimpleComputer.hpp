#pragma once
#include <iostream>
#include <string>
#include <fstream>
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
int sc_memorySave(int *ram, char *filename);
int sc_memoryLoad(int *ram, char *filename);
void sc_regInit(int *registr);
int sc_regSet(int *registr, int flag, int value);
int sc_regGet(int *registr, int flag, int *value);
int sc_commandEncode(int *registr, int command, int operand, int *value);
int sc_commandDecode(int *registr, int *value, int *command, int *operand);


int sc_interface();
void memory_output(int *ram, int cursor_position);
void accumulator_output(int *accumulator);
void big_char_output(int *ram, int cursor_position);
void instruction_output(int *ram, int cursor_position);
void flags_output(int *registr);
int cursor_position_check(int *ram, int *registr, int cursor_position);
int value_input();
void sc_run(int *ram, int cursor_position);


int ALU(int *ram, int *accumulator, int *registr, int *user_value, int command, int operand);
int CU(int *ram, int *accumulator, int *registr, int cursor_position);
