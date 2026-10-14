#ifndef POKEHEARTGOLD_LINK_RULESET_DATA_H
#define POKEHEARTGOLD_LINK_RULESET_DATA_H

#include "save_link_ruleset.h"

typedef enum StdLinkBattleRuleset {
    STD_RULESET_STANDARD,
    STD_RULESET_FANCY,
    STD_RULESET_LITTLE,
    STD_RULESET_LIGHT,
    STD_RULESET_DOUBLE,
    STD_RULESET_STANDARD_2,
    STD_RULESET_LIGHT_2,
    STD_RULESET_DOUBLE_2,
    STD_RULESET_DOUBLE_3,
    STD_RULESET_GS,
} StdLinkBattleRuleset;

LinkBattleRuleset *LinkBattleRuleset_GetRegulation(SaveData *saveData, int index);
void LinkBattleRuleset_GetRegulationName(SaveData *saveData, int index, String *string, enum HeapID heapID);
LinkBattleRuleset *LinkBattleRuleset_GetDefault(void);
int LinkBattleRuleset_GetRegulationIndex(const LinkBattleRuleset *ruleset);

#endif // POKEHEARTGOLD_LINK_RULESET_DATA_H
