#include "battle_greeting_task.h"

#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "launch_application.h"
#include "mail_message.h"
#include "mail_misc.h"
#include "message_format.h"
#include "save_misc_data.h"
#include "screen_fade.h"
#include "task.h"

extern void MessageFormat_Delete(MessageFormat *messageFormat);
extern void FieldMap_FadeScreen(int type);

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

BOOL Task_EditBattleGreeting(TaskManager *taskManager) {
    BattleGreetingTask *work = TaskManager_GetEnvironment(taskManager);

    switch (work->state) {
    case 0:
        EasyChatArgs_SetMailMessage(work->easyChat, &work->message);
        EasyChatArgs_SetCancelled(work->easyChat);
        EasyChat_LaunchApp(work->fieldSystem, work->easyChat);
        work->state = 1;
        break;
    case 1:
        if (!FieldSystem_ApplicationIsRunning(work->fieldSystem)) {
            FieldSystem_LoadFieldOverlay(work->fieldSystem);
            work->state = 2;
        }
        break;
    case 2:
        if (FieldSystem_IsOverworldRestored(work->fieldSystem)) {
            FieldMap_FadeScreen(1);
            work->state = 3;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            if (EasyChatArgs_IsCancelled(work->easyChat)) {
                *work->result = 0;
                work->state = 4;
            } else {
                *work->result = 1;
                EasyChatArgs_GetMailMessage(work->easyChat, &work->message);
                SaveMisc_SetBattleGreeting(work->misc, &work->message);
                work->state = 4;
            }
        }
        break;
    case 4:
        BattleGreetingTask_Free(work);
        return TRUE;
    }
    return FALSE;
}
