#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x34];void *state;} Work;
extern void ov14_021E76B8(Work *),ov14_021F0C0C(Work *),ov14_021E84A4(void *),ov14_021E8328(void *),ov14_021F40DC(Work *),ov14_021E88F8(void *),ov14_021F0234(Work *,void *,int),ov14_021F2A18(void *,int,int),ov14_021F2AC8(void *),ov14_021F2B88(void *),ov14_021F2BB8(Work *),ov14_021F4D10(void *);
extern u8 ov14_021EA674[];
extern void GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int);

void ov14_021F2914(Work *w);
void ov14_021F2930(Work *w);
void ov14_021F297C(Work *w);

void ov14_021F2914(Work *w){ov14_021E76B8(w);ov14_021F2A18(w->state,11,0);ov14_021F0C0C(w);}
void ov14_021F2930(Work *w){ov14_021E84A4(*(void **)((u8*)w->state+0x2f0));ov14_021E8328(*(void **)((u8*)w->state+0x2f0));ov14_021F40DC(w);if(*(u16*)((u8*)w->state+0x88c8))ov14_021E88F8(*(void **)((u8*)w->state+0x2f0));ov14_021F0234(w,ov14_021EA674,140);}
void ov14_021F297C(Work *w){GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);ov14_021F2AC8(w->state);ov14_021F2B88(w->state);ov14_021F2BB8(w);ov14_021F4D10(w->state);}
