#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad0[0x11];u8 page;u8 pad12[0x34-0x12];u8 selection;}Input;
typedef struct{u16 species;u8 a,b,pad4,c,d,e;}Entry;
typedef struct{Input*input;u8 pad4[0x34-4];Entry entries[1];}State;
extern const u32 ov08_02224FE0[];extern void FillWindowPixelBuffer(void*,int),ov08_0221DDCC(State*,int,int,int,int,int),ov08_0221E2E8(State*,int,int,int),ov08_0221E244(State*,int,int,u32,int,int,u32),ov08_0221ECE8(State*,int),ov08_0221ED30(State*,int,int),ov08_0221EE18(State*,int),ov08_0221EE60(State*,int,int),ov08_0221EF48(State*,int,int),ov08_0221EFA8(State*,int),ov08_0221F00C(State*,int,int),ov08_0221F07C(State*,int,int,int);
void ov08_0221FB18(State*);
#pragma opt_common_subs off
void ov08_0221FB18(State*s){int base=*((u8*)s+0x2075)*11;Entry*e;int page;FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+0x60,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+0x70,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+0x80,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+0x90,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+0xa0,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+(base+1)*16,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+base*16,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+(base+2)*16,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+(base+3)*16,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+(base+5)*16,0);FillWindowPixelBuffer((u8*)*(void**)((u8*)s+0x2070)+(base+4)*16,0);page=s->input->page;e=(Entry*)((u8*)s+0x34+page*80+s->input->selection*8);ov08_0221DDCC(s,6,0,page,0,0);ov08_0221E2E8(s,7,0,0);ov08_0221E244(s,e->species,base,ov08_02224FE0[s->input->selection],0,0,0xf0e00);ov08_0221ECE8(s,8);ov08_0221ED30(s,base+2,e->d);ov08_0221EE18(s,9);ov08_0221EE60(s,base+3,e->e);ov08_0221EF48(s,base+4,e->species);ov08_0221EFA8(s,10);ov08_0221F00C(s,base+5,e->c);ov08_0221F07C(s,base+1,e->a,e->b);*((u8*)s+0x2075)^=1;}
