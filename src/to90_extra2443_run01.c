#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W31780Table {u32 words[6];} W31780Table;
typedef struct W31780Pltt {u32 a[3];u8 *data;} W31780Pltt;
extern const W31780Table ov51_021E7E08,ov51_021E7DF0,ov51_021E7E20,ov51_021E7DD8;
extern void *GfGfxLoader_GetPlttData(int,int,W31780Pltt **,int);
extern void DC_FlushRange(const void *,u32),GXS_LoadBGPltt(const void *,u32,u32),Heap_Free(void *);
void ov51_021E60F4(u32,BOOL,u8);
void ov51_021E60F4(u32 index,BOOL special,u8 kind){u32 member;W31780Pltt *pltt;void *raw;u8 *data;if((u8)(kind+249u)<=1){if(special){W31780Table table=ov51_021E7E08;if(index>5){GF_AssertFail();member=(*(volatile u32 *)&table.words[0]);}else member=table.words[index];}else member=6;}else if(kind==10){W31780Table table=ov51_021E7DF0;if(index>5){GF_AssertFail();member=(*(volatile u32 *)&table.words[0]);}else member=table.words[index];}else if(kind==11){W31780Table table=ov51_021E7E20;if(index>5){GF_AssertFail();member=(*(volatile u32 *)&table.words[0]);}else member=table.words[index];}else if(kind==12){W31780Table table=ov51_021E7DD8;if(index>5){GF_AssertFail();member=(*(volatile u32 *)&table.words[0]);}else member=table.words[index];}else{GF_AssertFail();member=0;}raw=GfGfxLoader_GetPlttData(49,member,&pltt,25);DC_FlushRange(pltt->data,512);data=pltt->data;GXS_LoadBGPltt(data+32,32,256);GXS_LoadBGPltt(data+480,480,32);Heap_Free(raw);}
