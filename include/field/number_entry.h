#ifndef POKEHEARTGOLD_FIELD_NUMBER_ENTRY_H
#define POKEHEARTGOLD_FIELD_NUMBER_ENTRY_H

#include "bg_window.h"
#include "sys_task.h"

// Number pad on the bottom screen, started by ScrCmd_BankTransaction for the deposit/withdraw amount.
// args->result is -1 while running, the entered value (at most args->max) on confirm, or 0 on cancel.
typedef struct NumberEntryArgs {
    u32 max;
    int result;
} NumberEntryArgs;

SysTask *NumberEntry_Init(BgConfig *bgConfig, void *lastTouchMenuInput, void *fieldSystem, NumberEntryArgs *args);
void NumberEntry_Exit(BgConfig *bgConfig, SysTask *task);
BOOL NumberEntry_IsExitDone(BgConfig *bgConfig);

#endif // POKEHEARTGOLD_FIELD_NUMBER_ENTRY_H
