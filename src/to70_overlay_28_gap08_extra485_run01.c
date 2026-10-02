#pragma force_active on
#include "global.h"
typedef struct{int x,y,z;} Vec;typedef struct {u8 p[0x190];void *sprites[4];u8 q[0x210-0x1a0];int index;u8 r[8];Vec a[4];u8 s[0x24c-0x24c];u16 count:15,flag:1;} Work;
extern void ov28_0225DE04(Vec *,int),Sprite_SetMatrix(void *,Vec *),ov28_0225E424(void *,int);extern void (*ov28_0225EAAC[])(Vec *);void ov28_0225E43C(Work *);
void ov28_0225E43C(Work *w){u32 i;Vec pos;if(w->count){w->count--;if(!w->count){for(i=0;i<4;i++){w->a[i].x=w->a[i].y;ov28_0225DE04(&pos,w->a[i].x);Sprite_SetMatrix(w->sprites[i],&pos);ov28_0225E424(w->sprites[i],w->a[i].x);}}else{for(i=0;i<4;i++){w->a[i].x=w->a[i].z;ov28_0225DE04(&pos,w->a[i].x);Sprite_SetMatrix(w->sprites[i],&pos);ov28_0225E424(w->sprites[i],w->a[i].x);ov28_0225EAAC[w->index](&w->a[i]);}}}}
