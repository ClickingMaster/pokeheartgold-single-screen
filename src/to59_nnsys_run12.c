#include <nitro/code32.h>
#include <nitro/types.h>

extern void GXS_LoadBG0Char(const void *, u32, u32);
extern void GXS_LoadBG0Scr(const void *, u32, u32);
extern void GXS_LoadBG1Char(const void *, u32, u32);
extern void GXS_LoadBG1Scr(const void *, u32, u32);
extern void GXS_LoadBG2Char(const void *, u32, u32);
extern void GXS_LoadBG2Scr(const void *, u32, u32);
extern void GXS_LoadBG3Char(const void *, u32, u32);
extern void GXS_LoadBG3Scr(const void *, u32, u32);
extern void GXS_LoadBGPltt(const void *, u32, u32);
extern void GXS_LoadOAM(const void *, u32, u32);
extern void GXS_LoadOBJ(const void *, u32, u32);
extern void GXS_LoadOBJPltt(const void *, u32, u32);
extern void GX_LoadBG0Char(const void *, u32, u32);
extern void GX_LoadBG0Scr(const void *, u32, u32);
extern void GX_LoadBG1Char(const void *, u32, u32);
extern void GX_LoadBG1Scr(const void *, u32, u32);
extern void GX_LoadBG2Char(const void *, u32, u32);
extern void GX_LoadBG2Scr(const void *, u32, u32);
extern void GX_LoadBG3Char(const void *, u32, u32);
extern void GX_LoadBG3Scr(const void *, u32, u32);
extern void GX_LoadBGPltt(const void *, u32, u32);
extern void GX_LoadOAM(const void *, u32, u32);
extern void GX_LoadOBJ(const void *, u32, u32);
extern void GX_LoadOBJPltt(const void *, u32, u32);
extern void GX_BeginLoadTex(void);
extern void GX_LoadTex(const void *, u32, u32);
extern void GX_EndLoadTex(void);
extern void GX_BeginLoadTexPltt(void);
extern void GX_LoadTexPltt(const void *, u32, u32);
extern void GX_EndLoadTexPltt(void);
extern void GX_BeginLoadOBJExtPltt(void);
extern void GX_LoadOBJExtPltt(const void *, u32, u32);
extern void GX_EndLoadOBJExtPltt(void);
extern void GX_BeginLoadBGExtPltt(void);
extern void GX_LoadBGExtPltt(const void *, u32, u32);
extern void GX_EndLoadBGExtPltt(void);
extern void GXS_BeginLoadOBJExtPltt(void);
extern void GXS_LoadOBJExtPltt(const void *, u32, u32);
extern void GXS_EndLoadOBJExtPltt(void);
extern void GXS_BeginLoadBGExtPltt(void);
extern void GXS_LoadBGExtPltt(const void *, u32, u32);
extern void GXS_EndLoadBGExtPltt(void);
extern void GX_BeginLoadClearImage(void);
extern void GX_EndLoadClearImage(void);
extern void GX_LoadClearImageColor(const void *, u32);
extern void GX_LoadClearImageDepth(const void *, u32);
extern u32 _021D84C4[];
extern u32 NNS_GfdAllocFrmPlttVram(u32, BOOL, u32);
extern u32 (*NNS_GfdDefaultFuncAllocPlttVram)(u32, BOOL, u32);
extern u32 (*NNS_GfdDefaultFuncFreePlttVram)(u32);
void NNS_GfdResetFrmPlttVramState(void);
u32 NNS_GfdFreeFrmPlttVram(u32);
typedef struct {
    u32 ptr;
    u32 capacity;
    u16 front;
    u16 rear;
    u16 count;
} Queue;

