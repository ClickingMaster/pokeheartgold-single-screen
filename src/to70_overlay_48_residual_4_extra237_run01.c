#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Modes{u32 words[4];}Modes;typedef struct Template{u32 words[7];}Template;
extern const Modes ov48_0225B190;extern const Template ov48_0225B2A4[];extern const int ov48_0225B1B0[];extern u8 gSystem[];
void SetBothScreensModesAndDisable(const Modes *);void *BgConfig_Alloc(int);void GfGfx_SwapDisplay(void);
void InitBgFromTemplate(void *,u8,const Template *,int);void BG_ClearCharDataRange(int,int,int,int);void BgClearTilemapBufferAndCommit(void *,u8);
void GfGfxLoader_GXLoadPalFromOpenNarc(void *,int,int,int,int,int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *,int,void *,int,int,int,int,int);void GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int);
void LoadFontPal1(int,int,int);void LoadUserFrameGfx1(void *,int,int,int,int,int);void LoadUserFrameGfx2(void *,int,int,int,int,int);
int Options_GetFrame(void *);void BG_SetMaskColor(int,int);void ov48_022594F0(u8 *,void *,int);
#define P(o) (*(void **)(w+(o)))
void ov48_022594F0(u8 *w,void *options,int heap){volatile int i;int next;const Template *t;const int *ids;u8 frame;
 SetBothScreensModesAndDisable(&ov48_0225B190);P(0)=BgConfig_Alloc(heap);gSystem[0x69]=1;GfGfx_SwapDisplay();
 i=0;t=ov48_0225B2A4;ids=ov48_0225B1B0;do{InitBgFromTemplate(P(0),(u8)*ids,t,0);BG_ClearCharDataRange((u8)*ids,32,0,heap);BgClearTilemapBufferAndCommit(P(0),(u8)*ids);next=i+1;t++;i=next;ids++;}while(next<5);
 GfGfxLoader_GXLoadPalFromOpenNarc(P(324),5,4,0,0,heap);
 GfGfxLoader_LoadCharDataFromOpenNarc(P(324),11,P(0),4,0,0,0,heap);GfGfxLoader_LoadScrnDataFromOpenNarc(P(324),12,P(0),4,0,0,0,heap);
 GfGfxLoader_LoadCharDataFromOpenNarc(P(324),4,P(0),6,0,0,0,heap);GfGfxLoader_LoadScrnDataFromOpenNarc(P(324),18,P(0),7,0,0,0,heap);
 LoadFontPal1(0,32,heap);LoadFontPal1(4,352,heap);
 LoadUserFrameGfx1(P(0),1,31,0,0,heap);LoadUserFrameGfx1(P(0),1,31,0,0,heap);
 frame=(u8)Options_GetFrame(options);LoadUserFrameGfx2(P(0),4,10,10,frame,heap);LoadUserFrameGfx2(P(0),1,1,2,frame,heap);BG_SetMaskColor(0,0x72ca);
}
