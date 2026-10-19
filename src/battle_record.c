#include "battle_record.h"

#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"

#include "battle/battle_setup.h"

#include "battle_recording.h"
#include "frontier_save_access.h"
#include "link_ruleset_data.h"
#include "math_util.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save_arrays.h"
#include "system.h"

#define BATTLE_RECORD_MAGIC 0xE281

// Battler order used for the header's species list.
typedef struct {
    u8 order[2][4];
} BattlerOrder;

typedef struct {
    u8 order[4];
} MultiOrder;

// gBattleRecord->header: what the battle video lists show.
typedef struct {
    u16 species[12];
    u8 forms[12];
    u16 arg;
    u8 type, sealed;
    LinkBattleRuleset rules;
    u16 magic;
    u8 unk_4A[14];
    u64 videoNumber;
    u16 crc;
    u16 padding;
} BattleRecordHeader;

typedef struct {
    u16 capacity, count;
    struct BattleRecordMon mons[6];
} BattleRecordParty;

// gBattleRecord->data, encrypted while stored: the BattleSetup, the parties and every battler's inputs.
typedef struct {
    u32 flags; // battle type
    int winFlag;
    int trainerId[4];
    Trainer trainer[4];
    BattleBg battleBg;
    Terrain terrain;
    u32 mapSection, mapNumber;
    TIMEOFDAY timeOfDay;
    u32 evolutionLocation, unk_100;
    BOOL metBill;
    u32 caughtBattlerId, weatherType;
    int levelUpFlag;
    u32 aiFlags[4];
    u32 battleSpecial;
    int safariBalls;
    BOOL fixedDamageMovesBanned;
    int randSeed;
    int positions[4];
    u16 player, unk_146;
    int totalTurns;
    u8 chatot[4];
    u8 commands[4][1024];
    BattleRecordParty parties[4];
    PlayerProfile profiles[4];
    Options options;
    u16 magic, crc;
} BattleRecordData;


static void BattleRecord_Decrypt(void *data, u32 size, u32 seed);
static BOOL BattleRecord_IsEmpty(SaveData *save, BattleRecord *record);
static BOOL BattleRecord_IsValid(SaveData *save, BattleRecord *record);
static void BattleRecord_FillHeader(SaveData *save, BattleRecordHeader *header, void *data, int type, int arg);
static void BattleRecordParty_FromParty(Party *party, BattleRecordParty *record);
static void BattleRecordParty_ToParty(BattleRecordParty *record, Party *party);

static const MultiOrder sBattleRecordMultiOrder = {
    .order = { 0, 2, 1, 3 },
};

static const BattlerOrder sBattleRecordBattlerOrder = {
    .order = { { 0, 2, 3, 1 }, { 3, 1, 0, 2 } },
};

BattleRecord *gBattleRecord;

u32 BattleRecord_sizeof(void) {
    return sizeof(BattleRecord);
}

void BattleRecord_Init(BattleRecord *record) {
    MI_CpuClear32(record, sizeof(*record));
    record->unk_0000 = -1;
}

void BattleRecord_Create(SaveData *save, enum HeapID heapId, u32 *out) {
    if (gBattleRecord != NULL) {
        Heap_Free(gBattleRecord);
        gBattleRecord = NULL;
    }
    gBattleRecord = LoadBattleRecord(save, heapId, (int *)out, 0);
    BattleRecord_Init(gBattleRecord);
}

void BattleRecord_Delete(void) {
    GF_ASSERT(gBattleRecord != NULL);
    Heap_Free(gBattleRecord);
    gBattleRecord = NULL;
}

BOOL BattleRecord_Exists(void) {
    return gBattleRecord != NULL;
}

BattleRecord *BattleRecord_Get(void) {
    GF_ASSERT(gBattleRecord != NULL);
    return gBattleRecord;
}

void *BattleRecord_GetProfile2(void) {
    GF_ASSERT(gBattleRecord != NULL);
    return gBattleRecord->profile;
}

