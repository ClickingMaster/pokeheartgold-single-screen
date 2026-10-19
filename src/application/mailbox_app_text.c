#include "global.h"

#include "application/mailbox_app_internal.h"
#include "msgdata/msg/msg_0232.h"

#include "bg_cell_manager.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "mail.h"
#include "mail_message.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"

// Window indices: 0-19 one name plate per mail slot, 20 the plate of the selected mail, then:
#define MAILBOX_WIN_CANCEL   21
#define MAILBOX_WIN_CLOSE    22
#define MAILBOX_WIN_PAGE     23
#define MAILBOX_WIN_MESSAGE  24
#define MAILBOX_WIN_TITLE    25
#define MAILBOX_WIN_MAIL_MSG 26 // 26-28: the three lines of the mail being read
#define MAILBOX_WIN_AUTHOR   29

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);

static void MailboxApp_PrintString(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align);
static void MailboxAppData_PrintMsg(MailboxAppData *data, MsgData *msgData, u32 windowId, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align);
static void MailboxApp_BufferAuthorName(MailboxApp *app, u32 unused);
static inline u32 MailboxAppData_GetMessageWindowOffset(void);
static void MailboxApp_PrintMessageBox(MailboxApp *app);
static void MailboxApp_PrintMailLine(MailboxApp *app, u32 windowId, u32 line);

static const WindowTemplate sMailboxWindowTemplates[MAILBOX_NUM_WINDOWS] = {
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x3F0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x3E0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x3D0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x3C0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x3B0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x3A0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x390 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x380 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x370 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x360 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x350 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x340 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x330 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x320 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x310 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x300 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x2F0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x2E0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x2D0 },
    { GF_BG_LYR_MAIN_2, 4,  1,  8,  2, 1,  0x2C0 },
    { GF_BG_LYR_MAIN_1, 4,  1,  8,  2, 1,  0x3F0 },
    { GF_BG_LYR_MAIN_1, 25, 21, 6,  2, 15, 0x3E4 },
    { GF_BG_LYR_MAIN_1, 25, 21, 6,  2, 15, 0x3D8 },
    { GF_BG_LYR_MAIN_1, 10, 21, 5,  2, 15, 0x3CE },
    { GF_BG_LYR_MAIN_0, 2,  19, 27, 4, 14, 1     },
    { GF_BG_LYR_SUB_2,  1,  1,  11, 2, 15, 1     },
    { GF_BG_LYR_SUB_0,  3,  3,  26, 4, 1,  1     },
    { GF_BG_LYR_SUB_0,  3,  8,  26, 4, 1,  0x69  },
    { GF_BG_LYR_SUB_0,  3,  13, 26, 4, 1,  0xD1  },
    { GF_BG_LYR_SUB_0,  21, 20, 8,  2, 1,  0x139 },
};

void MailboxApp_AddWindows(MailboxApp *app) {
    u32 i;

    FontID_Alloc(4, HEAP_ID_157);
    for (i = 0; i < MAILBOX_NUM_WINDOWS; i++) {
        AddWindow(app->data->bgConfig, &app->data->windows[i], &sMailboxWindowTemplates[i]);
    }
}

void MailboxApp_RemoveWindows(MailboxApp *app) {
    u32 i;

    for (i = 0; i < MAILBOX_NUM_WINDOWS; i++) {
        RemoveWindow(&app->data->windows[i]);
    }
    FontID_Release(4);
}

// align: 0 left, 1 right, 2 centred on x
static void MailboxApp_PrintString(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFF, color, NULL);
}

static void MailboxAppData_PrintMsg(MailboxAppData *data, MsgData *msgData, u32 windowId, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    MailboxApp_PrintString(&data->windows[windowId], string, x, y, fontId, color, align);
    String_Delete(string);
}

