#pragma force_active on
#include "global.h"
typedef struct{u32 a,b,c,d;}Entry;extern Entry ov74_0223C6D0;extern void ClearWindowTilemapAndCopyToVram(void*),RemoveWindow(void*),AddWindowParameterized(void*,void*,int,u8,u8,u8,u8,int,u16),ov74_0222ED04(void*,Entry*,int,void*,int);typedef struct{u32 a,b,c,d,e;}Param;
int ov74_0222EF68(u8*w,void*window);
#pragma force_active on
int ov74_0222EF68(u8*w,void*window){volatile Entry e;const volatile Entry *source=&ov74_0223C6D0;u32 b=source->b,a=source->a;e.b=b;u32 c=source->c,d=source->d;e.a=a;e.d=d;e.c=c;Param*p=*(Param**)(w+0x2b9c);ClearWindowTilemapAndCopyToVram(window);RemoveWindow(window);AddWindowParameterized(*(void**)(w+0x29fc),window,2,(u8)p->b,(u8)p->c,(u8)p->d,(u8)p->e,15,(u16)*(u32*)(w+0x2b98));ov74_0222ED04(w,(Entry*)&e,2,window,0);return 1;}
