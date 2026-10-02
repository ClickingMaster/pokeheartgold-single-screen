#pragma force_active on
#include "global.h"
typedef struct {u32 a;void**arg;u8 pad[0x2c-8];s32 pos[2];u8 p[0x204-0x1a4-0x34];u32 unused:16;u32 flags:4;u32 unused2:12;u8 q[0xa8-(0x204-0x1a4)-4];}Entry;
typedef struct {u8 pad[0x1a4];Entry entries[1];}Work;
extern Work*PokeathlonCourse_GetHeapAllocPtr4(void*);
extern void ov96_021EB06C(void*,s32,s32,int*,int*);
s32*ov96_02215DD4(void*,int);void ov96_02215DEC(void*,int,s32*);u32 ov96_02215E2C(void*,int);
s32*ov96_02215DD4(void*arg,int n){u32 address=(u32)&PokeathlonCourse_GetHeapAllocPtr4(arg)->entries[n];return (s32*)(address+0x2c);}
void ov96_02215DEC(void*arg,int n,s32*out){int x,y;Entry*e=&PokeathlonCourse_GetHeapAllocPtr4(arg)->entries[n];ov96_021EB06C(*e->arg,e->pos[0]>>12,e->pos[1]>>12,&x,&y);out[0]=x<<12;out[1]=y<<12;}
u32 ov96_02215E2C(void*arg,int n){return PokeathlonCourse_GetHeapAllocPtr4(arg)->entries[n].flags;}
