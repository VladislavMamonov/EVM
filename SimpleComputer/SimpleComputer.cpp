#include "include/SimpleComputer.hpp"


bool isTimer = true;
bool is_CI = true;

void TimerOff(int signo)
{
  isTimer = false;
}


void CI_On(int signo)
{
  is_CI = true;
}


int sc_memoryInit(int *ram)
{
  for (int i = 0; i < SIZE; i++)
    ram[i] = 0;

  return *ram;
}


int sc_memorySet(int *ram, int *registr, int address, int value)
{
  if (address < 0 || address >= SIZE) {
    sc_regSet(registr, MEMORY_ACCESS, 1);
    return 1;
  }

  if (value > 32767 || value < -32768) {
    sc_regSet(registr, OVERFLOW, 1);
    return 1;
  }

  ram[address] = value;

  return 0;
}


int sc_memoryGet(int *ram, int *registr, int address, int *value)
{
  if (address >= 0 && address < SIZE) {
    *value = ram[address];
    return *value;
  }
  else {
    sc_regSet(registr, MEMORY_ACCESS, 1);
    return 1;
  }
}


int sc_memorySave(int *ram, int *registr, char *filename)
{
  FILE *data;

  if ((data = fopen(filename, "wb")) == NULL) {
    return 1;
  }

  fwrite(ram, sizeof(int), SIZE, data);
  fwrite(registr, sizeof(int), 1, data);
  fclose(data);

  return 0;
}


int sc_memoryLoad(int *ram, int *registr, char *filename)
{
  FILE *data;

  char buf[3];
  int j = 0;

  for (long unsigned int i = strlen(filename) - 3; i < strlen(filename); i++) {
    buf[j] = filename[i];
    j++;
  }

  if ((data = fopen(filename, "rb")) == NULL)
    return -1;

  if (strcmp(buf, ".sa") == 0) {
    return 1;
  }

  fread(ram, sizeof(int), SIZE, data);
  fread(registr, sizeof(int), 1, data);

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
    return 1;
  }

  *value = (*registr >> (flag - 1)) & 0x1;

  return *value;
}


int sc_commandEncode(int *registr, int command, int operand, int *value)
{
  if (command < 10 || (command > 11 && command < 20) ||
    (command > 21 && command < 30) || (command > 33 && command < 40) ||
    (command > 43 && command < 51) || command > 76) {
      sc_regSet(registr, UNKNOWN_COMMAND, 1);
      return 1;
    }

  *value = (command << 7) | operand;

  return 0;
}


int sc_commandDecode(int *registr, int *value, int *command, int *operand)
{
  *command = *value >> 7;
  *operand = *value & 0x7F;

  if (*command < 10 || (*command > 11 && *command < 20) ||
    (*command > 21 && *command < 30) || (*command > 33 && *command < 40) ||
    (*command > 43 && *command < 51) || *command > 76) {
      sc_regSet(registr, UNKNOWN_COMMAND, 1);
      return 1;
    }

  return 0;
}


int instruction_check(int *ram, int *registr, int instruction)
{
  if (instruction < 0 || instruction > 99) {
    sc_regSet(registr, MEMORY_ACCESS, 1);
    return 1;
  }

  return 0;
}


void memory_output(int *ram, int instruction)
{
  int x = 6, y = 6;
  mt_gotoXY(x, y);

  for (int i = 0; i < SIZE; i++)
  {
    if (i == instruction)
      mt_setbgcolor(COLOR_CYAN);

    if (ram[i] < 10) {
      cout << "+000" << ram[i] << " ";
    }

    if (ram[i] > 9 && ram[i] < 100) {
      cout << "+00" << ram[i] << " ";
    }

    if (ram[i] > 99 && ram[i] < 1000) {
      cout << "+0" << ram[i] << " ";
    }

    if (ram[i] > 999) {
      cout << "+" << ram[i] << " ";
    }

    mt_setbgcolor(COLOR_WHITE);

    if (i % 10 == 9) {
      x++;
      mt_gotoXY(x, y);
    }
  }
  for (int i = 0; i < 12; i++)
    cout << endl;               //убирает подальше стандартный курсор
}


