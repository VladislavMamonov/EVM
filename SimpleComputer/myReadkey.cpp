#include "include/myReadkey.hpp"


struct termios saved_attributes;


void rk_mytermsave()
{
  tcgetattr(0, &saved_attributes);
}


void rk_mytermrestore()
{
  tcsetattr(0, TCSANOW, &saved_attributes);
}


int rk_mytermregime(struct termios const *currState, int regime, int vtime, int vmin, int echo, int sigint)
{
  if (regime < 0 || regime > 1 || echo < 0 || echo > 1 || sigint < 0 || sigint > 1)
    return 1;

  struct termios newTermState = *currState;

  if (regime == 1)
      newTermState.c_lflag |= ICANON;
  else
      newTermState.c_lflag &= ~ICANON;

  if (echo == 1)
      newTermState.c_lflag |= ECHO;
  else
      newTermState.c_lflag &= ~ECHO;

  if (sigint == 1)
      newTermState.c_lflag |= ISIG;
  else
      newTermState.c_lflag &= ~ISIG;

  newTermState.c_cc[VMIN]  = vmin;
  newTermState.c_cc[VTIME] = vtime;
  tcsetattr(0, TCSANOW, &newTermState);

  return 0;
}



int rk_readkey(enum keys *key)
{
	char buf;

	struct termios termState;
  rk_mytermsave();

	rk_mytermregime(&termState, 0, 1, 1, 0, 0);
	read(0, &buf, 1);

  switch (buf)
  {
    case 'l':
      *key = KEY_LOAD;
      break;

    case 's':
      *key = KEY_SAVE;
      break;

    case 'r':
      *key = KEY_RUN;
      break;

    case 't':
      *key = KEY_STEP;
      break;

    case 'i':
      *key = KEY_RESET;
      break;

    case 'e':
      *key = KEY_EXIT;
      break;

    case '\n':
      *key = KEY_ENTER;
      break;
      

    case '\E':
      for (int i = 0; i < 2; i++) {
        read(0, &buf, 1);
      }

      switch (buf)
      {
        case '1':
          read(0, &buf, 1);
          if (buf == '5')
            *key = KEY_ACCUMULATOR;
          if (buf == '7')
            *key = KEY_INSTRUCTION;
          read(0, &buf, 1);
          break;

        case 'A':
          *key = KEY_UP;
          break;

        case 'B':
          *key = KEY_DOWN;
          break;

        case 'D':
          *key = KEY_LEFT;
          break;

        case 'C':
          *key = KEY_RIGHT;
          break;

        default:
          rk_mytermrestore();
          return 1;
      }

      break;

    default:
      rk_mytermrestore();
      return 1;
  }
  rk_mytermrestore();
  // cout << buf << endl;
  // cout << *key << endl;

  return 0;
}
