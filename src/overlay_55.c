#include "overlay_55.h"

#include <nitro/mi/memory.h>

#include "global.h"

#include "constants/game_stats.h"
#include "constants/mail.h"

#include "game_stats.h"
#include "player_data.h"

FS_EXTERN_OVERLAY(OVY_56);
FS_EXTERN_OVERLAY(OVY_102);

extern BOOL MailView_Init(OverlayManager *, int *);
extern BOOL MailView_Main(OverlayManager *, int *);
extern BOOL MailView_Exit(OverlayManager *, int *);

extern BOOL EasyChat_Init(OverlayManager *, int *);
extern BOOL EasyChat_Main(OverlayManager *, int *);
extern BOOL EasyChat_Exit(OverlayManager *, int *);

static const OverlayManagerTemplate sUseMailMailViewTemplate = {
    .init = MailView_Init,
    .exec = MailView_Main,
    .exit = MailView_Exit,
    .ovy_id = FS_OVERLAY_ID(OVY_56),
};

static const OverlayManagerTemplate sUseMailEasyChatTemplate = {
    .init = EasyChat_Init,
    .exec = EasyChat_Main,
    .exit = EasyChat_Exit,
    .ovy_id = FS_OVERLAY_ID(OVY_102),
};

static MailViewArgs *UseMail_CreateMailViewArgs(Mail *, enum HeapID);
static void UseMail_FreeMailViewArgs(MailViewArgs *);
static void UseMail_SaveMail(Mail *, MailViewArgs *);

static BOOL UseMail_RunSubApp(OverlayManager **manager) {
    if (*manager != NULL && OverlayManager_Run(*manager)) {
        OverlayManager_Delete(*manager);
        *manager = NULL;
        return TRUE;
    }

    return FALSE;
}

BOOL ov55_UnkApp_Init(OverlayManager *manager, int *state) {
    UseMailData *overlayData;
    UseMailArgs *args;

    args = OverlayManager_GetArgs(manager);
    Heap_Create(HEAP_ID_3, HEAP_ID_OV55, 0x1000);
    overlayData = OverlayManager_CreateAndGetData(manager, sizeof(UseMailData), HEAP_ID_OV55);
    MI_CpuFill8(overlayData, 0, sizeof(UseMailData));

    overlayData->heapID = HEAP_ID_OV55;
    overlayData->mailViewArgs = UseMail_CreateMailViewArgs(args->mail, HEAP_ID_OV55);
    overlayData->mailViewArgs->options = Save_PlayerData_GetOptionsAddr(args->saveData);
    if (args->unk0 == 1) {
        overlayData->mailViewArgs->mailType = args->mailType;
    }
    if (overlayData->mailViewArgs->mailType >= NUM_MAIL) {
        overlayData->mailViewArgs->mailType = MAIL_GRASS;
    }
    overlayData->mailViewArgs->unk0 = args->unk0;
    overlayData->mailViewArgs->menuInputStateMgr = args->menuInputStatePtr;
    return TRUE;
}

