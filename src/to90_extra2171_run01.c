#pragma force_active on
#include "global.h"
#include "heap.h"
extern int ov89_0225A878(void *,void *,int,int,void *,int);
extern void sub_020181B0(void *,void *),ov89_0225AC68(void *,u8,u8,int *,int *),sub_020182A8(void *,int,int,int),sub_020182C4(void *,int,int,int),sub_020182A0(void *,int);
typedef struct {u32 words[3];} Definition;
void *ov89_0225A7BC(void *,void *,const Definition *,int,int,int);
#pragma force_active on
void *ov89_0225A7BC(void *context,void *resource,const Definition *definition,int a,int b,int c){
 u8 *object=Heap_Alloc((enum HeapID)125,0x244);int x,y;
 MI_CpuFill8(object,0,0x244);*(u32 *)(object+0x240)=(*(u32 *)(object+0x240)&0xffffff)|0xff000000;*(Definition *)object=*definition;
 if(*(u16 *)(object+2)>0x1ed){GF_AssertFail();*(u16 *)(object+2)=0x84;}
 if(!ov89_0225A878(resource,object+12,a,b,object,c))return NULL;
 sub_020181B0(object+0x1c,object+12);ov89_0225AC68(context,((const u8 *)definition)[9],((const u8 *)definition)[10],&x,&y);sub_020182A8(object+0x1c,x,y,0x10000);sub_020182C4(object+0x1c,0x1000,0x1000,0x1000);sub_020182A0(object+0x1c,0);return object;
}
