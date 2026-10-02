#pragma force_active on
#include "global.h"
extern u32 WallpaperPasswordBank_GetCount(void*);extern u16 WallpaperPasswordBank_GetWordAtIndex(void*,u32);
void ov45_0222CA10(void*,u32,u16*);
void ov45_0222CA10(void*bank,u32 word,u16*out) {union {u32 word;u8 b[4];} code;u32 n,a,b,c,d;n=WallpaperPasswordBank_GetCount(bank);code.word=word;a=code.b[0];d=code.b[3];out[0]=WallpaperPasswordBank_GetWordAtIndex(bank,(d+a)%n);b=code.b[1];out[1]=WallpaperPasswordBank_GetWordAtIndex(bank,(a+b)%n);c=code.b[2];out[2]=WallpaperPasswordBank_GetWordAtIndex(bank,(b+c)%n);out[3]=WallpaperPasswordBank_GetWordAtIndex(bank,(c+d)%n);}
