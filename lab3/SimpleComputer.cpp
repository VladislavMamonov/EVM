#include "SimpleComputer.hpp"


int sc_memoryInit(int *ram)
{
  for (int i = 0; i < SIZE; i++)
    ram[i] = 0;

  return *ram;
}


int sc_memorySet(int *ram, int *registr, int address, int value)
{
  if (address >= 0 && address < SIZE) {
    ram[address] = value;
    return 0;
  }
  else {
    cout << "memory access error" << endl;
    sc_regSet(registr, MEMORY_ACCESS, 1);
    return 1;
  }
}


int sc_memoryGet(int *ram, int *registr, int address, int *value)
{
  if (address >= 0 && address < SIZE) {
    *value = ram[address];
    cout << "memory value: " << *value << endl;
    return *value;
  }
  else {
    cout << "memory access error" << endl;
    sc_regSet(registr, MEMORY_ACCESS, 1);
    return 1;
  }
}


int sc_memorySave(int *ram, char *filename)
{
  FILE *data;

  if ((data = fopen(filename, "wb")) == NULL) {
    cout << "cannot open file" << endl;
    return 1;
  }

  fwrite(ram, sizeof(int), SIZE, data);
  fclose(data);

  return 0;
}


int sc_memoryLoad(int *ram, char *filename)
{
  FILE *data;

  if ((data = fopen(filename, "rb")) == NULL) {
    cout << "cannot open file" << endl;
    return 1;
  }

  if (fread(ram, sizeof(int), SIZE, data) != SIZE) {
    if (feof(data))
      cout << "premature end of file" << endl;
    else cout << "file read error" << endl;

    return 1;
  }

  fread(ram, sizeof(int), SIZE, data);

  fclose(data);
  return 0;
}


void sc_regInit(int *registr)
{
  *registr = 0;
}


int sc_regSet(int *registr, int flag, int value)
{
  if (flag != OVERFLOW && flag != DIVISION_ZERO
    && flag != MEMORY_ACCESS && flag != CLOCK_IGNORE && flag != UNKNOWN_COMMAND) {
    cout << "unknown flag" << endl;
    return 1;
  }

  switch(value)
  {
    case 1:
      *registr = *registr | (1 << (flag - 1));
      break;

    case 0:
      *registr = *registr & (~(1 << (flag - 1)));
      break;

    default:
      cout << "invalid flag value" << endl;
      return 1;
      break;
  }
  return 0;
}


int sc_regGet(int *registr, int flag, int *value)
{
  if (flag != OVERFLOW && flag != DIVISION_ZERO
    && flag != MEMORY_ACCESS && flag != CLOCK_IGNORE && flag != UNKNOWN_COMMAND) {
    cout << "unknown flag" << endl;
    return 1;
  }

  *value = (*registr >> (flag - 1)) & 0x1;

  return *value;
}


int sc_commandEncode(int *registr, int command, int operand, long int *value)
{
  if (command < 10 || (command > 11 && command < 20) ||
    (command > 21 && command < 30) || (command > 33 && command < 40) ||
    (command > 43 && command < 51) || command > 76) {
      cout << "unknown command" << endl;
      sc_regSet(registr, UNKNOWN_COMMAND, 1);
      return 1;
    }

  if (operand > 79) {
    cout << "accumulator overflow" << endl;
    sc_regSet(registr, OVERFLOW, 1);
    return 1;
  }

  int HighDigit = operand / 10;
  operand = operand + HighDigit * 6;

  HighDigit = command / 10;
  command = command + HighDigit * 6;

  int OperandEncoded = 0;
  int CommandEncoded = 0;
  int digit = 1;

  while (operand != 0) {
    OperandEncoded += (operand % 2) * digit;
    operand = operand / 2;
    digit = digit * 10;
  }

  digit = 1;

  while (command != 0) {
    CommandEncoded += (command % 2) * digit;
    command = command / 2;
    digit = digit * 10;
  }

  string str_CommandEncoded = to_string(CommandEncoded);
  string str_OperandEncoded = to_string(OperandEncoded);

  while (str_CommandEncoded.size() < 7) {
    str_CommandEncoded.insert(str_CommandEncoded.begin(), '0');   //Добавляем нули в начало
  }                                                               //Чтобы число занимало 7 бит
                                                                  //10111 ----> 0010111
  while (str_OperandEncoded.size() < 7) {
    str_OperandEncoded.insert(str_OperandEncoded.begin(), '0');
  }

  string value_conc = str_CommandEncoded + str_OperandEncoded;;
  *value = atol(value_conc.c_str());

  return 0;
}


