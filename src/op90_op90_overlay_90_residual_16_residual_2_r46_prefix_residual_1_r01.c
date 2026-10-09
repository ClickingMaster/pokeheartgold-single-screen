#pragma force_active on
#include "global.h"
extern const u8 ov90_0225C324[][12];
extern const u8 ov90_0225C31C[][12];
extern const s16 ov90_0225C276[];
extern const s16 ov90_0225C274[];
typedef struct Ov90SpritePair {
    u8 pad00[0x10];
    void *sprite1;
    void *sprite2;
    u16 stage;
    u8 pad1A;
    u8 selection;
} Ov90SpritePair;
void Sprite_SetMatrix(void *, VecFx32 *);
void ov90_02259FC8(Ov90SpritePair *, u32);
// @fn ov90_02259FC8
#pragma opt_lifetimes off
void ov90_02259FC8(Ov90SpritePair *p, u32 x) {
    VecFx32 v;
    const u8 (*rows)[12] = ov90_0225C31C;
    u32 s = p->stage - 1;
    const u8 (*indices)[12] = ov90_0225C324;
    u8 idx = indices[s][p->selection];
    u8 py = ov90_0225C276[idx * 2] + rows[s][p->selection] * 8;
    u8 px = ov90_0225C274[idx * 2];
    v.x = (px + x) << 12;
    v.y = py << 12;
    Sprite_SetMatrix(p->sprite1, &v);
    x += 0x18;
    s = p->stage - 1;
    py = rows[s][p->selection] * 8 + 0x15;
    v.x = x << 12;
    v.y = py << 12;
    Sprite_SetMatrix(p->sprite2, &v);
}
#pragma opt_lifetimes reset
