#include "myBigChars.hpp"


void enter_charset_mode()
{
  cout << "\E(0";
}


void exit_charset_mode()
{
  cout << "\E(B";
}


int bc_printA(string *str)
{
  cout << *str << endl;

  return 0;
}


int bc_box(int x1, int y1, int x2, int y2)
{
  mt_gotoXY(x1, y1);
  cout << "l";

  for (int i = 0; i < x2 - 2; i++)
    cout << "q";

  cout << "k";

  for (int i = 1; i < y2; i++) {
    if (i + 1 == y2) {
      mt_gotoXY(x1 + i, y1);
      cout << "m";
      break;
    }
    mt_gotoXY(x1 + i, y1);
    cout << "x";
  }

  for (int i = 0; i < x2 - 2; i++)
    cout << "q";

  for (int i = 1; i < y2; i++) {
    if (i + 1 == y2) {
      mt_gotoXY(x1 + i, y1 + (x2 - 1));
      cout << "j";
      break;
    }
    mt_gotoXY(x1 + i, y1 + (x2 - 1));
    cout << "x";
  }

  cout << endl;
  return 0;
}


int bc_printbigchar(int arr[2], int x, int y, colors bg, colors fg)
{
  mt_setbgcolor(bg);
  mt_setfgcolor(fg);

  mt_gotoXY(x, y);
  int arr_size = 2;

  for (int j = 0; j < arr_size; j++)
    for (int i = 0; i < 32; i++) {
      int byte = (arr[j] >> (i - 1)) & 0x1;

      //cout << byte;
      if (byte == 0)
        cout << " ";
      else
        cout << ACS_CKBOARD;

      if (i % 8 == 0) {
        mt_gotoXY(x++, y);
      }
    }

  cout << endl;
  return 0;
}


int bc_setbigcharpos(int *big, int x, int y, int value)
{
  if (x < 1 || x > 8 || y < 1 || y > 8 || value > 1 || value < 0)
    return 1;

  int index = 0;

  if (x > 4) index = 1;

  if (value == 1) big[index] |= 1 << ((x - 1) * 8 + y - 1);
  else if (value == 0) big[index] &= ~(1 << ((x - 1) * 8 + y - 1));


  return 0;
}


int bc_getbigcharpos(int *big, int x, int y, int *value)
{
  if (x < 1 || x > 8 || y < 1 || y > 8)
    return 1;

  int index = 0;

  if (x > 4) index = 1;

  *value = (big[index] >> ((x - 1) * 8 + y - 1)) & 0x1;

  return 0;
}


int bc_bigcharwrite(int fd, int *big, int count)
{
  write(fd, big, sizeof(int) * count);

  return 0;
}


int bc_bigcharread(int fd, int *big, int need_count, int *count)
{
  *count = read(fd, big, sizeof(int) * need_count);

  if (*count == -1 || *count != need_count)
    return 1;

  return 0;
}
