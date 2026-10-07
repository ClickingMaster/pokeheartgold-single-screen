#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#pragma force_active on
extern const GraphicsModes _02244C44;
extern const GraphicsBanks ov40_02244CA0;
extern const BgTemplate ov40_02244CC8,ov40_02244CE4,ov40_02244D00,ov40_02244D1C,ov40_02244D38,ov40_02244D54,ov40_02244D70,ov40_02244D8C;
extern void ov40_0222BC44(int);
void ov40_0222BA90(BgConfig *);
void ov40_0222BA90(BgConfig *bg) {
GraphicsModes modes; GraphicsBanks banks;
GfGfx_DisableEngineAPlanes();modes=_02244C44;SetBothScreensModesAndDisable(&modes);
banks=ov40_02244CA0;GfGfx_SetBanks(&banks);
MIi_CpuClear32(0,(void *)0x06000000,0x80000);
MIi_CpuClear32(0,(void *)0x06200000,0x20000);
MIi_CpuClear32(0,(void *)0x06400000,0x40000);
MIi_CpuClear32(0,(void *)0x06600000,0x20000);
InitBgFromTemplate(bg,0,&ov40_02244CC8,0);
InitBgFromTemplate(bg,1,&ov40_02244CE4,0);
InitBgFromTemplate(bg,2,&ov40_02244D00,0);
InitBgFromTemplate(bg,3,&ov40_02244D1C,0);
BgClearTilemapBufferAndCommit(bg,0);
BgClearTilemapBufferAndCommit(bg,1);
BgClearTilemapBufferAndCommit(bg,2);
BgClearTilemapBufferAndCommit(bg,3);
InitBgFromTemplate(bg,4,&ov40_02244D38,0);
InitBgFromTemplate(bg,5,&ov40_02244D54,0);
InitBgFromTemplate(bg,6,&ov40_02244D70,0);
InitBgFromTemplate(bg,7,&ov40_02244D8C,0);
BgClearTilemapBufferAndCommit(bg,4);
BgClearTilemapBufferAndCommit(bg,5);
BgClearTilemapBufferAndCommit(bg,6);
BgClearTilemapBufferAndCommit(bg,7);
GfGfx_EngineATogglePlanes(1,0);
GfGfx_EngineATogglePlanes(2,1);
GfGfx_EngineATogglePlanes(4,0);
GfGfx_EngineATogglePlanes(8,0);
GfGfx_EngineATogglePlanes(16,1);
GfGfx_EngineBTogglePlanes(1,0);
GfGfx_EngineBTogglePlanes(2,1);
GfGfx_EngineBTogglePlanes(4,0);
GfGfx_EngineBTogglePlanes(8,0);
GfGfx_EngineBTogglePlanes(16,1);
ov40_0222BC44(1);G2x_SetBlendAlpha_(0x04000050,4,0x12,7,8);G2x_SetBlendAlpha_(0x04001050,4,0x12,7,8);
}
