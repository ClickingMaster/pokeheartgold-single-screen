#ifndef TO56_R3_W18_PRIVATE_H
#define TO56_R3_W18_PRIVATE_H
#include "global.h"
typedef struct Ov08WorkR3W18 {
    u8 pad[0x2077];
    u8 mode : 4;
    u8 upper : 4;
    u8 pad2078[2];
    u8 selection;
} Ov08WorkR3W18;
typedef struct Ov08OwnerR3W18 {
    void *unk0;
    u8 pad4[8];
    int heapId;
    u8 pad10[0x12];
    u16 item;
} Ov08OwnerR3W18;
extern void ov08_0221D840(void *work, int state);
extern void ov08_022213C8(void *work);
extern s32 GetItemAttr(u16 item, u16 attr, int heapID);
extern void ov08_02220C5C(void *work, u8 selection);
extern void ov08_0221DD70(void *work, u8 selection);
extern void ov08_022221CC(void *work, u8 selection);
extern void ov08_0221D6CC(void *work, u8 selection);
int ov08_0221C954(Ov08WorkR3W18 *work);
int ov08_0221C978(Ov08WorkR3W18 *work);
int ov08_0221C9A4(Ov08OwnerR3W18 **work);
int ov08_0221C9C8(Ov08WorkR3W18 *work);
#endif
