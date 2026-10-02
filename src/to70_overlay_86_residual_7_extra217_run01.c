#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[12];void*bg;u8 p10[0x25d-16];s8 flag;u8 p25E[0x334-0x25e];u8 one[32],two[32],three[32];};void LoadRectToBgTilemapRect(void*,int,const void*,int,int,int,int);void ScheduleBgTilemapBufferTransfer(void*,int);void ov86_021E7344(struct S*);
void ov86_021E7344(struct S*s){if(s->flag==0){LoadRectToBgTilemapRect(s->bg,2,s->three,34,52,4,4);LoadRectToBgTilemapRect(s->bg,2,s->three,38,52,4,4);}else{LoadRectToBgTilemapRect(s->bg,2,s->one,34,52,4,4);LoadRectToBgTilemapRect(s->bg,2,s->two,38,52,4,4);}ScheduleBgTilemapBufferTransfer(s->bg,2);}
