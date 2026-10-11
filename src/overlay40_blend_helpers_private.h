#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"
#define P(x) void x(void *)
P(VsRecorder_LoadMenuRowGfx);P(VsRecorder_MenuPop);P(sub_02088030);P(VsRecorder_CreateMenuRowSprites);void VsRecorder_SetMenuRowsDrawFlag(void *,int);P(VsRecorder_RefreshChoiceRows);P(VsRecorder_AttachMenuRowText);P(VsRecorder_LayoutMenuRows);P(VsRecorder_RefreshPathRows);
void ov40_0222D910(int *,int *,int,int,int,int,int);
void ov40_0222D9E8(int *,int *,int);
void ov40_0222D88C(void *);void ov40_0222D8C8(void *);int ov40_0222DA84(int *,int);
#endif
