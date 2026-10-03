#pragma force_active on
#include "global.h"
typedef struct{u8 p[12];void*bg;u8 q[0x50-16];u8 window[16];}State;
extern void ScheduleSetBgPosText(void*,int,int,int),ScheduleWindowCopyToVram(void*),ClearWindowTilemapAndScheduleTransfer(void*),ScheduleBgTilemapBufferTransfer(void*,int);
extern void ov86_021E7C70(State*),ov86_021E73CC(State*),ov86_021E7D8C(State*),ov86_021E7DA8(State*),ov86_021E757C(State*),ov86_021E7630(State*),ov86_021E77BC(State*),ov86_021E752C(State*),ov86_021E7DD4(State*),ov86_021E7B68(State*),ov86_021E7CF8(State*),ov86_021E7344(State*),ov86_021E7CA0(State*),ov86_021E7860(State*);
void ov86_021E7258(State*,int);
void ov86_021E7258(State*s,int mode){switch(mode){case 0:ScheduleSetBgPosText(s->bg,2,0,0);ScheduleSetBgPosText(s->bg,2,3,0);ov86_021E7C70(s);ov86_021E73CC(s);ov86_021E7D8C(s);ov86_021E7DA8(s);ov86_021E757C(s);break;case 1:ScheduleSetBgPosText(s->bg,2,0,0);ScheduleSetBgPosText(s->bg,2,3,256);ov86_021E7C70(s);ScheduleWindowCopyToVram(s->window);ov86_021E73CC(s);ov86_021E7630(s);ov86_021E77BC(s);ov86_021E752C(s);break;case 2:ScheduleSetBgPosText(s->bg,2,0,256);ScheduleSetBgPosText(s->bg,2,3,256);ClearWindowTilemapAndScheduleTransfer(s->window);ov86_021E7DD4(s);ov86_021E7B68(s);ov86_021E7CF8(s);ov86_021E7344(s);ov86_021E7CA0(s);ov86_021E73CC(s);ov86_021E7860(s);ov86_021E752C(s);break;}ScheduleBgTilemapBufferTransfer(s->bg,2);}
