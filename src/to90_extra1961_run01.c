#pragma force_active on
#include "global.h"
typedef struct Context{u8 padding[0x2bc4];u8 window[16];u8 other[16];} Context;
#define P(d,n) ((char*)(d)+(n))
#define W(d,n) (*(void**)P(d,n))
#define I(d,n) (*(int*)P(d,n))
extern u8 gSystem[];
void*OverlayManager_GetData(void*);void*OverlayManager_GetArgs(void*);void*Save_MysteryGift_Get(void*);void*Save_PlayerData_GetOptionsAddr(void*);u8 Options_GetFrame(void*);void*SaveMysteryGift_CardGetByIdx(void*,int);
void ov74_0222FCA4(void);void ov74_0222FCC4(void*);void ResetAllTextPrinters(void);void ov74_0222F314(void*,int);void LoadFontPal0(int,int,int);
void LoadUserFrameGfx1(void*,int,int,int,int,int);void LoadUserFrameGfx2(void*,int,int,int,int,int);
void ov74_0222F024(void*,int,int);void ov74_0222EFF0(void*,int,int,int*);void PlaySE(int);int ov74_0222F1BC(void*,void*,int,int);
void ov74_0222F404(void*,int*,int);void GfGfx_EngineATogglePlanes(int,int);void ov74_0222ECEC(void*,int);void ov74_0222ECD4(void*,int);void ov74_0222F624(void*,int,int,int);int ov74_0222F6C4(void);
void DrawFrameAndWindow2(void*,int,int,int);void DrawFrameAndWindow1(void*,int,int,int);void ov74_0222F688(void*);void ov74_0222EDC0(void*);void ClearWindowTilemapAndCopyToVram(void*);void RemoveWindow(void*);void ov74_0222EC60(void*);
int sub_02037D78(void);int IsPaletteFadeFinished(void);void SpriteList_RenderAndAnimateSprites(void);void ov74_022358BC(void);
int ov74_0222F7D4(void*,int*);
#pragma force_active on
#pragma opt_common_subs off
int ov74_0222F7D4(void*manager,int*state){void*d=OverlayManager_GetData(manager);
 switch(*state){
 case 0:W(d,0x2ba4)=W(OverlayManager_GetArgs(manager),8);W(d,0x2ba0)=Save_MysteryGift_Get(W(d,0x2ba4));W(d,0x2ba8)=Save_PlayerData_GetOptionsAddr(W(d,0x2ba4));I(d,0x2bac)=Options_GetFrame(W(d,0x2ba8));W(d,0x2bb4)=SaveMysteryGift_CardGetByIdx(W(d,0x2ba0),4);*state=1;break;
 case 1:ov74_0222FCA4();ov74_0222FCC4(W(d,0x29fc));*state=2;break;
 case 2:ResetAllTextPrinters();ov74_0222F314(d,0);LoadFontPal0(0,0x1e0,0x55);LoadUserFrameGfx1(W(d,0x29fc),0,1,13,0,0x55);LoadUserFrameGfx1(W(d,0x29fc),0,10,14,1,0x55);LoadUserFrameGfx2(W(d,0x29fc),0,19,10,(u8)I(d,0x2bac),0x55);ov74_0222F024(d,1,0);ov74_0222EFF0(d,1,3,state);break;
 case 3:if(I(gSystem,0x48)&2){PlaySE(1500);ov74_0222EFF0(d,0,29,state);}else if(I(gSystem,0x48)&1){PlaySE(1500);*state=4;}break;
 case 4:I(d,0x2be4)=ov74_0222F1BC(d,((Context*)d)->window,7,0x280);ov74_0222F1BC(d,((Context*)d)->other,8,I(d,0x2be4));*state=5;break;
 case 5:ov74_0222F404(manager,state,0);if(I(gSystem,0x48)&2){PlaySE(1500);*state=11;}break;
 case 6:PlaySE(1535);GfGfx_EngineATogglePlanes(16,0);ov74_0222ECEC(((Context*)d)->window,0);ov74_0222ECD4(((Context*)d)->other,0);ov74_0222F624(d,1,4096,0x66);*state=7;break;
 case 7:if(ov74_0222F6C4()){ov74_0222F024(d,1,1);ov74_0222F624(d,0,0x708000,0x384000);*state=8;}break;
 case 8:ov74_0222F6C4();if(I(gSystem,0x48)&3){PlaySE(1535);ov74_0222F624(d,1,4096,0x66);*state=9;}break;
 case 9:if(ov74_0222F6C4()){ov74_0222F024(d,1,0);ov74_0222F624(d,0,0x708000,0x384000);GfGfx_EngineATogglePlanes(16,0);*state=10;}break;
 case 10:if(ov74_0222F6C4()){DrawFrameAndWindow2(((Context*)d)->window,0,19,10);DrawFrameAndWindow1(((Context*)d)->other,0,10,14);GfGfx_EngineATogglePlanes(16,1);ov74_0222F688(d);*state=5;}break;
 case 11:ov74_0222EDC0(d);ov74_0222ECEC(((Context*)d)->window,0);ClearWindowTilemapAndCopyToVram(((Context*)d)->window);RemoveWindow(((Context*)d)->window);*state=3;break;
 case 20:ov74_0222EFF0(d,0,21,state);break;
 case 21:ov74_0222EC60(d);ov74_0222ECEC(((Context*)d)->window,0);ov74_0222ECD4(((Context*)d)->other,0);ov74_0222F024(d,1,0);LoadFontPal0(0,0x1e0,0x55);ov74_0222EFF0(d,1,4,state);break;
 case 23:if(!sub_02037D78())*state=I(d,0x2bf8);break;
 case 13:if(I(gSystem,0x48)&3){ov74_0222ECEC(((Context*)d)->window,0);ClearWindowTilemapAndCopyToVram(((Context*)d)->window);RemoveWindow(((Context*)d)->window);*state=3;}break;
 case 27:ov74_0222EFF0(d,0,29,state);break;
 case 28:if(IsPaletteFadeFinished())*state=I(d,0x2bf8);break;
 case 29:ov74_0222EC60(d);ov74_0222F688(d);return 1;
 case 12:case 14:case 15:case 16:case 17:case 18:case 19:case 22:case 24:case 25:case 26:case 30:default:break;
 }
 if(W(d,0x2bfc))SpriteList_RenderAndAnimateSprites();ov74_022358BC();if(W(d,0x3d00))(*(void(**)(void*))P(d,0x3d00))(d);return 0;
}
