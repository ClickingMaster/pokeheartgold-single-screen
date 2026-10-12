#ifndef POKEHEARTGOLD_SAFARI_DECORATION_TASK_H
#define POKEHEARTGOLD_SAFARI_DECORATION_TASK_H

#include "overlay_02.h"
#include "task.h"

typedef struct SafariDecorationTaskEnv {
    u32 state;
    u16 *unknown_4;
    SafariDecorationArgs *safariDecorationArgs;
} SafariDecorationTaskEnv;

void CallTask_SafariDecoration(TaskManager *taskManager, u16 *unknownPtr);

#endif // HEARTGOLD_UNK_02097720_HH
