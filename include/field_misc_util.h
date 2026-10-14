#ifndef POKEHEARTGOLD_FIELD_MISC_UTIL_H
#define POKEHEARTGOLD_FIELD_MISC_UTIL_H

#include "field_types_def.h"
#include "save.h"

u16 Save_GetPartyLead(SaveData *saveData);
u16 Save_GetPartyLeadAlive(SaveData *saveData);
u32 CountDigits(u32 num);
BOOL ItemIsTMOrHM(u16 item);
u16 GetOakJohtoDexRating(u16 numCaught, u16 gender, u16 *var_p);
u16 GetOakNationalDexRating(u16 numCaught, u16 gender, u16 *var_p);
BOOL Save_PlayerHasAllRegisInParty(SaveData *saveData);
void CallTask_SpinMapObject(TaskManager *taskManager, LocalMapObject *object, u16 turns, u16 step, u16 xRadius, u16 zRadius);
void CallTask_BlinkMapObject(TaskManager *taskManager, LocalMapObject *object, u16 cycles, u16 interval);

#endif // POKEHEARTGOLD_FIELD_MISC_UTIL_H
