#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/weather.h"

#include "field/field_effect_helpers.h"
#include "msgdata/msg.naix"
#include "overlay_2/overlay_02_02248728.h"

#include "bg_window.h"
#include "dialog_box.h"
#include "fashion_case.h"
#include "field_system.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "heap.h"
#include "item.h"
#include "map_events.h"
#include "map_events_internal.h"
#include "map_header.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "math_util.h"
#include "message_format.h"
#include "metatile_behavior.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "render_window.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sys_flags.h"
#include "system.h"
#include "task.h"
#include "field_terrain.h"
#include "unk_02055418.h"

// What the following Pokemon and its surroundings look like when the player talks to it; compared against the
// conditions of the interaction tables (FollowMonRule) to pick an interaction script.
typedef struct FollowMonSituation {
    u8 hasHeldItem;
    u8 heldItemPocket;
    u8 hpBucket;
    u8 status;
    u8 level;
    u8 type1;
    u8 type2;
    u8 friendship;
    u8 natureGroup;
    u8 gender;
    u8 speciesAttr;
    u8 shinyLeaves;
    u8 nearbyObjects;
    u8 hiddenItems;
    u8 nearBreakableRock;
    u8 nearRock;
    u8 nearTree;
    u8 weather;
    u8 metatileBehavior;
    u8 encounterTile;
    u8 timeOfDay;
    s8 mood;
    u8 pokeathlonStat;
    u8 facing;
    u8 filler_18[2];
    u16 mapId;
} FollowMonSituation;

// Results of FollowMonInteraction_Run.
enum FollowMonInteractionResult {
    FOLLOW_MON_INTERACTION_RUNNING,
    FOLLOW_MON_INTERACTION_DONE,
    FOLLOW_MON_INTERACTION_QUESTION,
    FOLLOW_MON_INTERACTION_GIVE_ACCESSORY,
    FOLLOW_MON_INTERACTION_GIVE_SHINY_LEAF,
};

typedef struct ShinyLeafMonDataIds {
    u32 ids[5];
} ShinyLeafMonDataIds;

typedef struct ShinyLeafMasks {
    u8 masks[5];
} ShinyLeafMasks;

typedef struct PokeathlonStarBits {
    u16 power : 3;
    u16 skill : 3;
    u16 speed : 3;
    u16 jump : 3;
    u16 stamina : 3;
} PokeathlonStarBits;

typedef struct Ov02_022507E8Env {
    u16 count;
    u16 done;
    EventObjectMovementMan *movement;
} Ov02_022507E8Env;

typedef struct Ov02_022508D8Env {
    u16 wait;
    u16 frame;
} Ov02_022508D8Env;

extern const ShinyLeafMasks sShinyLeafMasks;
extern const ShinyLeafMonDataIds sShinyLeafMonDataIds;
extern const u8 ov02_02253A70[];
extern const u32 sNatureGroups[];
extern const VecFx32 ov02_02253B24;
extern const VecFx32 ov02_02253B30;

// Overlay 1 map object model helpers.
void *MapObjectManager_GetFollowMonBillboard(MapObjectManager *manager);
void FollowMonBillboard_SetShadowAnchored(LocalMapObject *object, BOOL set);
void FollowMonBillboard_SetHeight(LocalMapObject *object, s8 a1);
void FollowMonBillboard_AddHeight(LocalMapObject *object, s8 a1);

int FollowMon_SelectInteraction(FieldSystem *fieldSystem);
void FollowMon_GetSituation(FieldSystem *fieldSystem, FollowMonSituation *situation);
BOOL FollowMonRule_Matches(FieldSystem *fieldSystem, const FollowMonRule *rule, const FollowMonSituation *situation);
void FollowMonSituation_SetMonCondition(Pokemon *mon, FollowMonSituation *situation);
void FollowMonSituation_SetNearbyObjects(FieldSystem *fieldSystem, FollowMonSituation *situation);
void FollowMonSituation_SetHiddenItems(FieldSystem *fieldSystem, FollowMonSituation *situation);
void FollowMonSituation_SetWeather(FieldSystem *fieldSystem, FollowMonSituation *situation);
void FollowMonSituation_SetTerrain(FieldSystem *fieldSystem, FollowMonSituation *situation);
void FollowMonSituation_SetMapId(FieldSystem *fieldSystem, FollowMonSituation *situation);
void FollowMonSituation_SetTimeOfDay(FieldSystem *fieldSystem, FollowMonSituation *situation);
void FollowMonSituation_SetMood(FieldSystem *fieldSystem, FollowMonSituation *situation);
void FollowMonSituation_SetBestPokeathlonStat(FieldSystem *fieldSystem, int species, int form, FollowMonSituation *situation);
void FollowMonSituation_SetFacing(FieldSystem *fieldSystem, FollowMonSituation *situation);
void FollowMonSituation_SetSpeciesAttr(int species, FollowMonSituation *situation);
int FollowMonSituation_ConvertType(int type);
int FollowMonSituation_ConvertItemPocket(int pocket);
void FollowMonInteraction_LoadScript(FollowMonInteraction *interaction, int script);
int FollowMonInteraction_Run(FieldSystem *fieldSystem, FollowMonInteraction *interaction);
BOOL FollowMonInteraction_StepHasMovement(FollowMonInteraction *interaction, FollowMonInteractionStep *step);
BOOL FollowMonInteraction_TryShowEmote(FieldSystem *fieldSystem, FollowMonInteraction *interaction, FollowMonInteractionStep *step);
BOOL FollowMon_TryPrintInteractionMessage(FieldSystem *fieldSystem, FollowMonInteraction *interaction, FollowMonInteractionStep *step);
void FollowMonInteraction_PrintGiftMessage(FieldSystem *fieldSystem, FollowMonInteraction *interaction, int flags);
BOOL FollowMonInteraction_StepHasWait(FollowMonInteraction *interaction, FollowMonInteractionStep *step);
void FollowMon_ExpandInteractionMessage(FieldSystem *fieldSystem, String *dest, enum HeapID heapId, int msgNo);
void FollowMon_ExpandGiftMessage(FieldSystem *fieldSystem, String *dest, enum HeapID heapId, int flags, u8 accessory);
void FollowMonMovement_ApplyOffset(const FollowMonMovementStep *move, LocalMapObject *object);
void FollowMonMovement_PlaySound(const FollowMonMovementStep *move, u16 sound, u16 species, u8 form);
void FollowMonMovement_ApplyFacing(FollowMonInteraction *interaction, const FollowMonMovementStep *move, LocalMapObject *object);
void FollowMon_TryShakeGrass(FollowMonInteraction *interaction, LocalMapObject *object, u8 oldDirection);
void FollowMon_GetCoordsInDirection(LocalMapObject *object, int direction, int *x, int *z);
BOOL FollowMonMovement_Step(FollowMonInteraction *interaction, LocalMapObject *object);
BOOL FollowMonMovement_HasNextStep(FollowMonInteraction *interaction);
BOOL FollowMonInteraction_RunMovement(FieldSystem *fieldSystem, FollowMonInteraction *interaction, int movement);
BOOL Task_FollowMonInteract(TaskManager *taskManager);
void FollowMon_PlaceholdersSet(FieldSystem *fieldSystem, MessageFormat *messageFormat);
void FollowMon_ApplyInteractionEffects(FieldSystem *fieldSystem, FollowMonInteraction *interaction);
BOOL FollowMonRule_CheckFriendship(int rule, int friendship);
BOOL FollowMonRule_CheckMood(int rule, int mood);
BOOL FollowMonRule_CheckSpeciesAttr(u32 rule, u32 value);
BOOL FollowMonRule_CheckShinyLeaf(int leaf, u8 shinyLeaves);
BOOL ov02_022507E8(TaskManager *taskManager);
BOOL ov02_022508D8(TaskManager *taskManager);

