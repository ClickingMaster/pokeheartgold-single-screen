#include "global.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))

typedef struct Ov40Entry {
    u8 raw[0xE4];
} Ov40Entry;

typedef struct Ov40Work {
    u8 pad0[0x2608];
    Ov40Entry *ptrs[30];
    Ov40Entry entries[30];
    s32 count;
} Ov40Work;

typedef void (*HitboxCallback)(u32 a, int b, void *p);

void ov40_0222C6C8(void *p, int a);
void GfGfx_EngineATogglePlanes(int plane, BOOL enable);
void InitWindow(void *window);
void AddWindowParameterized(void *bgConfig, void *window, int bgId, int x, int y, int width, int height, int palette, int baseTile);
void *VsRecorder_NewMessageFormat(int heapId);
void *String_New(u32 maxLength, int heapId);
void *FrontierCard_NewNameString(u32 p, int heapId);
void VsRecorder_SanitizeName(void *p, u32 value);
void *NewString_ReadMsgData(void *msgData, int msgId);
void BufferString(void *messageFormat, u32 idx, void *string, u32 a3, u32 a4, u32 a5);
void StringExpandPlaceholders(void *messageFormat, void *dest, void *src);
void String_Delete(void *string);
void MessageFormat_Delete(void *messageFormat);
void FillWindowPixelBuffer(void *window, u8 fillValue);
u8 AddTextPrinterParameterizedWithColor(void *window, int fontId, void *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
void ScheduleWindowCopyToVram(void *window);
void VsRecorder_SetVBlankPaused(void *p, void *value);
void VsRecorder_FreeVideoInfo(void *obj, void *p);
void VsRecorder_FreeProfile(void *obj, void *p);
BOOL VsRecorder_StepBlend(int *a, int *b, int c, int d);
void VsRecorder_CreateProfile(void *obj, void *p);
void VsRecorder_SetProfileDrawFlag(void *obj, int value);
void VsRecorder_LoadScreen(void *p, int value, int bgId);
void VsRecorder_CreateVideoInfo(void *obj, void *p);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *obj, int value);
int ov40_02230410(void *obj);
void GfGfx_EngineBTogglePlanes(int plane, BOOL enable);
void ov40_0223CCBC(void *p, int msgId);
void VsRecorder_SetSubState(void *p, int state);
void BeginNormalPaletteFade(int a0, int a1, int a2, int a3, int a4, int a5, int heapId);
BOOL IsPaletteFadeFinished(void);
void TouchHitboxController_Destroy(void *controller);
void ov40_0223A83C(void *p);
void ov40_0223CCA0(void *p);
void ov40_0223D504(void *p);
void VsRecorder_RebuildMenuRows(void *p);
void VsRecorder_SetResumeScreen(void *p, u32 value);
void sub_02087A84(void *p, int a, int b);
void Heap_Free(void *p);
void VsRecorder_ResetBgPriorities(void *p);
void VsRecorder_InitBgBlend(int *a, int *b, int c);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *narc, int memberNo, void *bgConfig, int layer, int tileStart, int size, int compressed, int heapId);
void VsRecorder_SetTextSpriteMsg(void *dst, void *src, int id);
void ManagedSprite_SetAnim(void *sprite, int anim);
void *TouchHitboxController_Create(const void *hitboxes, int count, HitboxCallback callback, void *arg, int heapId);
void ov40_0223CFA8(void *p);
int VsRecorder_StepFade(int *p, int direction);
void ov40_0223CD14(void *p);
void PaletteData_BlendPalettes(void *palette, int buffer, int mask, int coeff, int color);
void VsRecorder_ClearResumeState(void *p);
void *BattleRecord_GetHeader(void);
u64 BattleRecordHeader_GetField(void *p, int a, int b);
BOOL VsRecorder_IsVideoSaved(void *p, u64 value);
void BgClearTilemapBufferAndCommit(void *bgConfig, int bgId);
void VsRecorder_ShowTopMessage(void *p, int msgId);
BOOL System_GetTouchNew(void);
void VsRecorder_HideTopMessage(void *p);
void ov40_0223B374(void *p);
void ov40_0223B44C(void *p);
void ov40_0223A430(void *p);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *narc, int memberNo, void *bgConfig, int layer, int tileStart, int size, int compressed, int heapId);
void ov40_0223CE64(u32 a, int b, void *p);
void ov40_0223CF00(u32 a, int b, void *p);
void ov40_0223CF70(u32 a, int b, void *p);
extern const u8 ov40_022454E0[];
extern const u8 ov40_022454B0[];
extern const u8 ov40_0224549C[];
int ov40_0223C258(void *p);
int ov40_0223C3A4(void *p);
int ov40_0223C498(Ov40Work *p);
void ov40_0223C710(void *p, int msgId, int unused);
int ov40_0223C80C(Ov40Work *p);

