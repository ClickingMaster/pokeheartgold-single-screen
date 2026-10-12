#include "global.h"

#include "fashion_case.h"
#include "heap.h"
#include "pm_string.h"
#include "pm_version.h"
#include "pokemon.h"
#include "save.h"

// Fashion save data (save array 0xC): 11 dressed-up Pokemon entries, 5 larger entries and the fashion case
// (accessory counts, backdrop order). An entry's magic is 0x1234 while empty and 0x2345 once saved.

typedef struct SaveFashionMon {
    u32 personality;
    u32 otId;
    u16 species;
    u16 nickname[11];
    u16 secondText[8];
    s8 z;
    u8 x;
    u8 y;
    u8 form;
    u8 unk_34;
} SaveFashionMon;

// The real layout of SaveFashionDataSub (size 0x74)
typedef struct FashionEntry {
    u32 magic;
    SaveFashionMon mon;
    u32 mask; // bit n: items[n] is placed
    MailMessage mailMessage;
    u8 items[10][4]; // accessory id, x, y, z
    u8 unk_70;
    u8 language;
    u8 padding_72[2];
} FashionEntry;

// The real layout of SaveFashionDataSub3FC (size 0x98); only the magic is accessed here
typedef struct FashionEntryLarge {
    u32 magic;
    u8 data[0x94];
} FashionEntryLarge;

typedef struct FashionSource {
    u32 id;
    void *object; // graphics object: packed x/y position, z
} FashionSource;

typedef struct FashionItemCoords {
    u16 x;
    u16 y;
    u16 xCopy;
    u16 yCopy;
} FashionItemCoords;

extern void DressUpMon_GetCenterPos(int, int *, int *);
extern int DressUpMon_GetPriority(int);
u32 GraphicsObject_GetPosition(void *object);
int GraphicsObject_GetDepth(void *object);
void MATHi_CRC32InitTableRev(u32 *table, u32 polynomial);
u32 MATH_CalcCRC32(const u32 *table, const void *data, u32 size);
void *SaveArray_Get(SaveData *saveData, int id);

