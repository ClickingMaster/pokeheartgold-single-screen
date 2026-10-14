#include "global.h"

#include "constants/items.h"

#include "apricorn_internal.h"

// Apricorn box save data (save array 0x26): apricorn counts, the apricorn given to Kurt, the juice being mixed
// (Apricorn juice for the Pokeathlon: 5 flavor values and a mood, updated as the player walks) and the three
// stored juice records.

typedef struct ApricornPreset {
    u16 value[4];
} ApricornPreset;

// The strongest flavors of a juice: flavor index, strength level, amount
struct PackedMix {
    u16 index : 4;
    u16 strength : 4;
    u16 amount : 8;
};

typedef struct FlavorPair {
    volatile u8 id;
    volatile u8 value;
} FlavorPair;

typedef struct ApricornJuiceResult {
    struct PackedMix mixes[3];
    u8 order[5];
    u8 ties;
    u8 count;
    u8 other;
} ApricornJuiceResult;

// View of SaveApricornBox used while applying the queued walking actions to the juice
typedef struct ApricornMixState {
    u8 pad[9];
    u8 state;
    u16 actions;
    u8 base[2];
    u8 mood;
    u8 values[5];
} ApricornMixState;

// asm/apricorn_box_data.s
extern const u16 gApricornBallItems[];
extern const s8 gApricornFlavorDeltas[][5];
extern const ApricornPreset gApricornJuicePresets[];

void ApricornBox_ClearJuiceRecords(SaveApricornBox *apricornBox);
void ClearApricornJuiceRecord(void *ptr);
u32 Save_ApricornBox_sizeof(void);
u32 ApricornJuiceRecord_sizeof(void);
void InitApricornBox(SaveApricornBox *apricornBox);
void Save_ApricornBox_Init(SaveApricornBox *apricornBox);
u32 ApricornBox_GetMixState(SaveApricornBox *apricornBox);
BOOL ApricornBox_QueueApricorn(SaveApricornBox *box, int apricorn, u32 value);
int ApricornBox_GetMixProgressLevel(SaveApricornBox *box);
u8 ApricornBox_GetJuiceServings(SaveApricornBox *box);
void PackJuiceFlavor(u16 *out, u8 *pair, int type);
int ApricornJuice_GetTopMixIndex(void *data);
int ApricornBox_UseJuiceServing(SaveApricornBox *box);
ApricornRecord *ApricornRecord_New(SaveApricornBox *box, PlayerProfile *profile, u32 value, enum HeapID heapId);
BOOL ApricornRecord_IsSameTrainer(ApricornRecord *a, ApricornRecord *b);
void ApricornBox_StoreJuiceRecords(SaveApricornBox *box, ApricornRecord *records, int count, int skip);
void ApricornJuice_GetFlavors(ApricornJuice *data, u8 *out);

u32 Save_ApricornBox_sizeof(void) {
    return sizeof(SaveApricornBox);
}

u32 ApricornJuiceRecord_sizeof(void) {
    return 0x20;
}

SaveApricornBox *Save_ApricornBox_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, 0x26);
}

void InitApricornBox(SaveApricornBox *apricornBox) {
    MI_CpuFill8(apricornBox, 0, sizeof(SaveApricornBox));
    ClearApricornJuice(apricornBox->juice);
    ApricornBox_ClearJuiceRecords(apricornBox);
}

void ApricornBox_ClearJuiceRecords(SaveApricornBox *apricornBox) {
    int i;
    for (i = 0; i < 3; i++) {
        ClearApricornJuiceRecord(apricornBox->juiceRecords[i]);
    }
}

void Save_ApricornBox_Init(SaveApricornBox *apricornBox) {
    InitApricornBox(apricornBox);
}

void ClearApricornJuice(void *ptr) {
    MI_CpuFill8(ptr, 0, 8);
}

