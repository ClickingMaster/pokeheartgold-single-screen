#ifndef TO54_W20_PRIVATE_H
#define TO54_W20_PRIVATE_H

#include "global.h"

typedef struct Ov112State {
    u8 raw[0x20000];
} Ov112State;

typedef struct Ov112GiftMon {
    u16 species;
    u16 moves[4];
    u16 unkA;
    u8 level;
    struct {
        u8 form : 5;
        u8 gender : 2;
        u8 unk7 : 1;
    } flags;
} Ov112GiftMon;

typedef union Ov112FlagByte {
    u8 raw;
    struct {
        u8 bit0 : 1;
        u8 rest : 7;
    } bits;
} Ov112FlagByte;

#define U8_AT(p, o)   (*(u8 *)((p)->raw + (o)))
#define U16_AT(p, o)  (*(u16 *)((p)->raw + (o)))
#define U32_AT(p, o)  (*(u32 *)((p)->raw + (o)))
#define PTR_AT(p, o)  (*(void **)((p)->raw + (o)))
#define FLAG_AT(p, o) (*(Ov112FlagByte *)((p)->raw + (o)))

extern int TextPrinterCheckActive(u8 id);
extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern u32 GetBoxMonData(void *, int, void *);
extern u32 CalcBoxMonExpToNextLevel(void *);
extern void CopyBoxPokemonToPokemon(void *, void *);
extern void SetMonData(void *, int, const void *);
extern void SetBoxMonData(void *, int, const void *);
extern void CalcMonLevelAndStats(void *);
extern void MonApplyFriendshipMod(void *, int, int);
extern u16 sub_02032764(void *);
extern void sub_0203276C(void *);
extern int Bag_AddItem(void *, int, int, int);
extern void *PlayerProfile_GetPlayerName_NewString(void *, int);
extern u32 PlayerProfile_GetTrainerID(void *);
extern void String_Delete(void *);
extern u32 ChangePersonalityToNatureGenderAndAbility(u32, u16, u8, u8, u8, int);
extern void CreateMon(void *, int, int, int, int, int, int, int);
extern void ov112_021EE9E4(void *, u32, const Ov112GiftMon *, u32, u32);
extern void *SaveArray_PCStorage_Get(void *);
extern void *AllocMonZeroed(int);
extern void *Mon_GetBoxMon(void *);
extern void sub_02032688(void *, u16 *, u16 *);
extern int Pokewalker_TryGetBoxMon(void *, void *);
extern void Pokewalker_ClearBoxMon(void *);
extern u32 MTRandom(void);
extern void BoxMonSetTrainerMemo(void *, void *, int, int, int);
extern int PCStorage_FindFirstEmptySlot(void *, int *, int *);
extern int PCStorage_PlaceMonInBoxByIndexPair(void *, int, int, void *);
extern void *PCStorage_GetMonByIndexPair(void *, int, int);
extern void Pokedex_SetMonSeenFlag(void *, void *);
extern void Pokedex_SetMonCaughtFlag(void *, void *);
extern void Heap_Free(void *);
extern int ov112_021EE8BC(void *, void *, int);
extern void ov112_021EE970(void *, int);
extern void ov112_021EE9A4(void *, void *, const Ov112GiftMon *, u32, u32);
int ov112_021EE754(Ov112State *);
int ov112_021EE778(Ov112State *);
int ov112_021EE8BC(void *, void *, int);
void ov112_021EE920(Ov112State *);
void ov112_021EE970(void *, int);
void ov112_021EE9A4(void *, void *, const Ov112GiftMon *, u32, u32);
void ov112_021EEA7C(Ov112State *);
void ov112_021EE7A8(Ov112State *);

#endif
