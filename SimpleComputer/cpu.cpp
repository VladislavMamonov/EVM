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


int CU(int *ram, int *accumulator, int *registr, int *instruction)
{
  int value;
  int command;
  int operand;
  int user_value;

  if (sc_memoryGet(ram, registr, *instruction, &value) == 1)
    return 1;

  command = value / 100;
  operand = value % 100;

  sc_commandEncode(registr, command, operand, &value);
  if (sc_commandDecode(registr, &value, &command, &operand) == 1)
    return 1;

  if (command > 29 && command < 34) {
    ALU(ram, accumulator, registr, &user_value, command, operand);
    *instruction += 1;
  }

  switch (command)
  {
    case 10:   //READ
      user_value = value_input();
      sc_memorySet(ram, registr, operand, user_value);
      *instruction += 1;
      break;


    case 11:  //WRITE
      exit_charset_mode();
      sc_memoryGet(ram, registr, operand, &user_value);
      mt_gotoXY(29, 13);
      cout << user_value;
      enter_charset_mode();
      *instruction += 1;
      break;


    case 20:  //LOAD
      sc_memoryGet(ram, registr, operand, &user_value);
      *accumulator = user_value;
      *instruction += 1;
      break;


    case 21:  //STORE
      sc_memorySet(ram, registr, operand, *accumulator);
      *instruction += 1;
      break;


    case 40:  //JUMP
      if (operand < 0 || operand > 99) {
        sc_regSet(registr, MEMORY_ACCESS, 1);
        *instruction += 1;
        break;
      }
      *instruction += 1;
      break;


    case 41:  //JNEG
      if (operand < 0 || operand > 99) {
        sc_regSet(registr, MEMORY_ACCESS, 1);
        *instruction += 1;
        break;
      }
      if (*accumulator < 0) {
        *instruction = operand;
        break;
      }
      *instruction += 1;
      break;


    case 42:  //JZ
      if (operand < 0 || operand > 99) {
        sc_regSet(registr, MEMORY_ACCESS, 1);
        *instruction += 1;
        break;
      }
      if (*accumulator == 0) {
        *instruction = operand;
        break;
      }
      *instruction += 1;
      break;


    case 43:  //HALT
      return 1;
      break;


    case 58:  //JP
      if (operand < 0 || operand > 99) {
        sc_regSet(registr, MEMORY_ACCESS, 1);
        *instruction += 1;
        break;
      }
      if (*accumulator % 2 == 0) {
        *instruction = operand;
        break;
      }
      *instruction += 1;
      break;

    default:
      break;
  }

  return 0;
}


int command_to_code(string command)
{
  if (command == "READ")
    return 10;

  if (command == "WRITE")
    return 11;

  if (command == "LOAD")
    return 20;

  if (command == "STORE")
    return 21;

  if (command == "ADD")
    return 30;

  if (command == "SUB")
    return 31;

  if (command == "DIVIDE")
    return 32;

  if (command == "MUL")
    return 33;

  if (command == "JUMP")
    return 40;

  if (command == "JNEG")
    return 41;

  if (command == "JZ")
    return 42;

  if (command == "HALT")
    return 43;

  if (command == "JP")
    return 58;

  return -1;
}


int asmb(int *ram, int *accumulator, int *registr, int *instruction, char *filename)
{
  exit_charset_mode();
  mt_clrline(31, 6);

  ifstream file(filename);
  string line;
  string variable;
  vector<string> vec;

  while (getline(file, line))
  {
    stringstream ss(line);

    while (ss) {
      string buf;
      ss >> buf;
      if (buf == ";" || buf == "") break;

      vec.push_back(buf);
    }
  }

  int address, command, operand;
  int line_n = 0;

  for (long unsigned int i = 0; i < vec.size(); i += 3)
  {
    line_n++;

    if (i + 2 >= vec.size()) {
      mt_clrline(31, 6);
      mt_gotoXY(31, 6);
      cout << "line " << line_n << ": unknown symbol in operand";
      enter_charset_mode();
      return -1;
    }

    if (vec[i].find_first_not_of("+0123456789") != string::npos) {
      mt_clrline(31, 6);
      mt_gotoXY(31, 6);
      cout << "line " << line_n << ": unknown symbol in address";
      enter_charset_mode();
      return -1;
    }

    if ((vec[i + 2].find_first_not_of("+0123456789") != string::npos)) {
      mt_clrline(31, 6);
      mt_gotoXY(31, 6);
      cout << "line " << line_n << ": unknown symbol in operand";
      enter_charset_mode();
      return -1;
    }

    address = stoi(vec[i]);
    operand = stoi(vec[i + 2]);

    if (vec[i + 1] != "=") {
      command = command_to_code(vec[i + 1]);
      if (command == -1 || i + 1 >= vec.size()) {
        mt_gotoXY(31, 6);
        cout << "line " << line_n << ": unknown command";
        enter_charset_mode();
        return -1;
      }
      sc_memorySet(ram, registr, address, command * 100 + operand);
    } else
      sc_memorySet(ram, registr, address, stoi(vec[i + 2]));
  }

  enter_charset_mode();
  return 0;
}
