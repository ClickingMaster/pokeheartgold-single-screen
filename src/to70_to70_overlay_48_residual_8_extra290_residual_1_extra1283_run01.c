#pragma force_active on
#include "global.h"
extern void ov48_022598AC(void*,VecFx32*),ov48_022598BC(void*,MtxFx33*),sub_020182A8(void*,fx32,fx32,fx32),sub_020182C4(void*,fx32,fx32,fx32),sub_02018288(void*,MtxFx33*);
void ov48_022599A0(u8*,void*);
void ov48_022599A0(u8*w,void*actor){MtxFx33 result;MtxFx33 transform;VecFx32 position;u8*object,*matrix,*entry;int i;u32 j;ov48_022598AC(actor,&position);ov48_022598BC(actor,&transform);i=0;object=w+0xc010;for(;i<3;i++,object+=120){sub_020182A8(object,position.x,position.y,position.z);if(i==2)sub_020182C4(object,*(s32*)(w+0xc004),*(s32*)(w+0xc008),*(s32*)(w+0xc00c)+410);else sub_020182C4(object,*(s32*)(w+0xc004),*(s32*)(w+0xc008),*(s32*)(w+0xc00c));}MTX_Identity33(&result);sub_02018288(w+0xc100,&result);j=0;if(j<*(u32*)w){matrix=w+8;entry=w;object=w+0xc010;do{MTX_Concat33((MtxFx33*)matrix,&transform,&result);if(*(u16*)(entry+0x2c)!=3)sub_02018288(object+*(u16*)(entry+0x2c)*120,&result);j++;matrix+=48;entry+=48;}while(j<*(u32*)w);}}
