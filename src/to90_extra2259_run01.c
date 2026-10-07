#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Slot{u16 id,pad;}Slot;typedef struct W{u8 pad[0x18];void*bg;u8 pad1c[0x9d90];Slot first[3],second[10];u8 more[0xcdc];u8 flags:6;u8 enabled:1;u8 reserved:1;u8 more2[0x545];u16 last;u8 padb004[0x134a0];void*caption;}W;int ov112_021EFD4C(W*);
int ov112_021EFD4C(W*w){int count=0;{int i=0;W*p=w;for(;i<3;i++,p=(W*)((u8*)p+4)){if(p->first[0].id!=0)count++;}}{W*cursor;int j=0;cursor=w;do{if(cursor->second[0].id!=0)count++;j++;cursor=(W*)((u8*)cursor+4);}while(j<10);}if(w->enabled&&w->last!=0)count++;return count;}
