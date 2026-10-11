#pragma force_active on
#include "global.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#pragma force_active on
extern void PCBox_PrintAligned(Window *,String *,int,int,int,int,int);
void PCBox_DrawSummaryButton(void *);
void PCBox_DrawSummaryButton(void *input) {
 u8 *work=*(u8 **)((u8 *)input+0x34);void *allocation;int limit;int width=GetWindowWidth((Window *)(work+0x2e0));NNSG2dCharacterData *chars;u8 *pixels;int last;int edge;u8 i;String *str;
 allocation=GfGfxLoader_GetCharData((NarcId)19,14,TRUE,&chars,(enum HeapID)10);pixels=chars->pRawData;
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0x7a0,0,0,24,8,0,0,24,8,255);
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0xba0,0,0,24,8,0,8,24,8,255);
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0xfa0,0,0,24,8,0,16,24,8,255);
 last=width-1;edge=last*8;
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0xd00,0,0,8,8,(u16)edge,0,8,8,255);
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0xce0,0,0,8,8,(u16)edge,8,8,8,255);
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0xcc0,0,0,8,8,(u16)edge,16,8,8,255);
 i=3;if(last>3){limit=width-1;do {
 int x=i*8;
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0xc20,0,0,8,8,(u16)x,0,8,8,255);
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0xca0,0,0,8,8,(u16)x,8,8,8,255);
 BlitBitmapRect((Window *)(work+0x2e0),pixels+0xc80,0,0,8,8,(u16)x,16,8,8,255);
 i++;}while(i<limit); }
 Heap_Free(allocation);str=NewString_ReadMsgData(*(MsgData **)(*(u8 **)((u8 *)input+0x34)+0x20),65);
 PCBox_PrintAligned((Window *)(work+0x2e0),str,width*8/2,4,4,0x90a00,2);
 String_Delete(str);CopyWindowPixelsToVram_TextMode((Window *)(work+0x2e0));
}