BOOL IsValidFashionMagic(u32 value);
void GetFashionObjectPlacement(int a0, u8 *a1, u8 *a2, u8 *a3);
void FashionMon_Init(SaveFashionMon *dst, Pokemon *mon, u8 a2, u8 a3, s8 a4);
void FashionMon_InitAtObject(SaveFashionMon *dst, Pokemon *mon, int a2);
void FashionMon_SetSecondText(SaveFashionMon *dst, String *str, u8 a2);
void FashionMon_CreatePokemon(SaveFashionMon *src, Pokemon *mon);
void SetFashionItemPlacement(u8 *dst, u8 a1, u8 a2, u8 a3, u8 a4);
void SetStackableAccessoryCount(u32 *data, int value, u8 index);
u8 GetStackableAccessoryCount(u32 *data, u8 index);
void SetUniqueAccessoryFlag(u32 *data, u8 value, u8 index);
u8 GetUniqueAccessoryFlag(u32 *data, u8 index);
void SetWallpaperOrder(u32 *data, u32 value, u8 index);
u8 GetWallpaperOrder(u32 *data, u8 index);
u8 CountOwnedWallpapers(u32 *data);
BOOL IsStackableAccessory(u32 value);
void FashionCase_Init(FashionCase *fashionCase);
void Save_FashionData_Init(SaveFashionData *fashionData);
u32 Save_FashionData_sizeof(void);
u32 FashionEntry_sizeof(void);
SaveFashionDataSub *FashionEntry_New(enum HeapID heapId);
BOOL FashionEntry_IsSaved(SaveFashionDataSub *entry);
BOOL FashionEntryLarge_IsSaved(SaveFashionDataSub3FC *entry);
u8 FashionCase_GetWallpaperOrder(FashionCase *fashionCase, int index);
void FashionEntry_MarkSaved(SaveFashionDataSub *entry);
void FashionEntry_Clear(SaveFashionDataSub *entry);
void FashionEntry_SetMon(SaveFashionDataSub *entry, Pokemon *mon, int value);
void FashionEntry_Copy(SaveFashionDataSub *dst, SaveFashionDataSub *src);
void FashionMon_GetSecondText(SaveFashionMon *mon, String *string);
void FashionEntry_PlaceItem(SaveFashionDataSub *entry, FashionSource *source, int index);
void FashionEntry_SetBackdrop(SaveFashionDataSub *entry, int value);
void FashionEntry_SetMonSecondText(SaveFashionDataSub *entry, String *string, u8 value);
BOOL FashionEntry_IsItemPlaced(SaveFashionDataSub *entry, int index);
SaveFashionMon *FashionEntry_GetMon(SaveFashionDataSub *entry);
u8 *FashionEntry_GetItem(SaveFashionDataSub *entry, int index);
void FashionEntry_GetMonSecondText(SaveFashionDataSub *entry, String *string);
u8 FashionEntry_GetBackdrop(SaveFashionDataSub *entry);
u16 FashionEntry_GetMailMessageWord(SaveFashionDataSub *entry);
s8 FashionMon_GetZ(SaveFashionMon *mon);
u8 FashionMon_GetX(SaveFashionMon *mon);
u8 FashionMon_GetY(SaveFashionMon *mon);
void FashionMon_ToPokemon(SaveFashionMon *mon, Pokemon *pokemon);
u8 FashionItem_GetId(u8 *data);
u8 FashionItem_GetX(u8 *data);
u8 FashionItem_GetY(u8 *data);
s8 FashionItem_GetZ(s8 *data);
BOOL Save_FashionData_IsNewEntry(SaveFashionData *fashionData, SaveFashionDataSub *candidate);
void Save_FashionData_InsertEntries(u8 count, int skip, SaveFashionData *fashionData, SaveFashionDataSub **entries);

BOOL IsValidFashionMagic(u32 value) {
    if (value == 0x1234 || value == 0x2345) {
        return TRUE;
    }
    return FALSE;
}

void GetFashionObjectPlacement(int a0, u8 *a1, u8 *a2, u8 *a3) {
    int x;
    int y;
    int z;
    DressUpMon_GetCenterPos(a0, &x, &y);
    z = DressUpMon_GetPriority(a0);
    GF_ASSERT(x < 256);
    GF_ASSERT(y < 256);
    GF_ASSERT(z > -128);
    *a1 = x;
    *a2 = y;
    *a3 = z;
}

void FashionMon_Init(SaveFashionMon *dst, Pokemon *mon, u8 a2, u8 a3, s8 a4) {
    dst->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    GetMonData(mon, MON_DATA_NICKNAME, dst->nickname);
    dst->personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    dst->otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
    dst->form = GetMonData(mon, MON_DATA_FORM, NULL);
    dst->x = a2;
    dst->y = a3;
    dst->z = a4;
}

void FashionMon_InitAtObject(SaveFashionMon *dst, Pokemon *mon, int a2) {
    u8 x, y, z;
    GetFashionObjectPlacement(a2, &x, &y, &z);
    FashionMon_Init(dst, mon, x, y, (s8)z);
}

void FashionMon_SetSecondText(SaveFashionMon *dst, String *str, u8 a2) {
    CopyStringToU16Array(str, dst->secondText, 8);
    dst->unk_34 = a2;
}

void FashionMon_CreatePokemon(SaveFashionMon *src, Pokemon *mon) {
    CreateMon(mon, src->species, 0, 0, TRUE, src->personality, TRUE, src->otId);
    SetMonData(mon, MON_DATA_NICKNAME, src->nickname);
    SetMonData(mon, MON_DATA_FORM, &src->form);
}