int ov40_0223C258(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x10) == 0) {
            VsRecorder_FreeVideoInfo((u8 *)w + 0x220, p);
        } else {
            VsRecorder_FreeProfile((u8 *)w + 0x194, p);
        }
        GfGfx_EngineATogglePlanes(4, 0);
        VsRecorder_SetVBlankPaused(p, NULL);
        WORD(p, 8)
        ++;
        break;
    case 1:
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 1, 1)) {
            WORD(p, 8)
            ++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x10) == 0) {
            VsRecorder_CreateProfile((u8 *)w + 0x194, p);
            VsRecorder_SetProfileDrawFlag((u8 *)w + 0x194, 0);
            VsRecorder_LoadScreen(p, 0x50, 3);
        } else {
            VsRecorder_CreateVideoInfo((u8 *)w + 0x220, p);
            VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x220, 0);
            VsRecorder_LoadScreen(p, ov40_02230410((u8 *)w + 0x220), 3);
        }
        VsRecorder_SetVBlankPaused(p, NULL);
        WORD(p, 8)
        ++;
        break;
    case 3:
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 1)) {
            if (WORD(w, 0x10) == 0) {
                VsRecorder_SetProfileDrawFlag((u8 *)w + 0x194, 1);
            } else {
                VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x220, 1);
            }
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            WORD(p, 8)
            ++;
        }
        break;
    default:
        WORD(w, 0x10) = 1 ^ WORD(w, 0x10);
        ov40_0223CCBC(p, WORD(w, 0x10) + 0x79);
        VsRecorder_SetSubState(p, 7);
        break;
    }
    return 0;
}

int ov40_0223C3A4(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            WORD(p, 8)
            ++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x10) == 0) {
            VsRecorder_FreeVideoInfo((u8 *)w + 0x220, p);
        } else {
            VsRecorder_FreeProfile((u8 *)w + 0x194, p);
        }
        TouchHitboxController_Destroy(PTR(w, 0x188));
        TouchHitboxController_Destroy(PTR(w, 0x18C));
        TouchHitboxController_Destroy(PTR(w, 0x190));
        ov40_0223A83C(p);
        ov40_0223CCA0(p);
        ov40_0223D504(p);
        VsRecorder_SetVBlankPaused(p, NULL);
        VsRecorder_SetVBlankPaused(p, (void *)1);
        VsRecorder_RebuildMenuRows(p);
        VsRecorder_SetVBlankPaused(p, NULL);
        VsRecorder_SetResumeScreen(p, WORD(PTR(p, 0x10), 0));
        sub_02087A84(PTR(p, 0x868), 1, 0);
        WORD(p, 8)
        ++;
        break;
    default:
        Heap_Free(w);
        return 1;
    }
    return 0;
}

int ov40_0223C498(Ov40Work *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0: {
        int i;
        VsRecorder_ResetBgPriorities(p);
        VsRecorder_InitBgBlend(w, (int *)w + 1, 0);
        for (i = 0; i < p->count; i++) {
            p->ptrs[i] = &p->entries[i];
        }
        GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
        GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
        VsRecorder_LoadScreen(p, 6, 7);
        GfGfx_EngineATogglePlanes(8, 1);
        GfGfx_EngineBTogglePlanes(8, 1);
        VsRecorder_SetTextSpriteMsg((u8 *)w + 0x114, (u8 *)p + 0x14, 3);
        VsRecorder_SetTextSpriteMsg((u8 *)w + 0x130, (u8 *)p + 0x14, 0x5E);
        ManagedSprite_SetAnim(PTR(w, 0x118), 0);
        ManagedSprite_SetAnim(PTR(w, 0x134), 3);
        WORD(w, 0x2088) = 0x1F4;
        WORD(w, 0x2084) = *(s16 *)((u8 *)p + 0x4A4);
        WORD(p, 8)
        ++;
        break;
    }
    case 1:
        ov40_0223CFA8(p);
        PTR(w, 0x188) = TouchHitboxController_Create(ov40_022454E0, 4, ov40_0223CE64, p, 0x6D);
        PTR(w, 0x18C) = TouchHitboxController_Create(ov40_022454B0, 4, ov40_0223CF00, p, 0x6D);
        PTR(w, 0x190) = TouchHitboxController_Create(ov40_0224549C, 2, ov40_0223CF70, p, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 2:
        VsRecorder_SetVBlankPaused(p, (void *)1);
        VsRecorder_CreateVideoInfo((u8 *)w + 0x220, p);
        VsRecorder_LoadScreen(p, ov40_02230410((u8 *)w + 0x220), 3);
        VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x220, 0);
        VsRecorder_SetVBlankPaused(p, NULL);
        GfGfx_EngineATogglePlanes(8, 1);
        GfGfx_EngineBTogglePlanes(8, 1);
        GfGfx_EngineATogglePlanes(4, 0);
        GfGfx_EngineBTogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        break;
    case 3:
        VsRecorder_StepFade((int *)w + 2, 0);
        if (VsRecorder_StepBlend(w, (int *)w + 1, 0, 0)) {
            VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x220, 1);
            ov40_0223CD14(p);
            GfGfx_EngineATogglePlanes(4, 1);
            GfGfx_EngineBTogglePlanes(4, 1);
            WORD(p, 8)
            ++;
        }
        PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 8), (u16)WORD(p, 0x58));
        break;
    case 4:
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 5:
        if (IsPaletteFadeFinished() == TRUE) {
            WORD(p, 8)
            ++;
        }
        break;
    default:
        WORD(p, 0x6D8)
        ++;
        VsRecorder_ClearResumeState(p);
        VsRecorder_SetSubState(p, 7);
        break;
    }
    return 0;
}

