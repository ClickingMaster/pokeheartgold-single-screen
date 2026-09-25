#include "global.h"

typedef struct {
    u32 words[4];
} TransferTemplate;

extern TransferTemplate _0223B4E8;
extern void ObjCharTransfer_Init(void *);
extern void ObjPlttTransfer_Init(int, int);
extern void ObjCharTransfer_ClearBuffers(void);
extern void ObjPlttTransfer_Reset(void);
extern void CreateSpriteResourcesHeader(void *, int, int, int, int, int, int, int, int, void *, void *, void *, void *, void *, void *);
extern void GfGfx_EngineATogglePlanes(int, int);
extern void NNS_G2dInitOamManagerModule(void);
extern void OamManager_Create(int, int, int, int, int, int, int, int, int);
extern void *G2dRenderer_Init(int, void *, int);
extern void G2dRenderer_SetSubSurfaceCoords(void *, int, int);
extern void *Create2DGfxResObjMan(int, int, int);
extern void *AddCharResObjFromNarc(void *, int, int, int, int, int, int);
extern void *AddPlttResObjFromNarc(void *, int, int, int, int, int, int, int);
extern void *AddCellOrAnimResObjFromNarc(void *, int, int, int, int, int, int);
extern void SpriteTransfer_CreateCharTransferTask(void *);
extern void SpriteTransfer_CreateExtPlttTransferTask(void *);
extern int sub_02074490(void);
extern void GfGfxLoader_GXLoadPal(int, int, int, int, int, int);
extern void AddWindowParameterized(void *, void *, int, int, int, int, int, int, int);
extern void FillWindowPixelBuffer(void *, int);
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void *MessageFormat_New(int);
extern void MessageFormat_Delete(void *);
extern void DestroyMsgData(void *);
extern void String_SetEmpty(void *);
extern void *NewString_ReadMsgData(void *, int);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void String_Delete(void *);
extern int ov74_022319F8(void *, void *, int);
extern int AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
extern void CopyU16ArrayToStringN(void *, void *, int);
extern void CopyWindowToVram(void *);
extern void DrawFrameAndWindow1(void *, int, int, int);
extern void DrawFrameAndWindow2(void *, int, int, int);
void ov74_02231A1C(u8 *, u8 *, int);
void ov74_02231BC0(void);
void ov74_02231BF0(u8 *work);
void ov74_02231CFC(u8 *work);

void ov74_02231BC0(void) {
    TransferTemplate template = _0223B4E8;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, 76);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

#define WORD(p, o) (*(u32 *)((p) + (o)))
#define PTR(p, o) (*(void **)((p) + (o)))



void ov74_02231BF0(u8 *work) {
    int i;
    u8 *cursor;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, 76);
    *(void **)(work + 0x28) = G2dRenderer_Init(80, work + 0x2c, 76);
    G2dRenderer_SetSubSurfaceCoords(work + 0x2c, 0, 0x100000);
    for (i = 0, cursor = work; i < 6; i++, cursor += 4) {
        *(void **)(cursor + 0x154) = Create2DGfxResObjMan(3, i, 76);
    }
    *(void **)(work + 0x16c) = AddCharResObjFromNarc(*(void **)(work + 0x154), 0x71, 0x1e, 1, 0, 1, 76);
    *(void **)(work + 0x170) = AddPlttResObjFromNarc(*(void **)(work + 0x158), 0x71, 0x1b, 0, 0, 1, 4, 76);
    *(void **)(work + 0x174) = AddCellOrAnimResObjFromNarc(*(void **)(work + 0x15c), 0x71, 0x1d, 1, 0, 2, 76);
    *(void **)(work + 0x178) = AddCellOrAnimResObjFromNarc(*(void **)(work + 0x160), 0x71, 0x1c, 1, 0, 3, 76);
    SpriteTransfer_CreateCharTransferTask(*(void **)(work + 0x16c));
    SpriteTransfer_CreateExtPlttTransferTask(*(void **)(work + 0x170));
    GfGfxLoader_GXLoadPal(20, sub_02074490(), 1, 256, 0, 76);
}

void ov74_02231CFC(u8 *work) {
    CreateSpriteResourcesHeader(work + 0x184, 0, 0, 0, 0, -1, -1, 0, 0,
                                *(void **)(work + 0x154), *(void **)(work + 0x158),
                                *(void **)(work + 0x15c), *(void **)(work + 0x160),
                                0, 0);
    GfGfx_EngineATogglePlanes(0x10, 1);
}
