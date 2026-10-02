#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[0x3f4];struct{unsigned count:5,pad:9,page:5,rest:13;}bits;u8 p3f8[8];void*window;u8 p404[8];void*up,*down;};void ManagedSprite_SetDrawFlag(void*,int);void ov98_0221EDA4(void*,int,int,int);void ov98_0221ED48(void*,int,int,int,int);void ov99_021E7508(struct S*);
void ov99_021E7508(struct S*s){int up=0,down;if(s->bits.count && s->bits.page)up=1;if(s->bits.count && s->bits.page<s->bits.count)down=1;else down=0;ManagedSprite_SetDrawFlag(s->up,up);ManagedSprite_SetDrawFlag(s->down,down);ov98_0221EDA4(s->window,s->bits.page+1,2,0);ov98_0221EDA4(s->window,s->bits.count+1,2,1);ov98_0221ED48(s->window,3,129,0,0);}
