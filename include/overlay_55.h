#ifndef POKEHEARTGOLD_OVERLAY_55_H
#define POKEHEARTGOLD_OVERLAY_55_H

#include "mail.h"
#include "mail_misc.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"

typedef struct MailViewArgs {
    u16 unk0;
    u8 mailMessageIdx;
    Options *options;
    MenuInputStateMgr *menuInputStateMgr;
    u32 mailOTID;
    u8 unk10;
    u8 mailLanguage;
    u8 mailVersion;
    u8 mailType;
    String *mailAuthorName;
    u16 monIcons[3];
    MailMessage mailMessages[3];
} MailViewArgs;

typedef struct UseMailData {
    enum HeapID heapID;
    u8 unk4[0x4];
    EasyChatArgs *easyChatArgs;
    OverlayManager *subApp;
    MailViewArgs *mailViewArgs;
    MailMessage editMessage;
} UseMailData;

BOOL UseMail_Init(OverlayManager *manager, int *state);
BOOL UseMail_Main(OverlayManager *manager, int *state);
BOOL UseMail_Exit(OverlayManager *manager, int *state);

#endif
