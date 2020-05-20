#include "include/asm.hpp"


int asmb(int *ram, int *accumulator, int *registr, int *instruction, char *filename)
{
  ifstream file(filename);
  string line;
  string variable;
  vector<string> vec;

  exit_charset_mode();
  mt_gotoXY(32, 0);

  while (getline(file, line))
  {
    stringstream ss(line);

    while (ss) {
      string buf;
      ss >> buf;
      if (buf == ";") break;

      vec.push_back(buf);
    }
  }

  // for (auto i = 0; i < vec.size(); i++) {
  //   if (vec[i] == " ") vec.erase(i);
  //   cout << vec[i] << endl;
  // }

  enter_charset_mode();

  return 0;
}
