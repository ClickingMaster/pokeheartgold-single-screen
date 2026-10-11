#pragma force_active on
#include "global.h"
typedef struct{u8 a[0x1e448];void*format;u8 b[0x1e524-0x1e44c];int printer;u8 c[0x1f2e8-0x1e528];u8 nickname[0x88];s16 mode;}Work;
extern void BufferBoxMonNickname(void*,int,void*);extern int PokewalkerApp_StartMessageBox(void*,int,int);

int ov112_021EC2A8(Work*w);

int ov112_021EC2A8(Work*w){if(w->mode>=0){BufferBoxMonNickname(w->format,1,w->nickname);w->printer=PokewalkerApp_StartMessageBox(w,2,26);return 23;}return 29;}
