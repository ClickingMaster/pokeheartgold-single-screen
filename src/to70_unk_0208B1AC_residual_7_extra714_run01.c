#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x12];u8 kind;} Info;
typedef struct {u8 p[0x22c];Info *info;u8 q[0x428-0x230];void *sprite;u8 r[0x7bd-0x42c];u8 flags:4;u8 other:4;} Work;
extern void Sprite_SetPositionXY(void*,s16,s16);extern void Sprite_SetAnimCtrlSeq(void*,int);void sub_0208BBDC(Work*);
void sub_0208BBDC(Work*w){u32 mode=w->flags;switch(mode){case 4:if(w->info->kind==2)Sprite_SetPositionXY(w->sprite,68,(s16)(mode*32+40));else Sprite_SetPositionXY(w->sprite,68,(s16)(mode*32+24));Sprite_SetAnimCtrlSeq(w->sprite,0);break;case 5:if(w->info->kind==2){Sprite_SetPositionXY(w->sprite,220,176);Sprite_SetAnimCtrlSeq(w->sprite,3);}break;default:Sprite_SetPositionXY(w->sprite,68,(s16)(mode*32+24));Sprite_SetAnimCtrlSeq(w->sprite,0);break;}}