void accumulator_output(int *accumulator)
{
  mt_clrline(6, 76);
  mt_gotoXY(6, 76);

  if (*accumulator < 10 && *accumulator > -1)
    cout << "+000" << *accumulator;

  if (*accumulator > 9 && *accumulator < 100)
    cout << "+00" << *accumulator;

  if (*accumulator > 99 && *accumulator < 1000)
    cout << "+0" << *accumulator;

  if (*accumulator > 999)
    cout << "+" << *accumulator;

  if (*accumulator < 0 && *accumulator > -10)
    cout << "-000" << abs(*accumulator);

  if (*accumulator < -9 && *accumulator > -100)
    cout << "-00" << abs(*accumulator);

  if (*accumulator < -99 && *accumulator > -1000)
    cout << "-0" << abs(*accumulator);

  if (*accumulator < -999)
    cout << "-" << abs(*accumulator);
}


void big_char_output(int *ram, int instruction)
{
  if (ram[instruction] < 10) {
    for (int i = 0, y = 16; i < 3; i++) {
      bc_printnumber('0', 18, y, COLOR_WHITE, COLOR_GRAY);
      y += 9;
    }
  }

  if (ram[instruction] > 9 && ram[instruction] < 100) {
    for (int i = 0, y = 16; i < 2; i++) {
      bc_printnumber('0', 18, y, COLOR_WHITE, COLOR_GRAY);
      y += 9;
    }
  }

  if (ram[instruction] > 99 && ram[instruction] < 1000) {
    bc_printnumber('0', 18, 16, COLOR_WHITE, COLOR_GRAY);
  }

  string memory = to_string(ram[instruction]);

  bc_printnumber('+', 18, 6, COLOR_WHITE, COLOR_GRAY);

  for (int i = memory.size() - 1, y = 43; i >= 0; i--) {
    bc_printnumber(memory[i], 18, y, COLOR_WHITE, COLOR_GRAY);
    y -= 9;
  }
}


void instruction_output(int *ram, int instruction)
{
  mt_gotoXY(9, 76);
  exit_charset_mode();

  cout.unsetf(ios::dec);
  cout.setf(ios::hex);

  if (instruction < 10) {
    cout << "+000" << instruction;
  }

  if (instruction > 10) {
    cout << "+00" << instruction;
  }

  cout.unsetf(ios::hex);
  cout.setf(ios::dec);

  enter_charset_mode();
}


void operation_output(int command, int operand)
{
  mt_clrline(12, 76);
  mt_gotoXY(12, 76);

  if (command < 10) cout << "+0" << command;
  if (command > 9) cout << "+" << command;
  if (operand < 10) cout << ":0" << operand;
  if (operand > 9) cout << ":" << operand;
}


void flags_output(int *registr)
{
  string flag_value;
  int *value = new int;

  mt_clrline(15, 77);
  mt_gotoXY(15, 77);

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
    else cout << " ";
  }
  delete value;
}


int value_input()
{
  exit_charset_mode();

  mt_gotoXY(29, 13);

  string strValue;
  cin >> strValue;

  int value = atoi(strValue.c_str());

  mt_clrline(29, 13);
  enter_charset_mode();

  return value;
}


void file_input(char *filename)
{
  exit_charset_mode();
  mt_gotoXY(29, 13);

  scanf("%s", filename);

  mt_clrline(29, 13);
  enter_charset_mode();
}


void run_timer()
{
  isTimer = true;
  struct itimerval nval, oval;

  signal(SIGALRM, TimerOff);
  nval.it_interval.tv_sec = 1;
  nval.it_interval.tv_usec = 0;
  nval.it_value.tv_sec = 1;
  nval.it_value.tv_usec = 0;

  setitimer(ITIMER_REAL, &nval, &oval);

  while (isTimer != false)
    pause();
}


