#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
typedef struct{u32 active:1;u32 unused:4;u32 change:1;u32 seq:2;u32 rest:24;} Flags;
#define F(p) (*(Flags*)((u8*)(p)+0x51c))
typedef struct{u8 mode:6;u8 spare:1;u8 hidden:1;} FieldFlags;
#define G(p) (*(FieldFlags*)((u8*)(p)+0xd2))
extern const u8 ov27_0225CECC[];int ov27_0225A89C(void*),FieldSystem_IsPlayerMovementAllowed(void*),IsPaletteFadeFinished(void),FieldSystem_TaskIsRunning(void*),TouchscreenHitbox_FindRectAtTouchHeld(const void*),TouchscreenHitbox_FindRectAtTouchNew(const void*),ov27_0225A4D0(void*);void ov27_0225A8E8(void*,int),Sprite_TryChangeAnimSeq(void*,int),Sprite_SetAnimCtrlSeq(void*,int),ov27_0225A7FC(void*),ov27_0225A86C(void*),ov27_0225A66C(void*),ov27_0225B4D8(void*),ov27_0225A530(void*,int),ov27_0225A48C(void*,int),ov27_0225BDFC(void*),SpriteList_RenderAndAnimateSprites(void*),ov27_0225A320(void*,u8*);
void ov27_0225A320(void*ignored,u8*d){int busy=ov27_0225A89C(P(d,16)),allowed=FieldSystem_IsPlayerMovementAllowed(P(d,16)),held,touch;if(G(P(d,16)).hidden){if(allowed&&IsPaletteFadeFinished())G(P(d,16)).hidden=0;else *(vu16*)0x04001050=0;}else ov27_0225A8E8(d,busy);if(F(d).change)Sprite_TryChangeAnimSeq(P(d,0x390+U(d,20)*4),2);if(F(d).seq==1){Sprite_SetAnimCtrlSeq(P(d,0x3ac),2);F(d).seq=0;}else if(F(d).seq==2){Sprite_SetAnimCtrlSeq(P(d,0x3b0),2);F(d).seq=0;}if(FieldSystem_TaskIsRunning(P(d,16)))ov27_0225A7FC(d);else{if((G(P(d,16)).mode)==2){ov27_0225A86C(d);G(P(d,16)).mode=0;}if(allowed)ov27_0225A66C(d);if(!busy)ov27_0225B4D8(d);}held=TouchscreenHitbox_FindRectAtTouchHeld(ov27_0225CECC);touch=TouchscreenHitbox_FindRectAtTouchNew(ov27_0225CECC);ov27_0225A530(d,held);if((F(d).active)&&ov27_0225A4D0(d)&&!busy)ov27_0225A48C(d,touch);ov27_0225BDFC(d+0x520);SpriteList_RenderAndAnimateSprites(P(d,24));}
