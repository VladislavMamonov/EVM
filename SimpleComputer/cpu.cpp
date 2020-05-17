#include "include/SimpleComputer.hpp"


int CU(int *ram, int *registr, int cursor_position)
{
  int *value = new int;
  int *command = new int;
  int *operand = new int;
  int *user_value = new int;

  if (sc_memoryGet(ram, registr, cursor_position, value) == 1)
    return 1;

  if (sc_commandDecode(registr, value, command, operand) == 1)
    return 1;

  switch (*command)
  {
    case 10: {   //READ
      *user_value = value_input();
      sc_memorySet(ram, registr, *operand, *user_value);
      break;
    }

    case 11: {
      exit_charset_mode();

      sc_memoryGet(ram, registr, *operand, user_value);
      mt_gotoXY(29, 13);
      cout << *user_value;

      enter_charset_mode();
      break;
    }


    default:
      break;
  }


  delete value;
  delete command;
  delete operand;
  delete user_value;

  return 0;
}