void ov40_0223C710(void *p, int msgId, int unused) {
    void *win = (u8 *)p + 0x8A4;
    void *w = PTR(p, 0x860);
    void *str;
    (void)unused;
    ov40_0222C6C8(p, 2);
    GfGfx_EngineATogglePlanes(4, 1);
    InitWindow(win);
    AddWindowParameterized(PTR(p, 0x24), win, 2, 1, 0x13, 0x1E, 4, 0xE, 0x12C);
    if (msgId == 0x64) {
        u32 v = WORD(w, 0x194);
        void *fmt = VsRecorder_NewMessageFormat(0x6D);
        void *name;
        void *s;
        str = String_New(0xFF, 0x6D);
        s = FrontierCard_NewNameString(v, 0x6D);
        VsRecorder_SanitizeName(p, (u32)s);
        name = NewString_ReadMsgData(PTR(p, 0x48), msgId);
        BufferString(fmt, 0, s, 0, 1, 2);
        StringExpandPlaceholders(fmt, str, name);
        String_Delete(s);
        String_Delete(name);
        MessageFormat_Delete(fmt);
    } else {
        str = NewString_ReadMsgData(PTR(p, 0x48), msgId);
    }
    FillWindowPixelBuffer(win, 0xCC);
    AddTextPrinterParameterizedWithColor(win, 0, str, 0, 0, 0xFF, 0xF0D0C, NULL);
    ScheduleWindowCopyToVram(win);
    String_Delete(str);
}