void FieldSystem_FollowMonInteract(FieldSystem *fieldSystem) {
    TaskManager_Call(fieldSystem->taskman, Task_FollowMonInteract, NULL);
}

#pragma force_active on

int FollowMon_SelectInteraction(FieldSystem *fieldSystem) {
    FollowMonSituation situation;
    int order[100];
    int *scan;
    FollowMonRule *rules;
    int i;
    int j;
    int n;
    int *cursor;

    ReadWholeNarcMemberByIdPair(fieldSystem->followMonInteraction->rules, NARC_a_2_2_0, MapHeader_GetMapSec(fieldSystem->location->mapId) + 1);
    ReadWholeNarcMemberByIdPair(&fieldSystem->followMonInteraction->rules[30], NARC_a_2_2_0, 0);
    FollowMon_GetSituation(fieldSystem, &situation);
    rules = fieldSystem->followMonInteraction->rules;

    // Rules 30-41 first, then the map section's rules 0-29, then 42-99.
    n = 0;
    cursor = order;
    for (i = 0; i < 12; i++) {
        *cursor++ = i + 30;
        n++;
    }
    i = 0;
    cursor = &order[n];
    do {
        *cursor++ = i;
        n++;
        i++;
    } while (i < 30);
    i = 42;
    cursor = &order[n];
    do {
        *cursor++ = i;
        n++;
        i++;
    } while (i < 100);

    j = 0;
    scan = order;
    do {
        FollowMonRule *rule = &rules[*scan];
        if (rule->script != 0 && FollowMonRule_Matches(fieldSystem, rule, &situation)) {
            return rule->script;
        }
        j++;
        scan++;
    } while (j < 100);
    GF_AssertFail();
    return 0;
}

#pragma force_active reset

void FollowMon_GetSituation(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
    fieldSystem->followMonInteraction->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    fieldSystem->followMonInteraction->form = GetMonData(mon, MON_DATA_FORM, NULL);
    FollowMonSituation_SetMonCondition(mon, situation);
    FollowMonSituation_SetNearbyObjects(fieldSystem, situation);
    FollowMonSituation_SetHiddenItems(fieldSystem, situation);
    FollowMonSituation_SetWeather(fieldSystem, situation);
    FollowMonSituation_SetTerrain(fieldSystem, situation);
    FollowMonSituation_SetMapId(fieldSystem, situation);
    FollowMonSituation_SetTimeOfDay(fieldSystem, situation);
    FollowMonSituation_SetMood(fieldSystem, situation);
    FollowMonSituation_SetBestPokeathlonStat(fieldSystem, fieldSystem->followMonInteraction->species, fieldSystem->followMonInteraction->form, situation);
    FollowMonSituation_SetFacing(fieldSystem, situation);
    FollowMonSituation_SetSpeciesAttr(fieldSystem->followMonInteraction->species, situation);
}

#pragma force_active on

BOOL FollowMonRule_Matches(FieldSystem *fieldSystem, const FollowMonRule *rule, const FollowMonSituation *situation) {
    u16 flag;
    u32 nearbyObject;

    if ((int)LCRandom() % 100 >= rule->chance) {
        return FALSE;
    }
    flag = rule->flag;
    if (flag && !Save_VarsFlags_CheckFlagInArray(Save_VarsFlags_Get(fieldSystem->saveData), flag)) {
        return FALSE;
    }
    if (rule->heldItem) {
        if (rule->heldItem == 9) {
            if (!situation->hasHeldItem) {
                return FALSE;
            }
        } else if (rule->heldItem != situation->heldItemPocket) {
            return FALSE;
        }
    }
    if (rule->hp && rule->hp != situation->hpBucket) {
        return FALSE;
    }
    if (rule->status) {
        if (rule->status == 7) {
            if (situation->status != 2 && situation->status != 3 && situation->status != 4 && situation->status != 5 && situation->status != 8) {
                return FALSE;
            }
        } else if (rule->status != situation->status) {
            return FALSE;
        }
    }
    if (rule->nearbyObjects) {
        if (rule->nearbyObjects == 5) {
            if (situation->nearbyObjects < 5) {
                return FALSE;
            }
        } else if (rule->nearbyObjects != situation->nearbyObjects) {
            return FALSE;
        }
    }
    if (rule->hiddenItems) {
        if (rule->hiddenItems == 4) {
            if (situation->hiddenItems < 4) {
                return FALSE;
            }
        } else if (rule->hiddenItems != situation->hiddenItems) {
            return FALSE;
        }
    }
    if (rule->level && rule->level != situation->level) {
        return FALSE;
    }
    nearbyObject = rule->nearbyRock;
    if (nearbyObject) {
        if (nearbyObject == 3 && !situation->nearBreakableRock) {
            return FALSE;
        }
        if (nearbyObject == 1 && !situation->nearRock) {
            return FALSE;
        }
        if (nearbyObject == 2) {
            return FALSE;
        }
        return FALSE;
    }
    if (rule->type && rule->type != situation->type1 && rule->type != situation->type2) {
        return FALSE;
    }
    if (rule->weather && rule->weather != situation->weather) {
        return FALSE;
    }
    if (rule->metatileBehavior && rule->metatileBehavior != situation->metatileBehavior) {
        return FALSE;
    }
    if (rule->encounterTile && rule->encounterTile != situation->encounterTile) {
        return FALSE;
    }
    if (rule->mapId && rule->mapId - 1 != situation->mapId) {
        return FALSE;
    }
    if (rule->timeOfDay && rule->timeOfDay != situation->timeOfDay) {
        return FALSE;
    }
    if (rule->mood && !FollowMonRule_CheckMood(rule->mood, situation->mood)) {
        return FALSE;
    }
    if (rule->pokeathlonStat && rule->pokeathlonStat != situation->pokeathlonStat) {
        return FALSE;
    }
    if (rule->friendship && !FollowMonRule_CheckFriendship(rule->friendship, situation->friendship)) {
        return FALSE;
    }
    if (rule->natureGroup && rule->natureGroup != situation->natureGroup) {
        return FALSE;
    }
    if (rule->speciesAttr && !FollowMonRule_CheckSpeciesAttr(rule->speciesAttr, situation->speciesAttr)) {
        return FALSE;
    }
    if (rule->shinyLeaf && !FollowMonRule_CheckShinyLeaf(rule->shinyLeaf, situation->shinyLeaves)) {
        return FALSE;
    }
    if (rule->gender && rule->gender != situation->gender) {
        return FALSE;
    }
    if (rule->facing && rule->facing != situation->facing) {
        return FALSE;
    }
    return TRUE;
}

