#include "global.h"

typedef struct W12Work {
    u8 raw[0x1F378];
} W12Work;

#define U8(w, n) (*(u8 *)((w)->raw + (n)))
#define U16(w, n) (*(u16 *)((w)->raw + (n)))
#define U32(w, n) (*(u32 *)((w)->raw + (n)))
#define PTR(w, n) (*(void **)((w)->raw + (n)))

extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void ov112_021E7CA4(W12Work *, int, int);
extern void ov112_021E9FD8(W12Work *, int, void *, int);
extern int IsPaletteFadeFinished(void);
extern void BufferPokewalkerCourseName(void *, int, u8);
extern int sub_02032718(void *);
extern void *String_New(int, int);
extern void CopyU16ArrayToStringN(void *, void *, int);
extern void BufferString(void *, int, void *, u32, int, void *);
extern void String_Delete(void *);
extern int ov112_021EA08C(W12Work *, int, int);
extern void PlayFanfare(int);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern void ov112_021E9C10(W12Work *, int);
extern void ov112_021E9A78(void *, int);
extern void ov112_021EA688(W12Work *, int);
extern void GF_AssertFail(void);
extern int TextPrinterCheckActive(u8);
extern int IsFanfarePlaying(void);
extern void *Heap_Alloc(int, u32);
extern void Heap_Free(void *);
extern void GfGfx_EngineBTogglePlanes(int, int);
extern void SpriteList_RenderAndAnimateSprites(void);
extern void DestroyMsgData(void *);
extern void MessageFormat_Delete(void *);
extern void RemoveWindow(void *);
extern void FreeBgTilemapBuffer(void *, int);
extern void SpriteTransfer_DeleteCharTransferTask(void *);
extern void SpriteTransfer_DeletePlttTransferTask(void *);
extern void Destroy2DGfxResObjMan(void *);
extern void SpriteList_Delete(void *);
extern void Sprite_Delete(void *);
extern void GfGfxLoader_GXLoadPal(int, int, int, int, int, int);
extern void GfGfxLoader_LoadCharData(int, int, int, int, int, int, int, int);
extern void GfGfxLoader_LoadScrnData(int, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(int, int);
extern void LoadFontPal0(int, int, int);
extern void ov112_021F0668(int);
extern void ov112_021F06CC(void *, void *);
extern void ov112_021F0C8C(void *);
extern void ov112_021F0D04(void *);
extern void Sprite_SetDrawFlag(void *, int);
extern void Sprite_SetAnimCtrlSeq(void *, int);
extern void ov112_021F0C50(void *);
int ov112_021EF700(W12Work *);
int ov112_021EF8C0(void);
int ov112_021EF8D4(W12Work *);
int ov112_021EF958(W12Work *);
int ov112_021EF994(W12Work *);
int ov112_021EF9D4(W12Work *);
int ov112_021EFA1C(W12Work *);
int ov112_021EFA44(W12Work *);
int ov112_021EFA74(W12Work *);
int ov112_021EFA9C(void);
int ov112_021EFAC0(void);
int ov112_021EFAD4(u8 *, u8 *);
int ov112_021EFC9C(void);
int ov112_021EFCB0(W12Work *);
int ov112_021EFCD8(void);
int ov112_021EFCFC(W12Work *);
void ov112_021EFD24(u16 *, u16);
int ov112_021EFED8(void);
int ov112_021EFEEC(W12Work *);
int ov112_021EFF14(void);
int ov112_021EFF38(void);
int ov112_021EFF4C(void);
int ov112_021EFF70(void);
int ov112_021F00CC(void);
int ov112_021F00E0(W12Work *);
int ov112_021F0114(W12Work *);
int ov112_021F015C(void);
int ov112_021F0180(void);
int ov112_021F0194(void);
int ov112_021F01B8(void);
int ov112_021F0314(void);
int ov112_021F0328(W12Work *);
int ov112_021F0368(W12Work *);
int ov112_021F0394(W12Work *);
void *ov112_021F039C(int);
void ov112_021F04DC(u8 *, u8 *);
void ov112_021F050C(u8 *);
void ov112_021F051C(u8 *);
void ov112_021F05CC(u8 *);
void ov112_021F05FC(int, int);
void ov112_021F0908(u8 *, u8 *);
void ov112_021F0980(u8 *);
void ov112_021F0B28(u8 *);
extern void *ov112_021F0B9C(void *, int, int, int, int, int, int, int);


void ov112_021F0908(u8 *a, u8 *b) {
    u8 i;
    u8 *sub = a + 0x74;
    ov112_021F0C8C(sub);
    ov112_021F0D04(a + 0x74);
    Sprite_SetDrawFlag(*(void **)(a + 0x228), 1);
    for (i = 0; i < 6; i++) {
        if (b[0x20 + i] != 0) {
            u8 *entry = sub + i * 4;
            Sprite_SetDrawFlag(*(void **)(entry + 0x19c), 1);
            Sprite_SetAnimCtrlSeq(*(void **)(entry + 0x19c), i * 2 + 1);
        } else {
            u8 *entry = sub + i * 4;
            Sprite_SetDrawFlag(*(void **)(entry + 0x19c), 1);
            Sprite_SetAnimCtrlSeq(*(void **)(entry + 0x19c), i * 2);
        }
    }
}

void ov112_021F0980(u8 *a) {
    u8 i;
    u8 *sub = a + 0x74;
    for (i = 0; i < 6; i++) {
        Sprite_SetDrawFlag(*(void **)(sub + 0x19c + i * 4), 0);
    }
    Sprite_SetDrawFlag(*(void **)(sub + 0x1b4), 0);
}
