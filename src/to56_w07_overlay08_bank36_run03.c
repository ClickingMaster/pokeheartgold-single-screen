#include "global.h"

#include "constants/heap.h"

#include "battle/battle_finger.h"
#include "battle/battle_system.h"

#include "sprite_system.h"
extern const s32 ov08_02225D2C[];
extern const s32 ov08_02225D30[];
void ov08_0222419C(u8 *);
void ov08_022241F4(u8 *);
void ov08_0222421C(u8 *, int);
void ov08_0222458C(u16 *, const u16 *, u16, u16, u8, u8);

typedef struct Ov08StateTail {
    u8 pad[0x113e];
    u8 a, b, c;
    u8 low : 4;
    u8 high : 4;
} Ov08StateTail;
void ov08_02224938(Ov08StateTail *, u8, u8);

void ov08_02224938(Ov08StateTail *work, u8 a1, u8 a2) {
    work->a = 0;
    work->b = 0;
    work->c = a1;
    work->high = a2;
    work->low = 1;
}
