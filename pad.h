#ifndef PAD_H_
#define PAD_H_
#include <stdio.h>
#include "libpad.h"
#include <sifrpc.h>
#include <debug.h>

void loadModules();
int waitPadReady(int port, int slot);
int initializePad(int port, int slot);

#endif
