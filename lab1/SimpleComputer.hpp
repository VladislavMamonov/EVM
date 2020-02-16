#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

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
int sc_commandEncode(int *registr, int command, int operand, long int *value);
int sc_commandDecode(int *registr, long int *value, int *command, int *operand);