void SetFashionItemPlacement(u8 *dst, u8 a1, u8 a2, u8 a3, u8 a4) {
    dst[0] = a1;
    dst[1] = a2;
    dst[2] = a3;
    dst[3] = a4;
}

void SetStackableAccessoryCount(u32 *data, int value, u8 index) {
    u8 shift;
    u8 word;
    GF_ASSERT(index < 61);
    shift = index % 8;
    shift *= 4;
    word = index / 8;
    data[word] &= ~(15 << shift);
    data[word] |= value << shift;
}

u8 GetStackableAccessoryCount(u32 *data, u8 index) {
    u8 word;
    u8 shift;
    u8 value;
    GF_ASSERT(index < 61);
    word = index / 8;
    shift = index % 8;
    shift *= 4;
    value = (data[word] >> shift) & 15;
    if (value > 9) {
        value = 9;
    }
    return value;
}

void SetUniqueAccessoryFlag(u32 *data, u8 value, u8 index) {
    u8 shift;
    u8 word;
    GF_ASSERT(value < 2);
    shift = index % 32;
    word = index / 32;
    data[word] &= ~(1 << shift);
    data[word] |= value << shift;
}

u8 GetUniqueAccessoryFlag(u32 *data, u8 index) {
    u8 word = index / 32;
    u8 shift = index % 32;
    return (data[word] >> shift) & 1;
}

void SetWallpaperOrder(u32 *data, u32 value, u8 index) {
    u8 shift;
    u8 word;
    GF_ASSERT(value <= 18);
    shift = index % 4;
    shift *= 8;
    word = index / 4;
    data[word] &= ~(0xFF << shift);
    data[word] |= value << shift;
}

u8 GetWallpaperOrder(u32 *data, u8 index) {
    u8 word = index / 4;
    u8 shift = index % 4;
    shift *= 8;
    return data[word] >> shift;
}

u8 CountOwnedWallpapers(u32 *data) {
    int i = 0;
    int count = i;
    for (i = 0; i < 18; i++) {
        if (GetWallpaperOrder(data, (u8)i) != 18) {
            count++;
        }
    }
    return count;
}

BOOL IsStackableAccessory(u32 value) {
    if (value < 61) {
        return TRUE;
    }
    return FALSE;
}

void FashionCase_Init(FashionCase *fashionCase) {
    int i;
    memset(fashionCase, 0, 0x40);
    for (i = 0; i < 18; i++) {
        SetWallpaperOrder(&fashionCase->unk_28[0], 18, (u8)i);
    }
}

void Save_FashionData_Init(SaveFashionData *fashionData) {
    int i;
    for (i = 0; i < 11; i++) {
        memset(&fashionData->unk_000[i], 0, sizeof(SaveFashionDataSub));
        ((FashionEntry *)&fashionData->unk_000[i])->magic = 0x1234;
    }
    {
        int j = 0;
        u8 *clear = (u8 *)fashionData + 0x4FC;
        u8 *set = (u8 *)fashionData;
        u32 magic = 0x1234;
        for (; j < 5; j++) {
            memset(clear, 0, 0x98);
            *(u32 *)(set + 0x4FC) = magic;
            clear += 0x98;
            set += 0x98;
        }
    }
    FashionCase_Init(&fashionData->fashionCase);
}

u32 Save_FashionData_sizeof(void) {
    return 0x834;
}

u32 FashionEntry_sizeof(void) {
    return sizeof(SaveFashionDataSub);
}

SaveFashionDataSub *FashionEntry_New(enum HeapID heapId) {
    SaveFashionDataSub *entry = Heap_Alloc(heapId, sizeof(SaveFashionDataSub));
    memset(entry, 0, sizeof(SaveFashionDataSub));
    ((FashionEntry *)entry)->magic = 0x1234;
    return entry;
}

