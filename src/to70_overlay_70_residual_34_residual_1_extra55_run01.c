#pragma force_active on
#include "global.h"
#pragma force_active on
#define WORD(w,o) (*(u32*)((w)+(o)))
#define PTR(w,o) (*(void**)((w)+(o)))
extern void *AllocMonZeroed(int),*Mon_GetBoxMon(void*);
extern u32 ov70_0224182C(void*);
extern void sub_0202DB64(void*,void*);
extern const u8 ov70_02245D50[];
extern void *OverlayManager_New(const void*,void*,int);
int ov70_022414A0(u8*);
int ov70_022414A0(u8*w){PTR(w,0x11f0)=AllocMonZeroed(61);switch(WORD(w,0x24)){case 7:PTR(w,0xf8)=Mon_GetBoxMon(w+0x13c);PTR(w,0xfc)=PTR(w,0xf8);WORD(w,0x130)=ov70_0224182C(w+0x13c);WORD(w,0x100)=WORD(w,0x130);WORD(w,0x104)=3;WORD(w,0x108)=2;break;case 8:PTR(w,0xfc)=Mon_GetBoxMon(w+0x13c);PTR(w,0xf8)=PTR(w,0xfc);WORD(w,0x130)=ov70_0224182C(w+0x13c);WORD(w,0x100)=WORD(w,0x130);WORD(w,0x104)=3;WORD(w,0x108)=4;break;case 10:PTR(w,0xfc)=Mon_GetBoxMon(w+0x13c);sub_0202DB64(*(void**)PTR(w,0),PTR(w,0x11f0));PTR(w,0xf8)=Mon_GetBoxMon(PTR(w,0x11f0));WORD(w,0x130)=ov70_0224182C(w+0x13c);WORD(w,0x100)=WORD(w,0x130);WORD(w,0x104)=3;WORD(w,0x108)=1;break;case 9:sub_0202DB64(*(void**)PTR(w,0),PTR(w,0x11f0));PTR(w,0xf8)=Mon_GetBoxMon(PTR(w,0x11f0));PTR(w,0xfc)=Mon_GetBoxMon(w+0x260+WORD(w,0x12c)*0x124);WORD(w,0x130)=ov70_0224182C(w+0x260+WORD(w,0x12c)*0x124);WORD(w,0x100)=WORD(w,0x130);WORD(w,0x104)=3;WORD(w,0x108)=1;break;}WORD(w,0x10c)=*(u32*)((u8*)PTR(w,0)+0x24);PTR(w,0xb8)=OverlayManager_New(ov70_02245D50,w+0xf8,61);WORD(w,0x114)=1;return 2;}
