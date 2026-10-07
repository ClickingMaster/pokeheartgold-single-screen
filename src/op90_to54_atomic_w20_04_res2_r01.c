#pragma force_active on
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
    struct {
        u8 unk0 : 1;
        u8 mode : 1;
        u8 unk2 : 6;
    } flags2;
    u8 unkF;
} Ov112GiftMon;

typedef struct Ov112WalkerFlags {
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 hasGift : 1;
    u8 rest : 2;
} Ov112WalkerFlags;

typedef struct Ov112GiftFlags {
    u8 bit0 : 1;
    u8 mode : 1;
    u8 rest : 6;
} Ov112GiftFlags;

#define U8_AT(p, o)   (*(u8 *)((p)->raw + (o)))
#define U16_AT(p, o)  (*(u16 *)((p)->raw + (o)))
#define U32_AT(p, o)  (*(u32 *)((p)->raw + (o)))
#define PTR_AT(p, o)  (*(void **)((p)->raw + (o)))

extern void SetBoxMonData(void *, int, const void *);
extern void *PlayerProfile_GetPlayerName_NewString(void *, int);
extern void String_Delete(void *);
extern void *String_New(u32, int);
extern void CopyU16ArrayToString(void *, const u16 *);
extern void ZeroMonData(void *);
extern void ov112_021EE9A4(void *, void *, const Ov112GiftMon *, u32, u32);
extern void ov112_021EE9E4(void *, u32, const Ov112GiftMon *, u32, u32);
extern void *SaveArray_PCStorage_Get(void *);
extern void *AllocMonZeroed(int);
extern void *Mon_GetBoxMon(void *);
extern void sub_02032688(void *, u16 *, u16 *);
extern u32 MTRandom(void);
extern void BoxMonSetTrainerMemo(void *, void *, int, int, int);
extern int PCStorage_FindFirstEmptySlot(void *, int *, int *);
extern int PCStorage_PlaceMonInBoxByIndexPair(void *, int, int, void *);
extern void *PCStorage_GetMonByIndexPair(void *, int, int);
extern void Pokedex_SetMonSeenFlag(void *, void *);
extern void Pokedex_SetMonCaughtFlag(void *, void *);
extern void Heap_Free(void *);
void ov112_021EEAF0(Ov112State *p, BOOL useBox);

// @fn ov112_021EEAF0
void ov112_021EEAF0(Ov112State *p, BOOL useBox) {
    int box;
    int slot;
    u16 unk;
    u16 boxIdx;
    u8 caughtFlag;
    u8 one;
    void *boxMon;
    Ov112GiftMon *special;
    void *playerName;
    int i;
    void *nickname;
    void *pcStorage;
    void *mon;
    Ov112GiftMon *gift;
    void *boxMon2;

    pcStorage = SaveArray_PCStorage_Get(PTR_AT(p, 0x20));
    mon = AllocMonZeroed(0x9A);
    playerName = PlayerProfile_GetPlayerName_NewString(PTR_AT(p, 0x1E438), 0x9A);
    sub_02032688(PTR_AT(p, 0x1E440), &unk, &boxIdx);
    if (useBox) {
        box = boxIdx;
    } else {
        box = 0;
    }
    slot = 0;
    gift = (Ov112GiftMon *)&U8_AT(p, 0x9D7C);
    for (i = 0; i < 3; i++, gift++) {
        if (gift->species != 0) {
            ov112_021EE9A4(mon, PTR_AT(p, 0x1E438), gift, MTRandom() % 24, 0);
            BoxMonSetTrainerMemo(Mon_GetBoxMon(mon), PTR_AT(p, 0x1E438), 0, 0xE9, 0x9A);
            boxMon = Mon_GetBoxMon(mon);
            PCStorage_FindFirstEmptySlot(pcStorage, &box, &slot);
            PCStorage_PlaceMonInBoxByIndexPair(pcStorage, box, slot, boxMon);
            Pokedex_SetMonSeenFlag(PTR_AT(p, 0x1E444), mon);
            Pokedex_SetMonCaughtFlag(PTR_AT(p, 0x1E444), mon);
        }
    }
    if (((Ov112WalkerFlags *)&U8_AT(p, 0xAABC))->hasGift) {
        nickname = String_New(0x10, 0x9A);
        special = (Ov112GiftMon *)&U8_AT(p, 0xAD00);
        if (special->species != 0) {
            CopyU16ArrayToString(nickname, &U16_AT(p, 0xAD1E));
            ZeroMonData(mon);
            boxMon2 = Mon_GetBoxMon(mon);
            ov112_021EE9E4(mon, U32_AT(p, 0xAD14), special, MTRandom() % 24, special->flags2.mode);
            caughtFlag = ((Ov112GiftFlags *)&U8_AT(p, 0xAD2E))->bit0;
            SetBoxMonData(boxMon2, 0x9D, &caughtFlag);
            SetBoxMonData(boxMon2, 0x91, nickname);
            BoxMonSetTrainerMemo(Mon_GetBoxMon(mon), PTR_AT(p, 0x1E438), 4, U16_AT(p, 0xAD1A), 0x9A);
            SetBoxMonData(boxMon2, 0xA, &U8_AT(p, 0xAD2F));
            one = 1;
            SetBoxMonData(boxMon2, 0x6E, &one);
            SetBoxMonData(boxMon2, 0x9B, &U8_AT(p, 0xAD30));
            PCStorage_FindFirstEmptySlot(pcStorage, &box, &slot);
            PCStorage_PlaceMonInBoxByIndexPair(pcStorage, box, slot, boxMon2);
            Pokedex_SetMonSeenFlag(PTR_AT(p, 0x1E444), mon);
            Pokedex_SetMonCaughtFlag(PTR_AT(p, 0x1E444), mon);
            if (PTR_AT(p, 0x1E430) == NULL) {
                PTR_AT(p, 0x1E430) = PCStorage_GetMonByIndexPair(pcStorage, box, slot);
            }
        }
        String_Delete(nickname);
    }
    String_Delete(playerName);
    Heap_Free(mon);
}
