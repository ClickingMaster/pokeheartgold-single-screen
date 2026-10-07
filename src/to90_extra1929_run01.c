#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov49_0225D528(void*,void*,u32,int),ov45_0222D740(void*);
extern void*GfGfxLoader_LoadFromOpenNarc(void*,u32,int,int,int);
void ov49_0225DC2C(u8*,void*,void*,u32*,volatile int);
void ov49_0225DC2C(u8*work,void*narc,void*unused,u32*ids,volatile int heap){struct{u8*work;void*narc;u8*column;u32*extras;int i;}v;int j;u32*res;u8*slot;v.work=work;heap=heap;v.i=0;v.narc=narc;v.extras=ids;v.column=v.work;for(;v.i<18;v.i++){ov49_0225D528(v.work,v.narc,*ids,heap);if(v.i!=11&&v.i!=12)ov45_0222D740(*(void**)v.work);res=v.extras;slot=v.column;for(j=0;j<3;j++,res++,slot+=4){if(*ids!=res[18])*(void**)((u32)slot+0x120)=GfGfxLoader_LoadFromOpenNarc(v.narc,res[18],0,heap,0);else *(void**)((u32)slot+0x120)=0;}v.work+=16;ids++;v.extras+=3;v.column+=12;}}
