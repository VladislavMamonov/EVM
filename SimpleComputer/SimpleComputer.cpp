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
  if (address >= 0 && address < SIZE) {
    ram[address] = value;
    return 0;
  }
  else {
    sc_regSet(registr, MEMORY_ACCESS, 1);
    return 1;
  }
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

  if (operand > 99) {
    sc_regSet(registr, OVERFLOW, 1);
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

  if (*operand > 99) {
    sc_regSet(registr, UNKNOWN_COMMAND, 1);
    return 1;
  }

  return 0;
}


int cursor_position_check(int *ram, int *registr, int cursor_position)
{
  if (cursor_position < 0 || cursor_position > 99) {
    sc_regSet(registr, MEMORY_ACCESS, 1);
    return 1;
  }

  return 0;
}


void memory_output(int *ram, int cursor_position)
{
  int x = 6, y = 6;
  mt_gotoXY(x, y);

  for (int i = 0; i < SIZE; i++)
  {
    if (i == cursor_position)
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
  mt_gotoXY(6, 76);
  cout << "      ";  //clear
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


void big_char_output(int *ram, int cursor_position)
{
  if (ram[cursor_position] < 10) {
    for (int i = 0, y = 16; i < 3; i++) {
      bc_printnumber('0', 18, y, COLOR_WHITE, COLOR_GRAY);
      y += 9;
    }
  }

  if (ram[cursor_position] > 9 && ram[cursor_position] < 100) {
    for (int i = 0, y = 16; i < 2; i++) {
      bc_printnumber('0', 18, y, COLOR_WHITE, COLOR_GRAY);
      y += 9;
    }
  }

  if (ram[cursor_position] > 99 && ram[cursor_position] < 1000) {
    bc_printnumber('0', 18, 16, COLOR_WHITE, COLOR_GRAY);
  }

  string memory = to_string(ram[cursor_position]);

  bc_printnumber('+', 18, 6, COLOR_WHITE, COLOR_GRAY);

  for (int i = memory.size() - 1, y = 43; i >= 0; i--) {
    bc_printnumber(memory[i], 18, y, COLOR_WHITE, COLOR_GRAY);
    y -= 9;
  }
}


void instruction_output(int *ram, int cursor_position)
{
  mt_gotoXY(9, 76);
  exit_charset_mode();

  cout.unsetf(ios::dec);
  cout.setf(ios::hex);

  if (cursor_position < 10) {
    cout << "+000" << cursor_position;
  }

  if (cursor_position > 10) {
    cout << "+00" << cursor_position;
  }

  cout.unsetf(ios::hex);
  cout.setf(ios::dec);

  enter_charset_mode();
}


void flags_output(int *registr)
{
  mt_gotoXY(15, 77);
  string flag_value;
  int *value = new int;

  if (*registr == 0) {
    cout << "     ";
    return;
  }

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

  for (long unsigned int i = 0; i < strValue.size(); i++)
    strValue[i] = strValue[i + 1];

  int value = atoi(strValue.c_str());

  mt_gotoXY(29, 13);
  cout << "    ";       //чистка после ввода
  enter_charset_mode();

  return value;
}


void sc_run()
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
  mt_gotoXY(29, 6);
  cout << "value: ";


  enter_charset_mode();
  bool isExit = false;

  int ram[SIZE];
  int *registr = new int;
  sc_regSet(registr, CLOCK_IGNORE, 1);
  enum keys *key = new keys;
  int *registr_save = new int;

  sc_memoryInit(ram);

  mt_gotoXY(12, 76);
  int operation = 0;
  cout << "+" << "0" << operation << ":" << "00";

  int cursor_position = 0;
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
      sc_run();
      if (cursor_position == 99 || CU(ram, accumulator, registr, cursor_position) == 1) {
        sc_regSet(registr, CLOCK_IGNORE, 1);
        cursor_position = 0;
        raise(SIGUSR1);
      }
      big_char_output(ram, cursor_position);
      instruction_output(ram, cursor_position);
      accumulator_output(accumulator);
      flags_output(registr);
      memory_output(ram, cursor_position);
      cursor_position++;
    }

    rk_readkey(key);

    while ((*key > 7 && *key < 12) || *key == KEY_STEP)
    {
      rk_readkey(key);

      switch (*key) {
        case KEY_UP:
          cursor_position -= 10;
          if (cursor_position_check(ram, registr, cursor_position) == 1) cursor_position = 0;
          break;

        case KEY_DOWN:
          cursor_position += 10;
          if (cursor_position_check(ram, registr, cursor_position) == 1) cursor_position = 0;
          break;

        case KEY_LEFT:
          cursor_position--;
          if (cursor_position_check(ram, registr, cursor_position) == 1) cursor_position = 0;
          break;

        case KEY_RIGHT:
          cursor_position++;
          if (cursor_position_check(ram, registr, cursor_position) == 1) cursor_position = 0;
          break;

        case KEY_STEP:
          CU(ram, accumulator, registr, cursor_position);
          cursor_position++;
          if (cursor_position_check(ram, registr, cursor_position) == 1) cursor_position = 0;
          break;

        default:
          break;
      }
      big_char_output(ram, cursor_position);
      instruction_output(ram, cursor_position);
      accumulator_output(accumulator);
      flags_output(registr);
      memory_output(ram, cursor_position);
    }

    switch (*key) {
      case KEY_EXIT:
        isExit = true;
        break;

      case KEY_INSTRUCTION: {
        int value = value_input();
        cursor_position = value;
        if (cursor_position_check(ram, registr, cursor_position) == 1) cursor_position = 0;
        break;
      }

      case KEY_ACCUMULATOR: {
        int value = value_input();
        sc_memorySet(ram, registr, cursor_position, value);
        if (cursor_position_check(ram, registr, cursor_position) == 1) cursor_position = 0;
        break;
      }

      case KEY_SAVE: {
        *registr_save = *registr;
        sc_memorySave(ram, "SaveData.bin");
        break;
      }

      case KEY_LOAD:
        *registr = *registr_save;
        sc_memoryLoad(ram, "SaveData.bin");
        break;

      case KEY_RUN:
        sc_regSet(registr, CLOCK_IGNORE, 0);
        is_CI = false;
        break;

      case KEY_RESET:
        sc_memoryInit(ram);
        *registr = 8;

      default:
        break;
    }
    big_char_output(ram, cursor_position);
    instruction_output(ram, cursor_position);
    accumulator_output(accumulator);
    flags_output(registr);
    memory_output(ram, cursor_position);
  }

  mt_gotoXY(40, 0);
  exit_charset_mode();

  return 0;
}
