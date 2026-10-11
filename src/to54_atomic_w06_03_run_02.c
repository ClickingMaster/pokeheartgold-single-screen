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
extern void WirelessStatusIcon_Destroy(void);
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
extern void SetMasterBrightnessByColor(int, int);
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

void ov74_0222AAAC(void *manager, void *window, const u16 *text) {
    void *str;
    OverlayManager_GetData(manager);
    str = String_New(0x25, 0x54);
    CopyU16ArrayToStringN(str, text, 0x24);
    FillWindowPixelBuffer(window, GetFontAttribute(0, 6));
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, 0xff, 0x1020f, 0);
    DrawFrameAndWindow1(window, 0, 0x1f, 3);
    String_Delete(str);
}
void ov74_0222AB0C(void *manager, int *state, int (*cancel)(void *)) {
    void *data = OverlayManager_GetData(manager);
    u32 choice = ListMenu_ProcessInput(*(void **)((u8 *)data + 0x78));
    int result;
    switch (choice) {
    case (u32)-2:
        PlaySE(0x5dc);
        if (cancel != NULL) {
            result = cancel(manager);
            if (result != -1) {
                *state = result;
                return;
            }
        }
        break;
    case (u32)-1:
        break;
    default:
        PlaySE(0x5dc);
        if (choice != 0) {
            ((void **)&ov74_0223D0A8)[2] = (void *)choice;
            result = ((int (*)(void *))choice)(manager);
            if (result != -1) {
                *state = result;
                return;
            }
        }
        break;
    }
}
