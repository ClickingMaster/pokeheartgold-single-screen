#ifndef POKEHEARTGOLD_MAIL_MISC_H
#define POKEHEARTGOLD_MAIL_MISC_H

#include "easy_chat.h"
#include "heap.h"
#include "mail.h"
#include "mail_message.h"
#include "menu_input_state.h"
#include "pokedex.h"
#include "pokemon.h"
#include "save.h"

typedef struct MsgBankMsgNo {
    u16 msgBank;
    u16 msgNo;
} MsgBankMsgNo;

typedef struct {
    u8 type;
    u8 unk1;
    u8 cancelled;
    u8 changed;
    u8 gameCleared; // TODO: bool8
    u8 unk5;
    u8 unk6[0x2];
    u32 frame;
    Pokedex *pokedex;
    SaveEasyChat *easyChat;
    MailMessage mailMessage;
    u16 words[0x4];
    MenuInputStateMgr *menuInputPtr;
} EasyChatArgs;

typedef struct UseMailArgs {
    u16 unk0;
    u32 unk4;
    u32 unk8;
    u16 unkC;
    u8 partyIdx;
    u8 mailType;
    SaveData *saveData;
    Mail *mail;
    Mailbox *mailbox;
    MenuInputStateMgr *menuInputStatePtr;
} UseMailArgs;

EasyChatArgs *EasyChat_CreateArgs(u8 a0, u8 a1, SaveData *saveData, MenuInputStateMgr *menuInputStateMgr, enum HeapID heapID);
void EasyChat_FreeArgs(EasyChatArgs *args);
void EasyChatArgs_SetWord(EasyChatArgs *args, u16 word);
void EasyChatArgs_SetWords(EasyChatArgs *args, u16 word1, u16 word2);
void EasyChatArgs_SetMailMessage(EasyChatArgs *args, MailMessage *mailMessage);
void EasyChatArgs_SetCancelled(EasyChatArgs *args);
void EasyChatArgs_SetAlwaysConfirm(EasyChatArgs *args);
BOOL EasyChatArgs_IsCancelled(EasyChatArgs *args);
BOOL EasyChatArgs_IsChanged(EasyChatArgs *args);
u16 EasyChatArgs_GetWord(EasyChatArgs *args);
void EasyChatArgs_GetWords(EasyChatArgs *args, MsgBankMsgNo *mailMessage);
void EasyChatArgs_GetMailMessage(EasyChatArgs *args, MailMessage *mailMessage);
u8 EasyChatArgs_GetType(EasyChatArgs *args);
u8 EasyChatArgs_GetPromptMsgNo(EasyChatArgs *args);
u32 EasyChatArgs_GetFrame(EasyChatArgs *args);
Pokedex *EasyChatArgs_GetPokedex(EasyChatArgs *args);
SaveEasyChat *EasyChatArgs_GetEasyChat(EasyChatArgs *args);
u8 EasyChatArgs_GetGameCleared(EasyChatArgs *args);
u8 EasyChatArgs_GetAlwaysConfirm(EasyChatArgs *args);
void EasyChatArgs_CopyMessage(EasyChatArgs *args, MailMessage *msg1, MailMessage *msg2);
BOOL EasyChatArgs_MessageEquals(EasyChatArgs *args, MailMessage *msg1, MailMessage *msg2);
void EasyChatArgs_SetResult(EasyChatArgs *args, MailMessage *msg1, MailMessage *msg2);
int EasyChatArgs_GetMenuInputState(EasyChatArgs *args);
void EasyChatArgs_SetMenuInputState(EasyChatArgs *args, MenuInputState menuInputState);
UseMailArgs *UseMailArgs_NewCompose(SaveData *saveData, u16 a1, u8 partyIdx, u8 mailType, enum HeapID heapID);
UseMailArgs *UseMailArgs_NewFromMailbox(SaveData *saveData, int n, u16 i, enum HeapID heapID);
UseMailArgs *UseMailArgs_NewFromMon(SaveData *saveData, Pokemon *mon, enum HeapID heapID);
UseMailArgs *UseMailArgs_NewPreview(SaveData *saveData, u8 mailType, enum HeapID heapID);
u32 UseMailArgs_WasMailWritten(UseMailArgs *args);
BOOL UseMailArgs_GiveMailToMon(UseMailArgs *args, Pokemon *mon);
void UseMailArgs_Free(UseMailArgs *args);
int Mailbox_MoveMessageFromMon(Mailbox *mailbox, Pokemon *mon, enum HeapID heapID);
int Mailbox_MoveMessageToMon(Mail *msgs, int i, Pokemon *mon, enum HeapID heapID);

#endif // POKEHEARTGOLD_MAIL_MISC_H
