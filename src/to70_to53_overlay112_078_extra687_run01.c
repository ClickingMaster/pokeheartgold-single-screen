#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x18];void *bg;u8 q[0x1ea7c];u8 a[16],b[16];u8 r[0x82c];u16 state;} W;
extern int ov112_021E9750(W*);extern void ov112_021EA570(W*);extern void ClearWindowTilemapAndCopyToVram(void*);extern void FillBgTilemapRect(void*,int,int,int,int,int,int,int);
int ov112_021EBE78(W*w);
int ov112_021EBE78(W*w) {switch(ov112_021E9750(w)){case 0:ov112_021EA570(w);FillBgTilemapRect(w->bg,1,0,0,16,32,8,16);ClearWindowTilemapAndCopyToVram(w->a);ClearWindowTilemapAndCopyToVram(w->b);w->state=0;return 11;case 1:FillBgTilemapRect(w->bg,1,0,0,16,32,8,16);ClearWindowTilemapAndCopyToVram(w->a);ClearWindowTilemapAndCopyToVram(w->b);ov112_021EA570(w);w->state=1;return 11;}return 10;}
