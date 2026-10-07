#pragma force_active on
#include "global.h"
#pragma force_active on
#pragma opt_propagation off
typedef struct A{u8 pad[17];u8 index;}A;typedef struct Entry{u8 pad[40];u8 value[5];u8 gap[35];}Entry;typedef struct W{A*args;Entry entries[];}W;
extern void ov08_022213E4(void*,int),ov08_02220A8C(void*,int,int);extern int ov08_022214BC(u8,int,int);void ov08_02221500(W*);
void ov08_02221500(W*w){register int offset=80*w->args->index;register Entry*entries=w->entries;int x,y;
ov08_022213E4(*(void**)((u8*)w+0x2024),0);
ov08_022213E4(*(void**)((u8*)w+0x2028),1);
ov08_022213E4(*(void**)((u8*)w+0x202c),3);
ov08_022213E4(*(void**)((u8*)w+0x2030),4);
ov08_022213E4(*(void**)((u8*)w+0x2034),2);
x=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[0],144,144);y=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[0],2,24);ov08_02220A8C(*(void**)((u8*)w+0x2024),x,y);
x=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[1],164,144);y=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[1],16,24);ov08_02220A8C(*(void**)((u8*)w+0x2028),x,y);
x=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[2],156,144);y=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[2],41,24);ov08_02220A8C(*(void**)((u8*)w+0x202c),x,y);
x=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[3],131,143);y=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[3],41,24);ov08_02220A8C(*(void**)((u8*)w+0x2030),x,y);
x=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[4],123,143);y=ov08_022214BC(((Entry*)((u8*)entries+offset))->value[4],16,24);ov08_02220A8C(*(void**)((u8*)w+0x2034),x,y);
}