void NNS_GfdInitFrmPlttVramManager(u32 size, BOOL useDefault);
u32 NNS_GfdFreeFrmPlttVram(u32 key);
void NNS_GfdGetFrmPlttVramState(u32 *p);
void NNS_GfdSetFrmPlttVramState(const u32 *p);
void NNS_GfdResetFrmPlttVramState(void);
u16 GetNextIndex_(Queue *p, u32 n);
BOOL IsVramTransferTaskQueueFull_(Queue *p);
BOOL IsVramTransferTaskQueueEmpty_(Queue *p);
void DoTransfer3dTex(const void *, u32, u32);
void DoTransfer3dTexPltt(const void *, u32, u32);
void DoTransfer3dClearImageColor(const void *, u32, u32);
void DoTransfer3dClearImageDepth(const void *, u32, u32);
void DoTransfer2dBG0CharMain(const void *, u32, u32);
void DoTransfer2dBG1CharMain(const void *, u32, u32);
void DoTransfer2dBG2CharMain(const void *, u32, u32);
void DoTransfer2dBG3CharMain(const void *, u32, u32);
void DoTransfer2dBG0ScrMain(const void *, u32, u32);
void DoTransfer2dBG1ScrMain(const void *, u32, u32);
void DoTransfer2dBG2ScrMain(const void *, u32, u32);
void DoTransfer2dBG3ScrMain(const void *, u32, u32);
void DoTransfer2dBG2BmpMain(const void *, u32, u32);
void DoTransfer2dBG3BmpMain(const void *, u32, u32);
void DoTransfer2dObjPlttMain(const void *, u32, u32);
void DoTransfer2dBGPlttMain(const void *, u32, u32);
void DoTransfer2dObjExtPlttMain(const void *, u32, u32);
void DoTransfer2dBGExtPlttMain(const void *, u32, u32);
void DoTransfer2dObjOamMain(const void *, u32, u32);
void DoTransfer2dObjCharMain(const void *, u32, u32);
void DoTransfer2dBG0CharSub(const void *, u32, u32);
void DoTransfer2dBG1CharSub(const void *, u32, u32);
void DoTransfer2dBG2CharSub(const void *, u32, u32);
void DoTransfer2dBG3CharSub(const void *, u32, u32);
void DoTransfer2dBG0ScrSub(const void *, u32, u32);
void DoTransfer2dBG1ScrSub(const void *, u32, u32);
void DoTransfer2dBG2ScrSub(const void *, u32, u32);
void DoTransfer2dBG3ScrSub(const void *, u32, u32);
void DoTransfer2dBG2BmpSub(const void *, u32, u32);
void DoTransfer2dBG3BmpSub(const void *, u32, u32);
void DoTransfer2dObjPlttSub(const void *, u32, u32);
void DoTransfer2dBGPlttSub(const void *, u32, u32);
void DoTransfer2dObjExtPlttSub(const void *, u32, u32);
void DoTransfer2dBGExtPlttSub(const void *, u32, u32);
void DoTransfer2dObjOamSub(const void *, u32, u32);
void DoTransfer2dObjCharSub(const void *, u32, u32);

u32 NNS_GfdFreeFrmPlttVram(u32 key) {
    return 0;
}

void NNS_GfdGetFrmPlttVramState(u32 *p) {
    p[0] = _021D84C4[0];
    p[1] = _021D84C4[1];
}

void NNS_GfdSetFrmPlttVramState(const u32 *p) {
    _021D84C4[0] = p[0];
    _021D84C4[1] = p[1];
}

void NNS_GfdResetFrmPlttVramState(void) {
    _021D84C4[0] = 0;
    _021D84C4[1] = _021D84C4[2];
}

u16 GetNextIndex_(Queue *p, u32 n) {
    return (n + 1) % p->capacity;
}

BOOL IsVramTransferTaskQueueFull_(Queue *p) {
    return p->count == p->capacity;
}

BOOL IsVramTransferTaskQueueEmpty_(Queue *p) {
    return p->count == 0;
}

void DoTransfer3dTex(const void *src, u32 dst, u32 size) {
    GX_BeginLoadTex();
    GX_LoadTex(src, dst, size);
    GX_EndLoadTex();
}

void DoTransfer3dTexPltt(const void *src, u32 dst, u32 size) {
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(src, dst, size);
    GX_EndLoadTexPltt();
}

void DoTransfer3dClearImageColor(const void *src, u32 dst, u32 size) {
    GX_BeginLoadClearImage();
    GX_LoadClearImageColor(src, size);
    GX_EndLoadClearImage();
}

