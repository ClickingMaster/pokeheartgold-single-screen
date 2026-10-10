#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x14];void *bg;u8 q[0x18];u8 win[8][16];} W;
extern void FillWindowPixelBuffer(void*,int);extern void DrawFrame2AroundWindow(void*,int,int);extern void ClearFrameAndWindow2(void*,int);extern int GetWindowBgId(void*);extern void ScheduleBgTilemapBufferTransfer(void*,int);
void ov14_021F6638(void*);void ov14_021F6654(W*,int);
void ov14_021F6638(void*w) {FillWindowPixelBuffer(w,15);DrawFrame2AroundWindow(w,910,10);}
void ov14_021F6654(W*w,int id) {u8 *win=w->win[0];ClearFrameAndWindow2(win+id*16,1);ScheduleBgTilemapBufferTransfer(w->bg,GetWindowBgId(win+id*16));}
