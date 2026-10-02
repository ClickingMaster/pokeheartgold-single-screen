#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0xdf4];void *s[8];} Work;
extern void ov72_0223AD64(Work*);extern void Sprite_SetDrawFlag(void*,int);extern void Sprite_SetAnimCtrlSeq(void*,int);

void ov72_0223AD94(Work*w);

void ov72_0223AD94(Work*w){int i;ov72_0223AD64(w);Sprite_SetDrawFlag(w->s[0],0);Sprite_SetDrawFlag(w->s[1],0);Sprite_SetDrawFlag(w->s[2],0);Sprite_SetDrawFlag(w->s[3],0);Sprite_SetDrawFlag(w->s[4],0);Sprite_SetDrawFlag(w->s[5],0);Sprite_SetDrawFlag(w->s[6],0);Sprite_SetDrawFlag(w->s[7],0);for(i=0;i<3;i++)Sprite_SetAnimCtrlSeq(w->s[i],0);}
