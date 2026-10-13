#include "mail_misc.h"

#include "global.h"

#include "constants/items.h"
#include "constants/mail.h"
#include "constants/pokemon.h"

#include "item.h"
#include "mail.h"
#include "menu_input_state.h"
#include "options.h"
#include "player_data.h"
#include "save_vars_flags.h"
#include "sys_flags.h"

EasyChatArgs *EasyChat_CreateArgs(u8 args, u8 a1, SaveData *saveData, MenuInputStateMgr *menuInputStateMgr, enum HeapID heapID) {
    EasyChatArgs *ptr = Heap_Alloc(heapID, sizeof(EasyChatArgs));
    ptr->type = args;
    ptr->unk1 = a1;
    ptr->pokedex = Save_Pokedex_Get(saveData);
    ptr->easyChat = Save_EasyChat_Get(saveData);
    ptr->gameCleared = CheckGameClearFlag(Save_VarsFlags_Get(saveData));
    ptr->unk5 = 0;
    ptr->cancelled = 1;
    ptr->changed = 0;
    ptr->frame = Options_GetFrame(Save_PlayerData_GetOptionsAddr(saveData));
    ptr->menuInputPtr = menuInputStateMgr;

    if (args == 2) {
        MailMsg_Init_WithBank(&ptr->mailMessage, MAILMSG_BANK_0293_GMM);
    } else {
        for (int i = 0; i < 2; i++) {
            ptr->words[i] = -1;
        }
    }
    return ptr;
}

void EasyChat_FreeArgs(EasyChatArgs *args) {
    Heap_Free(args);
}

void EasyChatArgs_SetWord(EasyChatArgs *args, u16 word) {
    args->words[0] = word;
}

void EasyChatArgs_SetWords(EasyChatArgs *args, u16 word1, u16 word2) {
    args->words[0] = word1;
    args->words[1] = word2;
}

void EasyChatArgs_SetMailMessage(EasyChatArgs *args, MailMessage *mailMessage) {
    args->mailMessage = *mailMessage;
}

void EasyChatArgs_SetCancelled(EasyChatArgs *args) {
    args->cancelled = 1;
    args->changed = 0;
}

void EasyChatArgs_SetAlwaysConfirm(EasyChatArgs *args) {
    args->unk5 = 1;
}

BOOL EasyChatArgs_IsCancelled(EasyChatArgs *args) {
    return args->cancelled;
}

BOOL EasyChatArgs_IsChanged(EasyChatArgs *args) {
    return args->changed;
}

u16 EasyChatArgs_GetWord(EasyChatArgs *args) {
    return args->words[0];
}

void EasyChatArgs_GetWords(EasyChatArgs *args, MsgBankMsgNo *msgBankMsgNo) {
    msgBankMsgNo->msgBank = args->words[0];
    msgBankMsgNo->msgNo = args->words[1];
}

void EasyChatArgs_GetMailMessage(EasyChatArgs *args, MailMessage *mailMessage) {
    MailMsg_Copy(mailMessage, &args->mailMessage);
}

u8 EasyChatArgs_GetType(EasyChatArgs *args) {
    return args->type;
}

u8 EasyChatArgs_GetPromptMsgNo(EasyChatArgs *args) {
    return args->unk1;
}

u32 EasyChatArgs_GetFrame(EasyChatArgs *args) {
    return args->frame;
}

Pokedex *EasyChatArgs_GetPokedex(EasyChatArgs *args) {
    return args->pokedex;
}

SaveEasyChat *EasyChatArgs_GetEasyChat(EasyChatArgs *args) {
    return args->easyChat;
}

u8 EasyChatArgs_GetGameCleared(EasyChatArgs *args) {
    return args->gameCleared;
}

u8 EasyChatArgs_GetAlwaysConfirm(EasyChatArgs *args) {
    return args->unk5;
}

void EasyChatArgs_CopyMessage(EasyChatArgs *args, MailMessage *msg1, MailMessage *msg2) {
    switch (args->type) {
    case 0:
        msg1->msg_bank = args->words[0];
        return;
    case 1:
        msg1->msg_bank = args->words[0];
        msg1->msg_no = args->words[1];
        return;
    case 2:
        *msg2 = args->mailMessage;
        return;
    }
}

BOOL EasyChatArgs_MessageEquals(EasyChatArgs *args, MailMessage *msg1, MailMessage *msg2) {
    switch (args->type) {
    case 0:
        return msg1->msg_bank == args->words[0];
    case 1:
        return msg1->msg_bank == args->words[0] && msg1->msg_no == args->words[1];
    case 2:
    default:
        return MailMsg_Compare(&args->mailMessage, msg2);
    }
}

void EasyChatArgs_SetResult(EasyChatArgs *args, MailMessage *msg1, MailMessage *msg2) {
    args->changed = !EasyChatArgs_MessageEquals(args, msg1, msg2);
    args->cancelled = 0;

    u16 *src = (u16 *)&msg1->msg_bank;
    for (int i = 0; i < 2; i++) {
        args->words[i] = src[i];
    }

    args->mailMessage = *msg2;
}

