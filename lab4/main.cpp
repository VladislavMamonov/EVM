#include "myReadkey.hpp"


int main()
{
  enum keys *key = new keys;
  rk_readkey(key);

  return 0;
}
