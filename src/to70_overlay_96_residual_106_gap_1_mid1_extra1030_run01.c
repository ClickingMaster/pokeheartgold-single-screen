#pragma force_active on
#include "global.h"
typedef struct {u8 p[8];void *sys,*man;u8 q[0x5c];void *sprites[2];} W;
extern void *ov96_022104D8(void*,void*,s16,s16,u16,int);extern void ManagedSprite_SetPaletteOverride(void*,int);
void ov96_0221065C(W*);
void ov96_0221065C(W*w) {int i,x;for(i=0,x=230;i<2;x+=16,i++){w->sprites[i]=ov96_022104D8(w->sys,w->man,(s16)x,76,(u16)(i+9),0);ManagedSprite_SetPaletteOverride(w->sprites[i],1);}}
