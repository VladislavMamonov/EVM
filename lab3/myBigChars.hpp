#pragma once

#include "myTerm.hpp"
#include <iostream>

using namespace std;

#define ACS_CKBOARD "a";


void enter_charset_mode();
void exit_charset_mode();
int bc_printA(string *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printbigchar(int[2], int x, int y, colors bg, colors fg);
int bc_setbigcharpos(int *big, int x, int y, int value);
int bc_getbigcharpos(int *big, int x, int y, int *value);
int bc_bigcharwrite(int fd, int *big, int count);
int bc_bigcharread(int fd, int *big, int need_count, int *count);
