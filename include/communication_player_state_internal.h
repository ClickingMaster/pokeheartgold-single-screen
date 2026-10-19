#ifndef POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_INTERNAL_H
#define POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_INTERNAL_H

#include "global.h"

#include "dwc/dwc_account_internal.h"

#include "player_data.h"
#include "save_link_ruleset.h"

// compared byte by byte by CommPlayerInfo_RulesetsMatch
typedef union CommPlayerRules {
    LinkBattleRuleset ruleset;
    u8 bytes[sizeof(LinkBattleRuleset)];
} CommPlayerRules;

typedef struct CommPlayerInfoEntry {
    CommPlayerRules rules;
    PlayerProfile profile;
    DwcAccData token;
    u16 groupName[8];
    u8 mac[6];
    u8 index;
    u8 country;
    u8 region;
    u8 unk65;
    u8 pad66[2];
} CommPlayerInfoEntry;

typedef struct CommPlayerRecord {
    u16 wins;
    u16 losses;
    u16 trades;
} CommPlayerRecord;

typedef struct CommPlayerInfo {
    PlayerProfile *localProfile;
    LinkBattleRuleset *rules;
    SaveData *save;
    CommPlayerInfoEntry entries[8];
    PlayerProfile *profiles[8];
    CommPlayerRecord scores[8];
    u8 status[8];
    u8 unk3A4;
    u8 hasNewEntries;
    u8 curIdx;
    u8 unk3A7;
} CommPlayerInfo;

#endif
