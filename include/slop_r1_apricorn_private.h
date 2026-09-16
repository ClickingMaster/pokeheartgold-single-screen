#ifndef POKEHEARTGOLD_SLOP_R1_APRICORN_PRIVATE_H
#define POKEHEARTGOLD_SLOP_R1_APRICORN_PRIVATE_H

#include "global.h"

typedef struct UnkStruct_02031CEC {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u32 unk4;
} UnkStruct_02031CEC;

typedef struct SaveApricornBox {
    u8 apricorns[7];
    u8 kurtQuantity;
    u8 kurtApricorn;
    u8 unk_09;
    u16 unk_0A;
    u8 unk_0C[8];
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u8 unk_20[3][0x20];
} SaveApricornBox;

typedef struct ApricornRecord {
    u32 trainerId;
    u8 gender;
    u8 language;
    u8 version;
    u8 padding;
    u16 name[8];
    UnkStruct_02031CEC data;
} ApricornRecord;

BOOL sub_02031D6C(UnkStruct_02031CEC *data);
BOOL sub_02032158(ApricornRecord *a, ApricornRecord *b);

int sub_02031C08(SaveApricornBox *box);
void sub_020321A0(SaveApricornBox *box, ApricornRecord *records, int count, int skip);

#endif // POKEHEARTGOLD_SLOP_R1_APRICORN_PRIVATE_H
