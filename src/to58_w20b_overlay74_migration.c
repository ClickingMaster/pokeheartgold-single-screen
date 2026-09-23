#include "global.h"
#include "pokemon.h"
#include "item.h"
#include "system.h"
#include "pm_version.h"

extern u32 AGB_GetBoxMonData(void *, int, void *);
extern const u16 ov74_0223CBA0[];
int AGB_GetBoxMonAbility(void *agbMon, BoxPokemon *boxMon);
extern int TranslateAgbSpecies(int);
extern BOOL ConvertRSStringToDPStringInternational(const u8 *, u16 *, u32, u32);
void MigrateBoxMon(void *agbMon, BoxPokemon *boxMon);

int AGB_GetBoxMonAbility(void *agbMon, BoxPokemon *boxMon) {
    u16 species = GetBoxMonData(boxMon, 5, NULL);
    u32 abilityNum = AGB_GetBoxMonData(agbMon, 0x2E, NULL);
    int ability = GetMonBaseStat(species, 0x19);
    u32 i;

    if (ability != 0) {
        for (i = 0; i < 0x5E; i++) {
            if (ov74_0223CBA0[i] == species) {
                ability = GetMonBaseStat(species, 0x18);
                break;
            }
        }
        if (i == 0x5E && !(abilityNum & 1)) {
            return GetMonBaseStat(species, 0x18);
        }
    } else {
        ability = GetMonBaseStat(species, 0x18);
    }
    return ability;
}

#define COPY_FIELD(src, dst) do { data = AGB_GetBoxMonData(agbMon, (src), NULL); SetBoxMonData(boxMon, (dst), &data); } while (0)

void MigrateBoxMon(void *agbMon, BoxPokemon *boxMon) {
    u32 lock;
    u32 data;
    volatile u32 language;
    u8 srcString[12];
    u16 dstString[12];
    int j;
    int i;

    ZeroBoxMonData(boxMon);
    lock = AcquireBoxMonLock(boxMon);
    COPY_FIELD(0, 0);
    data = AGB_GetBoxMonData(agbMon, 11, NULL);
    data = TranslateAgbSpecies(data);
    SetBoxMonData(boxMon, 5, &data);
    data = AGB_GetBoxMonData(agbMon, 12, NULL);
    if (data != 0) {
        data = UpConvertItemId_Gen3to4((u16)data);
    }
    SetBoxMonData(boxMon, 6, &data);
    COPY_FIELD(1, 7);
    COPY_FIELD(0x19, 8);
    data = 0x46;
    SetBoxMonData(boxMon, 9, &data);
    data = AGB_GetBoxMonAbility(agbMon, boxMon);
    SetBoxMonData(boxMon, 10, &data);
    COPY_FIELD(8, 11);
    language = AGB_GetBoxMonData(agbMon, 3, NULL);
    SetBoxMonData(boxMon, 12, &language);
    COPY_FIELD(0x1a, 13);
    COPY_FIELD(0x1b, 14);
    COPY_FIELD(0x1c, 15);
    COPY_FIELD(0x1d, 16);
    COPY_FIELD(0x1e, 17);
    COPY_FIELD(0x1f, 18);
    COPY_FIELD(0x16, 19);
    COPY_FIELD(0x17, 20);
    COPY_FIELD(0x18, 21);
    COPY_FIELD(0x21, 22);
    COPY_FIELD(0x2f, 23);
    COPY_FIELD(0x30, 24);
    for (i = 0, j = 0; i < 4; i++, j += 2) {
        COPY_FIELD(i + 13, i + 0x36);
        data = AGB_GetBoxMonData(agbMon, 0x15, NULL);
        data = (data & (3 << j)) >> j;
        SetBoxMonData(boxMon, i + 0x3e, &data);
        data = GetBoxMonData(boxMon, i + 0x42, NULL);
        SetBoxMonData(boxMon, i + 0x3a, &data);
    }
    COPY_FIELD(0x27, 0x46);
    COPY_FIELD(0x28, 0x47);
    COPY_FIELD(0x29, 0x48);
    COPY_FIELD(0x2a, 0x49);
    COPY_FIELD(0x2b, 0x4a);
    COPY_FIELD(0x2c, 0x4b);
    COPY_FIELD(0x2d, 0x4c);
#define COPY_MARKS(src, start) do { \
    j = AGB_GetBoxMonData(agbMon, (src), NULL); \
    if (j <= 4) { \
        for (i = 0; i < j; i++) { \
            data = 1; \
            SetBoxMonData(boxMon, i + (start), &data); \
        } \
    } \
} while (0)
    COPY_MARKS(0x32, 0x4e);
    COPY_MARKS(0x33, 0x52);
    COPY_MARKS(0x34, 0x56);
    COPY_MARKS(0x35, 0x5a);
    COPY_MARKS(0x36, 0x5e);
    COPY_FIELD(0x43, 0x62);
    COPY_FIELD(0x44, 0x63);
    COPY_FIELD(0x45, 0x64);
    COPY_FIELD(0x46, 0x65);
    COPY_FIELD(0x47, 0x66);
    COPY_FIELD(0x48, 0x67);
    COPY_FIELD(0x49, 0x68);
    COPY_FIELD(0x4a, 0x69);
    COPY_FIELD(0x4b, 0x6a);
    COPY_FIELD(0x4c, 0x6b);
    COPY_FIELD(0x4d, 0x6c);
    COPY_FIELD(0x4e, 0x6d);
    COPY_FIELD(0x50, 0x6e);
    data = GetBoxMonGender(boxMon);
    SetBoxMonData(boxMon, 0x6f, &data);
    if (GetBoxMonData(boxMon, 5, NULL) == 0xc9) {
        u32 personality = AGB_GetBoxMonData(agbMon, 0, NULL);
        data = personality;
        u32 mask = 3;
        u32 a = ((mask << 8) & personality) >> 6;
        u32 b = ((mask << 24) & personality) >> 18;
        u32 c = ((mask << 16) & personality) >> 12;
        u32 d = personality & mask;
        data = (b | c | a | d) % 28;
        SetBoxMonData(boxMon, 0x70, &data);
    }
    if (GetBoxMonData(boxMon, 5, NULL) == 0x182) {
        switch (gSystem.unk6A) {
        case 0:
        case 1:
        case 2:
        default:
            data = 0;
            break;
        case 4: data = 1; break;
        case 5: data = 2; break;
        case 3: data = 3; break;
        }
        SetBoxMonData(boxMon, 0x70, &data);
    }
    AGB_GetBoxMonData(agbMon, 2, srcString);
    ConvertRSStringToDPStringInternational(srcString, dstString, 12, language);
    SetBoxMonData(boxMon, 0x76, dstString);
    if (AGB_GetBoxMonData(agbMon, 3, NULL) != gGameLanguage) {
        data = 1;
        SetBoxMonData(boxMon, 0x4d, &data);
    }
    COPY_FIELD(0x25, 0x7a);
    AGB_GetBoxMonData(agbMon, 7, srcString);
    ConvertRSStringToDPStringInternational(srcString, dstString, 8, language);
    SetBoxMonData(boxMon, 0x90, dstString);
    COPY_FIELD(0x23, 0x99);
    COPY_FIELD(0x22, 0x9a);
    COPY_FIELD(0x26, 0x9b);
    COPY_FIELD(0x24, 0x9c);
    COPY_FIELD(0x31, 0x9d);
    ReleaseBoxMonLock(boxMon, lock);
}
