#pragma force_active on
#include "global.h"
#include <string.h>
#pragma force_active on
typedef struct E{u8 fields[40];}E;typedef struct W{u8 pad[60];u16 a,b;u8 data[72];int indices[3];u8 gap[12];E entries[5];u8 controller[1];}W;typedef struct Triple{int a,b,c;}Triple;extern void ov48_0225A95C(E*,void*),ov48_0225AA50(E*),ov48_0225ACAC(void*),ov48_0225AA38(E*,int,int),ov48_0225A790(W*,int,Triple*,void*),ov48_0225ACD8(void*,u8,u8,u8);extern int ov48_0225A30C(W*,Triple*,int),ov48_0225B038(int);void ov48_0225A338(W*,void*),ov48_0225A354(W*,void*);
void ov48_0225A338(W*w,void*ctx){int i=0;E*entry=w->entries;for(;i<5;i++,entry++)ov48_0225A95C(entry,ctx);}
