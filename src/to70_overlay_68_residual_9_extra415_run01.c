#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Entry {void *name;u32 move;} Entry;
void ov68_021E66A0(void *,int,u8,int,int,int,int),AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *),BufferIntegerAsString(void *,int,int,int,int,int),StringExpandPlaceholders(void *,void *,void *),ov68_021E66F0(u8 *,int,int);int GetMoveMaxPP(u16,int);
void ov68_021E66F0(u8 *s,int offset,int row){int index=offset+row;int y=row*32;if(index>=s[440]){ov68_021E66A0(s,0,(u8)(row*4+4),16,4,0,4);return;}AddTextPrinterParameterizedWithColor(s+168,0,(*(Entry **)(s+272))[index].name,0,y,255,0xf0e00,NULL);AddTextPrinterParameterizedWithColor(s+168,0,*(void **)(s+260),16,y+16,255,0x10200,NULL);int pp=GetMoveMaxPP((u16)((*(Entry **)(s+272))[index].move),0);BufferIntegerAsString(*(void **)(s+252),0,pp,2,1,1);BufferIntegerAsString(*(void **)(s+252),1,pp,2,0,1);StringExpandPlaceholders(*(void **)(s+252),*(void **)(s+256),*(void **)(s+264));AddTextPrinterParameterizedWithColor(s+168,0,*(void **)(s+256),45,y+16,255,0x10200,NULL);}