int ApricornBox_GiveApricorn(SaveApricornBox *apricornBox, int apricorn, u8 amount) {
    int quantity = amount;
    if (apricorn >= 7) {
        GF_ASSERT(FALSE);
        return 0;
    }
    quantity += apricornBox->apricorns[apricorn];
    if (quantity > 99) {
        apricornBox->apricorns[apricorn] = 99;
    } else {
        apricornBox->apricorns[apricorn] = quantity;
    }
    return apricornBox->apricorns[apricorn];
}

int ApricornBox_TakeApricorn(SaveApricornBox *apricornBox, int apricorn, u32 quantity) {
    u8 count;
    if (apricorn >= 7) {
        GF_ASSERT(FALSE);
        return 0;
    }
    count = apricornBox->apricorns[apricorn];
    if (count >= quantity) {
        apricornBox->apricorns[apricorn] = count - quantity;
    }
    return apricornBox->apricorns[apricorn];
}

int ApricornBox_CountApricorn(SaveApricornBox *apricornBox, int apricorn) {
    if (apricorn >= 7) {
        GF_ASSERT(FALSE);
        return 0;
    }
    return apricornBox->apricorns[apricorn];
}

void ApricornBox_SetKurtApricorn(SaveApricornBox *apricornBox, u32 apricorn, u32 quantity) {
    if (apricornBox->apricorns[apricorn] < quantity) {
        GF_ASSERT(FALSE);
        return;
    }
    apricornBox->kurtApricorn = apricorn;
    apricornBox->kurtQuantity = quantity;
    ApricornBox_TakeApricorn(apricornBox, apricorn, quantity);
}

int ApricornBox_GetKurtQuantity(SaveApricornBox *apricornBox) {
    return apricornBox->kurtQuantity;
}

int ApricornBox_GetKurtBall(SaveApricornBox *apricornBox) {
    if (apricornBox->kurtApricorn >= 7) {
        return ITEM_POKE_BALL;
    }
    return gApricornBallItems[apricornBox->kurtApricorn];
}

void ApricornBox_SetMixState(SaveApricornBox *apricornBox, u32 value) {
    apricornBox->mixWord = (apricornBox->mixWord & 0x3FFFFFFF) | (value << 30);
}

u32 ApricornBox_GetMixState(SaveApricornBox *apricornBox) {
    return apricornBox->mixWord >> 30;
}

int ApricornBox_CountQueuedApricorns(SaveApricornBox *box) {
    int i;
    u8 count;
    u16 value;

    count = 0;
    value = box->queuedApricorns;
    for (i = 0; i < 5; i++) {
        if (value & 7) {
            count++;
        }
        value >>= 3;
    }
    return count;
}

BOOL ApricornBox_QueueApricorn(SaveApricornBox *box, int apricorn, u32 value) {
    u16 slots = box->queuedApricorns;
    u16 encoded;
    int i;
    if (apricorn >= 7) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    for (i = 0; i < 5; i++) {
        if (slots & 7) {
            slots >>= 3;
            continue;
        }
        encoded = ((apricorn + 1) & 7) << (i * 3);
        box->queuedApricorns |= encoded;
        box->queuedApricorns |= 2 << 14;
        box->mixStartSteps = value;
        if (box->juiceServings == 0) {
            box->juiceAgingSteps = value;
        }
        *(u32 *)((u8 *)box + 0x1C) &= 0xC0000000;
        ApricornBox_TakeApricorn(box, apricorn, 1);
        return TRUE;
    }
    return FALSE;
}

int ApricornBox_GetMixProgressLevel(SaveApricornBox *box) {
    u32 value;
    if (ApricornBox_CountQueuedApricorns(box) == 0) {
        return 0;
    }
    value = box->mixProgress;
    if (value < 50) {
        return 1;
    }
    if (value < 80) {
        return 2;
    }
    return 3;
}

void ApricornBox_AddJuiceMildnessForDays(SaveApricornBox *box, int amount) {
    int increment = amount * 10;
    int value;
    if (box->juiceServings != 0) {
        value = box->juice[2];
        value += increment;
        if (value > 255) {
            box->juice[2] = 255;
        } else {
            box->juice[2] = value;
        }
    }
}

