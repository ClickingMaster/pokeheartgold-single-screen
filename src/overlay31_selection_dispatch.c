#include "global.h"

typedef struct Overlay31State {
    u8 data[0x300];
} Overlay31State;

extern void *MessageFormat_New_Custom(int, int, int);
extern void MessageFormat_Delete(void *);
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void DestroyMsgData(void *);
extern void *NewString_ReadMsgData(void *, int);
extern void *String_New(int, int);
extern void String_Delete(void *);
extern void *SysTask_GetData(void *);
extern void TextFlags_SetCanTouchSpeedUpPrint(int);
extern void DestroySysTaskAndEnvironment(void *);
extern void FreeBgTilemapBuffer(void *, int);
extern void Heap_Destroy(int);
extern void RemoveWindow(void *);
extern void ClearWindowTilemapAndCopyToVram(void *);
extern void ScheduleWindowCopyToVram(void *);
extern void PlaySE(int);
extern int YesNoPrompt_HandleInputForSave(void *);
extern void YesNoPrompt_Destroy(void *);
extern void BufferItemName(void *, int);
extern void BufferItemNamePlural(void *, int);
extern void BufferItemNameWithIndefArticle(void *, int);
extern void BufferDecorationName(void *, int);
extern void GF_AssertFail(void);
extern int TouchscreenHitbox_FindRectAtTouchNew(const void *);
extern int AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
extern void FillWindowPixelBuffer(void *, int);
extern void ClearWindowTilemapAndScheduleTransfer(void *);
extern void ClearFrameAndWindow2(void *, int);
extern void *GfGfxLoader_GetScrnData(int, int, int, void **, int);
extern void BG_LoadScreenTilemapData(void *, int, void *, int);
extern void *ScheduleBgTilemapBufferTransfer(void *, int);
extern void Heap_Free(void *);
extern void ov31_0225E060(void *, int, int);
extern int PokeathlonSave_GetUnkB7C_AtIndex(void *, int);
extern int PokeathlonSave_GetUnkB78_AtIndex(void *, int);
extern int PokeathlonSave_GetAthletePoints(void *);
extern int PlayerProfile_GetMoney(void *);
extern int ov03_02257814(void *, int);
extern int IsPaletteFadeFinished(void);
extern void ov31_0225D83C(Overlay31State *, int);
extern void AddWindowParameterized(void *, void *, int, int, int, int, int, int);
extern int FontID_String_GetWidth(int, void *, int);
typedef struct Overlay31WindowTemplate {
    u32 x, y, base;
} Overlay31WindowTemplate;
extern const Overlay31WindowTemplate ov31_0225EE88[6];
extern const void *_0225EF40[];

void ov31_0225DBA0(Overlay31State *state);
void ov31_0225DCF4(Overlay31State *state);
void ov31_0225D60C(Overlay31State *state);
void ov31_0225D654(Overlay31State *state);
void ov31_0225D710(void *bgConfig, void *task);
int ov31_0225D758(void);
int ov31_0225D75C(Overlay31State *state, u32 selection);
int ov31_0225DAA4(Overlay31State *state, int which);
void ov31_0225E4BC(int kind, void *format, int unused, int value);
void ov31_0225E4EC(int kind, void *format, int unused, int value);
void ov31_0225E51C(int kind, void *format, int unused, int value);
int ov31_0225E774(Overlay31State *state);
int ov31_0225E948(void *unused, int printerState);
void *ov31_0225E9CC(const u8 *args, void *msgData);
void ov31_0225DE00(Overlay31State *state, void *window, void *string, int y);
void ov31_0225E0E4(Overlay31State *state, int screen);
int ov31_0225E12C(u8 *args, int index, int value);
void ov31_0225E474(Overlay31State *state);
void ov31_0225EDA0(Overlay31State *state);
void *ov31_0225E95C(u8 *args, void *msgData);
void ov31_0225D60C(Overlay31State *state);
void ov31_0225D654(Overlay31State *state);

extern void GfGfx_EngineBTogglePlanes(int, int);
extern void BgFillTilemapBufferAndSchedule(void *, int, int);
extern void ov31_0225D684(Overlay31State *, int);
extern void ov31_0225DD14(Overlay31State *);
extern void ov31_0225DF98(Overlay31State *);
extern void ov31_0225DCA8(Overlay31State *);
extern void ov31_0225DE84(Overlay31State *);
extern void ov31_0225EC58(Overlay31State *);
extern void ov31_0225E184(Overlay31State *);
extern void ov31_0225E20C(Overlay31State *, int);
extern void ov31_0225E2D4(Overlay31State *, int);
extern void ov31_0225E54C(Overlay31State *);
extern void ov31_0225E5FC(Overlay31State *);
extern void ov31_0225E700(Overlay31State *);
extern void ov31_0225E7D4(Overlay31State *);
extern void ov31_0225EA08(Overlay31State *);
extern void ov31_0225EA9C(Overlay31State *);
extern void ov31_0225EB30(Overlay31State *);
extern void ov31_0225EBC4(Overlay31State *);
void ov31_0225D7A0(void *, Overlay31State *);
void ov31_0225D9D4(Overlay31State *, int);

