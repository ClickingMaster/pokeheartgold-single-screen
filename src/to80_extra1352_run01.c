#pragma force_active on
#include "global.h"
#define V(w,o) (*(int *)((u8 *)(w)+(o)))
extern void ov07_0222260C(void *),ov07_02222644(void *,float *,float *),ManagedSprite_SetAffineScale(void *,float,float),ov07_022226FC(void *,s16,s16,int,int),ov07_02222590(void *,int,int,int,int,int,int),ManagedSprite_SetDrawFlag(void *,int);
extern int ov07_02222558(void *);
int ov07_0222F4D4(u8 *);
int ov07_0222F4D4(u8 *w){float x,y;int done=0;switch(V(w,0x94)){case 0:ov07_0222260C(w+0x28);ov07_02222644(w+0x28,&x,&y);if(V(w,0xc8)==1)x=0.0f-x;ManagedSprite_SetAffineScale((void *)V(w,0x18),x,y);ov07_022226FC((void *)V(w,0x18),V(w,0x14),V(w,0x10),V(w,0x3c),0);V(w,0x98)--;if(V(w,0x98)<0){V(w,0x94)++;ov07_02222590(w+0x28,10,1,20,20,10,4);}break;case 1:ov07_0222260C(w+0x28);if(ov07_02222558(w+0x4c)){*(s16 *)(w+0x28)=*(s16 *)(w+0x4c);ov07_02222644(w+0x28,&x,&y);if(V(w,0xc8)==1)x=0.0f-x;ManagedSprite_SetAffineScale((void *)V(w,0x18),x,y);ov07_022226FC((void *)V(w,0x18),V(w,0x14),V(w,0x10),V(w,0x3c),0);}else{V(w,0x94)++;ManagedSprite_SetDrawFlag((void *)V(w,0x20),0);ManagedSprite_SetDrawFlag((void *)V(w,0x18),0);done=1;}break;case 2:done=1;break;}return done;}