// Draws a name plate (author name on the plate tiles) into a window and puts it into a cell.
void MailboxApp_DrawNameplate(MailboxApp *app, u32 cell, u32 windowId, u32 mailIdx) {
    Window *window = &app->data->windows[windowId];
    Mail *mail = app->data->mail[mailIdx];
    u32 i;
    u32 x;

    CopyU16ArrayToString(app->data->string, Mail_GetAuthorNamePtr(mail));
    x = 0;
    for (i = 0; i < 8; i++) {
        BlitBitmapRectToWindow(window, app->data->nameplateTileLeft, 0, 0, 8, 8, (u16)x, 0, 8, 8);
        BlitBitmapRectToWindow(window, app->data->nameplateTileRight, 0, 0, 8, 8, (u16)x, 8, 8, 8);
        x += 8;
    }
    if (Mail_GetAuthorGender(mail) == 0) {
        MailboxApp_PrintString(window, app->data->string, 0, 0, 4, 0x30200, 0);
    } else {
        MailboxApp_PrintString(window, app->data->string, 0, 0, 4, 0x50400, 0);
    }
    CopyWindowPixelsToVram_TextMode(window);
    BgCellManager_PutWindowTiles(app->data->cellManager, cell, window);
}

void MailboxApp_PrintTitleAndButtons(MailboxApp *app) {
    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_TITLE], 0);
    MailboxAppData_PrintMsg(app->data, app->data->msgData, MAILBOX_WIN_TITLE, msg_0232_00000, 44, 0, 0, 0x10200, 2);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_TITLE]);
    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_CANCEL], 0);
    MailboxAppData_PrintMsg(app->data, app->data->msgData, MAILBOX_WIN_CANCEL, msg_0232_00003, 24, 0, 4, 0xF0100, 2);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_CANCEL]);
    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_CLOSE], 0);
    MailboxAppData_PrintMsg(app->data, app->data->msgData, MAILBOX_WIN_CLOSE, msg_0232_00004, 24, 0, 4, 0xF0100, 2);
}

void MailboxAppData_ShowCloseButton(MailboxAppData *data) {
    ClearWindowTilemapAndScheduleTransfer(&data->windows[MAILBOX_WIN_CANCEL]);
    ScheduleWindowCopyToVram(&data->windows[MAILBOX_WIN_CLOSE]);
}

void MailboxAppData_ShowCancelButton(MailboxAppData *data) {
    ClearWindowTilemapAndScheduleTransfer(&data->windows[MAILBOX_WIN_CLOSE]);
    ScheduleWindowCopyToVram(&data->windows[MAILBOX_WIN_CANCEL]);
}

// "page / last page"
void MailboxApp_PrintPageNumber(MailboxApp *app) {
    String *template;
    u16 slashWidth;
    u16 pageWidth;

    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_PAGE], 0);
    ReadMsgDataIntoString(app->data->msgData, msg_0232_00002, app->data->string);
    MailboxApp_PrintString(&app->data->windows[MAILBOX_WIN_PAGE], app->data->string, 20, 0, 0, 0x10200, 2);
    slashWidth = FontID_String_GetWidth(0, app->data->string, 0);
    BufferIntegerAsString(app->data->msgFormat, 0, app->page + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    template = NewString_ReadMsgData(app->data->msgData, msg_0232_00001);
    StringExpandPlaceholders(app->data->msgFormat, app->data->string, template);
    String_Delete(template);
    pageWidth = FontID_String_GetWidth(0, app->data->string, 0);
    MailboxApp_PrintString(&app->data->windows[MAILBOX_WIN_PAGE], app->data->string, 20 - (slashWidth / 2) - pageWidth, 0, 0, 0x10200, 0);
    BufferIntegerAsString(app->data->msgFormat, 0, app->data->lastPage + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    template = NewString_ReadMsgData(app->data->msgData, msg_0232_00001);
    StringExpandPlaceholders(app->data->msgFormat, app->data->string, template);
    String_Delete(template);
    MailboxApp_PrintString(&app->data->windows[MAILBOX_WIN_PAGE], app->data->string, slashWidth / 2 + 20, 0, 0, 0x10200, 0);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_PAGE]);
}