BOOL BattleRecord_Load(SaveData *save, enum HeapID heapId, int *out, void *battle, int index) {
    if (gBattleRecord != NULL) {
        Heap_Free(gBattleRecord);
        gBattleRecord = NULL;
    }
    gBattleRecord = LoadBattleRecord(save, heapId, out, index);
    if (*out != LOAD_STATUS_IS_GOOD) {
        *out = LOAD_STATUS_TOTAL_FAIL;
        return TRUE;
    }
    BattleRecordData *data = (BattleRecordData *)gBattleRecord->data;
    u32 crc = data->crc;
    BattleRecord_Decrypt(data, 0x1c64, crc + ((crc ^ 0xffff) << 16));
    if (BattleRecord_IsEmpty(save, gBattleRecord) == TRUE) {
        *out = LOAD_STATUS_NOT_EXIST;
        return TRUE;
    }
    if (!BattleRecord_IsValid(save, gBattleRecord)) {
        *out = LOAD_STATUS_SLOT_FAIL;
        return TRUE;
    }
    if (battle != NULL) {
        BattleRecord_RestoreSetup(battle, save);
    }
    *out = LOAD_STATUS_IS_GOOD;
    return TRUE;
}

BOOL BattleRecord_IsSlotValid(SaveData *save, enum HeapID heapId, int *out, int index) {
    BattleRecord *record = LoadBattleRecord(save, heapId, out, index);
    if (*out != LOAD_STATUS_IS_GOOD) {
        *out = LOAD_STATUS_TOTAL_FAIL;
        Heap_Free(record);
        return FALSE;
    }
    BattleRecordData *data = (BattleRecordData *)record->data;
    u32 crc = data->crc;
    BattleRecord_Decrypt(data, 0x1c64, crc + ((crc ^ 0xffff) << 16));
    if (BattleRecord_IsEmpty(save, record) == TRUE) {
        *out = LOAD_STATUS_NOT_EXIST;
        Heap_Free(record);
        return FALSE;
    }
    if (!BattleRecord_IsValid(save, record)) {
        *out = LOAD_STATUS_SLOT_FAIL;
        Heap_Free(record);
        return FALSE;
    }
    *out = LOAD_STATUS_IS_GOOD;
    Heap_Free(record);
    return TRUE;
}

int BattleRecord_WriteSlot(SaveData *save, BattleRecord *record, int index, u16 *state) {
    switch (*state) {
    case 0: {
        Sys_SetSoftResetDisableFlag(8);
        Sys_SetAsyncSaveMarker(HEAP_ID_FIELD2);
        int result = SaveBattleRecord(save, record, index);
        if (result == WRITE_STATUS_SUCCESS) {
            Save_PrepareForAsyncWrite(save, 2);
            (*state)++;
            return WRITE_STATUS_CONTINUE;
        }
        Sys_ClearSoftResetDisableFlag(8);
        return result;
    }
    case 1: {
        int result = Save_WriteFileAsync(save);
        if (result == WRITE_STATUS_SUCCESS || result == WRITE_STATUS_TOTAL_FAIL) {
            *state = 0;
            Sys_ClearAsyncSaveMarker();
            Sys_ClearSoftResetDisableFlag(8);
        }
        return result;
    }
    }
    return WRITE_STATUS_CONTINUE;
}

int BattleRecord_Save(SaveData *save, int type, int arg, int index, u16 *state, u16 *writeState) {
    switch (*state) {
    case 0: {
        if (gBattleRecord == NULL) {
            return WRITE_STATUS_TOTAL_FAIL;
        }
        BattleRecordHeader *header = (BattleRecordHeader *)gBattleRecord->header;
        BattleRecordData *data = (BattleRecordData *)gBattleRecord->data;
        BattleRecord_FillHeader(save, header, data, type, arg);
        header->magic = BATTLE_RECORD_MAGIC;
        header->crc = SaveArray_CalcCRC16(save, header, offsetof(BattleRecordHeader, videoNumber));
        data->magic = BATTLE_RECORD_MAGIC;
        data->crc = SaveArray_CalcCRC16(save, data, 0x1c64);
        u32 crc = data->crc;
        BattleRecord_Encrypt(data, 0x1c64, crc + ((crc ^ 0xffff) << 16));
        *writeState = 0;
        (*state)++;
        break;
    }
    case 1:
        return BattleRecord_WriteSlot(save, gBattleRecord, index, writeState);
    }
    return WRITE_STATUS_CONTINUE;
}

