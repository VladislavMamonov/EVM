#include "myBigChars.hpp"


int main()
{
  enter_charset_mode();

  string *str = new string;
  *str = "dkgdjfkdlfmdk";

  int arr[2] = {255, 180};
  int *value = new int;
  int *count = new int;

  bc_printA(str);
  bc_box(5, 10, 8, 8);
  bc_box(5, 20, 8, 8);
  bc_box(5, 30, 8, 8);
  bc_printbigchar(arr, 15, 10, COLOR_WHITE, COLOR_CYAN);
  bc_setbigcharpos(arr, 1, 7, 0);
  bc_setbigcharpos(arr, 5, 4, 1);
  bc_getbigcharpos(arr, 5, 4, value);

  for (int i = 0; i < 2; i++)
    cout << arr[i] << endl;

  bc_printbigchar(arr, 24, 10, COLOR_WHITE, COLOR_CYAN);

  int fd = open("qwerty.txt", O_WRONLY);

  if (fd == -1) {
    cout << "Ошибка открытия файла" << endl;
    exit_charset_mode();
    return 1;
  }

  bc_bigcharwrite(fd, arr, 2);
  close(fd);

  bc_setbigcharpos(arr, 1, 7, 1);

  fd = open("qwerty.txt", O_RDONLY);
  bc_bigcharread(fd, arr, 2, count);
  close(fd);

  bc_printbigchar(arr, 24, 10, COLOR_WHITE, COLOR_CYAN);

  exit_charset_mode();
  mt_gotoXY(35, 0);
  cout << "Value: " << *value << endl;

  return 0;
}
