#pragma force_active on
#include "global.h"
typedef struct{u32 x;void*sprite,*text;}Entry;
extern void*ov40_0222D800(void*,int);extern void VsRecorder_CreateTextSprite(Entry*,void*,int),VsRecorder_SetTextSpriteMsg(Entry*,void*,int);extern void ManagedSprite_SetPositionXY(void*,s16,s16),TextOBJ_SetPosition(void*,s16,s16),TextOBJ_SetSpritesDrawFlag(void*,int),ManagedSprite_SetAnim(void*,int);void ov40_02230970(void*,Entry*,int,int,s16,s16,s16,s16,int,int);
void ov40_02230970(void*w,Entry*e,int kind,int value,s16 x,s16 y,s16 tx,s16 ty,int anim,int draw){e->sprite=ov40_0222D800(w,kind);VsRecorder_CreateTextSprite(e,(u8*)w+20,kind);VsRecorder_SetTextSpriteMsg(e,(u8*)w+20,value);ManagedSprite_SetPositionXY(e->sprite,x,y);TextOBJ_SetPosition(e->text,tx,ty);TextOBJ_SetSpritesDrawFlag(e->text,draw);ManagedSprite_SetAnim(e->sprite,anim);}
