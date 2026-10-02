#pragma force_active on
#include "global.h"
#pragma force_active on
void LoadFontPal1(int,int,int),LoadUserFrameGfx2(void *,int,int,int,u8,int),GfGfxLoader_LoadCharData(int,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnData(int,int,void *,int,int,int,int,int),GfGfxLoader_GXLoadPal(int,int,int,int,int,int),ov70_02238B54(void *,void *,void *,int),Sprite_SetAnimActiveFlag(void *,int),Sprite_SetAnimCtrlSeq(void *,int),sub_0203A880(void);void *Sprite_CreateAffine(void *);int Options_GetFrame(void *);void ov70_0223ABF4(void **),ov70_0223AC98(u8 *);
void ov70_0223ABF4(void **s){void *bg=s[1];LoadFontPal1(0,416,61);LoadUserFrameGfx2(bg,4,1,2,(u8)Options_GetFrame(*(void **)((u8 *)s[0]+36)),61);GfGfxLoader_LoadCharData(100,20,bg,1,0,3072,1,61);GfGfxLoader_GXLoadPal(100,8,0,0,96,61);GfGfxLoader_LoadCharData(100,18,bg,6,0,0,1,61);GfGfxLoader_LoadScrnData(100,42,bg,6,0,0,1,61);LoadFontPal1(4,32,61);}
void ov70_0223AC98(u8 *s){u32 params[12];ov70_02238B54(params,s,s+3424,1);params[2]=851968;params[3]=237568;*(void **)(s+3808)=Sprite_CreateAffine(params);Sprite_SetAnimActiveFlag(*(void **)(s+3808),1);Sprite_SetAnimCtrlSeq(*(void **)(s+3808),37);sub_0203A880();}
