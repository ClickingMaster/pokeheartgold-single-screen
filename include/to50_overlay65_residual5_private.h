#ifndef TO50_OV65_R5_PRIVATE_H
#define TO50_OV65_R5_PRIVATE_H
#include "global.h"

#include "player_data.h"
#include "string_util.h"
typedef struct Ov65RecordR5 {
    u8 pad0[0x10];
    u32 trainerId;
    u8 language;
    u8 version;
    u8 gender;
    u8 pad17[0x71];
} Ov65RecordR5;
typedef struct Ov65PacketR5 {
    u16 name[8];
    u32 trainerId;
    u8 language;
    u8 version;
    u8 gender;
    u8 pad17;
    u32 ids[16];
    u8 versions[16];
    u8 languages[16];
    u8 genders[16];
} Ov65PacketR5;
extern s32 sub_02036FD8(u32, void *, u32);
s32 ov65_0221DEA0(PlayerProfile *, Ov65RecordR5 *, Ov65PacketR5 *);
#endif