#pragma force_active reset

void FollowMonSituation_SetMonCondition(Pokemon *mon, FollowMonSituation *situation) {
    ShinyLeafMonDataIds leafIds;
    int item;
    int hp;
    int hpPercent;
    int status;
    int level;
    int type1;
    int type2;
    int i;

    item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    if (item != ITEM_NONE) {
        situation->hasHeldItem = TRUE;
        situation->heldItemPocket = FollowMonSituation_ConvertItemPocket(GetItemAttr(item, ITEMATTR_FIELD_POCKET, HEAP_ID_FIELD2));
    } else {
        situation->hasHeldItem = FALSE;
        situation->heldItemPocket = 8;
    }
    hp = GetMonData(mon, MON_DATA_HP, NULL);
    hpPercent = hp * 100 / (int)GetMonData(mon, MON_DATA_MAX_HP, NULL);
    if (hpPercent == 100) {
        situation->hpBucket = 1;
    } else if (hpPercent >= 75) {
        situation->hpBucket = 2;
    } else if (hpPercent >= 50) {
        situation->hpBucket = 3;
    } else if (hpPercent >= 25) {
        situation->hpBucket = 4;
    } else {
        situation->hpBucket = 5;
    }
    status = GetMonData(mon, MON_DATA_STATUS, NULL);
    if (status & (MON_STATUS_PSN_MASK | MON_STATUS_TOX_MASK)) {
        situation->status = 5;
    } else if (status & MON_STATUS_SLP_MASK) {
        situation->status = 8;
    } else if (status & MON_STATUS_BRN_MASK) {
        situation->status = 2;
    } else if (status & MON_STATUS_FRZ_MASK) {
        situation->status = 3;
    } else if (status & MON_STATUS_PRZ_MASK) {
        situation->status = 4;
    } else if (status == 0) {
        situation->status = 1;
    } else {
        GF_AssertFail();
        situation->status = 1;
    }
    level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    if (level + 2 >= 50) {
        situation->level = 4;
    } else if (level - 2 <= 50) {
        situation->level = 6;
    } else {
        situation->level = 5;
    }
    type1 = GetMonData(mon, MON_DATA_TYPE_1, NULL);
    type2 = GetMonData(mon, MON_DATA_TYPE_2, NULL);
    situation->type1 = FollowMonSituation_ConvertType(type1);
    situation->type2 = FollowMonSituation_ConvertType(type2);
    situation->friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);
    situation->natureGroup = sNatureGroups[GetMonNature(mon)];
    if (GetMonData(mon, MON_DATA_GENDER, NULL) == MON_MALE) {
        situation->gender = 1;
    } else {
        situation->gender = 2;
    }
    leafIds = sShinyLeafMonDataIds;
    situation->shinyLeaves = 0;
    for (i = 0; i < 5; i++) {
        situation->shinyLeaves |= (u8)GetMonData(mon, leafIds.ids[i], NULL) << i;
    }
}

void FollowMonSituation_SetNearbyObjects(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    LocalMapObject *object;
    int i;
    int count;
    int playerX;
    int playerZ;

    situation->nearbyObjects = 0;
    situation->nearBreakableRock = 0;
    situation->nearRock = 0;
    situation->nearTree = 0;
    playerX = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    playerZ = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    count = MapObjectManager_GetObjectCount(fieldSystem->mapObjectManager);
    object = MapObjectManager_GetObjects(fieldSystem->mapObjectManager);
    for (i = 0; i < count; i++) {
        if (MapObject_CheckActive(object) == TRUE) {
            int x = MapObject_GetXCoord(object);
            int z = MapObject_GetZCoord(object);
            int dx = playerX - x;
            int dz = playerZ - z;
            int sprite = MapObject_GetSpriteID(object);
            if (sprite == SPRITE_ROCK) {
                situation->nearRock = TRUE;
            } else if (sprite == SPRITE_BREAKROCK) {
                situation->nearBreakableRock = TRUE;
            } else if (sprite == SPRITE_TREE) {
                situation->nearTree = TRUE;
            } else if (dx >= -1 && dx <= 1 && dz >= -1 && dz <= 1) {
                int id = MapObject_GetID(object);
                if (id != 0xFD && id != 0xFF) {
                    situation->nearbyObjects++;
                }
            }
        }
        MapObjectArray_NextObject2(&object);
    }
}

void FollowMonSituation_SetHiddenItems(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    int count = 0;
    BgEvent *events = Field_GetBgEvents(fieldSystem);
    int numEvents = Field_GetNumBgEvents(fieldSystem);
    int i;

    if (numEvents != 0 && events != NULL) {
        for (i = 0; i < numEvents; i++) {
            if (events->type == BG_TYPE_HIDDEN_ITEM && !FieldSystem_FlagCheck(fieldSystem, HiddenItemScriptNoToFlagId(events->scriptId))) {
                count++;
            }
            events++;
        }
    }
    situation->hiddenItems = count;
}

void FollowMonSituation_SetWeather(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    switch (LocalFieldData_GetWeatherType(Save_LocalFieldData_Get(fieldSystem->saveData))) {
    case WEATHER_SUNNY:
        situation->weather = 1;
        break;
    case WEATHER_RAIN:
        situation->weather = 3;
        break;
    default:
        situation->weather = 0;
        break;
    }
}

void FollowMonSituation_SetTerrain(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    int x = MapObject_GetXCoord(FollowMon_GetMapObject(fieldSystem));
    int z = MapObject_GetZCoord(FollowMon_GetMapObject(fieldSystem));
    u8 behavior = GetMetatileBehavior(fieldSystem, x, z);
    situation->metatileBehavior = behavior;
    fieldSystem->followMonInteraction->metatileBehavior = behavior;
    if (MetatileBehavior_CanGenerateWalkingEncounters(behavior)) {
        situation->encounterTile = 1;
    } else {
        situation->encounterTile = 2;
    }
}

void FollowMonSituation_SetMapId(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    situation->mapId = fieldSystem->location->mapId;
}

