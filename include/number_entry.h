#ifndef POKEHEARTGOLD_NUMBER_ENTRY_H
#define POKEHEARTGOLD_NUMBER_ENTRY_H

#include "bg_window.h"
#include "sys_task.h"

// Numeric entry app (sub-screen number pad, field bottom-screen app 5). Started by
// ScrCmd_BankTransaction for the deposit/withdraw amount.
//
// args->max is the upper bound on the entered value; args->result is set to
// -1 on launch, the entered value on confirm, or 0 on cancel.
typedef struct NumberEntryArgs {
    u32 max;
    int result;
} NumberEntryArgs;

SysTask *NumberEntry_Init(BgConfig *bgConfig, void *a1, void *a2, NumberEntryArgs *args);
void NumberEntry_Exit(BgConfig *bgConfig, SysTask *task);
BOOL NumberEntry_IsExitDone(void *a0);

#endif // POKEHEARTGOLD_NUMBER_ENTRY_H
