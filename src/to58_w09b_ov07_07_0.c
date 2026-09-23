#include "global.h"

typedef struct { int time; int sound; } SoundEvent;
extern const s16 ov07_0223730E[];
extern const u16 ov07_02237310[];
extern const SoundEvent ov07_02237200[];
extern void ManagedSprite_SetAnim(void *, int);
extern void ManagedSprite_SetAnimationFrame(void *, int);
extern void ManagedSprite_OffsetPositionXY(void *, int, int);
extern void sub_0200602C(int, int);
extern void ov07_02232F74(void *, int);
int ov07_02233228(u8 *);

int ov07_02233228(u8 *state) {
    switch (*(int *)(state + 8)) {
    case 0:
        ManagedSprite_SetAnim(*(void **)(state + 0x30), 1);
        ManagedSprite_SetAnimationFrame(*(void **)(state + 0x30), 0);
        *(int *)(state + 8) += 1;
    case 1: {
        int index = *(int *)(state + 0xc);
        int offset = index * 6;
        int amount = *(const s16 *)((const u8 *)ov07_0223730E + offset);
        int frame = *(const u16 *)((const u8 *)ov07_02237310 + offset);
        ManagedSprite_SetAnimationFrame(*(void **)(state + 0x30), frame);
        *(int *)(state + 0xc) += 1;
        for (int i = 0; i < 5; i++) {
            if (*(int *)(state + 0xc) == ov07_02237200[i].time) {
                sub_0200602C((u16)ov07_02237200[i].sound, 0x75);
            }
        }
        if (*(int *)(state + 0xc) > 20 || amount == 255) {
            *(int *)(state + 8) += 1;
        } else {
            ManagedSprite_OffsetPositionXY(*(void **)(state + 0x30), 0, amount);
        }
        break;
    }
    default:
        ov07_02232F74(state, 14);
        break;
    }
    return 1;
}