u8 ApricornBox_GetJuiceServings(SaveApricornBox *box) {
    return box->juiceServings;
}

BOOL ApricornBox_GetJuice(SaveApricornBox *box, int index, ApricornJuice *out) {
    const u16 *src;
    u16 *dst = (u16 *)out;

    if (index >= 9) {
        index = 0;
    }
    if (index <= 4) {
        *(ApricornPreset *)out = gApricornJuicePresets[index];
    } else if (index <= 7) {
        src = (const u16 *)((u8 *)box + 0x38 + (index - 5) * 0x20);
        if (ApricornJuice_GetFlavorTotal((ApricornJuice *)src) == 0) {
            ClearApricornJuice(out);
            return FALSE;
        }
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    } else {
        if (*((u8 *)box + 9) == 0) {
            ClearApricornJuice(out);
            return FALSE;
        }
        dst[0] = *(u16 *)((u8 *)box + 0xC);
        dst[1] = *(u16 *)((u8 *)box + 0xE);
        dst[2] = *(u16 *)((u8 *)box + 0x10);
        dst[3] = *(u16 *)((u8 *)box + 0x12);
    }
    return TRUE;
}

BOOL ApricornJuice_IsValid(ApricornJuice *data) {
    if (ApricornJuice_GetFlavorTotal(data) != 0) {
        return TRUE;
    }
    return FALSE;
}

int ApricornJuice_GetFlavorTotal(ApricornJuice *data) {
    ApricornMix *mix = (ApricornMix *)data;
    int i;
    u16 total = 0;
    for (i = 0; i < 5; i++) {
        total += mix->values[i];
    }
    if (total > 100) {
        total = 100;
    }
    return (u8)total;
}

#pragma force_active on

void ApricornJuice_RankFlavors(void *input_, void *out_) {
    const u8 *input = input_;
    ApricornJuiceResult *out = out_;
    FlavorPair pairs[5];
    FlavorPair tmp;
    u8 count;
    int i, j;
    u8 minimum;
    MI_CpuFill8(out, 0, 14);
    MI_CpuFill8(pairs, 0, 10);
    count = 0;
    for (i = 0; i < 5; i++) {
        u8 value = input[i + 3];
        pairs[i].id = i;
        pairs[i].value = value;
        if (value) {
            count++;
        }
    }
    if (count) {
        for (i = 0; i < 5; i++) {
            for (j = i + 1; j < 5; j++) {
                u8 b = pairs[j].value, a = pairs[i].value;
                if (a > b) {
                    continue;
                }
                if (a == b && pairs[i].id < pairs[j].id) {
                    continue;
                }
                tmp.id = pairs[i].id;
                tmp.value = pairs[i].value;
                pairs[i].id = pairs[j].id;
                pairs[i].value = pairs[j].value;
                pairs[j].id = tmp.id;
                pairs[j].value = tmp.value;
            }
        }
    }
    minimum = pairs[4].value;
    for (i = 0; i < 5; i++) {
        out->order[i] = pairs[i].id;
        if (minimum == pairs[i].value) {
            out->ties++;
        }
    }
    out->count = count;
    switch (count) {
    case 0:
        out->mixes[0].index = 6;
        return;
    case 1:
        PackJuiceFlavor((u16 *)&out->mixes[0], (u8 *)&pairs[0], 0);
        out->mixes[1].index = 6;
        break;
    case 2:
        PackJuiceFlavor((u16 *)&out->mixes[0], (u8 *)&pairs[0], 0);
        PackJuiceFlavor((u16 *)&out->mixes[1], (u8 *)&pairs[1], 1);
        out->mixes[2].index = 6;
        break;
    case 3:
        PackJuiceFlavor((u16 *)&out->mixes[0], (u8 *)&pairs[0], 0);
        PackJuiceFlavor((u16 *)&out->mixes[1], (u8 *)&pairs[1], 1);
        PackJuiceFlavor((u16 *)&out->mixes[2], (u8 *)&pairs[2], 2);
        break;
    case 4:
    case 5:
        if (pairs[0].value - pairs[3].value <= 12) {
            out->mixes[0].index = 5;
            out->mixes[0].amount = pairs[0].value;
            out->mixes[1].index = 6;
            out->mixes[1].amount = pairs[1].value;
            out->mixes[2].index = 6;
            out->mixes[2].amount = pairs[2].value;
            out->count = 1;
            {
                u8 top = pairs[0].value;
                if (top > 20) {
                    out->mixes[0].strength = 3;
                } else if (top > 7) {
                    out->mixes[0].strength = 2;
                } else {
                    out->mixes[0].strength = 1;
                }
            }
        } else {
            PackJuiceFlavor((u16 *)&out->mixes[0], (u8 *)&pairs[0], 0);
            PackJuiceFlavor((u16 *)&out->mixes[1], (u8 *)&pairs[1], 1);
            if (count == 4) {
                PackJuiceFlavor((u16 *)&out->mixes[2], (u8 *)&pairs[4], 2);
            } else {
                out->mixes[2].index = 5;
                out->mixes[2].strength = 1;
                out->mixes[2].amount = pairs[2].value;
            }
        }
        break;
    }
    out->count = count;
}

