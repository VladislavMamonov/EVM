#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


using namespace std;

enum colors {
  COLOR_DARKGREEN,
  COLOR_RED,
  COLOR_GREEN,
  COLOR_YELLOW,
  COLOR_BLUE,
  COLOR_PURPLE,
  COLOR_CYAN,
  COLOR_GRAY,
  COLOR_WHITE = 9,
};


int mt_clrscr();
int mt_gotoXY(int x, int y);
int mt_getscreensize(unsigned short *rows, unsigned short *cols);
int mt_setfgcolor(colors color);
int mt_setbgcolor(colors color);
int mt_clrline(int x, int y);