void FollowMonSituation_SetTimeOfDay(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    switch (Field_GetTimeOfDay(fieldSystem)) {
    case RTC_TIMEOFDAY_MORN:
        situation->timeOfDay = 1;
        break;
    case RTC_TIMEOFDAY_DAY:
        situation->timeOfDay = 2;
        break;
    case RTC_TIMEOFDAY_EVE:
        situation->timeOfDay = 3;
        break;
    case RTC_TIMEOFDAY_NITE:
        situation->timeOfDay = 4;
        break;
    case RTC_TIMEOFDAY_LATE:
        situation->timeOfDay = 5;
        break;
    default:
        GF_AssertFail();
        situation->timeOfDay = 0;
        break;
    }
}

void FollowMonSituation_SetMood(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    situation->mood = FieldSystem_UnkSub108_GetMonMood(fieldSystem->unk108);
}

void FollowMonSituation_SetBestPokeathlonStat(FieldSystem *fieldSystem, int species, int form, FollowMonSituation *situation) {
    PartyExtraSub aprijuice;
    struct PokeathlonPerformanceStars stars;
    int stat;
    u32 best;
    Party *party = SaveArray_Party_Get(fieldSystem->saveData);
    int slot = GetIdxOfFirstAliveMonInParty_CrashIfNone(party);

    Party_GetMonAprijuiceModifiers(party, &aprijuice, slot);
    CalcMonPokeathlonStars(&stars, Party_GetMonByIndex(party, slot), (s8 *)&aprijuice, HEAP_ID_FIELD2);
    best = ((PokeathlonStarBits *)&stars.stars)->power;
    stat = 1;
    if (best < ((PokeathlonStarBits *)&stars.stars)->stamina) {
        best = ((PokeathlonStarBits *)&stars.stars)->stamina;
        stat = 2;
    }
    if (best < ((PokeathlonStarBits *)&stars.stars)->jump) {
        best = ((PokeathlonStarBits *)&stars.stars)->jump;
        stat = 4;
    }
    if (best < ((PokeathlonStarBits *)&stars.stars)->skill) {
        best = ((PokeathlonStarBits *)&stars.stars)->skill;
        stat = 3;
    }
    if (best < ((PokeathlonStarBits *)&stars.stars)->speed) {
        stat = 5;
    }
    situation->pokeathlonStat = stat;
}

void FollowMonSituation_SetFacing(FieldSystem *fieldSystem, FollowMonSituation *situation) {
    switch (MapObject_GetFacingDirection(fieldSystem->followMon.mapObject)) {
    case DIR_NORTH:
        situation->facing = 3;
        break;
    case DIR_SOUTH:
        situation->facing = 4;
        break;
    case DIR_WEST:
        situation->facing = 2;
        break;
    case DIR_EAST:
        situation->facing = 1;
        break;
    default:
        situation->facing = 0;
        break;
    }
}

void FollowMonSituation_SetSpeciesAttr(int species, FollowMonSituation *situation) {
    u8 *attrs;
    if (species <= SPECIES_NONE || species > SPECIES_ARCEUS) {
        GF_AssertFail();
        return;
    }
    attrs = AllocAtEndAndReadWholeNarcMemberByIdPair(NARC_a_2_3_1, 0, HEAP_ID_FIELD2);
    situation->speciesAttr = attrs[species - 1];
    Heap_Free(attrs);
}

int FollowMonSituation_ConvertType(int type) {
    switch (type) {
    case TYPE_NORMAL:
        return 1;
    case TYPE_FIGHTING:
        return 7;
    case TYPE_FLYING:
        return 10;
    case TYPE_POISON:
        return 8;
    case TYPE_GROUND:
        return 9;
    case TYPE_ROCK:
        return 13;
    case TYPE_BUG:
        return 12;
    case TYPE_GHOST:
        return 14;
    case TYPE_STEEL:
        return 17;
    case TYPE_FIRE:
        return 2;
    case TYPE_WATER:
        return 3;
    case TYPE_GRASS:
        return 5;
    case TYPE_ELECTRIC:
        return 4;
    case TYPE_PSYCHIC:
        return 11;
    case TYPE_ICE:
        return 6;
    case TYPE_DRAGON:
        return 15;
    case TYPE_DARK:
        return 16;
    }
    GF_AssertFail();
    return 0;
}

int FollowMonSituation_ConvertItemPocket(int pocket) {
    switch (pocket) {
    case 0:
        return 4;
    case 1:
        return 2;
    case 2:
        return 1;
    case 3:
        return 7;
    case 4:
        return 6;
    case 5:
        return 5;
    case 6:
        return 3;
    default:
        GF_AssertFail();
        return 8;
    }
}

FollowMonInteraction *FollowMonInteraction_New(enum HeapID heapId) {
    FollowMonInteraction *interaction = Heap_Alloc(heapId, sizeof(FollowMonInteraction));
    MI_CpuFill8(interaction, 0, sizeof(FollowMonInteraction));
    return interaction;
}

void FollowMonInteraction_LoadScript(FollowMonInteraction *interaction, int script) {
    int i;

    ReadWholeNarcMemberByIdPair(interaction->steps, NARC_a_2_2_1, script - 1);
    interaction->movementState = 0;
    interaction->state = 0;
    interaction->movementIndex = 0;
    interaction->stepIndex = 0;
    interaction->movementFrame = 0;
    interaction->waitFrames = 0;
    interaction->printerId = 0;
    for (i = 0; i < 5; i++) {
        if (interaction->steps[i].movement == 0xFFFF) {
            break;
        }
    }
    interaction->stepCount = i;
}

void FollowMonInteraction_Delete(FollowMonInteraction *interaction) {
    Heap_Free(interaction);
}

static inline int FollowMonInteraction_Finish(FieldSystem *fieldSystem, FollowMonInteraction *interaction) {
    FollowMon_ApplyInteractionEffects(fieldSystem, interaction);
    if (interaction->askQuestion) {
        return FOLLOW_MON_INTERACTION_QUESTION;
    }
    if (interaction->accessory) {
        return FOLLOW_MON_INTERACTION_GIVE_ACCESSORY;
    }
    if (interaction->shinyLeaf) {
        return FOLLOW_MON_INTERACTION_GIVE_SHINY_LEAF;
    }
    return FOLLOW_MON_INTERACTION_DONE;
}

static inline void FollowMonInteraction_CloseMessage(FieldSystem *fieldSystem, FollowMonInteraction *interaction) {
    ClearFrameAndWindow2(&interaction->window, FALSE);
    RemoveWindow(&interaction->window);
    String_Delete(interaction->string);
    fieldSystem->textbox_open = FALSE;
    interaction->state = 3;
}

#pragma force_active on

