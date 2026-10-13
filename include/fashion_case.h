#ifndef POKEHEARTGOLD_FASHION_CASE_H
#define POKEHEARTGOLD_FASHION_CASE_H

#include "mail_message.h"

typedef struct SaveFashionDataSub {
    u8 filler_00[0x40];
    MailMessage mailMessage;
    u8 filler_48[0x2C];
} SaveFashionDataSub; // size=0x74

typedef struct SaveFashionDataSub3FC {
    u8 filler_00[0x98];
} SaveFashionDataSub3FC; // size=0x98

typedef struct FashionCase {
    u32 accessories[40 / sizeof(u32)];
    u32 wallpaperOrder[(18 + sizeof(u32) - 1) / sizeof(u32)];
    u8 padding_3C[4];
} FashionCase;

typedef struct SaveFashionData {      // todo: rename to SaveFashionData
    SaveFashionDataSub entries[11];   // 000
    SaveFashionDataSub3FC largeEntries[5]; // 4FC
    FashionCase fashionCase;          // 7F4
} SaveFashionData;                    // size=0x834

SaveFashionData *Save_FashionData_Get(SaveData *saveData);
FashionCase *Save_FashionData_GetFashionCase(SaveFashionData *unk);
u32 FashionCase_CountAccessories(FashionCase *fashionCase);
u32 FashionCase_CountWallpapers(FashionCase *fashionCase);
BOOL Save_FashionData_IsEntrySaved(SaveFashionData *fashionData, int a1);
BOOL Save_FashionData_IsLargeEntrySaved(SaveFashionData *fashionData, int a1);
SaveFashionDataSub *Save_FashionData_GetEntry(SaveFashionData *fashionData, int a1);
void FashionEntry_SetMailMessageWord(SaveFashionDataSub *fashionDataSub, int a1);
void FashionCase_GiveFashionItem(FashionCase *fashionCase, int id, int quantity);
int FashionCase_CanGiveAccessory(FashionCase *fashionCase, int a1, int a2);
u32 FashionCase_GetAccessoryCount(FashionCase *fashionCase, int a1);
void FashionCase_GiveContestBackground(FashionCase *fashionCase, int id);
int FashionCase_HasWallpaper(FashionCase *fashionCase, int a1);
void FashionCase_TakeAccessory(FashionCase *fashionCase, int a1, int a2);

#endif // POKEHEARTGOLD_FASHION_CASE_H
