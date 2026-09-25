#include "global.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S32(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

extern const u8 ov65_0221FE6C[];
extern int TouchscreenHitbox_FindRectAtTouchNew(const void *);
extern void *Save_Chatot_Get(void *);
extern void sub_02006E4C(void *, u32, u32, s32);
extern void PlayCry(u16, u8);
extern void Sprite_SetAnimActiveFlag(void *, u32);
extern void Sprite_SetAnimCtrlSeq(void *, u32);

void ov65_0221DC34(void *work);

void ov65_0221DC34(void *work) {
    int hit;
    int cryForm;
    u16 species;

    if (S32(work, 0x94) >= 6) {
        return;
    }
    hit = TouchscreenHitbox_FindRectAtTouchNew(ov65_0221FE6C);
    if (hit == -1) {
        return;
    }
    if (hit >= 6) {
        return;
    }
    if (U16((u8 *)work + hit * 16, 0x69c) == 0) {
        return;
    }
    if (U8((u8 *)work + S32(work, 0x94) * 16, 0x6a1) != 0) {
        return;
    }
    species = U16((u8 *)work + S32(work, 0x94) * 16, 0x69c);
    cryForm = U8((u8 *)work + S32(work, 0x94) * 16, 0x6a2);
    if (species == 0x1b9) {
        sub_02006E4C(Save_Chatot_Get(PTR(work, 4)), 0, 0x64, 0);
    } else {
        PlayCry(species, (u8)cryForm);
    }
    Sprite_SetAnimActiveFlag(PTR(work, 0x40c), TRUE);
    Sprite_SetAnimCtrlSeq(PTR(work, 0x40c), 4);
}
