#ifndef TO50_UNK932_R13_PREFIX_H
#define TO50_UNK932_R13_PREFIX_H
#include "global.h"

#include "sprite.h"
#include "grid_cursor.h"
typedef struct Entry34R13 {
    Sprite *sprite;
    u8 pad[0x30];
} Entry34R13;
typedef struct TripleR13 {
    Sprite *sprite;
    s32 a;
    u32 b;
} TripleR13;
typedef struct Row8R13 {
    u32 a;
    u32 b;
} Row8R13;
typedef struct WorkR13Prefix {
    u8 pad000[0x0D];
    u8 count;
    u8 pad00E[0x14 - 0x0E];
    s32 state;
    u8 pad018[0x198 - 0x18];
    Entry34R13 entriesB[30];
    u8 pad7B0[0x7E8 - 0x7B0];
    Entry34R13 entries[3];
    u8 pad884[0x8B4 - 0x884];
    Sprite *sprite8B4;
    Sprite *sprite8B8;
    Sprite *sprite8BC;
    Sprite *sprite8C0;
    Sprite *sprite8C4;
    Sprite *extra[2];
    TripleR13 *list;
    u8 pad8D4[0x4684 - 0x8D4];
    s32 index4684;
    u8 pad4688[0x4694 - 0x4688];
    s32 posX;
    s32 posY;
    u8 pad469C[0x46A0 - 0x469C];
    Entry34R13 *entriesPtr;
    struct Row8R13 *rowsPtr;
    u8 pad46A8[0x46B8 - 0x46A8];
    void *cursor;
    s32 flag46BC;
} WorkR13Prefix;
void sub_0209515C(WorkR13Prefix *);
void sub_02095238(WorkR13Prefix *);
struct BoxMonSelector;
int sub_02095354(struct BoxMonSelector *);
int sub_02094794(WorkR13Prefix *, int);
int sub_02094860(WorkR13Prefix *, int);
void sub_020948C4(WorkR13Prefix *, int, int);
void sub_02094A70(WorkR13Prefix *);
void sub_0209569C(WorkR13Prefix *);
void sub_020956B8(WorkR13Prefix *);
#endif
