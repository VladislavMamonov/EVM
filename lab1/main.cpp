#include "SimpleComputer.hpp"


int main()
{
  int ram[SIZE];
  int *value = new int;
  int *registr = new int;
  int *flag_value = new int;
  long int *EncodedCommand_value = new long int;
  int *DecodedCommand, *DecodedOperand = new int;

  sc_memoryInit(ram);
  sc_regInit(registr);

  sc_memorySet(ram, registr, 99, 128);
  sc_memoryGet(ram, registr, 99, value);

  sc_memorySave(ram, "data.bin");
  sc_memorySet(ram, registr, 98, 444);
  sc_memoryLoad(ram, "data.bin");

  sc_memorySet(ram, registr, 100, 54);
  *flag_value = sc_regGet(registr, MEMORY_ACCESS, flag_value);

  sc_commandEncode(registr, 56, 76, EncodedCommand_value);
  sc_commandDecode(registr, EncodedCommand_value, DecodedCommand, DecodedOperand);

  cout << "flag value = " << *flag_value << endl;
  cout << "registr = " << *registr << endl;
  cout << "encoded command value = " << *EncodedCommand_value << endl;
  cout << "decoded command = " << *DecodedCommand << "\t";
  cout << "decoded operand = " << *DecodedOperand << endl;

  /*
  for (int i = 0; i < SIZE; i++)
    cout << ram[i] << endl;
  */
}
