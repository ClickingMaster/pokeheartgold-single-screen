#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x198];u32 a;void *sprite;void *text;u8 p[0x1b4-0x1a4];u32 b;void *sprite2;void *text2;} View;
typedef struct {u8 pad[0x860];View *view;} State;
extern void ov40_0222D6EC(State *,int),VsRecorder_CreateTextSprite(void *,void *,int),VsRecorder_SetTextSpriteMsg(void *,void *,int),ManagedSprite_SetAnim(void *,int),ManagedSprite_SetPositionXY(void *,int,int),TextOBJ_SetPosition(void *,int,int),TextOBJ_SetSpritesDrawFlag(void *,int),ov40_022359B4(State *,int);
extern void *ov40_0222D800(State *,int);
void ov40_02235A30(State *);
void ov40_02235A30(State *s){View *v=s->view;ov40_0222D6EC(s,2);v->sprite=ov40_0222D800(s,2);v->sprite2=ov40_0222D800(s,2);VsRecorder_CreateTextSprite(&v->a,(u8*)s+20,2);VsRecorder_CreateTextSprite(&v->b,(u8*)s+20,2);VsRecorder_SetTextSpriteMsg(&v->a,(u8*)s+20,3);VsRecorder_SetTextSpriteMsg(&v->b,(u8*)s+20,53);ManagedSprite_SetAnim(v->sprite,0);ManagedSprite_SetAnim(v->sprite2,4);ManagedSprite_SetPositionXY(v->sprite,32,232);ManagedSprite_SetPositionXY(v->sprite2,128,232);TextOBJ_SetPosition(v->text,36,-8);TextOBJ_SetPosition(v->text2,36,-8);TextOBJ_SetSpritesDrawFlag(v->text,1);TextOBJ_SetSpritesDrawFlag(v->text2,1);ov40_022359B4(s,0);}
