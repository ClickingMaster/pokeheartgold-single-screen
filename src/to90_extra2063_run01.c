#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct {u32 speed:9,reserved:9,life:8,pad:6;} Entry;
typedef struct {void*context;Entry entries[3];int timer;u32 counter:8,pad0:22,slot:2;u32 pad1:30,owner:2;u32 value:8,enabled:1,skip:1,pad2:22;} Work;
void ov96_021E8228(void*,u8,u8,int,int);u8 ov96_0220D33C(u32,u16);void ov96_0220CD84(void*,int);void ov96_0220D014(void*,int,u16);
void ov96_0220CE04(Work*w,int extra);
void ov96_0220CE04(Work*w,int extra){u16 count=0;s16 speed;Entry*entries;u32 slot;GF_ASSERT(w);
 entries=w->entries;slot=w->slot;speed=entries[slot].speed;if(extra)speed=(s16)(speed+160);
 while(1){w->timer-=speed;if(w->timer>0)break;speed=(s16)-w->timer;w->timer=100;ov96_021E8228(w->context,(u8)w->owner,(u8)w->slot,3,1);count++;}
 w->counter+=count;
 if(!w->skip){if(w->enabled){entries[slot].life=ov96_0220D33C(entries[slot].life,(u16)w->value);if(!entries[slot].life){ov96_021E8228(w->context,(u8)w->owner,(u8)w->slot,1,1);ov96_0220CD84(w,2);return;}}
 ov96_0220D014(w,extra,count);}
 if(count&&w->counter&&w->counter%10==0)ov96_0220CD84(w,3);
 w->enabled=1;w->value=0;ov96_021E8228(w->context,(u8)w->owner,(u8)w->slot,4,1);
}