int FollowMonInteraction_Run(FieldSystem *fieldSystem, FollowMonInteraction *interaction) {
    FollowMonInteractionStep *step;
    u16 movement;
    u32 index = interaction->stepIndex;

    if (index >= 5) {
        return FollowMonInteraction_Finish(fieldSystem, interaction);
    }
    step = &interaction->steps[index];
    movement = step->movement;
    if (movement == 0xFFFF) {
        return FollowMonInteraction_Finish(fieldSystem, interaction);
    }
    switch (interaction->state) {
    case 0:
        if (FollowMonInteraction_StepHasMovement(interaction, step)) {
            MapObject_UnpauseMovement(fieldSystem->followMon.mapObject);
            interaction->movementState = 0;
            interaction->state = 5;
            break;
        }
    case 1:
        if (FollowMonInteraction_TryShowEmote(fieldSystem, interaction, step)) {
            MapObject_UnpauseMovement(fieldSystem->followMon.mapObject);
            interaction->state = 2;
            break;
        }
    case 2:
        if (FollowMon_TryPrintInteractionMessage(fieldSystem, interaction, step)) {
            MapObject_PauseMovement(fieldSystem->followMon.mapObject);
            interaction->state = 6;
            break;
        }
    case 3:
        if (FollowMonInteraction_StepHasWait(interaction, step)) {
            interaction->state = 7;
            break;
        }
    case 4:
        interaction->stepIndex = (u8)(interaction->stepIndex + 1);
        interaction->state = 0;
        break;
    case 5:
        if (FollowMonInteraction_RunMovement(fieldSystem, interaction, movement)) {
            interaction->state = 1;
        }
        break;
    case 6:
        if (DialogBox_IsPrintFinished((u8)interaction->printerId) == TRUE) {
            if (interaction->stepCount > interaction->stepIndex + 1) {
                if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                    FollowMonInteraction_CloseMessage(fieldSystem, interaction);
                }
            } else {
                if (interaction->askQuestion) {
                    interaction->state = 3;
                } else if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                    FollowMonInteraction_CloseMessage(fieldSystem, interaction);
                }
            }
        }
        break;
    case 7:
        if (interaction->waitFrames >= step->wait) {
            interaction->stepIndex = (u8)(interaction->stepIndex + 1);
            interaction->state = 0;
        } else {
            interaction->waitFrames++;
        }
        break;
    }
    return FOLLOW_MON_INTERACTION_RUNNING;
}

#pragma force_active reset

BOOL FollowMonInteraction_StepHasMovement(FollowMonInteraction *interaction, FollowMonInteractionStep *step) {
    return step->movement != 0;
}

BOOL FollowMonInteraction_TryShowEmote(FieldSystem *fieldSystem, FollowMonInteraction *interaction, FollowMonInteractionStep *step) {
    u8 emote;
    if (sub_0205BB04((u8)fieldSystem->followMonInteraction->metatileBehavior) != FALSE) {
        return FALSE;
    }
    emote = step->emote;
    if (emote != 0) {
        if (emote > 14) {
            return FALSE;
        }
        FieldSystem_ShowEmoteBubble(fieldSystem, fieldSystem->followMon.mapObject, emote - 1);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL FollowMon_TryPrintInteractionMessage(FieldSystem *fieldSystem, FollowMonInteraction *interaction, FollowMonInteractionStep *step) {
    Options *options;
    if (step->message != 0) {
        interaction->string = String_New(1024, HEAP_ID_FIELD2);
        DialogBox_AddWindowToLayer3(fieldSystem->bgConfig, &interaction->window, GF_BG_LYR_MAIN_3);
        FollowMon_ExpandInteractionMessage(fieldSystem, interaction->string, HEAP_ID_FIELD2, step->message - 1);
        options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
        DialogBox_LoadFrame(&interaction->window, options);
        interaction->printerId = DialogBox_PrintMessage(&interaction->window, interaction->string, options, TRUE);
        fieldSystem->textbox_open = TRUE;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void FollowMonInteraction_PrintGiftMessage(FieldSystem *fieldSystem, FollowMonInteraction *interaction, int flags) {
    Options *options;
    interaction->string = String_New(1024, HEAP_ID_FIELD2);
    DialogBox_AddWindowToLayer3(fieldSystem->bgConfig, &interaction->window, GF_BG_LYR_MAIN_3);
    FollowMon_ExpandGiftMessage(fieldSystem, interaction->string, HEAP_ID_FIELD2, flags, interaction->accessory);
    options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    DialogBox_LoadFrame(&interaction->window, options);
    interaction->printerId = DialogBox_PrintMessage(&interaction->window, interaction->string, options, TRUE);
    fieldSystem->textbox_open = TRUE;
}

BOOL FollowMonInteraction_StepHasWait(FollowMonInteraction *interaction, FollowMonInteractionStep *step) {
    interaction->waitFrames = 0;
    return step->wait != 0;
}

void FollowMon_ExpandInteractionMessage(FieldSystem *fieldSystem, String *dest, enum HeapID heapId, int msgNo) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0265_bin, heapId);
    MessageFormat *messageFormat = MessageFormat_New(heapId);
    String *string;
    FollowMon_PlaceholdersSet(fieldSystem, messageFormat);
    string = NewString_ReadMsgData(msgData, msgNo);
    StringExpandPlaceholders(messageFormat, dest, string);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
}

// flags: bit 1 set for an accessory, clear for a shiny leaf; bit 0 set when it was given.
void FollowMon_ExpandGiftMessage(FieldSystem *fieldSystem, String *dest, enum HeapID heapId, int flags, u8 accessory) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0040_bin, heapId);
    MessageFormat *messageFormat = MessageFormat_New(heapId);
    String *string;
    int msgNo;
    if (flags & 2) {
        BufferPlayersName(messageFormat, 0, Save_PlayerData_GetProfile(fieldSystem->saveData));
        if (flags & 1) {
            BufferFashionName(messageFormat, 1, accessory - 1);
            msgNo = 0x20;
        } else {
            BufferFashionNameWithArticle(messageFormat, 1, accessory - 1);
            msgNo = 0x5F;
        }
    } else {
        Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
        BoxPokemon *boxMon = Mon_GetBoxMon(mon);
        BufferPlayersName(messageFormat, 0, Save_PlayerData_GetProfile(fieldSystem->saveData));
        BufferBoxMonNickname(messageFormat, 1, boxMon);
        msgNo = (flags & 1) ? 0x61 : 0x62;
    }
    string = NewString_ReadMsgData(msgData, msgNo);
    StringExpandPlaceholders(messageFormat, dest, string);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
}