#pragma force_active off

int ApricornJuice_GetTopMixIndex(void *data) {
    struct {
        u16 value : 4;
        u16 padding : 12;
        u8 rest[14];
    } result;

    u32 value;
    ApricornJuice_RankFlavors(data, &result);
    value = result.value;
    if (value >= 5) {
        value = 5;
    }
    return value;
}

int ApricornJuice_GetFlavorNameIndex(void *data) {
    u8 result[16];
    ApricornJuice_RankFlavors(data, result);
    if (result[12] == 0) {
        return 0;
    }
    return result[6] + 1;
}

int ApricornBox_UseJuiceServing(SaveApricornBox *box) {
    if (box->juiceServings == 0) {
        return 0;
    }
    box->juiceServings--;
    if (box->juiceServings == 0) {
        ClearApricornJuice(box->juice);
        box->juiceAgingSteps = 0;
        box->mixStartSteps = 0;
        box->mixProgress = 0;
    }
    return box->juiceServings;
}

void ApricornBox_UpdateSteps(SaveApricornBox *box, u32 steps) {
    u32 difference;
    if ((((u32)box->queuedApricorns << 16) >> 31) != 0 || box->juiceServings != 0) {
        difference = steps - box->juiceAgingSteps;
        if (difference >= 100) {
            ApricornBox_AddJuiceMildness(box, difference / 100);
            box->juiceAgingSteps = steps - difference % 100;
        }
        if ((((u32)box->queuedApricorns << 16) >> 31) != 0) {
            box->mixProgress = steps - box->mixStartSteps;
            if (box->mixProgress >= 100) {
                if (box->juiceServings != 0) {
                    ApricornBox_SetMixState(box, 3);
                } else {
                    ApricornBox_SetMixState(box, 2);
                }
                ApricornBox_MixQueuedApricorns(box);
            }
        }
    }
}

ApricornRecord *ApricornRecord_New(SaveApricornBox *box, PlayerProfile *profile, u32 value, enum HeapID heapId) {
    ApricornRecord *record = Heap_AllocAtEnd(heapId, sizeof(ApricornRecord));
    MI_CpuFill8(record, 0, sizeof(ApricornRecord));
    record->trainerId = PlayerProfile_GetTrainerID(profile);
    record->gender = PlayerProfile_GetTrainerGender(profile);
    record->language = PlayerProfile_GetLanguage(profile);
    record->version = PlayerProfile_GetVersion(profile);
    StringFillEOS(record->name, 8);
    CopyU16StringArrayN(record->name, PlayerProfile_GetNamePtr(profile), 7);
    ApricornBox_GetJuice(box, 8, &record->data);
    if (value > 0xFFFF) {
        record->data.unk0 = 0xFFFF;
    } else {
        record->data.unk0 = value;
    }
    return record;
}

