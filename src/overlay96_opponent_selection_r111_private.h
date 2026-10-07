#ifndef OVERLAY96_OPPONENT_SELECTION_R111_PRIVATE_H
#define OVERLAY96_OPPONENT_SELECTION_R111_PRIVATE_H
#include "global.h"
typedef struct {u32 a[2];VecFx32 pos;u8 pad[0x24];u8 enabled,disabled;u8 tail[0x12];} W40081Actor;
typedef struct {u8 pad[0x30];VecFx32 pos;u8 gap[12];int busy;u8 gap2[0x2c];int state;} W40081Parent;
int ov96_02214EB4(void *,u8,u8,W40081Parent *,W40081Actor *);
#endif
