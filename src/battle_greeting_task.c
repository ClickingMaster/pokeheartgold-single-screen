#include "battle_greeting_task.h"

#include "global.h"

#include "field/fieldmap.h"

#include "field_system.h"
#include "heap.h"
#include "launch_application.h"
#include "mail_message.h"
#include "mail_misc.h"
#include "message_format.h"
#include "save_misc_data.h"
#include "screen_fade.h"
#include "task.h"

typedef struct BattleGreetingTask {
    FieldSystem *fieldSystem;
    MessageFormat *format;
    MailMessage message;
    EasyChatArgs *easyChat;
    SAVE_MISC_DATA *misc;
    u32 state;
    u32 unused;
    u16 *result;
} BattleGreetingTask;

void CallTask_EditBattleGreeting(TaskManager *taskManager, u16 *result);
void BattleGreetingTask_Free(BattleGreetingTask *work);
BOOL Task_EditBattleGreeting(TaskManager *taskManager);

void CallTask_EditBattleGreeting(TaskManager *taskManager, u16 *result) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    BattleGreetingTask *work = Heap_Alloc(HEAP_ID_FIELD3, sizeof(BattleGreetingTask));

    work->fieldSystem = fieldSystem;
    work->format = MessageFormat_New(HEAP_ID_FIELD3);
    work->easyChat = EasyChat_CreateArgs(2, 0, work->fieldSystem->saveData, &fieldSystem->menuInputState, HEAP_ID_FIELD3);
    work->misc = Save_Misc_Get(fieldSystem->saveData);
    work->result = result;
    MailMsg_Init_WithBank(&work->message, MAILMSG_BANK_0295_GMM);
    SaveMisc_GetBattleGreeting(work->misc, &work->message);
    EasyChatArgs_SetAlwaysConfirm(work->easyChat);
    work->state = 0;
    TaskManager_Call(taskManager, Task_EditBattleGreeting, work);
}

void BattleGreetingTask_Free(BattleGreetingTask *work) {
    EasyChat_FreeArgs(work->easyChat);
    MessageFormat_Delete(work->format);
    Heap_Free(work);
}

enum {
    BATTLE_GREETING_STATE_LAUNCH_EASY_CHAT,
    BATTLE_GREETING_STATE_WAIT_EASY_CHAT,
    BATTLE_GREETING_STATE_WAIT_OVERWORLD,
    BATTLE_GREETING_STATE_SAVE_GREETING,
    BATTLE_GREETING_STATE_END,
};

BOOL Task_EditBattleGreeting(TaskManager *taskManager) {
    BattleGreetingTask *work = TaskManager_GetEnvironment(taskManager);

    switch (work->state) {
    case BATTLE_GREETING_STATE_LAUNCH_EASY_CHAT:
        EasyChatArgs_SetMailMessage(work->easyChat, &work->message);
        EasyChatArgs_SetCancelled(work->easyChat);
        EasyChat_LaunchApp(work->fieldSystem, work->easyChat);
        work->state = BATTLE_GREETING_STATE_WAIT_EASY_CHAT;
        break;
    case BATTLE_GREETING_STATE_WAIT_EASY_CHAT:
        if (!FieldSystem_ApplicationIsRunning(work->fieldSystem)) {
            FieldSystem_LoadFieldOverlay(work->fieldSystem);
            work->state = BATTLE_GREETING_STATE_WAIT_OVERWORLD;
        }
        break;
    case BATTLE_GREETING_STATE_WAIT_OVERWORLD:
        if (FieldSystem_IsOverworldRestored(work->fieldSystem)) {
            FieldMap_FadeScreen(1);
            work->state = BATTLE_GREETING_STATE_SAVE_GREETING;
        }
        break;
    case BATTLE_GREETING_STATE_SAVE_GREETING:
        if (IsPaletteFadeFinished()) {
            if (EasyChatArgs_IsCancelled(work->easyChat)) {
                *work->result = 0;
                work->state = BATTLE_GREETING_STATE_END;
            } else {
                *work->result = 1;
                EasyChatArgs_GetMailMessage(work->easyChat, &work->message);
                SaveMisc_SetBattleGreeting(work->misc, &work->message);
                work->state = BATTLE_GREETING_STATE_END;
            }
        }
        break;
    case BATTLE_GREETING_STATE_END:
        BattleGreetingTask_Free(work);
        return TRUE;
    }
    return FALSE;
}
