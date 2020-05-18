#include "include/SimpleComputer.hpp"


int ALU(int *ram, int *accumulator, int *registr, int *user_value, int command, int operand)
{
  switch (command)
  {
    case 30:  //ADD
      sc_memoryGet(ram, registr, operand, user_value);
      *accumulator += *user_value;
      if (*accumulator > 32767 || *accumulator < -32768) {
        sc_regSet(registr, OVERFLOW, 1);
        *accumulator = 0;
      }
      break;

    case 31:  //SUB
      sc_memoryGet(ram, registr, operand, user_value);
      *accumulator -= *user_value;
      if (*accumulator > 32767 || *accumulator < -32768) {
        sc_regSet(registr, OVERFLOW, 1);
        *accumulator = 0;
      }
      break;

    case 32:  //DIVIDE
      sc_memoryGet(ram, registr, operand, user_value);
      if (*user_value == 0) {
        sc_regSet(registr, DIVISION_ZERO, 1);
        break;
      }
      *accumulator /= *user_value;
      if (*accumulator > 32767 || *accumulator < -32768) {
        sc_regSet(registr, OVERFLOW, 1);
        *accumulator = 0;
      }
      break;

    case 33:  //MUL
      sc_memoryGet(ram, registr, operand, user_value);
      *accumulator *= *user_value;
      if (*accumulator > 32767 || *accumulator < -32768) {
        sc_regSet(registr, OVERFLOW, 1);
        *accumulator = 0;
      }
      break;

    default:
      break;
  }

  return 0;
}


int CU(int *ram, int *accumulator, int *registr, int cursor_position)
{
  int *value = new int;
  int *command = new int;
  int *operand = new int;
  int *user_value = new int;

  if (sc_memoryGet(ram, registr, cursor_position, value) == 1)
    return 1;

  if (sc_commandDecode(registr, value, command, operand) == 1)
    return 1;

  if (*command > 29 && *command < 34)
    ALU(ram, accumulator, registr, user_value, *command, *operand);

  switch (*command)
  {
    case 10:   //READ
      *user_value = value_input();
      sc_memorySet(ram, registr, *operand, *user_value);
      break;

    case 11:  //WRITE
      exit_charset_mode();
      sc_memoryGet(ram, registr, *operand, user_value);
      mt_gotoXY(29, 13);
      cout << *user_value;
      enter_charset_mode();
      break;

    case 20:  //LOAD
      sc_memoryGet(ram, registr, *operand, user_value);
      *accumulator = *user_value;
      break;

    case 21:  //STORE
      sc_memorySet(ram, registr, *operand, *accumulator);
      break;

    default:
      break;
  }

  delete value;
  delete command;
  delete operand;
  delete user_value;

  return 0;
}
