#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x18];void *bg;u8 q[0x1ea7c];u8 a[16],b[16];u8 r[0x82c];u16 state;} W;
extern int PokewalkerApp_HandleTwoChoiceMenu(W*);extern void PokewalkerApp_HideCursor(W*);extern void ClearWindowTilemapAndCopyToVram(void*);extern void FillBgTilemapRect(void*,int,int,int,int,int,int,int);
int ov112_021EBE78(W*w);
int ov112_021EBE78(W*w) {switch(PokewalkerApp_HandleTwoChoiceMenu(w)){case 0:PokewalkerApp_HideCursor(w);FillBgTilemapRect(w->bg,1,0,0,16,32,8,16);ClearWindowTilemapAndCopyToVram(w->a);ClearWindowTilemapAndCopyToVram(w->b);w->state=0;return 11;case 1:FillBgTilemapRect(w->bg,1,0,0,16,32,8,16);ClearWindowTilemapAndCopyToVram(w->a);ClearWindowTilemapAndCopyToVram(w->b);PokewalkerApp_HideCursor(w);w->state=1;return 11;}return 10;}
