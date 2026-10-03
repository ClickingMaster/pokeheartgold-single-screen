#pragma force_active on
#include "global.h"
typedef struct {int a;void *b;u8 p[0x60];void *a1,*a2,*a3,*a4;} W;
extern int ov102_021E8F6C(void*);extern void Sprite_SetDrawFlag(void*,int);extern void Sprite_SetAnimCtrlSeq(void*,int);
void ov102_021EAFAC(W*);
void ov102_021EAFAC(W*w) {if(ov102_021E8F6C(w->b)==2){Sprite_SetDrawFlag(w->a1,1);Sprite_SetDrawFlag(w->a2,1);Sprite_SetDrawFlag(w->a4,1);Sprite_SetDrawFlag(w->a3,1);Sprite_SetAnimCtrlSeq(w->a1,19);Sprite_SetAnimCtrlSeq(w->a2,21);}}
