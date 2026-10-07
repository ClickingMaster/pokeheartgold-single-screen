#pragma force_active on
#include "global.h"
typedef struct{signed int unused:1,skip:1,other:30;} Flags;
typedef struct{u8 pad[280];Flags flags;} Child;
typedef struct{u8 pad[24];u32*pc;u8 pad1c[0xa4];Child*child;} State;
void ov07_0221D23C(State*);
#pragma force_active on
void ov07_0221D23C(State*s){u32**pc=&s->pc;State*saved=s;++*pc;if(saved->child->flags.skip)*pc=**(u32**)((u8*)s+24)+*(u32*volatile*)pc;else ++*pc;}
