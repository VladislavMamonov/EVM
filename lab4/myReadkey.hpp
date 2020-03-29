#pragma once
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

using namespace std;

enum keys {
  KEY_LOAD,
  KEY_SAVE,
  KEY_RUN,
  KEY_STEP,
  KEY_RESET,
  KEY_ACCUMULATOR,
  KEY_INSTRUCTION,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
};


void rk_mytermsave();
void rk_mytermrestore();
int rk_mytermregime(struct termios const *currState, int regime, int vtime, int vmin, int echo, int sigint);
int rk_readkey(enum keys *key);
