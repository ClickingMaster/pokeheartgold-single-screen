#pragma force_active on
#include "global.h"
#include <nitro/gx/gx.h>
#include <nitro/gx/g2.h>
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "palette.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"
#pragma force_active on
typedef struct {u8 p[0x10];BgConfig*bg;void*system,*manager;PaletteData*palette;void*done;NARC*narc;u8 pad[0x40-0x28];u16 colors[128];SysTask*task;u8 tail[0x168-0x144];} State;
extern void ov80_0223AA4C(SysTask*,void*);
void ov80_0223A938(void*,const u8*);
#pragma opt_common_subs off
#pragma opt_propagation off
void ov80_0223A938(void*s,const u8*spec){NNSG2dPaletteData*pltt;void*buf;State*w=s;GX_SetVisibleWnd(3);G2_SetWnd0InsidePlane(31,TRUE);G2_SetWnd1InsidePlane(31,TRUE);G2_SetWndOutsidePlane(29,TRUE);G2_SetWnd0Position(0,0,0,0);G2_SetWnd1Position(0,0,0,0);PaletteData_LoadNarc(w->palette,(NarcId)109,spec[8],(enum HeapID)101,(PaletteBufferId)0,32,192);GfGfxLoader_LoadCharDataFromOpenNarc(w->narc,spec[9],w->bg,(GFBgLayer)1,0,0,0,(enum HeapID)101);GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc,spec[10],w->bg,(GFBgLayer)1,0,0,0,(enum HeapID)101);BgTilemapRectChangePalette(w->bg,1,0,0,32,32,12);ScheduleBgTilemapBufferTransfer(w->bg,1);buf=GfGfxLoader_GetPlttData((NarcId)109,spec[8],&pltt,(enum HeapID)101);MIi_CpuCopy16(pltt->pRawData,w->colors,256);Heap_Free(buf);w->task=SysTask_CreateOnMainQueue(ov80_0223AA4C,s,1100);}