void ov31_0225D7A0(void *task, Overlay31State *state) {
    int selection;
    switch (*(u32 *)&state->data[0xC]) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            *(u32 *)&state->data[0xC] = 1;
        }
        return;
    case 1:
        switch ((*(u8 **)&state->data[0x14])[0x272]) {
        case 3:
            selection = ov31_0225DAA4(state, 0);
            if (selection != -1 && ov31_0225D75C(state, selection)) {
                *(u32 *)(*(u8 **)&state->data[0x14] + 0x294) = selection;
            }
            break;
        case 7:
            selection = ov31_0225DAA4(state, 1);
            if (selection != -1) {
                *(u32 *)(*(u8 **)&state->data[0x14] + 0x294) = selection;
            }
            break;
        case 11:
            ov31_0225E774(state);
            break;
        }
        if (*(u32 *)(*(u8 **)&state->data[0x14] + 0x298) != 0) {
            ov31_0225D83C(state, *(u32 *)(*(u8 **)&state->data[0x14] + 0x298));
            *(u32 *)(*(u8 **)&state->data[0x14] + 0x298) = 0;
        }
        return;
    }
}

void ov31_0225D83C(Overlay31State *state, int action) {
    switch (action) {
    case 0:
        break;
    case 1:
        ov31_0225DD14(state);
        ov31_0225DF98(state);
        ov31_0225D684(state, 0);
        break;
    case 2:
        ov31_0225D9D4(state, 1);
        ov31_0225DCF4(state);
        ClearWindowTilemapAndScheduleTransfer(&state->data[0x54]);
        ov31_0225EC58(state);
        ov31_0225E184(state);
        ov31_0225E20C(state, *(u16 *)(*(u8 **)&state->data[0x14] + 0x284));
        ov31_0225E2D4(state, *(s16 *)(*(u8 **)&state->data[0x14] + 0x286));
        ov31_0225E474(state);
        ov31_0225D684(state, 1);
        ov31_0225E54C(state);
        break;
    case 3:
        ov31_0225D9D4(state, 2);
        ov31_0225DCF4(state);
        ov31_0225EC58(state);
        ov31_0225E184(state);
        ov31_0225E20C(state, *(u16 *)(*(u8 **)&state->data[0x14] + 0x284));
        ov31_0225E5FC(state);
        ClearWindowTilemapAndScheduleTransfer(&state->data[0x54]);
        ClearWindowTilemapAndScheduleTransfer(&state->data[0x64]);
        break;
    case 4:
        ov31_0225D9D4(state, 0);
        ov31_0225EDA0(state);
        ov31_0225DF98(state);
        ov31_0225D684(state, 0);
        ov31_0225DCA8(state);
        ClearFrameAndWindow2(&state->data[0x44], 1);
        ov31_0225DD14(state);
        ov31_0225DE84(state);
        ScheduleWindowCopyToVram(&state->data[0x64]);
        break;
    case 5:
        ov31_0225D9D4(state, 0);
        ov31_0225DF98(state);
        ClearFrameAndWindow2(&state->data[0x44], 0);
        ov31_0225DD14(state);
        ov31_0225DE84(state);
        break;
    case 6:
        ov31_0225E2D4(state, *(s16 *)(*(u8 **)&state->data[0x14] + 0x286));
        break;
    case 7:
        ClearWindowTilemapAndScheduleTransfer(&state->data[0x64]);
        ClearWindowTilemapAndScheduleTransfer(&state->data[0x114]);
        ov31_0225E5FC(state);
        break;
    case 8:
        ov31_0225E700(state);
        break;
    case 9:
        ov31_0225E7D4(state);
        break;
    case 10:
        ov31_0225EA08(state);
        break;
    case 11:
        ClearWindowTilemapAndScheduleTransfer(&state->data[0x64]);
        ClearWindowTilemapAndScheduleTransfer(&state->data[0x114]);
        ov31_0225EA9C(state);
        break;
    case 12:
        ov31_0225EB30(state);
        break;
    case 13:
        ov31_0225EBC4(state);
        break;
    }
}

typedef struct ScrnData {
    u8 pad[8];
    u32 szByte;
    u8 rawData[1];
} ScrnData;
void ov31_0225D9D4(Overlay31State *state, int which) {
    void *raw;
    ScrnData *scr;
    switch (which) {
    case 0:
        raw = GfGfxLoader_GetScrnData(0x3c, 0x11, 0, (void **)&scr, 8);
        BG_LoadScreenTilemapData(*(void **)&state->data[4], 6, scr->rawData, scr->szByte);
        ScheduleBgTilemapBufferTransfer(*(void **)&state->data[4], 6);
        Heap_Free(raw);
        GfGfx_EngineBTogglePlanes(2, 1);
        break;
    case 1:
        BgFillTilemapBufferAndSchedule(*(void **)&state->data[4], 5, 0);
        raw = GfGfxLoader_GetScrnData(0x3c, 0x13, 0, (void **)&scr, 8);
        BG_LoadScreenTilemapData(*(void **)&state->data[4], 6, scr->rawData, scr->szByte);
        ScheduleBgTilemapBufferTransfer(*(void **)&state->data[4], 6);
        Heap_Free(raw);
        break;
    case 2:
        BgFillTilemapBufferAndSchedule(*(void **)&state->data[4], 5, 0);
        raw = GfGfxLoader_GetScrnData(0x3c, 0x14, 0, (void **)&scr, 8);
        BG_LoadScreenTilemapData(*(void **)&state->data[4], 6, scr->rawData, scr->szByte);
        ScheduleBgTilemapBufferTransfer(*(void **)&state->data[4], 6);
        Heap_Free(raw);
        break;
    }
}
