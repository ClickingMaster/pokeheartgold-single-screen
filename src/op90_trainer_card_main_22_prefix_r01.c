#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct { u32 v[2]; } VramArr;
typedef struct { u8 f[2][4]; } FileArr;
typedef struct { u32 m[16]; } MemberArr;
typedef struct Work {
    void *renderer;
    u8 rend[0x12C - 4];
    void *mgr[2][4];
    void *obj[2][4];
    u8 pad16C[0x1F8 - 0x16C];
    void *raw[16];
    NNSG2dPaletteData *pltt[16];
} Work;
extern const u8 ov51_021E7FB8[4];
extern const VramArr ov51_021E7FBC;
extern const FileArr ov51_021E7FC4;
extern const MemberArr ov51_021E7FDC;
extern void ov51_021E7D68(void);
extern void NNS_G2dInitOamManagerModule(void);
extern void OamManager_Create(int, int, int, int, int, int, int, int, int);
extern void *G2dRenderer_Init(int, void *, int);
extern void G2dRenderer_SetSubSurfaceCoords(void *, int, int);
extern void *Create2DGfxResObjMan(int, int, int);
extern void *AddCharResObjFromNarc(void *, int, int, int, int, u32, int);
extern void *AddPlttResObjFromNarc(void *, int, int, int, int, u32, int, int);
extern void *AddCellOrAnimResObjFromNarc(void *, int, int, int, int, int, int);
extern void SpriteTransfer_CreateCharTransferTask(void *);
extern void SpriteTransfer_CreateExtPlttTransferTask(void *);
extern void *NARC_New(int, int);
extern void *NARC_AllocAndReadWholeMember(void *, u32, int);
extern void NARC_Delete(void *);
extern void GfGfx_EngineATogglePlanes(u32, int);
extern void GfGfx_EngineBTogglePlanes(u32, int);
void ov51_021E78F8(Work *w);
// @fn ov51_021E78F8
void ov51_021E78F8(Work *w){MemberArr members; VramArr vram; FileArr files; void *narc; int i; u8 j;
GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K); GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
ov51_021E7D68(); NNS_G2dInitOamManagerModule(); OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, 25);
w->renderer = G2dRenderer_Init(33, w->rend, 25); G2dRenderer_SetSubSurfaceCoords(w->rend, 0, FX32_CONST(224));
for (i = 0; i < 4; i++) { w->mgr[0][i] = Create2DGfxResObjMan(ov51_021E7FB8[i], i, 25); w->mgr[1][i] = Create2DGfxResObjMan(ov51_021E7FB8[i], i, 25); }
i = 0; vram = ov51_021E7FBC; files = ov51_021E7FC4;
for (; i < 2; i++) { w->obj[i][0] = AddCharResObjFromNarc(w->mgr[i][0], 49, files.f[i][0], 0, i, vram.v[i], 25); w->obj[i][1] = AddPlttResObjFromNarc(w->mgr[i][1], 49, files.f[i][1], 0, i, vram.v[i], 16, 25); w->obj[i][2] = AddCellOrAnimResObjFromNarc(w->mgr[i][2], 49, files.f[i][2], 0, i, 2, 25); w->obj[i][3] = AddCellOrAnimResObjFromNarc(w->mgr[i][3], 49, files.f[i][3], 0, i, 3, 25); SpriteTransfer_CreateCharTransferTask(w->obj[i][0]); SpriteTransfer_CreateExtPlttTransferTask(w->obj[i][1]); }
members = ov51_021E7FDC; narc = NARC_New(49, 25);
for (j = 0; j < 16; j++) { w->raw[j] = NARC_AllocAndReadWholeMember(narc, members.m[j], 25); if (w->raw[j] != NULL) { if (!NNS_G2dGetUnpackedPaletteData(w->raw[j], &w->pltt[j])) { Heap_Free(w->pltt[j]); GF_AssertFail(); } } else { GF_AssertFail(); } }
NARC_Delete(narc); GfGfx_EngineATogglePlanes(16, 1); GfGfx_EngineBTogglePlanes(16, 1);}
