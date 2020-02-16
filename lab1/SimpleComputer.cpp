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
