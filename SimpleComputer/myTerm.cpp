#include "include/myTerm.hpp"


int mt_getscreensize(unsigned short *rows, unsigned short *cols)
{
  struct winsize ws;

  if (!ioctl(1, TIOCGWINSZ, &ws)) {
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
  }
  else fprintf(stderr, "Ошибка.\n");

  return 1;
}


int mt_clrscr()
{
  cout << "\E[H\E[J";

  return 0;
}


int mt_gotoXY(int x, int y)
{
  cout << "\E[" << x << ";" << y << "H";

  return 0;
}


int mt_setfgcolor(colors color)
{
  cout << "\E[3" << color << "m";

  return 0;
}


int mt_setbgcolor(colors color)
{
  cout << "\E[4" << color << "m";

  return 0;
}


int mt_clrline(int x, int y)
{
  mt_gotoXY(x, y);
  cout << "                                            ";

  return 0;
}
