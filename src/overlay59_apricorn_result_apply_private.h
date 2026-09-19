#ifndef OV59_R12_PRIVATE_H
#define OV59_R12_PRIVATE_H
#include "global.h"

#include "party.h"
#include "pokemon.h"
#include "unk_02031B0C.h"

typedef struct Ov59R12Entry {
    Pokemon *mon;
    u16 species;
    u8 unk06;
    u8 unk07;
    u8 slot;
    u8 unk09;
    u8 pad0A[2];
    void *name;
    PartyExtraSub aprijuice;
    u8 pad15;
    struct PokeathlonPerformanceStars stars;
    u8 pad1E[0x16];
} Ov59R12Entry;

typedef struct Ov59R12Mode {
    u8 pad00[0x14];
    int mode;
} Ov59R12Mode;

typedef struct Ov59R12Work {
    enum HeapID heap;
    Ov59R12Mode *mode;
    u8 pad08[8];
    Party *party;
    SaveApricornBox *box;
    UnkStruct_02031CEC record;
    u8 pad20[0xE];
    u16 oldStars[4];
    u8 pad36[3];
    s8 starDelta[5];
    u8 changedCount;
    u8 pad3F[9];
    u8 ready;
    u8 pad49;
    u8 selected;
    u8 pad4B[0x31];
    Ov59R12Entry entries[6];
} Ov59R12Work;

extern const u32 ov59_0223C99C[];
void ov58_022379C0(UnkStruct_02031CEC *record, Party *party, int slot);
int sub_02032024(SaveApricornBox *box);
void ov59_0223BAE8(Ov59R12Work *work);
#endif