SaveFashionDataSub *Save_FashionData_GetEntry(SaveFashionData *fashionData, int index) {
    GF_ASSERT(index < 11);
    GF_ASSERT(IsValidFashionMagic(((FashionEntry *)&fashionData->unk_000[index])->magic));
    return &fashionData->unk_000[index];
}

FashionCase *Save_FashionData_GetFashionCase(SaveFashionData *fashionData) {
    return &fashionData->fashionCase;
}

BOOL Save_FashionData_IsEntrySaved(SaveFashionData *fashionData, int index) {
    GF_ASSERT(index < 11);
    return FashionEntry_IsSaved(&fashionData->unk_000[index]);
}

BOOL Save_FashionData_IsLargeEntrySaved(SaveFashionData *fashionData, int index) {
    GF_ASSERT(index < 5);
    return FashionEntryLarge_IsSaved(&fashionData->unk_3FC[index]);
}

int FashionCase_CanGiveAccessory(FashionCase *fashionCase, int id, int quantity) {
    BOOL result = TRUE;
    u32 current = FashionCase_GetAccessoryCount(fashionCase, id);
    if (IsStackableAccessory(id)) {
        if (current + quantity > 9) {
            result = FALSE;
        }
    } else if (current + quantity > 1) {
        result = FALSE;
    }
    return result;
}

int FashionCase_HasWallpaper(FashionCase *fashionCase, int index) {
    if (FashionCase_GetWallpaperOrder(fashionCase, index) != 18) {
        return TRUE;
    }
    return FALSE;
}

u32 FashionCase_GetAccessoryCount(FashionCase *fashionCase, int id) {
    GF_ASSERT((u32)id < 100);
    if (IsStackableAccessory(id)) {
        return GetStackableAccessoryCount(fashionCase->unk_00, (u8)id);
    }
    GF_ASSERT((u32)id >= 61);
    id = (u8)(id - 61);
    return GetUniqueAccessoryFlag(&fashionCase->unk_00[8], (u8)id);
}

u8 FashionCase_GetWallpaperOrder(FashionCase *fashionCase, int index) {
    GF_ASSERT((u32)index < 18);
    return GetWallpaperOrder(&fashionCase->unk_28[0], (u8)index);
}

u32 FashionCase_CountAccessories(FashionCase *fashionCase) {
    int i = 0;
    u32 count = i;
    for (i = 0; i < 100; i++) {
        count += FashionCase_GetAccessoryCount(fashionCase, i);
    }
    return count;
}

u32 FashionCase_CountWallpapers(FashionCase *fashionCase) {
    int i = 0;
    u32 count = i;
    for (i = 0; i < 18; i++) {
        if (FashionCase_GetWallpaperOrder(fashionCase, i) != 18) {
            count++;
        }
    }
    return count;
}

void FashionCase_GiveFashionItem(FashionCase *fashionCase, int id, int quantity) {
    GF_ASSERT((u32)id < 100);
    if (IsStackableAccessory(id)) {
        u8 count = GetStackableAccessoryCount(fashionCase->unk_00, (u8)id);
        count += quantity;
        if (count > 9) {
            count = 9;
        }
        SetStackableAccessoryCount(fashionCase->unk_00, count, (u8)id);
    } else {
        u8 count = GetUniqueAccessoryFlag(&fashionCase->unk_00[8], (u8)id);
        count += quantity;
        if (count > 1) {
            count = 1;
        }
        GF_ASSERT((u32)id >= 61);
        id = (u8)(id - 61);
        SetUniqueAccessoryFlag(&fashionCase->unk_00[8], count, (u8)id);
    }
}

void FashionCase_TakeAccessory(FashionCase *fashionCase, int id, int quantity) {
    GF_ASSERT((u32)id < 100);
    if (IsStackableAccessory(id)) {
        u8 count = GetStackableAccessoryCount(fashionCase->unk_00, (u8)id);
        if ((u32)count > (u32)quantity) {
            count -= quantity;
        } else {
            count = 0;
        }
        SetStackableAccessoryCount(fashionCase->unk_00, count, (u8)id);
    } else {
        GF_ASSERT((u32)id >= 61);
        id = (u8)(id - 61);
        SetUniqueAccessoryFlag(&fashionCase->unk_00[8], 0, (u8)id);
    }
}