void DoTransfer3dClearImageDepth(const void *src, u32 dst, u32 size) {
    GX_BeginLoadClearImage();
    GX_LoadClearImageDepth(src, size);
    GX_EndLoadClearImage();
}

void DoTransfer2dBG0CharMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG0Char(src, dst, size);
}

void DoTransfer2dBG1CharMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG1Char(src, dst, size);
}

void DoTransfer2dBG2CharMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG2Char(src, dst, size);
}

void DoTransfer2dBG3CharMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG3Char(src, dst, size);
}

void DoTransfer2dBG0ScrMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG0Scr(src, dst, size);
}

void DoTransfer2dBG1ScrMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG1Scr(src, dst, size);
}

void DoTransfer2dBG2ScrMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG2Scr(src, dst, size);
}

void DoTransfer2dBG3ScrMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG3Scr(src, dst, size);
}

void DoTransfer2dBG2BmpMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG2Scr(src, dst, size);
}

void DoTransfer2dBG3BmpMain(const void *src, u32 dst, u32 size) {
    GX_LoadBG3Scr(src, dst, size);
}

void DoTransfer2dObjPlttMain(const void *src, u32 dst, u32 size) {
    GX_LoadOBJPltt(src, dst, size);
}

void DoTransfer2dBGPlttMain(const void *src, u32 dst, u32 size) {
    GX_LoadBGPltt(src, dst, size);
}

void DoTransfer2dObjExtPlttMain(const void *src, u32 dst, u32 size) {
    GX_BeginLoadOBJExtPltt();
    GX_LoadOBJExtPltt(src, dst, size);
    GX_EndLoadOBJExtPltt();
}

void DoTransfer2dBGExtPlttMain(const void *src, u32 dst, u32 size) {
    GX_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(src, dst, size);
    GX_EndLoadBGExtPltt();
}

void DoTransfer2dObjOamMain(const void *src, u32 dst, u32 size) {
    GX_LoadOAM(src, dst, size);
}

void DoTransfer2dObjCharMain(const void *src, u32 dst, u32 size) {
    GX_LoadOBJ(src, dst, size);
}

void DoTransfer2dBG0CharSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG0Char(src, dst, size);
}

void DoTransfer2dBG1CharSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG1Char(src, dst, size);
}

void DoTransfer2dBG2CharSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG2Char(src, dst, size);
}

void DoTransfer2dBG3CharSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG3Char(src, dst, size);
}

void DoTransfer2dBG0ScrSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG0Scr(src, dst, size);
}

void DoTransfer2dBG1ScrSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG1Scr(src, dst, size);
}

void DoTransfer2dBG2ScrSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG2Scr(src, dst, size);
}

void DoTransfer2dBG3ScrSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG3Scr(src, dst, size);
}

void DoTransfer2dBG2BmpSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG2Scr(src, dst, size);
}

void DoTransfer2dBG3BmpSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBG3Scr(src, dst, size);
}

void DoTransfer2dObjPlttSub(const void *src, u32 dst, u32 size) {
    GXS_LoadOBJPltt(src, dst, size);
}

void DoTransfer2dBGPlttSub(const void *src, u32 dst, u32 size) {
    GXS_LoadBGPltt(src, dst, size);
}

void DoTransfer2dObjExtPlttSub(const void *src, u32 dst, u32 size) {
    GXS_BeginLoadOBJExtPltt();
    GXS_LoadOBJExtPltt(src, dst, size);
    GXS_EndLoadOBJExtPltt();
}

void DoTransfer2dBGExtPlttSub(const void *src, u32 dst, u32 size) {
    GXS_BeginLoadBGExtPltt();
    GXS_LoadBGExtPltt(src, dst, size);
    GXS_EndLoadBGExtPltt();
}

void DoTransfer2dObjOamSub(const void *src, u32 dst, u32 size) {
    GXS_LoadOAM(src, dst, size);
}

void DoTransfer2dObjCharSub(const void *src, u32 dst, u32 size) {
    GXS_LoadOBJ(src, dst, size);
}
