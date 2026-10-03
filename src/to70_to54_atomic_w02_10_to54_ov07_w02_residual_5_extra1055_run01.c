#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov07_02222268(void*,int,int,s16,s16,u16);
extern void ov07_022220FC(void*,int,int,int,int,int,void*,u16);
void ov07_02222338(void*,u16*,int,int,s16,s16,u16,void*);
void ov07_02222338(void*w,u16*dest,int a,int b,s16 c,s16 d,u16 e,void*f){ov07_02222268(w,a,b,c,d,e);dest[0]=0;dest[1]=0;ov07_022220FC(dest,0,0,0x3fff,0xbfff,0,f,e);}
