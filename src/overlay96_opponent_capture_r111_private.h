#ifndef OVERLAY96_OPPONENT_CAPTURE_R111_PRIVATE_H
#define OVERLAY96_OPPONENT_CAPTURE_R111_PRIVATE_H
#include "global.h"
typedef VecFx32 Vec;
typedef struct {void *body;u8 pad[0x2c];Vec pos;u8 pad2[0xc];int busy;u8 pad3[0x2c];int result;} Actor;
typedef struct {u8 pad[8];Vec pos;u8 pad2[0x24];u8 state,disabled;u8 pad3[0x12];} Target;
int ov96_02214C3C(void*,u8,u8,Actor*,Target*);
#endif