int ov40_0223C80C(Ov40Work *p) {
    void *w = PTR(p, 0x860);
    if (VsRecorder_IsVideoSaved(p, BattleRecordHeader_GetField(BattleRecord_GetHeader(), 4, 0))) {
        switch (WORD(p, 8)) {
        case 0:
            VsRecorder_SetVBlankPaused(p, (void *)1);
            if (WORD(w, 0x10) == 0) {
                VsRecorder_FreeVideoInfo((u8 *)w + 0x220, p);
            } else {
                VsRecorder_FreeProfile((u8 *)w + 0x194, p);
            }
            ov40_0223CCA0(p);
            VsRecorder_SetVBlankPaused(p, NULL);
            VsRecorder_SetVBlankPaused(p, (void *)1);
            ov40_0223D504(p);
            VsRecorder_SetVBlankPaused(p, NULL);
            GfGfx_EngineATogglePlanes(4, 0);
            GfGfx_EngineBTogglePlanes(4, 0);
            WORD(p, 8)
            ++;
            break;
        case 1:
            VsRecorder_StepFade((int *)w + 2, 1);
            if (VsRecorder_StepBlend(w, (int *)w + 1, 1, 0)) {
                VsRecorder_SetVBlankPaused(p, (void *)1);
                ov40_0223B374(p);
                VsRecorder_SetVBlankPaused(p, NULL);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
                WORD(p, 8)
                ++;
            }
            PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 8), (u16)WORD(p, 0x58));
            break;
        case 2:
            VsRecorder_ShowTopMessage(p, 0x115);
            WORD(p, 8)
            ++;
            break;
        case 3:
            if (System_GetTouchNew()) {
                VsRecorder_HideTopMessage(p);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
                GfGfx_EngineBTogglePlanes(4, 0);
                GfGfx_EngineATogglePlanes(4, 0);
                WORD(p, 8)
                ++;
            }
            break;
        case 4:
            VsRecorder_StepFade((int *)w + 2, 1);
            if (VsRecorder_StepBlend(w, (int *)w + 1, 1, 0)) {
                VsRecorder_SetVBlankPaused(p, (void *)1);
                ov40_0223B44C(p);
                VsRecorder_SetVBlankPaused(p, NULL);
                WORD(p, 8)
                ++;
            }
            PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 8), (u16)WORD(p, 0x58));
            break;
        case 5:
            VsRecorder_SetVBlankPaused(p, (void *)1);
            ov40_0223A430(p);
            VsRecorder_SetVBlankPaused(p, NULL);
            if (WORD(p, 0x86C) == 0xD2) {
                VsRecorder_SetTextSpriteMsg((u8 *)w + 0x114, (u8 *)p + 0x14, 3);
                VsRecorder_SetTextSpriteMsg((u8 *)w + 0x130, (u8 *)p + 0x14, 0x5E);
                ManagedSprite_SetAnim(PTR(w, 0x118), 0);
                ManagedSprite_SetAnim(PTR(w, 0x134), 3);
            }
            ov40_0223CD14(p);
            GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
            GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
            VsRecorder_SetVBlankPaused(p, (void *)1);
            if (WORD(w, 0x10) == 0) {
                VsRecorder_CreateVideoInfo((u8 *)w + 0x220, p);
                VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x220, 0);
                VsRecorder_LoadScreen(p, ov40_02230410((u8 *)w + 0x220), 3);
            } else {
                VsRecorder_CreateProfile((u8 *)w + 0x194, p);
                VsRecorder_SetProfileDrawFlag((u8 *)w + 0x194, 0);
                GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 0x50, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
            }
            VsRecorder_SetVBlankPaused(p, NULL);
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 6, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
            WORD(p, 8)
            ++;
            break;
        case 6:
            VsRecorder_StepFade((int *)w + 2, 0);
            if (VsRecorder_StepBlend(w, (int *)w + 1, 0, 0)) {
                if (WORD(w, 0x10) == 0) {
                    VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x220, 1);
                } else {
                    VsRecorder_SetProfileDrawFlag((u8 *)w + 0x194, 1);
                }
                GfGfx_EngineBTogglePlanes(4, 1);
                GfGfx_EngineATogglePlanes(4, 1);
                WORD(p, 8)
                ++;
            }
            PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 8), (u16)WORD(p, 0x58));
            break;
        default: {
            int i;
            for (i = 0; i < p->count; i++) {
                p->ptrs[i] = &p->entries[i];
            }
            VsRecorder_SetSubState(p, 7);
            break;
        }
        }
    } else {
        switch (WORD(p, 8)) {
        case 0:
            VsRecorder_SetVBlankPaused(p, (void *)1);
            if (WORD(w, 0x10) == 0) {
                VsRecorder_FreeVideoInfo((u8 *)w + 0x220, p);
            } else {
                VsRecorder_FreeProfile((u8 *)w + 0x194, p);
            }
            VsRecorder_SetVBlankPaused(p, NULL);
            VsRecorder_SetVBlankPaused(p, (void *)1);
            ov40_0223D504(p);
            VsRecorder_SetVBlankPaused(p, NULL);
            ov40_0223CCA0(p);
            GfGfx_EngineATogglePlanes(4, 0);
            GfGfx_EngineBTogglePlanes(4, 0);
            WORD(p, 8)
            ++;
            break;
        case 1:
            VsRecorder_StepFade((int *)w + 2, 1);
            if (VsRecorder_StepBlend(w, (int *)w + 1, 1, 0)) {
                VsRecorder_SetVBlankPaused(p, (void *)1);
                ov40_0223B374(p);
                VsRecorder_SetVBlankPaused(p, NULL);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
                WORD(p, 8)
                ++;
            }
            PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 8), (u16)WORD(p, 0x58));
            break;
        case 2:
            VsRecorder_StepFade((int *)w + 2, 0);
            if (VsRecorder_StepBlend(w, (int *)w + 1, 0, 1)) {
                ov40_0223C710(p, 0x64, 0);
                WORD(p, 8)
                ++;
            }
            PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 8), (u16)WORD(p, 0x58));
            break;
        default:
            VsRecorder_SetSubState(p, 0xC);
            break;
        }
    }
    return 0;
}
