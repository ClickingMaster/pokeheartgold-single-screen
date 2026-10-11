#ifndef POKEHEARTGOLD_FIELD_OVERWORLD_TRANSITION_H
#define POKEHEARTGOLD_FIELD_OVERWORLD_TRANSITION_H

#include "task.h"

void CallTask_LeaveOverworld(TaskManager *taskManager);
void PaletteFadeUntilFinished(TaskManager *taskManager);
void CallTask_FadeFromBlack(TaskManager *taskManager);
void CallTask_FadeOutAndLeaveOverworld(TaskManager *taskManager);
void CallTask_RestoreOverworldAndFadeIn(TaskManager *taskManager);

#endif // POKEHEARTGOLD_FIELD_OVERWORLD_TRANSITION_H