BOOL ApricornRecord_IsSameTrainer(ApricornRecord *a, ApricornRecord *b) {
    if (a->trainerId != b->trainerId) {
        return FALSE;
    }
    if (a->gender != b->gender) {
        return FALSE;
    }
    if (a->version != b->version) {
        return FALSE;
    }
    if (a->language != b->language) {
        return FALSE;
    }
    if (StringNotEqual(a->name, b->name) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void ApricornBox_StoreJuiceRecords(SaveApricornBox *box, ApricornRecord *records, int count, int skip) {
    volatile int found;
    int numRecords = 0;
    int outerIndex;
    int initialIndex = 0;
    int foundIndex;
    int j;
    u32 destIndex;

    do {
        if (!ApricornJuice_IsValid((ApricornJuice *)&box->juiceRecords[initialIndex][0x18])) {
            break;
        }
        numRecords++;
        initialIndex++;
    } while (initialIndex < 3);

    outerIndex = 0;
    if (count <= 0) {
        return;
    }
    do {
        if (outerIndex == skip) {
            goto next_record;
        }
        if (!ApricornJuice_IsValid(&records->data)) {
            goto next_record;
        }
        foundIndex = 0;
        found = FALSE;
        {
            ApricornRecord *boxRecord = (ApricornRecord *)box->juiceRecords;
            j = found;
            do {
                if (ApricornRecord_IsSameTrainer(records, boxRecord)) {
                    foundIndex = (u8)j;
                    found = TRUE;
                    break;
                }
                j++;
                boxRecord++;
            } while (j < 3);
        }
        if (numRecords >= 3) {
            destIndex = 2;
            found = TRUE;
        } else if (found) {
            destIndex = (u8)(numRecords - 1);
        } else {
            destIndex = (u8)numRecords;
            numRecords++;
        }
        if (found && (u32)foundIndex < destIndex) {
            int limit = numRecords - 1;
            if (foundIndex < limit) {
                u8 *cursor = (u8 *)box + foundIndex * 0x20;
                do {
                    *(ApricornRecord *)(cursor + 0x20) = *(ApricornRecord *)(cursor + 0x40);
                    foundIndex++;
                    cursor += 0x20;
                } while (foundIndex < limit);
            }
        }
        *(ApricornRecord *)((u8 *)box + 0x20 + destIndex * 0x20) = *records;
next_record:
        records++;
        outerIndex++;
    } while (outerIndex < count);
}

String *ApricornBox_GetJuiceName(SaveApricornBox *box, int type, u32 heapId) {
    String *result = String_New(9, (enum HeapID)heapId);
    if (type < 5) {
        MsgData *msgData = NewMsgDataFromNarc((MsgDataLoadType)1, (NarcId)0x1B, 0x15, (enum HeapID)heapId);
        ReadMsgDataIntoString(msgData, type + 0xE, result);
        DestroyMsgData(msgData);
    } else if (type < 8) {
        u32 offset = (u8)(type - 5) << 5;
        if (ApricornJuice_IsValid((ApricornJuice *)((u8 *)box + 0x38 + offset))) {
            CopyU16ArrayToString(result, (u16 *)((u8 *)box + 0x28 + offset));
        }
    }
    return result;
}

String *ApricornBox_GetJuiceFlavorName(SaveApricornBox *box, u16 type, u32 heapId) {
    ApricornJuice data;
    MsgData *msgData;
    String *result;
    ApricornBox_GetJuice(box, type, &data);
    msgData = NewMsgDataFromNarc((MsgDataLoadType)1, (NarcId)0x1B, 0x15, (enum HeapID)heapId);
    result = NewString_ReadMsgData(msgData, ApricornJuice_GetFlavorNameIndex(&data) + 0x13);
    DestroyMsgData(msgData);
    return result;
}

void ApricornBox_AddJuiceMildness(SaveApricornBox *box, u8 amount) {
    int value = box->juice[2] + amount;
    if (value > 255) {
        box->juice[2] = 255;
    } else {
        box->juice[2] = value;
    }
}

#pragma force_active on

void ApricornBox_MixQueuedApricorns(SaveApricornBox *box) {
    ApricornMixState *s = (ApricornMixState *)box;
    s8 values[5], order[5];
    int n;
    u8 total, action, max, index;
    u16 actions = s->actions;
    int i, j;
    u8 reduce;
    ApricornJuice_GetFlavors((ApricornJuice *)s->base, (u8 *)values);
    for (n = 0; n < 5; n++) {
        reduce = 0;
        action = actions & 7;
        if (action == 0) {
            break;
        }
        total = 0;
        index = 0;
        max = 0;
        action--;
        for (i = 0; i < 5; i++) {
            order[i] = i;
        }
        for (i = 0; i < 2; i++) {
            for (j = i + 1; j < 5; j++) {
                s8 tmp;
                if (values[order[i]] > values[order[j]] || (values[order[i]] == values[order[j]] && order[i] < order[j])) {
                    continue;
                }
                tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }
        }
        {
            s8 value;
            const s8 *row;
            s8 *v;
            s8 change;
            s8 old;
            row = gApricornFlavorDeltas[action];
            v = &values[0];
            for (i = 0; i < 5; i++) {
                change = *row;
                old = *v;
                value = old + change;
                if (change > 0 && values[order[0]] != 0 && order[0] != i && values[order[1]] != 0 && order[1] != i) {
                    reduce = 1;
                }
                if (value >= 63) {
                    value = 63;
                } else if (value < 0) {
                    value = 0;
                }
                if ((action == 6 || value <= old) && value > max) {
                    max = value;
                    index = i;
                }
                *v = value;
                total += value;
                row++;
                v++;
            }
        }
        actions >>= 3;
        if (reduce) {
            if (s->mood < 10) {
                s->mood = 0;
            } else {
                s->mood -= 10;
            }
        }
        if (action != 5 && total > 100) {
            values[index] -= total - 100;
        }
    }
    for (i = 0; i < 5; i++) {
        s->values[i] = values[i];
    }
    s->state = 3;
    s->actions = 0;
}

#pragma force_active off

void ApricornJuice_GetFlavors(ApricornJuice *data, u8 *out) {
    int i;
    ApricornMix *mix = (ApricornMix *)data;
    for (i = 0; i < 5; i++) {
        out[i] = mix->values[i];
    }
}

u8 GetJuiceFlavorLevel(int type, u8 value) {
    if (value == 0) {
        return 0;
    }
    switch (type) {
    case 0:
        if (value > 62) {
            return 6;
        }
        if (value > 50) {
            return 5;
        }
        if (value > 40) {
            return 4;
        }
        if (value > 30) {
            return 3;
        }
        if (value > 20) {
            return 2;
        }
        if (value != 0) {
            return 1;
        }
        break;
    case 1:
        if (value > 40) {
            return 4;
        }
        if (value > 30) {
            return 3;
        }
        if (value > 20) {
            return 2;
        }
        if (value != 0) {
            return 1;
        }
        break;
    case 2:
        if (value > 20) {
            return 3;
        }
        if (value > 10) {
            return 2;
        }
        if (value != 0) {
            return 1;
        }
        break;
    }
    return 0;
}

void PackJuiceFlavor(u16 *out, u8 *pair, int type) {
    struct PackedMix *packed = (struct PackedMix *)out;
    packed->index = pair[0];
    packed->amount = pair[1];
    packed->strength = GetJuiceFlavorLevel((u8)type, pair[1]);
}

void ClearApricornJuiceRecord(void *data) {
    u8 *bytes = data;
    MI_CpuFill8(data, 0, 0x20);
    ClearApricornJuice(bytes + 0x18);
    StringFillEOS((u16 *)(bytes + 8), 8);
}