int sc_interface()
{
  enter_charset_mode();

  bc_box(5, 5, 62, 12);
  bc_box(17, 5, 48, 10);
  bc_box(5, 67, 25, 3);
  bc_box(8, 67, 25, 3);
  bc_box(11, 67, 25, 3);
  bc_box(14, 67, 25, 3);
  bc_box(17, 54, 38, 10);
  bc_box(28, 5, 25, 3);

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
  mt_gotoXY(25, 55);
  cout << "ENTER - cell value";
  mt_gotoXY(29, 6);
  cout << "value: ";


  enter_charset_mode();
  bool isExit = false;

  int ram[SIZE];
  int *registr = new int;
  sc_regSet(registr, CLOCK_IGNORE, 1);
  enum keys *key = new keys;
  char *filename = new char;

  sc_memoryInit(ram);
  operation_output(0, 0);

  int instruction = 0;
  int *accumulator = new int;
  *accumulator = 0;

  big_char_output(ram, 0);
  instruction_output(ram, 0);
  accumulator_output(accumulator);
  memory_output(ram, 0);


  while (isExit == false)
  {
    cout << endl;
    mt_gotoXY(6, 6);

    signal(SIGUSR1, CI_On);

    while (is_CI == false) {
      run_timer();
      if (instruction == 99 || CU(ram, accumulator, registr, &instruction) == 1) {
        sc_regSet(registr, CLOCK_IGNORE, 1);
        raise(SIGUSR1);
        big_char_output(ram, instruction);
        instruction_output(ram, instruction);
        accumulator_output(accumulator);
        flags_output(registr);
        memory_output(ram, instruction);
        break;
      }
      big_char_output(ram, instruction);
      instruction_output(ram, instruction);
      accumulator_output(accumulator);
      flags_output(registr);
      memory_output(ram, instruction);
    }

    rk_readkey(key);

    while ((*key > 7 && *key < 12) || *key == KEY_STEP)
    {
      rk_readkey(key);

      switch (*key) {
        case KEY_UP:
          instruction -= 10;
          if (instruction_check(ram, registr, instruction) == 1) instruction = 0;
          break;

        case KEY_DOWN:
          instruction += 10;
          if (instruction_check(ram, registr, instruction) == 1) instruction = 0;
          break;

        case KEY_LEFT:
          instruction--;
          if (instruction_check(ram, registr, instruction) == 1) instruction = 0;
          break;

        case KEY_RIGHT:
          instruction++;
          if (instruction_check(ram, registr, instruction) == 1) instruction = 0;
          break;

        case KEY_STEP:
          CU(ram, accumulator, registr, &instruction);
          if (instruction_check(ram, registr, instruction) == 1) instruction = 0;
          break;

        default:
          break;
      }
      big_char_output(ram, instruction);
      instruction_output(ram, instruction);
      accumulator_output(accumulator);
      flags_output(registr);
      memory_output(ram, instruction);
    }

    switch (*key) {
      case KEY_EXIT:
        isExit = true;
        break;

      case KEY_ACCUMULATOR: {
        *accumulator = value_input();
        break;
      }

      case KEY_INSTRUCTION: {
        instruction = value_input();
        if (instruction_check(ram, registr, instruction) == 1) instruction = 0;
        break;
      }

      case KEY_ENTER: {
        int value = value_input();
        int command = value / 100;
        int operand = value % 100;
        sc_commandEncode(registr, command, operand, &value);
        sc_memorySet(ram, registr, instruction, value);
        if (instruction_check(ram, registr, instruction) == 1) instruction = 0;
        break;
      }

      case KEY_SAVE: {
        file_input(filename);
        sc_memorySave(ram, registr, filename);
        break;
      }

      case KEY_LOAD: {
        file_input(filename);
        if (sc_memoryLoad(ram, registr, filename) == 1) {
          asmb(ram, accumulator, registr, &instruction, filename);
        }
        break;
      }

      case KEY_RUN:
        sc_regSet(registr, CLOCK_IGNORE, 0);
        is_CI = false;
        break;

      case KEY_RESET:
        sc_memoryInit(ram);
        *registr = 8;
        *accumulator = 0;
        break;

      default:
        break;
    }
    big_char_output(ram, instruction);
    instruction_output(ram, instruction);
    accumulator_output(accumulator);
    flags_output(registr);
    memory_output(ram, instruction);
  }

  mt_gotoXY(40, 0);
  exit_charset_mode();

  return 0;
}
