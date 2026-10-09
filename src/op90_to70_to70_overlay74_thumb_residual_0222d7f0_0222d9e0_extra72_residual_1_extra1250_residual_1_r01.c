#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "message_format.h"
typedef struct Work74 Work74;
typedef BOOL (*Page74Func)(Work74 *work, Window *window, u32 color);
typedef struct Page74Entry {
    int page;
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    u32 font;
    u32 color;
    u32 attr;
    u32 msgId;
    Page74Func func;
    u32 textX;
    u32 textY;
} Page74Entry;
struct Work74 {
    enum HeapID heapId;
    u8 unk0004[0x29F8];
    BgConfig *bgConfig;
    MessageFormat *msgFormat;
    MsgData *msgData;
    Window windows[19];
    u8 unk2B38[0x10];
    u32 baseTiles[19];
    u8 unk2B94[0x30];
    int page;
};
u8 GetFontAttribute(u8 fontId, u8 attr);
u32 FontID_String_GetCenterAlignmentX(u32 fontId, String *string, u32 letterSpacing, u32 windowWidth);
u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
extern const Page74Entry ov74_0223C340[];
void ov74_0222DB70(Work74 *work, int page);
void ov74_0222DCD4(Work74 *work);
void ov74_0222D824(Work74 *work, int unused, int page);
// @fn ov74_0222D824
void ov74_0222D824(Work74 *work, int unused, int page) {
    const Page74Entry *entries = ov74_0223C340;
    u32 i;
    u32 tile;
    u32 color;
    u32 x;
    String *string;
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 247, work->heapId);
    work->msgFormat = MessageFormat_New(work->heapId);
    work->page = page;
    ov74_0222DB70(work, page);
    for (i = 0; i < 19; i++) {
        if (page != entries[i].page && entries[i].page != 2 && work->windows[i].bgConfig != NULL) {
            ClearWindowTilemapAndCopyToVram(&work->windows[i]);
            RemoveWindow(&work->windows[i]);
        }
    }
    tile = 0x31;
    for (i = 0; i < 19; i++) {
        if (page == entries[i].page) {
            if (work->windows[i].bgConfig == NULL) {
                work->baseTiles[i] = tile;
                AddWindowParameterized(work->bgConfig, &work->windows[i], 0, entries[i].x, entries[i].y, entries[i].width, entries[i].height, 15, tile);
            }
            FillWindowPixelBuffer(&work->windows[i], GetFontAttribute(entries[i].font, entries[i].attr));
            color = entries[i].color;
            if (entries[i].func(work, &work->windows[i], color) == TRUE && entries[i].msgId != 0) {
                string = ReadMsgData_ExpandPlaceholders(work->msgFormat, work->msgData, entries[i].msgId, work->heapId);
                x = entries[i].textX;
                if (x == -1) {
                    x = FontID_String_GetCenterAlignmentX(entries[i].font, string, 0, entries[i].width * 8);
                }
                AddTextPrinterParameterizedWithColor(&work->windows[i], entries[i].font, string, x, entries[i].textY, 0xFF, color, NULL);
                String_Delete(string);
            }
            CopyWindowToVram(&work->windows[i]);
            tile += entries[i].width * entries[i].height;
        }
    }
    DestroyMsgData(work->msgData);
    MessageFormat_Delete(work->msgFormat);
    if (page == 0) {
        ov74_0222DCD4(work);
    }
}
