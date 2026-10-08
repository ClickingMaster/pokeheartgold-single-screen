#pragma force_active on
#include "global.h"
#include "msgdata.h"
#include "bg_window.h"
typedef struct {
    u8 pad0[8];
    int mode;
    int rightAlign;
    Window *window;
    MessageFormat *msgFormat;
    u8 pad18[8];
    int x;
    int y;
    u8 pad28[0xc];
    int fileId;
    u8 pad38[8];
    int fontId;
    u32 color;
    u8 fillColor;
    u8 pad49[3];
    int msgId;
    int speed;
} Printer;
typedef struct {
    u8 pad0[8];
    enum HeapID heapId;
} OvGlobal;
extern OvGlobal ov74_0223D454;
int AddTextPrinterParameterizedWithColor(Window *, int, String *, int, int, int, u32, void *);
int FontID_String_GetWidth(int, String *, int);
u8 GetFontAttribute(u8, int);
int ov74_0223547C(Printer *p, int msgId);
// @fn ov74_0223547C
int ov74_0223547C(Printer *p, int msgId) {
    OvGlobal *g = &ov74_0223D454;
    int printerId;
    if (msgId != -1 && p->msgId != msgId) {
        p->msgId = msgId;
        if (p->mode == 1) {
            FillWindowPixelBuffer(p->window, p->fillColor);
        }
        if (p->msgId != -1) {
            MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, p->fileId, g->heapId);
            MessageFormat *fmt = p->msgFormat;
            String *str;
            if (fmt == NULL) {
                fmt = MessageFormat_New(g->heapId);
            }
            str = ReadMsgData_ExpandPlaceholders(fmt, msgData, p->msgId, g->heapId);
            if (p->rightAlign == 0) {
                printerId = AddTextPrinterParameterizedWithColor(p->window, p->fontId, str, p->x, p->y, p->speed, p->color, NULL);
            } else {
                int width = FontID_String_GetWidth(p->fontId, str, GetFontAttribute(p->fontId, 2));
                int x = GetWindowWidth(p->window) * 8 - width;
                printerId = AddTextPrinterParameterizedWithColor(p->window, p->fontId, str, x, p->y, p->speed, p->color, NULL);
                p->rightAlign = 0;
            }
            String_Delete(str);
            if (p->msgFormat == NULL) {
                MessageFormat_Delete(fmt);
            }
            DestroyMsgData(msgData);
        }
    }
    p->speed = 0xFF;
    return printerId;
}
