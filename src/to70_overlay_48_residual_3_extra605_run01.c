#pragma force_active on
#include "global.h"
#include <stdarg.h>
typedef struct{u32 a,b,c;}Triple;extern u32 ov48_02259B10(void*,int,int),ov48_02259BBC(void*);extern void ov48_02259B3C(void*,Triple*,u32);
int ov48_0225932C(u8*w,u32 index,volatile u8 angle,...);
int ov48_0225932C(u8*w,u32 index,volatile u8 angle,...){Triple a,b;u32 found;int value=angle-4;found=ov48_02259B10(w+548,219,3);GF_ASSERT(found<ov48_02259BBC(w+548));ov48_02259B3C(w+548,&a,found);ov48_02259B3C(w+548,&b,index);value+=(24*(u16)(a.b-b.b))/65535;if(value<0)value+=24;if(value>=24)value%=24;return value;}
