#include "global.h"

#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef struct RawBgTemplate {
    u32 words[7];
} RawBgTemplate;
typedef struct RawModes {
    u32 words[4];
} RawModes;
typedef struct RawSpriteTemplate {
    void *list;
    void *header;
    s32 x, y, z;
    s32 sx, sy, sz;
    u16 rotation;
    u16 pad;
    u32 priority;
    u32 screen;
    u32 heap;
} RawSpriteTemplate;

extern const RawBgTemplate _0223B3A0;
extern const RawModes _0223B390;
extern void *ov74_0223D0A8;

extern void *OverlayManager_GetData(void *);
extern void InitBgFromTemplate(void *, u8, const void *, int);
extern void BgClearTilemapBufferAndCommit(void *, u8);
extern void SetBothScreensModesAndDisable(const void *);
extern void GfGfxLoader_GXLoadPal(int, int, int, int, int, int);
extern void GfGfxLoader_LoadCharData(int, int, void *, int, int, int, int, int);
extern void GfGfxLoader_LoadScrnData(int, int, void *, int, int, int, int, int);
extern void BgTilemapRectChangePalette(void *, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void *String_New(int, int);
extern void CopyU16ArrayToStringN(void *, const u16 *, int);
extern int GetFontAttribute(int, int);
extern void FillWindowPixelBuffer(void *, int);
extern void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, int);
extern void DrawFrameAndWindow1(void *, int, int, int);
extern void String_Delete(void *);
extern int ListMenu_ProcessInput(void *);
extern void PlaySE(int);
extern void ResetAllTextPrinters(void);
extern void LoadFontPal0(int, int, int);
extern int Options_GetFrame(void *);
extern void LoadUserFrameGfx2(void *, int, int, int, int, int);
extern void LoadUserFrameGfx1(void *, int, int, int, int, int);
extern int WindowIsInUse(void *);
extern void AddWindowParameterized(void *, void *, int, int, int, int, int, int);
extern void ov74_0222AA18(void *, void *, int);
extern void ov74_0222A94C(void *, int, int);
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void *MessageFormat_New(int);
extern void *ReadMsgData_ExpandPlaceholders(void *, void *, int, int);
extern void MessageFormat_Delete(void *);
extern void DestroyMsgData(void *);
extern int TextPrinterCheckActive(u8);
extern void ov74_02229DF8(void);
extern void sub_0203A914(void);
extern void *Sprite_CreateAffine(const RawSpriteTemplate *);
extern void Sprite_SetAnimActiveFlag(void *, int);
extern void Sprite_SetPriority(void *, int);
extern void Sprite_SetAnimCtrlSeq(void *, int);
extern void Sprite_SetDrawFlag(void *, int);
extern void ov74_0222CD88(void *);
extern void Heap_Create(int, int, int);
extern void *OverlayManager_CreateAndGetData(void *, int, int);
extern void *memset(void *, int, u32);
extern void *BgConfig_Alloc(int);
extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void ov74_0222FCA4(void);
extern void sub_0200FBF4(int, int);
extern void Sound_SetSceneAndPlayBGM(int, int, int);
extern void ov74_0223512C(int);
extern int ov74_02235230(void);
extern void sub_0201A4B0(int);
extern void ov74_02236034(int);
extern void *OverlayManager_GetArgs(void *);
extern void *Save_PlayerData_GetOptionsAddr(void *);
extern const u8 gGameVersion;

void ov74_0222A6F8(void *bg, u32 id, u32 a, u32 b);
void ov74_0222A744(void *bg);
void ov74_0222A7A0(void *bg);
void ov74_0222A81C(void *bg);
void ov74_0222AAAC(void *manager, void *window, const u16 *text);
void ov74_0222AB0C(void *manager, int *state, int (*cancel)(void *));
void ov74_0222AD6C(void *manager);
int ov74_0222ADBC(void *manager, void *window, int msg, int result);
void ov74_0222AE3C(void *data);
int ov74_0222AE6C(void *manager);
void *ov74_0222B0C8(void *data, int which);

void ov74_0222AD6C(void *manager) {
    void *data = OverlayManager_GetData(manager);
    u32 kind = U32(data, 0x80);
    int msg;
    if (kind == 3) {
        msg = 0x16;
    } else if (kind == 4) {
        msg = 0x1a;
    } else if (kind == 2) {
        msg = 0x17;
    } else if (kind == 1) {
        msg = 0x1c;
    } else if (kind == 5) {
        ov74_0222ADBC(manager, (u8 *)data + 0x18, 9, 0x34);
        return;
    }
    ov74_0222ADBC(manager, (u8 *)data + 0x18, msg, 0x33);
}
int ov74_0222ADBC(void *manager, void *window, int msg, int result) {
    void *data = OverlayManager_GetData(manager);
    if (window != NULL && msg != 0) {
        void *messages = NewMsgDataFromNarc(1, 0x1b, 0xf7, 0x54);
        void *fmt = MessageFormat_New(0x54);
        *(void **)((u8 *)data + 0x14) = ReadMsgData_ExpandPlaceholders(fmt, messages, msg, 0x54);
        U32(data, 0x68) = 1;
        ov74_0222AA18(manager, window, msg);
        U32(data, 0x74) = result;
        MessageFormat_Delete(fmt);
        DestroyMsgData(messages);
    } else if (!TextPrinterCheckActive((u8)U32(data, 0x6c))) {
        String_Delete(*(void **)((u8 *)data + 0x14));
        U32(data, 0x14) = 0;
        U32(data, 0x68) = 0;
        return U32(data, 0x74);
    }
    return 0x32;
}
void ov74_0222AE3C(void *data) {
    switch (U32(data, 0x15d4)) {
    case 1:
        ov74_02229DF8();
        sub_0203A914();
        break;
    case 0:
    case 2:
    case 3:
    case 4:
        break;
    }
}
int ov74_0222AE6C(void *manager) {
    void *data;
    void *args;
    ov74_0222CD88(manager);
    Heap_Create(3, 0x54, 0x30000);
    data = OverlayManager_CreateAndGetData(manager, 0x29f8, 0x54);
    memset(data, 0, 0x29f8);
    *(void **)data = BgConfig_Alloc(0x54);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    ov74_0222FCA4();
    ov74_0222A744(*(void **)data);
    sub_0200FBF4(0, 0);
    sub_0200FBF4(1, 0);
    Sound_SetSceneAndPlayBGM(10, 0x47e, 1);
    ov74_0223512C(0x54);
    if (ov74_02235230()) {
        U32(data, 0x15d4) = 2;
        sub_0201A4B0(gGameVersion);
        ov74_02236034(1);
    }
    U32(data, 0x5c8) = 0x1d;
    args = OverlayManager_GetArgs(manager);
    *(void **)((u8 *)data + 4) = *(void **)((u8 *)args + 8);
    *(void **)((u8 *)data + 8) = Save_PlayerData_GetOptionsAddr(*(void **)((u8 *)data + 4));
    U32(data, 0x68) = 0xff;
    Heap_Create(0, 0x59, 0x570);
    return 1;
}