int sc_commandDecode(int *registr, long int *value, int *command, int *operand)
{
  string value_str = to_string(*value);
  string command_str, operand_str;

  for (long unsigned int i = 0; i < value_str.size() - 7; i++)    //извлекаем код операции
    command_str.insert(command_str.begin() + i, value_str[i]);

  for (long unsigned int i = value_str.size() - 7, j = 0; i < value_str.size(); i++, j++)    //извлекаем операнд
    operand_str.insert(operand_str.begin() + j, value_str[i]);

  *command = stoi(command_str, nullptr, 2);
  *operand = stoi(operand_str, nullptr, 2);     //перевод в бинарный вид


  int OperandDecoded = 0;
  int CommandDecoded = 0;
  int digit = 1;

  while (*command != 0) {
    CommandDecoded += (*command % 16) * digit;
    *command = *command / 16;
    digit = digit * 10;
  }

  digit = 1;

  while (*operand != 0) {
    OperandDecoded += (*operand % 16) * digit;
    *operand = *operand / 16;
    digit = digit * 10;
  }

  *command = CommandDecoded;
  *operand = OperandDecoded;

  if (*command < 10 || (*command > 11 && *command < 20) ||
    (*command > 21 && *command < 30) || (*command > 33 && *command < 40) ||
    (*command > 43 && *command < 51) || *command > 76) {
      cout << "unknown command" << endl;
      sc_regSet(registr, UNKNOWN_COMMAND, 1);
      return 1;
    }

  if (*operand > 79) {
    cout << "unknown command" << endl;
    sc_regSet(registr, UNKNOWN_COMMAND, 1);
    return 1;
  }

  return 0;
}


int sc_interface()
{
  bc_box(5, 5, 62, 12);
  bc_box(17, 5, 48, 10);
  bc_box(5, 67, 25, 3);
  bc_box(8, 67, 25, 3);
  bc_box(11, 67, 25, 3);
  bc_box(14, 67, 25, 3);
  bc_box(17, 54, 38, 10);

  exit_charset_mode();

  mt_gotoXY(5, 33);
  cout << "Memory";
  mt_gotoXY(5, 74);
  cout << "Accumulator";
  mt_gotoXY(8, 70);
  cout << "InstructionCounter";
  mt_gotoXY(11, 75);
  cout << "Operation";
  mt_gotoXY(14, 77);
  cout << "Flags";
  mt_gotoXY(17, 70);
  cout << "Keys:";
  mt_gotoXY(18, 55);
  cout << "l - load";
  mt_gotoXY(19, 55);
  cout << "s - save";
  mt_gotoXY(20, 55);
  cout << "r - run";
  mt_gotoXY(21, 55);
  cout << "t - step";
  mt_gotoXY(22, 55);
  cout << "i - reset";
  mt_gotoXY(23, 55);
  cout << "F5 - accumulator";
  mt_gotoXY(24, 55);
  cout << "F6 - InstructionCounter";


  enter_charset_mode();

  int x = 6, y = 6;
  mt_gotoXY(x, y);

  cout << "+4583" << " ";       //temp
  for (int i = 1; i < SIZE; i++) {
    cout << "+0000" << " ";
    if (i % 10 == 9) {
      x++;
      mt_gotoXY(x, y);
    }
  }

  mt_gotoXY(6, 76);
  char accumulator[4] = {'4', '5', '8', '3'};
  cout << "+" << accumulator;

  mt_gotoXY(9, 76);
  int InstructionCounter = 0;
  cout << "+" << "000" << InstructionCounter;

  mt_gotoXY(12, 76);
  int Operation = 0;
  cout << "+" << "0" << Operation << ":" << "00";

  mt_gotoXY(15, 77);
  int *registr = new int;
  int *value = new int;
  string flag_value;

  sc_regSet(registr, OVERFLOW, 1);
  sc_regSet(registr, MEMORY_ACCESS, 1);
  sc_regSet(registr, UNKNOWN_COMMAND, 1);

  for (int i = 1; i <= 5; i++) {
    if (sc_regGet(registr, i, value) == 1) {
      switch (i)
      {
        case 1:
          flag_value = "O";
          break;

        case 2:
          flag_value = "D";
          break;

        case 3:
          flag_value = "M";
          break;

        case 4:
          flag_value = "C";
          break;

        case 5:
          flag_value = "U";
          break;
      }
      cout << flag_value << " ";
    }
  }


  bc_printnumber('+', 18, 6, COLOR_WHITE, COLOR_GRAY);

  for (long unsigned int i = 0, y = 16; i < 4; i++) {
    bc_printnumber(accumulator[i], 18, y, COLOR_WHITE, COLOR_GRAY);
    y += 9;
  }


  mt_gotoXY(40, 0);

  return 0;
}
