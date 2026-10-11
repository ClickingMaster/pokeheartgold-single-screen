#include "global.h"

typedef struct W12Work {
    u8 raw[0x1F378];
} W12Work;

#define U8(w, n) (*(u8 *)((w)->raw + (n)))
#define U16(w, n) (*(u16 *)((w)->raw + (n)))
#define U32(w, n) (*(u32 *)((w)->raw + (n)))
#define PTR(w, n) (*(void **)((w)->raw + (n)))

extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void PokewalkerApp_LoadTilemap(W12Work *, int, int);
extern void PokewalkerApp_PrintStringDefaultColor(W12Work *, int, void *, int);
extern int IsPaletteFadeFinished(void);
extern void BufferPokewalkerCourseName(void *, int, u8);
extern int sub_02032718(void *);
extern void *String_New(int, int);
extern void CopyU16ArrayToStringN(void *, void *, int);
extern void BufferString(void *, int, void *, u32, int, void *);
extern void String_Delete(void *);
extern int PokewalkerApp_StartMessageBox(W12Work *, int, int);
extern void PlayFanfare(int);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern void PokewalkerApp_RemoveWindowBank(W12Work *, int);
extern void PokewalkerApp_AddWindowBank(void *, int);
extern void PokewalkerApp_HideSprite(W12Work *, int);
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


void ov112_021F04DC(u8 *a, u8 *b) {
    if (b[9] == 2) {
        ov112_021F0908(a, b);
        ov112_021F06CC(a, b);
        GfGfx_EngineBTogglePlanes(2, 1);
    } else {
        ov112_021F0980(a);
        GfGfx_EngineBTogglePlanes(2, 0);
    }
}

void ov112_021F050C(u8 *a) {
    if (*(void **)(a + 0x78) != NULL) {
        SpriteList_RenderAndAnimateSprites();
    }
}

void ov112_021F051C(u8 *a) {
    u8 i;
    u8 *sub;
    DestroyMsgData(*(void **)(a + 0x68));
    MessageFormat_Delete(*(void **)(a + 0x6c));
    String_Delete(*(void **)(a + 0x70));
    RemoveWindow(a + 8);
    RemoveWindow(a + 0x18);
    RemoveWindow(a + 0x28);
    RemoveWindow(a + 0x38);
    RemoveWindow(a + 0x48);
    RemoveWindow(a + 0x58);
    FreeBgTilemapBuffer(*(void **)(a + 4), 5);
    FreeBgTilemapBuffer(*(void **)(a + 4), 7);
    sub = a + 0x74;
    ov112_021F05CC(sub);
    for (i = 0; i < 2; i++) {
        u8 *entry = sub + 0x18 * i;
        SpriteTransfer_DeleteCharTransferTask(*(void **)(entry + 0x148));
        SpriteTransfer_DeletePlttTransferTask(*(void **)(entry + 0x14c));
    }
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(*(void **)(sub + 0x130 + i * 4));
    }
    SpriteList_Delete(*(void **)(sub + 4));
    Heap_Free(a);
}

void ov112_021F05CC(u8 *a) {
    int i = 0;
    u8 *p = a;
    do {
        Sprite_Delete(*(void **)(p + 0x19c));
        i++;
        p += 4;
    } while (i < 6);
    Sprite_Delete(*(void **)(a + 0x1b4));
    Heap_Free(*(void **)(a + 0x1b8));
}

void ov112_021F05FC(int bg, int heap) {
    ov112_021F0668(bg);
    GfGfxLoader_GXLoadPal(0x103, 0, 4, 0, 0, heap);
    GfGfxLoader_LoadCharData(0x103, 1, bg, 7, 0, 0, 1, heap);
    GfGfxLoader_LoadScrnData(0x103, 2, bg, 7, 0, 0x600, 1, heap);
    BgCommitTilemapBufferToVram(bg, 7);
    LoadFontPal0(4, 0x1e0, heap);
}
