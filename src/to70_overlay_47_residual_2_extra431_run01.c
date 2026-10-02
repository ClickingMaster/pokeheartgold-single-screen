#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[7];} Template;
extern u8 ov47_02259E48[],gSystem[];extern Template ov47_02259F18[];extern u32 ov47_02259E68[];
extern void SetBothScreensModesAndDisable(void *),GfGfx_SwapDisplay(void),InitBgFromTemplate(void *,u8,Template *,int),BG_ClearCharDataRange(u8,int,int,int),BgClearTilemapBufferAndCommit(void *,u8),LoadFontPal1(int,int,int),LoadUserFrameGfx2(void *,int,int,int,u8,int),LoadUserFrameGfx1(void *,int,int,int,int,int),GfGfxLoader_LoadCharData(int,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnData(int,int,void *,int,int,int,int,int),GfGfxLoader_GXLoadPal(int,int,int,int,int,int);extern void *BgConfig_Alloc(int);extern int Options_GetFrame(void *);
void ov47_02258AA0(void **,void *,int);
void ov47_02258AA0(void **s,void *options,int heap){int i;SetBothScreensModesAndDisable(ov47_02259E48);*s=BgConfig_Alloc(heap);gSystem[0x69]=0;GfGfx_SwapDisplay();for(i=0;i<4;i++){InitBgFromTemplate(*s,ov47_02259E68[i],&ov47_02259F18[i],0);BG_ClearCharDataRange(ov47_02259E68[i],32,0,heap);BgClearTilemapBufferAndCommit(*s,ov47_02259E68[i]);}LoadFontPal1(0,128,heap);LoadUserFrameGfx2(*s,1,1,5,Options_GetFrame(options),heap);LoadUserFrameGfx1(*s,1,31,6,0,heap);GfGfxLoader_LoadCharData(198,2,*s,4,0,0,0,heap);GfGfxLoader_LoadScrnData(198,6,*s,4,0,0,0,heap);GfGfxLoader_GXLoadPal(198,0,4,0,0,heap);}
