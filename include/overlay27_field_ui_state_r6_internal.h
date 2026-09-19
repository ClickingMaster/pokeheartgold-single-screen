#ifndef TO52_OVERLAY27_R6_H
#define TO52_OVERLAY27_R6_H
#include "global.h"
typedef struct Sprite Sprite;
typedef struct Bag Bag;
BOOL IsPaletteFadeFinished(void);
BOOL FieldSystem_IsPlayerMovementAllowed(void *);
BOOL ov27_0225BD44(void *);
BOOL MapSceneScriptCheck(void *);
BOOL sub_02058AA0(void);
void ov27_0225B398(void *, int);
void Sprite_SetOamMode(Sprite *, u32);
void ov27_0225B4AC(void *, int);
void G2x_SetBlendAlpha_(u32, int, int, int, int);
BOOL ov27_0225BDAC(void *);
Bag *Save_Bag_Get(void *);
u16 Bag_GetRegisteredItem1(Bag *);
u16 Bag_GetRegisteredItem2(Bag *);
void Sprite_SetDrawFlag(Sprite *, int);
BOOL ov27_0225A89C(void *);
void ov27_0225A8E8(void *, BOOL);
void ov27_0225A9C0(void *, BOOL);
#endif
