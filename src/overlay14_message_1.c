#include "global.h"
extern void *PCBox_LoadBoxMessages(void *);
extern void PCBox_DrawMessageFrame(void *);
extern void PCBox_PrintExpandedMsg(void *, void *, int, int, int, int, int, int, int);
extern void ScheduleWindowCopyToVram(void *);
extern void DestroyMsgData(void *);
void PCBox_PrintMessage(void *, int, int);
extern void PCBox_PrintMessage(void *, int, int);
extern void BufferItemName(void *, int, int);
void PCBox_PrintExitPrompt(void *, int, int);
void PCBox_PrintTakeItemPrompt(void *, int, int);
void PCBox_PrintMailCantBeTaken(void *, int);
void PCBox_PrintMailCantBeMoved(void *, int);
void PCBox_PrintTookItem(void *, int, int);
void PCBox_PrintBagFull(void *, int);
void PCBox_PrintItemHeld(void *, int, int);
void PCBox_PrintTouchTheMark(void *, int);

void PCBox_PrintMessage(void *p, int msgno, int window) {
    void *msg = PCBox_LoadBoxMessages(p);
    int off = window << 4;
    PCBox_DrawMessageFrame((u8 *)*(void **)((u8 *)p + 0x34) + 0x30 + off);
    PCBox_PrintExpandedMsg(*(void **)((u8 *)p + 0x34), msg, window, msgno, 0, 0, 1, 0x1020f, 0);
    ScheduleWindowCopyToVram((u8 *)*(void **)((u8 *)p + 0x34) + 0x30 + off);
    DestroyMsgData(msg);
}

void PCBox_PrintExitPrompt(void *p, int x, int kind) {
    if (kind == 1) {
        PCBox_PrintMessage(p, 11, x);
    } else {
        PCBox_PrintMessage(p, 12, x);
    }
}

void PCBox_PrintTakeItemPrompt(void *p, int item, int x) {
    void *q = *(void **)((u8 *)p + 0x34);
    BufferItemName(*(void **)((u8 *)q + 0x24), 0, item);
    PCBox_PrintMessage(p, 23, x);
}

void PCBox_PrintMailCantBeTaken(void *p, int x) {
    PCBox_PrintMessage(p, 24, x);
}

void PCBox_PrintMailCantBeMoved(void *p, int x) {
    PCBox_PrintMessage(p, 54, x);
}

void PCBox_PrintTookItem(void *p, int item, int x) {
    void *q = *(void **)((u8 *)p + 0x34);
    BufferItemName(*(void **)((u8 *)q + 0x24), 0, item);
    PCBox_PrintMessage(p, 15, x);
}

void PCBox_PrintBagFull(void *p, int x) {
    PCBox_PrintMessage(p, 14, x);
}

void PCBox_PrintItemHeld(void *p, int item, int x) {
    if (item == 0) {
        BufferItemName(*(void **)((u8 *)*(void **)((u8 *)p + 0x34) + 0x24), 0, 0x70);
        PCBox_PrintMessage(p, 55, x);
    } else {
        BufferItemName(*(void **)((u8 *)*(void **)((u8 *)p + 0x34) + 0x24), 0, item);
        PCBox_PrintMessage(p, 16, x);
    }
}

void PCBox_PrintTouchTheMark(void *p, int x) {
    PCBox_PrintMessage(p, 35, x);
}
