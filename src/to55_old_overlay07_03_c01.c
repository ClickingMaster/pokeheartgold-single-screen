#include "to55_old_overlay07_private.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

u16 BattleAnimSystem_GetBattlerSpecies(void *, int);
void *BattleAnimSystem_GetBattlerPersonality(void *, int);
void BattleAnimUtil_FreeWork(void *);
void *ov07_02232A54(void *);

void *ov07_02232A54(void *p) {
    return PTR_AT(p, 0x88);
}