void BattleRecord_GetBattlerAndPartyCount(int type, int *count, int *last) {
    switch (type) {
    case 14:
    case 17:
    case 20:
    case 23:
    case 26:
    case 29:
    case 32:
        *count = 4;
        *last = 3;
        break;
    default:
        *count = 2;
        *last = 6;
        break;
    }
}

static void BattleRecord_FillHeader(SaveData *save, BattleRecordHeader *header, void *recordData, int type, int arg) {
    BattleRecordData *data = recordData;
    int count, partySize;
    BattlerOrder order = sBattleRecordBattlerOrder;
    MultiOrder multi = sBattleRecordMultiOrder;
    int player;
    int k;
    MI_CpuFill8(header, 0, sizeof(*header));
    BattleRecord_GetBattlerAndPartyCount(type, &count, &partySize);
    k = 0;
    if (data->flags & BATTLE_TYPE_LINK) {
        if (data->flags & BATTLE_TYPE_FRONTIER) {
            player = data->player * 2;
        } else {
            player = data->player;
        }
    } else {
        player = 0;
    }
    for (int i = 0; i < count; i++) {
        int battler;
        if ((data->flags & BATTLE_TYPE_MULTI) && !(data->flags & BATTLE_TYPE_FRONTIER)) {
            for (battler = 0; battler < count; battler++) {
                if (data->positions[battler] == order.order[data->positions[player] & 1][i]) {
                    break;
                }
            }
        } else if ((data->flags & BATTLE_TYPE_MULTI) && (data->flags & BATTLE_TYPE_FRONTIER)) {
            battler = multi.order[i];
        } else {
            battler = i;
            if (player & 1) {
                battler = i ^ 1;
            }
        }
        for (int j = 0; j < partySize; j++) {
            struct BattleRecordMon *mon = &data->parties[battler].mons[j];
            if (!mon->isEgg && !mon->checksumFailed) {
                header->species[k] = mon->species;
                header->forms[k] = mon->form;
            }
            k++;
        }
    }
    switch (type) {
    case 1:
    case 8:
        header->rules = *LinkBattleRuleset_GetRegulation(save, 0);
        break;
    case 2:
    case 9:
        header->rules = *LinkBattleRuleset_GetRegulation(save, 1);
        break;
    case 3:
    case 10:
        header->rules = *LinkBattleRuleset_GetRegulation(save, 2);
        break;
    case 4:
    case 11:
        header->rules = *LinkBattleRuleset_GetRegulation(save, 3);
        break;
    case 5:
    case 12:
        header->rules = *LinkBattleRuleset_GetRegulation(save, 4);
        break;
    case 6:
    case 13:
        header->rules = *LinkBattleRuleset_GetRegulation(save, 5);
        break;
    default:
        header->rules = *LinkBattleRuleset_GetDefault();
        break;
    }
    header->arg = arg;
    header->type = type;
}

static BOOL BattleRecord_IsEmpty(SaveData *save, BattleRecord *record) {
    BattleRecordData *data = (BattleRecordData *)record->data;
    BattleRecordHeader *header = (BattleRecordHeader *)record->header;
    if (!Save_CheckExtraChunksExist(save)) {
        return TRUE;
    }
    if (data->magic != BATTLE_RECORD_MAGIC || header->magic != BATTLE_RECORD_MAGIC) {
        return TRUE;
    }
    return FALSE;
}