void FashionCase_GiveContestBackground(FashionCase *fashionCase, int id) {
    GF_ASSERT((u32)id < 18);
    if (GetWallpaperOrder(&fashionCase->unk_28[0], (u8)id) == 18) {
        u8 count = CountOwnedWallpapers(&fashionCase->unk_28[0]);
        SetWallpaperOrder(&fashionCase->unk_28[0], count, (u8)id);
    }
}

BOOL FashionEntry_IsSaved(SaveFashionDataSub *entry_) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    return entry->magic == 0x2345;
}

void FashionEntry_MarkSaved(SaveFashionDataSub *entry_) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    entry->magic = 0x2345;
    entry->language = gGameLanguage;
}

void FashionEntry_Clear(SaveFashionDataSub *entry_) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    memset(entry, 0, sizeof(FashionEntry));
    entry->magic = 0x1234;
}

void FashionEntry_SetMon(SaveFashionDataSub *entry_, Pokemon *mon, int value) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    FashionMon_InitAtObject(&entry->mon, mon, value);
}

void FashionEntry_PlaceItem(SaveFashionDataSub *entry_, FashionSource *source, int index) {
    FashionEntry *entry = (FashionEntry *)entry_;
    volatile FashionItemCoords coords;
    u32 packed;
    int z;
    int y;
    u32 mask;

    packed = GraphicsObject_GetPosition(source->object);
    coords.x = packed;
    coords.y = packed >> 16;
    coords.xCopy = coords.x;
    coords.yCopy = coords.y;
    z = GraphicsObject_GetDepth(source->object);
    if (index >= 10) {
        GF_AssertFail();
    }
    if ((s16)coords.xCopy >= 256) {
        GF_AssertFail();
    }
    y = (s16)coords.yCopy;
    // y < 256, read back from its stack slot (needed to match)
    if (((volatile int *)&coords)[-1] >= 256) {
        GF_AssertFail();
    }
    if (z <= -128) {
        GF_AssertFail();
    }
    mask = 1 << index;
    if (entry->mask & mask) {
        GF_AssertFail();
    }
    if (!IsValidFashionMagic(entry->magic)) {
        GF_AssertFail();
    }
    SetFashionItemPlacement(entry->items[index], (u8)source->id, (u8)(s16)coords.xCopy, (u8)y, (u8)z);
    entry->mask |= mask;
}

void FashionEntry_SetBackdrop(SaveFashionDataSub *entry_, int value) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    entry->unk_70 = value;
}

void FashionEntry_SetMailMessageWord(SaveFashionDataSub *entry_, int value) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    MailMsg_Init(&entry->mailMessage);
    ((void (*)(MailMessage *, int, int))MailMsg_SetFieldI)(&entry->mailMessage, 0, value);
}

void FashionEntry_Copy(SaveFashionDataSub *dst_, SaveFashionDataSub *src_) {
    FashionEntry *dst = (FashionEntry *)dst_;
    GF_ASSERT(IsValidFashionMagic(dst->magic));
    memcpy(dst, src_, sizeof(FashionEntry));
}

void FashionEntry_SetMonSecondText(SaveFashionDataSub *entry_, String *string, u8 value) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    FashionMon_SetSecondText(&entry->mon, string, value);
}

BOOL FashionEntry_IsItemPlaced(SaveFashionDataSub *entry_, int index) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(index < 10);
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    return entry->mask & (1 << index);
}

SaveFashionMon *FashionEntry_GetMon(SaveFashionDataSub *entry_) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    return &entry->mon;
}