void FollowMonMovement_ApplyOffset(const FollowMonMovementStep *move, LocalMapObject *object) {
    VecFx32 pos;
    int species;
    MapObject_CopyPositionVector(object, &pos);
    if (move->dx != 0) {
        pos.x += move->dx << FX32_SHIFT;
    }
    if (move->unk3 != 0) {
        species = FollowMon_GetSpecies(object);
        if (species != SPECIES_DIGLETT && species != SPECIES_DUGTRIO) {
            FollowMonBillboard_AddHeight(object, move->unk3);
        }
    }
    if (move->dz != 0) {
        pos.z += move->dz << FX32_SHIFT;
    }
    MapObject_SetPositionVector(object, &pos);
}

void FollowMonMovement_PlaySound(const FollowMonMovementStep *move, u16 sound, u16 species, u8 form) {
    if (move->playSound == 0 || sound == 0) {
        return;
    }
    if (sound > SEQ_SE_END) {
        PlayCryEx(sound == SEQ_SE_END + 1 ? 0 : 11, species, 0x1FF, 0x1FF, 0x1FF, form);
    } else {
        PlaySE(sound);
    }
}

void FollowMonMovement_ApplyFacing(FollowMonInteraction *interaction, const FollowMonMovementStep *move, LocalMapObject *object) {
    if (move->facing != 0) {
        u8 direction = MapObject_GetFacingDirection(object);
        MapObject_SetFacingDirectionDirect(object, move->facing - 1);
        FollowMon_TryShakeGrass(interaction, object, direction);
    }
}

// When a large following Pokemon turned while facing east or west, rustle the grass beside it.
void FollowMon_TryShakeGrass(FollowMonInteraction *interaction, LocalMapObject *object, u8 oldDirection) {
    int x;
    int z;
    int behavior;
    FieldSystem *fieldSystem;
    if (!FollowMonObject_IsLarge(object) || oldDirection == (u8)MapObject_GetFacingDirection(object)) {
        return;
    }
    if (interaction->savedFacing != DIR_WEST && interaction->savedFacing != DIR_EAST) {
        return;
    }
    fieldSystem = MapObject_GetFieldSystem(object);
    FollowMon_GetCoordsInDirection(object, interaction->savedFacing, &x, &z);
    behavior = GetMetatileBehavior(fieldSystem, x, z);
    if (MetatileBehavior_IsTallGrass((u8)behavior) == TRUE) {
        TallGrass_SpawnAt(object, 0, x, z, 1);
    } else if (MetatileBehavior_IsVeryTallGrass((u8)behavior) == TRUE) {
        VeryTallGrass_SpawnAt(object, 0, x, z, 1);
    }
}

void FollowMon_GetCoordsInDirection(LocalMapObject *object, int direction, int *x, int *z) {
    *x = MapObject_GetXCoord(object);
    *z = MapObject_GetZCoord(object);
    switch (direction) {
    case DIR_NORTH:
        (*z)++;
        break;
    case DIR_SOUTH:
        (*z)--;
        break;
    case DIR_WEST:
        (*x)++;
        break;
    case DIR_EAST:
        (*x)--;
        break;
    default:
        GF_AssertFail();
        break;
    }
}

BOOL FollowMonMovement_Step(FollowMonInteraction *interaction, LocalMapObject *object) {
    FollowMonMovementStep *move = &interaction->movements[interaction->movementIndex];
    if (interaction->movementFrame == 0) {
        FollowMonMovement_ApplyOffset(move, object);
        FollowMonMovement_PlaySound(move, interaction->steps[interaction->stepIndex].sound, interaction->species, interaction->form);
        FollowMonMovement_ApplyFacing(interaction, move, object);
    }
    interaction->movementFrame++;
    if (interaction->movementFrame >= move->duration) {
        interaction->movementIndex++;
        return TRUE;
    }
    return FALSE;
}

BOOL FollowMonMovement_HasNextStep(FollowMonInteraction *interaction) {
    if (interaction->movementIndex >= 10) {
        return FALSE;
    }
    if (interaction->movements[interaction->movementIndex].facing == 0xFF) {
        return FALSE;
    }
    interaction->movementFrame = 0;
    return TRUE;
}

