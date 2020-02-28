#include "myTerm.hpp"


int main()
{
  if (isatty(0)) {
    cout << "Поток ввода связан с терминалом" << ttyname(0) << endl;
  } else
    cout << "Поток ввода не связан с терминалом." << endl;


  if (isatty(1)) {
    cout << "Поток вывода связан с терминалом" << ttyname(1) << endl;
  } else
    cout << "Поток вывода не связан с терминалом." << endl;


  if (isatty(2)) {
    cout << "Поток ошибок связан с терминалом" << ttyname(2) << endl;
  } else
    cout << "Поток ошибок не связан с терминалом." << endl;


  int fd, read_chars;
  char buf[200];

  fd = open("/dev/tty3", O_WRONLY);

  if (fd == -1) {
    fprintf(stderr, "Ошибка открытия терминала.\n");
  }

  if ((read_chars = read(0, buf, 199)) > 0) {
    write(fd, buf, read_chars);
  } else {
    write(fd, "Ошибка", 6);
  }
  close(fd);


  unsigned short *rows = new unsigned short();
  unsigned short *cols = new unsigned short();

  mt_getscreensize(rows, cols);
  mt_clrscr();

  cout << "Число строк - " << *rows << endl;
  cout << "Число столбцов - " << *cols << endl;

  mt_gotoXY(10, 5);
  mt_setfgcolor(COLOR_GRAY);
  mt_setbgcolor(COLOR_RED);
  cout << "LOL" << endl;

  mt_setbgcolor(COLOR_WHITE);
  mt_gotoXY(12, 1);

  return 0;
}
