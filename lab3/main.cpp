#include "SimpleComputer.hpp"


int main()
{
  enter_charset_mode();

  string *str = new string;
  *str = "dkgdjfkdlfmdk";

  mt_clrscr();
  sc_interface();

  exit_charset_mode();

  return 0;
}