BOOL ov55_UnkApp_Main(OverlayManager *manager, int *state) {
    UseMailData *overlayData = OverlayManager_GetData(manager);
    UseMailArgs *args = OverlayManager_GetArgs(manager);

    switch (*state) {
    case 0:
        overlayData->mailViewArgs->unk0 = args->unk0;
        overlayData->subApp = OverlayManager_New(&sUseMailMailViewTemplate, overlayData->mailViewArgs, overlayData->heapID);
        *state = 1;
        break;

    case 1:
        if (!UseMail_RunSubApp(&overlayData->subApp)) {
            break;
        }

        switch (overlayData->mailViewArgs->unk0) {
        case 0xFFFF:
            *state = 2;
            break;
        case 3:
            *state = 2;
            break;
        default:
            *state = 3;
            break;
        }

        break;

    case 2:
        if (args->unk0 == 1) {
            if (overlayData->mailViewArgs->unk0 == 3) {
                UseMail_SaveMail(args->mail, overlayData->mailViewArgs);
                GameStats_AddScore(Save_GameStats_Get(args->saveData), SCORE_EVENT_WROTE_MAIL);
                GameStats_Inc(Save_GameStats_Get(args->saveData), GAME_STAT_MAIL_WRITTEN);
                args->unk4 = 1;
            } else {
                args->unk4 = 0;
            }
        }

        return TRUE;

    case 3:
        overlayData->easyChatArgs = EasyChat_CreateArgs(2, 0, args->saveData, args->menuInputStatePtr, overlayData->heapID);
        if (MailMsg_IsInit(&overlayData->mailViewArgs->mailMessages[overlayData->mailViewArgs->mailMessageIdx])) {
            MailMsg_Copy(&overlayData->editMessage, &overlayData->mailViewArgs->mailMessages[overlayData->mailViewArgs->mailMessageIdx]);
        } else {
            MailMsg_Init_WithBank(&overlayData->editMessage, MAILMSG_BANK_0293_GMM);
        }
        EasyChatArgs_SetMailMessage(overlayData->easyChatArgs, &overlayData->editMessage);
        overlayData->subApp = OverlayManager_New(&sUseMailEasyChatTemplate, overlayData->easyChatArgs, overlayData->heapID);
        *state = 4;
        break;

    case 4:
        if (UseMail_RunSubApp(&overlayData->subApp)) {
            if (EasyChatArgs_IsCancelled(overlayData->easyChatArgs) == 0) {
                EasyChatArgs_GetMailMessage(overlayData->easyChatArgs, &overlayData->mailViewArgs->mailMessages[overlayData->mailViewArgs->mailMessageIdx]);
            }
            EasyChat_FreeArgs(overlayData->easyChatArgs);
            *state = 0;
        }

        break;
    }

    return FALSE;
}

BOOL ov55_UnkApp_Exit(OverlayManager *manager, int *state) {
    UseMailData *overlayData = OverlayManager_GetData(manager);
    UseMail_FreeMailViewArgs(overlayData->mailViewArgs);
    OverlayManager_FreeData(manager);
    Heap_Destroy(overlayData->heapID);
    return TRUE;
}

static MailViewArgs *UseMail_CreateMailViewArgs(Mail *mail, enum HeapID heapID) {
    MailViewArgs *ret = Heap_Alloc(heapID, sizeof(MailViewArgs));
    MI_CpuFill8(ret, 0, sizeof(MailViewArgs));

    ret->unk0 = 0;
    ret->mailOTID = Mail_GetOTID(mail);
    ret->mailAuthorName = String_New(PLAYER_NAME_LENGTH + 1, heapID);
    CopyU16ArrayToString(ret->mailAuthorName, Mail_GetAuthorNamePtr(mail));
    ret->mailType = Mail_GetType(mail);
    ret->mailLanguage = Mail_GetLanguage(mail);
    ret->mailVersion = Mail_GetVersion(mail);

    for (u16 i = 0; i < 3; i++) {
        ret->monIcons[i] = Mail_GetMonIconData(mail, (u8)i, 2, Mail_GetFormFlags(mail));
    }

    for (u16 i = 0; i < 3; i++) {
        MailMsg_Copy(&ret->mailMessages[i], Mail_GetUnk20Array(mail, (u8)i));
    }

    return ret;
}

static void UseMail_FreeMailViewArgs(MailViewArgs *unk) {
    if (unk->mailAuthorName != NULL) {
        String_Delete(unk->mailAuthorName);
    }

    Heap_Free(unk);
}

static void UseMail_SaveMail(Mail *mail, MailViewArgs *unk) {
    for (u16 i = 0; i < 3; i++) {
        Mail_SetMessage(mail, &unk->mailMessages[i], (u8)i);
    }
    Mail_SetType(mail, unk->mailType);
}