BOOL FollowMonInteraction_RunMovement(FieldSystem *fieldSystem, FollowMonInteraction *interaction, int movement) {
    u8 direction;
    if (movement == 0) {
        GF_AssertFail();
        return TRUE;
    }
    switch (interaction->movementState) {
    case 0:
        ReadWholeNarcMemberByIdPair(interaction->movements, NARC_a_2_2_2, movement - 1);
    case 1:
        MapObject_CopyPositionVector(fieldSystem->followMon.mapObject, &interaction->savedPosition);
        interaction->savedFacing = MapObject_GetFacingDirection(fieldSystem->followMon.mapObject);
        FollowMonBillboard_SetHeight(fieldSystem->followMon.mapObject, 0);
        FollowMonBillboard_SetShadowAnchored(fieldSystem->followMon.mapObject, TRUE);
        interaction->movementState = 2;
        interaction->movementIndex = 0;
        break;
    case 2:
        if (!FollowMonMovement_HasNextStep(interaction)) {
            interaction->movementState = 4;
            break;
        }
        interaction->movementState = 3;
    case 3:
        if (FollowMonMovement_Step(interaction, fieldSystem->followMon.mapObject)) {
            interaction->movementState = 2;
        }
        break;
    case 4:
        direction = MapObject_GetFacingDirection(fieldSystem->followMon.mapObject);
        FollowMonBillboard_SetHeight(fieldSystem->followMon.mapObject, 0);
        FollowMonBillboard_SetShadowAnchored(fieldSystem->followMon.mapObject, FALSE);
        MapObject_SetPositionVector(fieldSystem->followMon.mapObject, &interaction->savedPosition);
        MapObject_SetFacingDirectionDirect(fieldSystem->followMon.mapObject, interaction->savedFacing);
        FollowMon_TryShakeGrass(interaction, fieldSystem->followMon.mapObject, direction);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

static inline void FieldSystem_CloseFollowMonMessage(FieldSystem *fieldSystem) {
    ClearFrameAndWindow2(&fieldSystem->followMonInteraction->window, FALSE);
    RemoveWindow(&fieldSystem->followMonInteraction->window);
    String_Delete(fieldSystem->followMonInteraction->string);
    fieldSystem->textbox_open = FALSE;
}

BOOL Task_FollowMonInteract(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);

    switch (*state) {
    case 0:
        FollowMonInteraction_LoadScript(fieldSystem->followMonInteraction, FollowMon_SelectInteraction(fieldSystem));
        (*state)++;
        break;
    case 1: {
        int result = FollowMonInteraction_Run(fieldSystem, fieldSystem->followMonInteraction);
        if (result == FOLLOW_MON_INTERACTION_DONE) {
            MapObject_PauseMovement(fieldSystem->followMon.mapObject);
            return TRUE;
        }
        if (result == FOLLOW_MON_INTERACTION_QUESTION) {
            fieldSystem->followMonInteraction->state = 10;
            *state = 2;
        } else if (result == FOLLOW_MON_INTERACTION_GIVE_ACCESSORY) {
            *state = 3;
        } else if (result == FOLLOW_MON_INTERACTION_GIVE_SHINY_LEAF) {
            *state = 4;
        }
        break;
    }
    case 2:
        switch (fieldSystem->followMonInteraction->state) {
        case 10:
            FieldSystem_SwitchBottomScreenApp(fieldSystem, 3, 0);
            fieldSystem->followMonInteraction->state = 11;
            break;
        case 11: {
            int a = FieldSystem_GetBottomScreenAppId(fieldSystem);
            int b = FieldSystem_GetBottomScreenAppState(fieldSystem);
            if (a == 3 && b == 1) {
                FieldSystem_SetTouchscreenChoiceMenuState(fieldSystem, 3, 3, &fieldSystem->followMonInteraction->answer);
                fieldSystem->followMonInteraction->state = 12;
            }
            break;
        }
        case 12: {
            int a = FieldSystem_GetBottomScreenAppId(fieldSystem);
            int b = FieldSystem_GetTouchscreenChoiceMenuState(fieldSystem);
            if (a == 3 && b == 6) {
                FieldSystem_SwitchBottomScreenApp(fieldSystem, 0, 0);
                fieldSystem->followMonInteraction->state = 13;
            }
            break;
        }
        case 13: {
            int a = FieldSystem_GetBottomScreenAppId(fieldSystem);
            int b = FieldSystem_GetBottomScreenAppState(fieldSystem);
            if (a == 0 && b == 1) {
                switch (fieldSystem->followMonInteraction->answer) {
                case 0:
                    FieldSystem_CloseFollowMonMessage(fieldSystem);
                    if (!fieldSystem->followMonInteraction->yesScript) {
                        MapObject_PauseMovement(fieldSystem->followMon.mapObject);
                        return TRUE;
                    }
                    FollowMonInteraction_LoadScript(fieldSystem->followMonInteraction, fieldSystem->followMonInteraction->yesScript);
                    *state = 1;
                    break;
                case 1:
                    FieldSystem_CloseFollowMonMessage(fieldSystem);
                    if (!fieldSystem->followMonInteraction->noScript) {
                        MapObject_PauseMovement(fieldSystem->followMon.mapObject);
                        return TRUE;
                    }
                    FollowMonInteraction_LoadScript(fieldSystem->followMonInteraction, fieldSystem->followMonInteraction->noScript);
                    *state = 1;
                    break;
                }
            }
            break;
        }
        }
        break;
    case 3: {
        FashionCase *fashionCase = Save_FashionData_GetFashionCase(Save_FashionData_Get(fieldSystem->saveData));
        int accessory = fieldSystem->followMonInteraction->accessory - 1;
        if (accessory < 0 || accessory >= 100) {
            GF_AssertFail();
        }
        if (FashionCase_CanGiveAccessory(fashionCase, accessory, 1)) {
            FashionCase_GiveFashionItem(fashionCase, accessory, 1);
            FollowMonInteraction_PrintGiftMessage(fieldSystem, fieldSystem->followMonInteraction, 3);
            PlayFanfare(SEQ_ME_ACCE);
        } else {
            FollowMonInteraction_PrintGiftMessage(fieldSystem, fieldSystem->followMonInteraction, 2);
        }
        *state = 5;
        break;
    }
    case 4: {
        int attr;
        Pokemon *mon;
        switch (fieldSystem->followMonInteraction->shinyLeaf) {
        case 1:
            attr = MON_DATA_SHINY_LEAF_A;
            break;
        case 2:
            attr = MON_DATA_SHINY_LEAF_B;
            break;
        case 3:
            attr = MON_DATA_SHINY_LEAF_C;
            break;
        case 4:
            attr = MON_DATA_SHINY_LEAF_D;
            break;
        case 5:
            attr = MON_DATA_SHINY_LEAF_E;
            break;
        default:
            GF_AssertFail();
            return TRUE;
        }
        mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
        if (!(u8)GetMonData(mon, attr, NULL)) {
            u8 value;
            SetFlag99C(Save_VarsFlags_Get(fieldSystem->saveData));
            value = TRUE;
            SetMonData(mon, attr, &value);
            FollowMonInteraction_PrintGiftMessage(fieldSystem, fieldSystem->followMonInteraction, 1);
            PlayFanfare(SEQ_ME_ACCE);
        } else {
            FollowMonInteraction_PrintGiftMessage(fieldSystem, fieldSystem->followMonInteraction, 0);
        }
        *state = 5;
        break;
    }
    case 5:
        if (DialogBox_IsPrintFinished((u8)fieldSystem->followMonInteraction->printerId) == TRUE && !IsFanfarePlaying() && (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            FieldSystem_CloseFollowMonMessage(fieldSystem);
            MapObject_PauseMovement(fieldSystem->followMon.mapObject);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

void FollowMon_PlaceholdersSet(FieldSystem *fieldSystem, MessageFormat *messageFormat) {
    Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
    BoxPokemon *boxMon = Mon_GetBoxMon(mon);
    BufferBoxMonNickname(messageFormat, 0, boxMon);
    BufferBoxMonSpeciesName(messageFormat, 1, boxMon);
    BufferPlayersName(messageFormat, 2, Save_PlayerData_GetProfile(fieldSystem->saveData));
    BufferLocationName(messageFormat, 3, MapHeader_GetMapSec(fieldSystem->location->mapId));
    BufferItemName(messageFormat, 4, GetMonData(mon, MON_DATA_HELD_ITEM, NULL));
}

void FollowMon_ApplyInteractionEffects(FieldSystem *fieldSystem, FollowMonInteraction *interaction) {
    Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
    int mood = FieldSystem_UnkSub108_GetMonMood(fieldSystem->unk108);
    int value = mood;

    value += fieldSystem->followMonInteraction->moodChange;
    if (value > 127) {
        value = 127;
    } else if (value < -127) {
        value = -127;
    }
    FieldSystem_UnkSub108_SetMonMood(fieldSystem->unk108, (s8)value);
    value = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);
    value += fieldSystem->followMonInteraction->friendshipChange;
    if (value > 255) {
        value = 255;
    } else if (value < 0) {
        value = 0;
    }
    SetMonData(mon, MON_DATA_FRIENDSHIP, &value);
}

BOOL FollowMonRule_CheckFriendship(int rule, int friendship) {
    switch (rule) {
    case 1:
        if (friendship != 255) {
            break;
        }
        return TRUE;
    case 2:
        if (friendship < 200) {
            break;
        }
        if (friendship >= 255) {
            break;
        }
        return TRUE;
    case 3:
        if (friendship < 150) {
            break;
        }
        if (friendship >= 200) {
            break;
        }
        return TRUE;
    case 4:
        if (friendship < 90) {
            break;
        }
        if (friendship >= 150) {
            break;
        }
        return TRUE;
    case 5:
        if (friendship < 60) {
            break;
        }
        if (friendship >= 90) {
            break;
        }
        return TRUE;
    case 6:
        if (friendship < 30) {
            break;
        }
        if (friendship >= 60) {
            break;
        }
        return TRUE;
    case 7:
        if (friendship < 1) {
            break;
        }
        if (friendship >= 30) {
            break;
        }
        return TRUE;
    case 8:
        if (friendship != 0) {
            break;
        }
        return TRUE;
    case 9:
        if (friendship < 90) {
            break;
        }
        return TRUE;
    case 10:
        if (friendship >= 60) {
            break;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL FollowMonRule_CheckMood(int rule, int mood) {
    switch (rule) {
    case 1:
        if (mood != 127) {
            break;
        }
        return TRUE;
    case 2:
        if (mood < 100) {
            break;
        }
        if (mood >= 127) {
            break;
        }
        return TRUE;
    case 3:
        if (mood < 50) {
            break;
        }
        if (mood >= 100) {
            break;
        }
        return TRUE;
    case 4:
        if (mood < 30) {
            break;
        }
        if (mood >= 50) {
            break;
        }
        return TRUE;
    case 5:
        if (mood <= -30) {
            break;
        }
        if (mood >= 30) {
            break;
        }
        return TRUE;
    case 6:
        if (mood <= -50) {
            break;
        }
        if (mood > -30) {
            break;
        }
        return TRUE;
    case 7:
        if (mood <= -127) {
            break;
        }
        if (mood > -50) {
            break;
        }
        return TRUE;
    case 8:
        if (mood != -127) {
            break;
        }
        return TRUE;
    case 9:
        if (mood < 0) {
            break;
        }
        return TRUE;
    case 10:
        if (mood > -1) {
            break;
        }
        return TRUE;
    }
    return FALSE;
}

// Rules up to 249 ask for that exact value; 250-254 ask for a range.
BOOL FollowMonRule_CheckSpeciesAttr(u32 rule, u32 value) {
    if (rule <= 249) {
        if (rule == value) {
            return TRUE;
        }
    } else {
        switch (rule) {
        case 250:
            if (value > 19) {
                break;
            }
            return TRUE;
        case 251:
            if (value > 130) {
                break;
            }
            return TRUE;
        case 252:
            if (value < 140) {
                break;
            }
            if (value > 149) {
                break;
            }
            return TRUE;
        case 253:
            if (value < 160) {
                break;
            }
            return TRUE;
        case 254:
            if (value < 220) {
                break;
            }
            return TRUE;
        default:
            GF_AssertFail();
            return FALSE;
        }
    }
    return FALSE;
}

#pragma force_active on

// TRUE when the Pokemon does not have shiny leaf `leaf` (1-5) yet.
BOOL FollowMonRule_CheckShinyLeaf(int leaf, u8 shinyLeaves) {
    ShinyLeafMasks masks = sShinyLeafMasks;
    int i;
    for (i = 0; i < 5; i++) {
        if ((masks.masks[leaf - 1] & shinyLeaves) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active reset

BOOL FieldSystem_FirstAliveMonHasType(FieldSystem *fieldSystem, u8 type) {
    Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
    int type1 = GetMonData(mon, MON_DATA_TYPE_1, NULL);
    int type2 = GetMonData(mon, MON_DATA_TYPE_2, NULL);
    if (type1 == type || type2 == type) {
        return TRUE;
    }
    return FALSE;
}

void ov02_022507B4(FieldSystem *fieldSystem, u8 count) {
    if ((u32)(FollowMon_GetSpecies(fieldSystem->followMon.mapObject) - SPECIES_DIGLETT) > 1) {
        Ov02_022507E8Env *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(Ov02_022507E8Env));
        env->count = count;
        env->done = 0;
        TaskManager_Call(fieldSystem->taskman, ov02_022507E8, env);
    }
}

BOOL ov02_022507E8(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    Ov02_022507E8Env *env = TaskManager_GetEnvironment(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);

    switch (*state) {
    case 0:
        MapObject_UnpauseMovement(fieldSystem->followMon.mapObject);
        (*state)++;
    case 1:
        if (MapObject_AreBitsSetForMovementScriptInit(FollowMon_GetMapObject(fieldSystem))) {
            MapObject_PauseMovement(fieldSystem->followMon.mapObject);
            (*state)++;
        }
        break;
    case 2: {
        u8 direction = MapObject_GetFacingDirection(fieldSystem->followMon.mapObject);
        env->movement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, (const MovementScriptCommand *)(ov02_02253A70 + direction * 0x14));
        (*state)++;
        break;
    }
    case 3:
        if (EventObjectMovementMan_IsFinish(env->movement) == TRUE) {
            EventObjectMovementMan_Delete(env->movement);
            env->done++;
            if (env->done >= env->count) {
                (*state)++;
            } else {
                *state = 0;
            }
        }
        break;
    case 4:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

BOOL ov02_022508B4(FieldSystem *fieldSystem) {
    Ov02_022508D8Env *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(Ov02_022508D8Env));
    env->wait = 0;
    env->frame = 0;
    TaskManager_Call(fieldSystem->taskman, ov02_022508D8, env);
    return TRUE;
}

// Grows the billboard to 1.5 times its size over 10 frames, holds it for 10 frames, then shrinks it back.
BOOL ov02_022508D8(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);
    Ov02_022508D8Env *env = TaskManager_GetEnvironment(taskManager);

    switch (*state) {
    case 0: {
        VecFx32 scale;
        scale = ov02_02253B24;
        env->frame++;
        scale.x = (s32)((double)env->frame * 2048.0 / 10.0 + 4096.0);
        scale.y = (s32)((double)env->frame * 2048.0 / 10.0 + 4096.0);
        BillboardObject_SetScale(MapObjectManager_GetFollowMonBillboard(fieldSystem->mapObjectManager), &scale);
        if (env->frame >= 10) {
            env->frame = 0;
            env->wait = 0;
            (*state)++;
        }
        break;
    }
    case 1:
        env->wait++;
        if (env->wait >= 10) {
            (*state)++;
        }
        break;
    case 2: {
        VecFx32 scale;
        scale = ov02_02253B30;
        env->frame++;
        if (env->frame >= 10) {
            (*state)++;
        } else {
            scale.x = (s32)((double)(10 - env->frame) * 2048.0 / 10.0 + 4096.0);
            scale.y = (s32)((double)(10 - env->frame) * 2048.0 / 10.0 + 4096.0);
        }
        BillboardObject_SetScale(MapObjectManager_GetFollowMonBillboard(fieldSystem->mapObjectManager), &scale);
        break;
    }
    case 3:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
