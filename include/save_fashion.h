#ifndef POKEHEARTGOLD_SAVE_FASHION_H
#define POKEHEARTGOLD_SAVE_FASHION_H

#include "application/dress_up/dress_up_internal.h"

#include "fashion_case.h"
#include "heap.h"
#include "pm_string.h"
#include "pokemon.h"

typedef struct SaveFashionMon SaveFashionMon;

void Save_FashionData_Init(SaveFashionData *);
u32 Save_FashionData_sizeof(void);
u32 FashionEntry_sizeof(void);
SaveFashionDataSub *FashionEntry_New(enum HeapID);
BOOL FashionEntry_IsSaved(SaveFashionDataSub *);
u8 FashionCase_GetWallpaperOrder(FashionCase *, int);
void FashionEntry_MarkSaved(SaveFashionDataSub *);
void FashionEntry_Clear(SaveFashionDataSub *);
void FashionEntry_SetMon(SaveFashionDataSub *, Pokemon *, DressUpMonPic *);
void FashionEntry_PlaceItem(SaveFashionDataSub *, DressUpSlot *, int);
void FashionEntry_SetBackdrop(SaveFashionDataSub *, int);
void FashionEntry_SetMonSecondText(SaveFashionDataSub *, String *, u8);
BOOL FashionEntry_IsItemPlaced(SaveFashionDataSub *, int);
SaveFashionMon *FashionEntry_GetMon(SaveFashionDataSub *);
u8 *FashionEntry_GetItem(SaveFashionDataSub *, int);
void FashionEntry_GetMonSecondText(SaveFashionDataSub *, String *);
u8 FashionEntry_GetBackdrop(SaveFashionDataSub *);
u16 FashionEntry_GetMailMessageWord(SaveFashionDataSub *);
s8 FashionMon_GetZ(SaveFashionMon *);
u8 FashionMon_GetX(SaveFashionMon *);
u8 FashionMon_GetY(SaveFashionMon *);
void FashionMon_ToPokemon(SaveFashionMon *, Pokemon *);
u8 FashionItem_GetId(u8 *);
u8 FashionItem_GetX(u8 *);
u8 FashionItem_GetY(u8 *);
s8 FashionItem_GetZ(s8 *);
void Save_FashionData_InsertEntries(u8, int, SaveFashionData *, SaveFashionDataSub **);

#endif // POKEHEARTGOLD_SAVE_FASHION_H
