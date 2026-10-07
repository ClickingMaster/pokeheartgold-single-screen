#pragma force_active on
#include "global.h"
typedef union {u32 raw;struct{u32 low:20;u32 a:2;u32 b:2;u32 state:2;u32 active:1;u32 unused:1;u32 other:1;u32 rest:3;} bits;} Flags;
typedef struct {void *owner;u8 pad[0x1c];VecFx32 current,previous;u8 gap[0x28];Flags flags;} Context;
extern void ov96_021E8228(void *,u8,u8,u32,BOOL);void ov96_0221910C(Context *);
#pragma force_active on
void ov96_0221910C(Context *p){GF_ASSERT(!p->flags.bits.active);GF_ASSERT(!p->flags.bits.other);p->flags.bits.active=1;p->flags.bits.state=1;p->current=p->previous;ov96_021E8228(p->owner,p->flags.bits.a,p->flags.bits.b,7,TRUE);}
