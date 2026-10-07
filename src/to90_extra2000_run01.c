#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
typedef struct {unsigned char toggle:4,state:3,pad:1;} Flags;
extern void *GfGfxLoader_GetCharData(int,int,int,void**,int),*NewMsgDataFromNarc(int,int,int,int);
extern void BlitBitmapRect(void*,const void*,u16,u16,u16,u16,u16,u16,u16,u16,u16),ov14_021F4F84(void*,void*,int,int,int,int,int,int,int),CopyWindowPixelsToVram_TextMode(void*),DestroyMsgData(void*),Heap_Free(void*);
int ov14_021F5404(void*,void*);
#pragma opt_common_subs off
#pragma force_active on
int ov14_021F5404(void *w,void *desc){void *charData;const u8 *second;void *allocation,*pixels,*msg;u16 i,j;int index,start;void *base=P(w,0x34);if(((Flags*)((u8*)base+0x44e))->toggle==0)start=9;else start=13;u8 next=((Flags*)((u8*)base+0x44e))->toggle^1;((Flags*)((u8*)base+0x44e))->toggle=next;
 allocation=GfGfxLoader_GetCharData(19,6,1,&charData,10);pixels=P(charData,0x14);msg=NewMsgDataFromNarc(0,27,750,10);
 if(!((u32)B(desc,0x12)<<24>>31)){for(i=0;i<4;i++){index=start+i;second=pixels;second+=32;for(j=0;j<11;j++){int x=j*8;BlitBitmapRect((u8*)P(w,0x34)+0x30+16*index,(u8*)pixels+0x160,0,0,8,8,x,0,8,8,255);BlitBitmapRect((u8*)P(w,0x34)+0x30+16*index,second,0,0,8,8,x,8,8,8,255);}ov14_021F4F84(P(w,0x34),msg,index,H((u8*)desc+i*2,0x14),0,0,0,0x10200,0);CopyWindowPixelsToVram_TextMode((u8*)P(w,0x34)+0x30+16*index);}((Flags*)((u8*)P(w,0x34)+0x44e))->state=1;}else((Flags*)((u8*)P(w,0x34)+0x44e))->state=0;
 DestroyMsgData(msg);Heap_Free(allocation);return start;
}