static void MailboxApp_BufferAuthorName(MailboxApp *app, u32 unused) {
    Mail *mail = app->data->mail[app->selectedMail];
    String *name = String_New(8, HEAP_ID_157);

    CopyU16ArrayToString(name, Mail_GetAuthorNamePtr(mail));
    BufferString(app->data->msgFormat, 0, name, Mail_GetAuthorGender(mail), 1, 2);
    String_Delete(name);
}

static inline u32 MailboxAppData_GetMessageWindowOffset(void) {
    return offsetof(MailboxAppData, windows[MAILBOX_WIN_MESSAGE]);
}

static void MailboxApp_PrintMessageBox(MailboxApp *app) {
    MailboxAppData *data = app->data;
    u32 offset = MailboxAppData_GetMessageWindowOffset();

    FillWindowPixelBuffer((Window *)((u32)data + offset), 15);
    *(u8 *)((u32)app->data + offset + (offsetof(MailboxAppData, printerId) - offsetof(MailboxAppData, windows[MAILBOX_WIN_MESSAGE]))) = AddTextPrinterParameterizedWithColor((Window *)((u32)data + offset), 1, *(String **)((u32)app->data + offset + (offsetof(MailboxAppData, string) - offsetof(MailboxAppData, windows[MAILBOX_WIN_MESSAGE]))), 0, 0, Options_GetTextFrameDelay(app->options), 0x1020F, NULL);
}

void MailboxApp_PrintMailPrompt(MailboxApp *app) {
    String *template;

    MailboxApp_BufferAuthorName(app, 0);
    template = NewString_ReadMsgData(app->data->msgData, msg_0232_00009);
    StringExpandPlaceholders(app->data->msgFormat, app->data->string, template);
    String_Delete(template);
    MailboxApp_PrintMessageBox(app);
}

void MailboxApp_DrawMessageBox(MailboxApp *app) {
    DrawFrameAndWindow2(&app->data->windows[MAILBOX_WIN_MESSAGE], TRUE, 0x3E2, 13);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_MESSAGE]);
}

void MailboxApp_ClearMessageBox(MailboxApp *app) {
    ClearFrameAndWindow2(&app->data->windows[MAILBOX_WIN_MESSAGE], TRUE);
    ScheduleBgTilemapBufferTransfer(app->data->bgConfig, GF_BG_LYR_MAIN_0);
}

void MailboxApp_PrintMessage(MailboxApp *app, u32 message) {
    switch (message) {
    case 0:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00010, app->data->string);
        break;
    case 1:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00011, app->data->string);
        break;
    case 2:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00012, app->data->string);
        break;
    case 3:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00013, app->data->string);
        break;
    case 4:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00014, app->data->string);
        break;
    }
    MailboxApp_PrintMessageBox(app);
}

void MailboxApp_PrintMail(MailboxApp *app) {
    u32 i;

    for (i = 0; i < 3; i++) {
        FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_MAIL_MSG + i], 0);
        MailboxApp_PrintMailLine(app, MAILBOX_WIN_MAIL_MSG + i, i);
        ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_MAIL_MSG + i]);
    }
    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_AUTHOR], 0);
    CopyU16ArrayToString(app->data->string, Mail_GetAuthorNamePtr(app->data->mail[app->selectedMail]));
    MailboxApp_PrintString(&app->data->windows[MAILBOX_WIN_AUTHOR], app->data->string, 0, 1, 1, 0x10200, 0);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_AUTHOR]);
}

static void MailboxApp_PrintMailLine(MailboxApp *app, u32 windowId, u32 line) {
    MailMessage *message;
    String *string;
    u8 selectedMail = app->selectedMail;
    Mail *mail = app->data->mail[selectedMail];

    message = Mail_GetMessage(mail, (u8)line);
    if (MailMsg_IsInit(message)) {
        string = MailMsg_GetExpandedString(message, HEAP_ID_157);
        MailboxApp_PrintString(&app->data->windows[windowId], string, 0, 0, 1, 0x10200, 0);
        String_Delete(string);
    }
}
