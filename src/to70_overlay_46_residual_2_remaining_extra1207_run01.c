#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "render_window.h"
#include "font.h"
#include "palette.h"
#include "sys_task_api.h"
#include "options.h"
#include "save.h"
typedef struct { SaveData *save; u32 x4,x8; BgConfig *bg; u8 pad10[0xc4]; SysTask *task; int active; u16 colors[64],blend[336]; u32 pad3fc; int end; } Work;
extern const GraphicsBanks ov46_022595B4;
extern const GraphicsModes ov46_022595A4;
extern const BgTemplate ov46_022595DC[];
extern const int _02259598[];
extern Options *Save_PlayerData_GetOptionsAddr(SaveData *);
extern void ov46_02259210(SysTask *,void *);
void ov46_02258F78(Work *,enum HeapID);
void ov46_02258F78(Work *w, enum HeapID heap) {
 int i,j; const BgTemplate *t; NARC *narc; int outer,n,amount,final; u16 *dst,*src; Work *base; u8 blend; NNSG2dPaletteData *pal; void *raw; u16 *sc,*dc; u8 frame;
 *(vu16 *)0x04000050=0; *(vu16 *)0x04001050=0;
 GfGfx_SetBanks(&ov46_022595B4); BG_SetMaskColor(0,0);SetBothScreensModesAndDisable(&ov46_022595A4);
 w->bg=BgConfig_Alloc(heap);
 for(i=0;i<3;i++) { t=ov46_022595DC+i;
 InitBgFromTemplate(w->bg,(u8)_02259598[i],t,0); BG_ClearCharDataRange((u8)_02259598[i],32,0,heap);BgClearTilemapBufferAndCommit(w->bg,(u8)_02259598[i]); }
 frame=Options_GetFrame(Save_PlayerData_GetOptionsAddr(w->save));
 LoadFontPal0((enum GFPalLoadLocation)0,(enum GFPalSlotOffset)320,heap);LoadFontPal1((enum GFPalLoadLocation)0,(enum GFPalSlotOffset)288,heap);
 LoadUserFrameGfx1(w->bg,(GFBgLayer)1,31,12,0,heap);LoadUserFrameGfx2(w->bg,(GFBgLayer)1,1,11,frame,heap);
 GfGfxLoader_GXLoadPal((NarcId)88,3,(enum GFPalLoadLocation)0,(enum GFPalSlotOffset)0,288,heap);GfGfxLoader_GXLoadPal((NarcId)88,3,(enum GFPalLoadLocation)4,(enum GFPalSlotOffset)0,288,heap);
 GfGfxLoader_LoadCharData((NarcId)88,2,w->bg,(GFBgLayer)0,0,0,0,heap);GfGfxLoader_LoadCharData((NarcId)88,11,w->bg,(GFBgLayer)4,0,0,0,heap);
 GfGfxLoader_LoadScrnData((NarcId)88,6,w->bg,(GFBgLayer)0,0,0,0,heap);GfGfxLoader_LoadScrnData((NarcId)88,12,w->bg,(GFBgLayer)4,0,0,0,heap);
 GfGfx_EngineATogglePlanes(16,1);narc=NARC_New((NarcId)88,heap);
 MI_CpuFill8(&w->task,0,0x330);
 raw=GfGfxLoader_GetPlttDataFromOpenNarc(narc,5,&pal,heap);
 MIi_CpuCopy16(pal->pRawData,w->colors,128);MIi_CpuCopy16(pal->pRawData,w->blend,128);Heap_Free(raw);
 n=0;outer=0;dst=w->blend;src=w->colors;
 do { amount=0;final=0;base=(Work *)((u8 *)w+(outer+1)*32);
  while(1) { GF_ASSERT(n<21);
   for(j=1;j<16;j++) { sc=src+j;dc=dst+j;blend=amount>>8;BlendPalette(sc,dc,1,blend,base->colors[j]); }
   dst+=16;n++;
   if(final==1)break;
   amount+=0x300;if(amount>=0x1000){amount=0x1000;final=1;}
  }
  src+=16;outer++;
 }while(outer<3);
 DC_FlushRange(w->blend,0x2a0);w->active=1;w->end=0;
 w->task=SysTask_CreateOnVBlankQueue(ov46_02259210,&w->task,20);NARC_Delete(narc);
}
