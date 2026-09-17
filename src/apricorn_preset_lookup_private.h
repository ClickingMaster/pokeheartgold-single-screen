#ifndef TO51_W01_PRIVATE_H
#define TO51_W01_PRIVATE_H
#include "global.h"
typedef struct SaveApricornBox SaveApricornBox;
typedef struct UnkStruct_02031CEC {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u32 unk4;
} UnkStruct_02031CEC;
typedef struct ApricornPreset {
    u16 value[4];
} ApricornPreset;
extern const ApricornPreset _020F6902[];
void sub_02031B5C(void *out);
int sub_02031D80(UnkStruct_02031CEC *data);
BOOL sub_02031CEC(SaveApricornBox *, int, UnkStruct_02031CEC *);
#endif
