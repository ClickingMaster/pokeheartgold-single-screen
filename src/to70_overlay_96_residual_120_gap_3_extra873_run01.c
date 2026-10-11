#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x18];u8 active;} Meta;
typedef struct {u32 first;Meta *meta;} Child;
typedef union {u32 raw;struct {u32 unused:20;u32 a:2;u32 b:2;u32 more:2;u32 flag26:1;u32 other:1;u32 flag28:1;u32 flag29:1;u32 end:2;} bits;} Flags;
typedef struct {void *course;Child *child;u8 pad[12];int kind;u8 pad2[0x5a-0x18];u8 state;u8 pad3[5];Flags flags;} Work;
extern void PokeathlonCourse_AddMonCounter(void *,u8,u8,int,int);extern void ov96_02218578(Work *,int);
void ov96_02218F58(Work *);
void ov96_02218F58(Work *w){GF_ASSERT(w->kind==1||w->kind==4);GF_ASSERT(w->flags.bits.flag28==0);GF_ASSERT(w->flags.bits.flag26==0);w->flags.raw&=0xdfffffff;w->state=5;if(w->child->meta->active)PokeathlonCourse_AddMonCounter(w->course,(u8)w->flags.bits.a,(u8)w->flags.bits.b,2,1);ov96_02218578(w,8);}
