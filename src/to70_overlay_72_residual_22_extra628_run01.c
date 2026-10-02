#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x1c];int mode;u8 pad2[0xbb4];void*window;u8 pad3[0x1c];u32 printer;u8 pad4[0x3d8];s16 state;volatile s16 timer;}Work;
extern void ov72_0223A350(Work*,void*,int,int,int),sub_0203946C(void),ov00_021EC8D8(void),ov72_022387D0(Work*,int,int);extern int TextPrinterCheckActive(u8);
int ov72_0223A0E0(Work*);
int ov72_0223A0E0(Work*w){switch(w->state){case 0:ov72_0223A350(w,w->window,18,1,0xf0f);w->state++;break;case 1:if(TextPrinterCheckActive((u8)w->printer)==0){sub_0203946C();ov00_021EC8D8();w->state++;}break;case 2:ov72_0223A350(w,w->window,19,1,0xf0f);w->state++;break;case 3:if(TextPrinterCheckActive((u8)w->printer)==0)w->state++;break;default:w->timer++;if(w->timer>30){ov72_022387D0(w,0,0);w->mode=46;}break;}return 3;}
