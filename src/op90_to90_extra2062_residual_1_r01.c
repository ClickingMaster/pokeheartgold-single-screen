#pragma force_active on
#include "global.h"
#include "sprite_system.h"

typedef struct Slot {
    u32 lo : 18;
    u32 val : 8;
    u32 hi : 6;
} Slot;

typedef struct Racer {
    u8 pad0[0xc];
    void *course;
    ManagedSprite *sprite;
    u8 pad14[0x20];
    Slot slots[3];
    struct {
        u32 kind : 2;
        u32 state : 8;
        u32 rest : 22;
    } f40;
    struct {
        u32 low : 8;
        u32 flag : 1;
        u32 pad : 7;
        u32 mid : 8;
        u32 hi : 8;
    } f44;
} Racer;

extern int ov96_0221E5A4;

u8 ov96_0220D33C(u32 val, u16 mid);
void ov96_0220C578(Racer *racer, int state);
void ov96_0220C714(Racer *racer, void *a, u8 b);
int ov96_021E5F24(void *course);
void ov96_021E8228(void *course, u8 a, u8 b, int c, int d);
void ov96_0220B634(Racer *racer, void *a, u8 b);

// @fn ov96_0220B634
void ov96_0220B634(Racer *racer, void *a, u8 b) {
    ov96_0221E5A4 = 0;
    GF_ASSERT(racer->f40.state != 2);
    if (racer->f40.state == 1) {
        Slot *slot = &racer->slots[racer->f40.kind];
        slot->val = ov96_0220D33C(slot->val, racer->f44.mid);
        if (slot->val == 0) {
            ov96_0220C578(racer, 2);
            return;
        }
    }
    if (!racer->f44.flag) {
        ov96_0220C714(racer, a, b);
    }
    ManagedSprite_SetAnim(racer->sprite, 2);
    ov96_021E8228(racer->course, ov96_021E5F24(racer->course), racer->f40.kind, 4, 1);
    ov96_0220C578(racer, 1);
}