u8 *FashionEntry_GetItem(SaveFashionDataSub *entry_, int index) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(index < 10);
    GF_ASSERT(entry->mask & (1 << index));
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    return entry->items[index];
}

void FashionEntry_GetMonSecondText(SaveFashionDataSub *entry_, String *string) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    FashionMon_GetSecondText(&entry->mon, string);
}

u8 FashionEntry_GetBackdrop(SaveFashionDataSub *entry_) {
    FashionEntry *entry = (FashionEntry *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    return entry->unk_70;
}

u16 FashionEntry_GetMailMessageWord(SaveFashionDataSub *entry_) {
    FashionEntry *entry = (FashionEntry *)entry_;
    return MailMsg_GetFieldI(&entry->mailMessage, 0);
}

BOOL FashionEntryLarge_IsSaved(SaveFashionDataSub3FC *entry_) {
    FashionEntryLarge *entry = (FashionEntryLarge *)entry_;
    GF_ASSERT(IsValidFashionMagic(entry->magic));
    return entry->magic == 0x2345;
}

void FashionMon_GetSecondText(SaveFashionMon *mon, String *string) {
    CopyU16ArrayToString(string, mon->secondText);
}

s8 FashionMon_GetZ(SaveFashionMon *mon) {
    return mon->z;
}

u8 FashionMon_GetX(SaveFashionMon *mon) {
    return mon->x;
}

u8 FashionMon_GetY(SaveFashionMon *mon) {
    return mon->y;
}

void FashionMon_ToPokemon(SaveFashionMon *mon, Pokemon *pokemon) {
    FashionMon_CreatePokemon(mon, pokemon);
}

u8 FashionItem_GetId(u8 *data) {
    return data[0];
}

u8 FashionItem_GetX(u8 *data) {
    return data[1];
}

u8 FashionItem_GetY(u8 *data) {
    return data[2];
}

s8 FashionItem_GetZ(s8 *data) {
    return data[3];
}

BOOL Save_FashionData_IsNewEntry(SaveFashionData *fashionData, SaveFashionDataSub *candidate) {
    u32 table[256];
    BOOL result = TRUE;
    u32 crc;
    int i;
    if (FashionEntry_IsSaved(candidate) == TRUE) {
        MATHi_CRC32InitTableRev(table, 0xEDB88320);
        crc = MATH_CalcCRC32(table, candidate, sizeof(SaveFashionDataSub));
        for (i = 0; i < 11; i++) {
            SaveFashionDataSub *entry = Save_FashionData_GetEntry(fashionData, i);
            u32 other;
            MATHi_CRC32InitTableRev(table, 0xEDB88320);
            other = MATH_CalcCRC32(table, entry, sizeof(SaveFashionDataSub));
            if (other == crc) {
                result = FALSE;
                break;
            }
        }
    } else {
        result = FALSE;
    }
    return result;
}

void Save_FashionData_InsertEntries(u8 count, int skip, SaveFashionData *fashionData, SaveFashionDataSub **entries) {
    int accepted = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (i != skip && entries[i] != NULL && Save_FashionData_IsNewEntry(fashionData, entries[i]) == TRUE) {
            accepted++;
        }
    }
    for (i = 10; i >= 1; i--) {
        if (i + accepted < 11) {
            FashionEntry_Copy(Save_FashionData_GetEntry(fashionData, i + accepted), Save_FashionData_GetEntry(fashionData, i));
        }
    }
    {
        int dst = 1;
        for (i = 0; i < count; i++) {
            SaveFashionDataSub *entry;
            if (i == skip) {
                continue;
            }
            entry = entries[i];
            if (entry != NULL && Save_FashionData_IsNewEntry(fashionData, entry) == TRUE) {
                FashionEntry_Copy(Save_FashionData_GetEntry(fashionData, dst++), entry);
            }
        }
    }
}

SaveFashionData *Save_FashionData_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, 0xC);
}
