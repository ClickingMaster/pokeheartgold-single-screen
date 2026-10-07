#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x924];void *sprites[6];u8 pad2[0x1098];s32 target;volatile s32 current; s32 decay;} W;
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void *,s16,s16,int);
void ov89_0225A398(void *);
static inline int addY(int y){return y+176;}
void ov89_0225A398(void *opaque){W*w=opaque;int i,x;s16 y;if(w->target==w->current){w->target+=0x88;if(w->target>8192)w->target=8192;w->current=w->target;}else{w->current-=w->decay;if(w->current<w->target)w->current=w->target;}i=0;y=addY(32-(w->current>>8));x=16;do{ManagedSprite_SetPositionXYWithSubscreenOffset(w->sprites[0],x,y,0x110000);i++;x+=32;w=(W*)((u8*)w+4);}while(i<6);}