static BOOL BattleRecord_IsValid(SaveData *save, BattleRecord *record) {
    BattleRecordData *data = (BattleRecordData *)record->data;
    BattleRecordHeader *header = (BattleRecordHeader *)record->header;
    if (data->magic != BATTLE_RECORD_MAGIC || header->magic != BATTLE_RECORD_MAGIC) {
        return FALSE;
    }
    u32 crc = SaveArray_CalcCRC16(save, header, offsetof(BattleRecordHeader, videoNumber));
    if (crc != header->crc) {
        return FALSE;
    }
    crc = SaveArray_CalcCRC16(save, data, 0x1c64);
    if (crc != data->crc) {
        return FALSE;
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            struct BattleRecordMon *mon = &data->parties[i].mons[j];
            if (mon->species > SPECIES_BAD_EGG) {
                return FALSE;
            }
            if (mon->heldItem > ITEM_MAX) {
                return FALSE;
            }
            for (int k = 0; k < 4; k++) {
                if (mon->moves[k] > NUM_MOVES) {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

void BattleRecord_Encrypt(void *data, u32 size, u32 seed) {
    _MonEncryptSegment(data, size, seed);
}

static void BattleRecord_Decrypt(void *data, u32 size, u32 seed) {
    _MonDecryptSegment(data, size, seed);
}

void BattleRecord_SetCommand(int battlerId, u32 offset, u8 value) {
    if (gBattleRecord != NULL) {
        BattleRecordData *data = (BattleRecordData *)gBattleRecord->data;
        data->commands[battlerId][offset] = value;
    }
}

u8 BattleRecord_GetCommand(int battlerId, u32 offset) {
    GF_ASSERT(gBattleRecord != NULL);
    BattleRecordData *data = (BattleRecordData *)gBattleRecord->data;
    return data->commands[battlerId][offset];
}

void BattleRecord_StoreSetup(BattleSetup *setup) {
    if (gBattleRecord == NULL) {
        return;
    }
    BattleRecordData *data = (BattleRecordData *)(gBattleRecord->data);
    data->flags = setup->battleType;
    data->winFlag = setup->winFlag;
    data->battleBg = setup->battleBg;
    data->terrain = setup->terrain;
    data->mapSection = setup->mapSection;
    data->mapNumber = setup->mapNumber;
    data->timeOfDay = setup->timeOfDay;
    data->evolutionLocation = setup->evolutionLocation;
    data->unk_100 = setup->unk_164;
    data->metBill = setup->metBill;
    data->caughtBattlerId = setup->caughtBattlerId;
    data->weatherType = setup->weatherType;
    data->levelUpFlag = setup->levelUpFlag;
    data->battleSpecial = setup->battleSpecial;
    data->safariBalls = setup->safariBalls;
    data->fixedDamageMovesBanned = setup->fixedDamaageMovesBanned;
    data->randSeed = setup->randSeed;
    data->player = setup->networkId;
    data->unk_146 = setup->unk1B3;
    data->totalTurns = setup->totalTurns;
    for (int i = 0; i < 4; i++) {
        data->trainerId[i] = setup->trainerId[i];
        data->trainer[i] = setup->trainer[i];
        if (setup->aiFlags[i] == 0) {
            data->aiFlags[i] = 320;
        } else {
            data->aiFlags[i] = setup->aiFlags[i];
        }
        data->positions[i] = setup->linkPositions[i];
        data->chatot[i] = setup->chatotVoiceParam[i];
    }
    for (int i = 0; i < 4; i++) {
        BattleRecordParty_FromParty(setup->party[i], &data->parties[i]);
        PlayerProfile_Copy(setup->profile[i], &data->profiles[i]);
        data->chatot[i] = Chatot_GetVoiceParam(setup->chatot[i]);
    }
    Options_Copy(setup->options, &data->options);
}

void BattleRecord_SetAiFlags(int battlerId, u32 flags) {
    if (gBattleRecord != NULL) {
        ((BattleRecordData *)(gBattleRecord->data))->aiFlags[battlerId] = flags;
    }
}

BOOL BattleRecord_AreAiFlagsValid(void) {
    if (gBattleRecord == NULL) {
        return TRUE;
    }
    BattleRecordData *data = (BattleRecordData *)(gBattleRecord->data);
    for (int i = 0; i < 4; i++) {
        if (data->aiFlags[i] > 320) {
            return FALSE;
        }
    }
    return TRUE;
}

void BattleRecord_RestoreSetup(BattleSetup *setup, SaveData *save) {
    BattleRecordData *data = (BattleRecordData *)(gBattleRecord->data);
    setup->battleType = data->flags;
    setup->battleBg = data->battleBg;
    setup->terrain = data->terrain;
    setup->mapSection = data->mapSection;
    setup->mapNumber = data->mapNumber;
    setup->timeOfDay = data->timeOfDay;
    setup->evolutionLocation = data->evolutionLocation;
    setup->unk_164 = data->unk_100;
    setup->metBill = data->metBill;
    setup->caughtBattlerId = data->caughtBattlerId;
    setup->weatherType = data->weatherType;
    setup->battleSpecial = data->battleSpecial | 16;
    setup->safariBalls = data->safariBalls;
    setup->fixedDamaageMovesBanned = data->fixedDamageMovesBanned;
    setup->randSeed = data->randSeed;
    setup->networkId = data->player;
    setup->winFlag = 0;
    setup->levelUpFlag = 0;
    Pokedex_Copy(Save_Pokedex_Get(save), setup->pokedex);
    for (int i = 0; i < 4; i++) {
        setup->trainerId[i] = data->trainerId[i];
        setup->trainer[i] = data->trainer[i];
        setup->aiFlags[i] = data->aiFlags[i];
        setup->linkPositions[i] = data->positions[i];
        BattleRecordParty_ToParty(&data->parties[i], setup->party[i]);
        PlayerProfile_Copy(&data->profiles[i], setup->profile[i]);
        setup->chatotVoiceParam[i] = data->chatot[i];
    }
    Options_Copy(Save_PlayerData_GetOptionsAddr(save), setup->options);
    setup->options->frame = data->options.frame;
    if (setup->options->frame >= 20) {
        setup->options->frame = 0;
    }
}

static void BattleRecordParty_FromParty(Party *party, BattleRecordParty *record) {
    MI_CpuFill8(record, 0, sizeof(*record));
    record->capacity = Party_GetMaxCount(party);
    record->count = Party_GetCount(party);
    for (int i = 0; i < record->count; i++) {
        Pokemon_ToBattleRecordMon(Party_GetMonByIndex(party, i), &record->mons[i]);
    }
}

static void BattleRecordParty_ToParty(BattleRecordParty *record, Party *party) {
    u8 value = 0;
    Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);
    Party_InitWithMaxSize(party, record->capacity);
    for (int i = 0; i < record->count; i++) {
        Pokemon_FromBattleRecordMon(&record->mons[i], mon);
        SetMonData(mon, MON_DATA_BALL_CAPSULE_ID, &value);
        Party_AddMon(party, mon);
    }
    Heap_Free(mon);
}

void *BattleRecord_CopyHeader(enum HeapID heapId) {
    GF_ASSERT(gBattleRecord != NULL);
    void *result = Heap_Alloc(heapId, sizeof(gBattleRecord->header));
    MI_CpuCopy32(gBattleRecord->header, result, sizeof(gBattleRecord->header));
    return result;
}

void *BattleRecord_CopyProfile(enum HeapID heapId) {
    GF_ASSERT(gBattleRecord != NULL);
    void *result = Heap_Alloc(heapId, sizeof(gBattleRecord->profile));
    MI_CpuCopy32(gBattleRecord->profile, result, sizeof(gBattleRecord->profile));
    return result;
}

void *BattleRecord_GetProfile(void) {
    GF_ASSERT(gBattleRecord != NULL);
    return gBattleRecord->profile;
}

void *BattleRecord_GetHeader(void) {
    GF_ASSERT(gBattleRecord != NULL);
    return gBattleRecord->header;
}

void BattleRecord_Import(void *profile, void *summary, void *data, void *battle, SaveData *save) {
    GF_ASSERT(gBattleRecord != NULL);
    MI_CpuCopy8(summary, gBattleRecord->header, sizeof(gBattleRecord->header));
    MI_CpuCopy8(data, gBattleRecord->data, sizeof(gBattleRecord->data));
    MI_CpuCopy8(profile, gBattleRecord->profile, sizeof(gBattleRecord->profile));
    u32 crc = ((BattleRecordData *)gBattleRecord->data)->crc;
    BattleRecord_Decrypt(gBattleRecord->data, 0x1c64, crc + ((crc ^ 0xffff) << 16));
    if (battle != NULL) {
        BattleRecord_RestoreSetup(battle, save);
    }
}

u64 BattleRecordHeader_GetField(void *summary, int field, int index) {
    BattleRecordHeader *header = summary;
    switch (field) {
    case 0:
        GF_ASSERT(index < 12);
        if (header->species[index] > SPECIES_ARCEUS) {
            return 0;
        }
        return header->species[index];
    case 1:
        GF_ASSERT(index < 12);
        return header->forms[index];
    case 2:
        if (header->arg > 9999) {
            return 9999;
        }
        return header->arg;
    case 3:
        if (header->type >= 33) {
            return 0;
        }
        return header->type;
    case 4:
        return header->videoNumber;
    case 5:
        return header->sealed;
    }
    GF_ASSERT(FALSE);
    return 0;
}

void *BattleRecordHeader_New(enum HeapID heapId) {
    void *result = Heap_Alloc(heapId, sizeof(BattleRecordHeader));
    MI_CpuFill8(result, 0, sizeof(BattleRecordHeader));
    return result;
}

void BattleRecordHeader_Free(void *summary) {
    Heap_Free(summary);
}

void FactoryChallenge_Clear(FactoryChallenge *challenge) {
    MI_CpuFill8(challenge, 0, sizeof(FactoryChallenge));
}

FactoryChallenge *Save_Frontier_GetFactoryChallenge(SaveData *save) {
    return (FactoryChallenge *)((u8 *)Save_Frontier_GetStatic(save) + 0x8e0);
}

u32 FactoryChallenge_GetSaved(FactoryChallenge *challenge) {
    return challenge->saved;
}

void FactoryChallenge_SetSaved(FactoryChallenge *challenge, int saved) {
    challenge->saved = (u8)saved;
}

void FactoryChallenge_SetField(FactoryChallenge *challenge, int field, u8 index, void *value) {
    switch (field) {
    case FACTORY_CHALLENGE_LEVEL_MODE:
        challenge->levelMode = *(u8 *)value;
        break;
    case FACTORY_CHALLENGE_TYPE:
        challenge->type = *(u8 *)value;
        break;
    case FACTORY_CHALLENGE_BATTLE_NO:
        challenge->battleNo = *(u8 *)value;
        break;
    case FACTORY_CHALLENGE_TRAINER_IDS:
        challenge->trainerIds[index] = *(u16 *)value;
        break;
    case FACTORY_CHALLENGE_RENTAL_SET_IDS:
        challenge->rentalSetIds[index] = *(u16 *)value;
        break;
    case FACTORY_CHALLENGE_RENTAL_IVS:
        challenge->rentalIvs[index] = *(u8 *)value;
        break;
    case FACTORY_CHALLENGE_RENTAL_PERSONALITIES:
        challenge->rentalPersonalities[index] = *(u32 *)value;
        break;
    case FACTORY_CHALLENGE_OPPONENT_SET_IDS:
        challenge->opponentSetIds[index] = *(u16 *)value;
        break;
    case FACTORY_CHALLENGE_OPPONENT_IVS:
        challenge->opponentIvs[index] = *(u8 *)value;
        break;
    case FACTORY_CHALLENGE_OPPONENT_PERSONALITIES:
        challenge->opponentPersonalities[index] = *(u32 *)value;
        break;
    }
}

u32 FactoryChallenge_GetField(FactoryChallenge *challenge, int field, u8 index, int unused) {
    switch (field) {
    case FACTORY_CHALLENGE_TYPE:
        return challenge->type;
    case FACTORY_CHALLENGE_LEVEL_MODE:
        return challenge->levelMode;
    case FACTORY_CHALLENGE_BATTLE_NO:
        return challenge->battleNo;
    case FACTORY_CHALLENGE_TRAINER_IDS:
        return challenge->trainerIds[index];
    case FACTORY_CHALLENGE_RENTAL_SET_IDS:
        return challenge->rentalSetIds[index];
    case FACTORY_CHALLENGE_RENTAL_IVS:
        return challenge->rentalIvs[index];
    case FACTORY_CHALLENGE_RENTAL_PERSONALITIES:
        return challenge->rentalPersonalities[index];
    case FACTORY_CHALLENGE_OPPONENT_SET_IDS:
        return challenge->opponentSetIds[index];
    case FACTORY_CHALLENGE_OPPONENT_IVS:
        return challenge->opponentIvs[index];
    case FACTORY_CHALLENGE_OPPONENT_PERSONALITIES:
        return challenge->opponentPersonalities[index];
    }
    return 0;
}