int EasyChatArgs_GetMenuInputState(EasyChatArgs *args) {
    if (args == NULL || args->menuInputPtr == 0) {
        return 0;
    }

    return MenuInputStateMgr_GetState(args->menuInputPtr);
}

void EasyChatArgs_SetMenuInputState(EasyChatArgs *args, MenuInputState state) {
    MenuInputStateMgr_SetState(args->menuInputPtr, state);
}

UseMailArgs *UseMailArgs_NewCompose(SaveData *saveData, u16 a1, u8 partyIdx, u8 mailType, enum HeapID heapID) {
    Mailbox *mailbox = Save_Mailbox_Get(saveData);
    UseMailArgs *ptr = Heap_AllocAtEnd(heapID, sizeof(UseMailArgs));
    MI_CpuFill8(ptr, 0, sizeof(UseMailArgs));

    ptr->mailType = mailType;
    ptr->partyIdx = partyIdx;
    ptr->mailbox = mailbox;
    ptr->unk0 = 1;
    ptr->unk8 = a1;
    ptr->unkC = 0;
    ptr->saveData = saveData;

    Mail *mail = Mail_New(heapID);
    ptr->mail = mail;
    Mail_Init(mail);
    Mail_SetNewMessageDetails(ptr->mail, MAIL_NONE, partyIdx, saveData);

    return ptr;
}

UseMailArgs *UseMailArgs_NewFromMailbox(SaveData *saveData, int n, u16 i, enum HeapID heapID) {
    UseMailArgs *ptr = Heap_AllocAtEnd(heapID, sizeof(UseMailArgs));
    MI_CpuFill8(ptr, 0, sizeof(UseMailArgs));

    ptr->unk0 = 0;
    ptr->unk8 = n;
    ptr->unkC = i;
    ptr->saveData = saveData;

    Mailbox *mailbox = Save_Mailbox_Get(saveData);
    ptr->mailbox = mailbox;
    ptr->mail = Mailbox_AllocAndFetchMailI(&mailbox->msgs[0], n, i, heapID);

    return ptr;
}

UseMailArgs *UseMailArgs_NewFromMon(SaveData *saveData, Pokemon *mon, enum HeapID heapID) {
    UseMailArgs *ptr = Heap_AllocAtEnd(heapID, sizeof(UseMailArgs));
    MI_CpuFill8(ptr, 0, sizeof(UseMailArgs));

    ptr->unk0 = 0;
    ptr->saveData = saveData;

    Mail *mail = Mail_New(heapID);
    ptr->mail = mail;
    GetMonData(mon, MON_DATA_MAIL, ptr->mail);

    return ptr;
}

UseMailArgs *UseMailArgs_NewPreview(SaveData *saveData, u8 mailType, enum HeapID heapID) {
    UseMailArgs *ptr = Heap_AllocAtEnd(heapID, sizeof(UseMailArgs));
    MI_CpuFill8(ptr, 0, sizeof(UseMailArgs));

    ptr->unk0 = 0;
    ptr->saveData = saveData;

    Mail *mail = Mail_New(heapID);
    ptr->mail = mail;
    Mail_SetType(mail, mailType);

    return ptr;
}

u32 UseMailArgs_WasMailWritten(UseMailArgs *args) {
    return args->unk4;
}

BOOL UseMailArgs_GiveMailToMon(UseMailArgs *args, Pokemon *mon) {
    if (UseMailArgs_WasMailWritten(args) == 0) {
        return FALSE;
    }
    SetMonData(mon, MON_DATA_MAIL, args->mail);
    return TRUE;
}

void UseMailArgs_Free(UseMailArgs *args) {
    if (args->mail != NULL) {
        Heap_Free(args->mail);
    }
    Heap_Free(args);
}

int Mailbox_MoveMessageFromMon(Mailbox *mailbox, Pokemon *mon, enum HeapID heapID) {
    int item = ITEM_NONE;
    int idx = Mailbox_GetFirstEmptySlotIdx(mailbox);

    if (idx != -1) {
        Mail *mail = Mail_New(heapID);

        GetMonData(mon, MON_DATA_MAIL, mail);
        Mailbox_CopyMailToSlotI(&mailbox->msgs[0], 0, idx, mail);
        Mail_Init(mail);
        SetMonData(mon, MON_DATA_MAIL, mail);
        SetMonData(mon, MON_DATA_HELD_ITEM, &item);
        Heap_Free(mail);
        return idx;
    }
    return -1;
}

int Mailbox_MoveMessageToMon(Mail *msgs, int i, Pokemon *mon, enum HeapID heapID) {
    int item = ITEM_NONE;

    Mail *mail = Mailbox_AllocAndFetchMailI(msgs, 0, i, heapID);
    if (mail == NULL) {
        return -1;
    }

    item = MailToItemId(Mail_GetType(mail));
    SetMonData(mon, MON_DATA_MAIL, mail);
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    Mailbox_DeleteSlotI(msgs, 0, i);
    Heap_Free(mail);

    return i;
}
