#include "overlay96_course_collision_update_internal.h"

void ov96_021F6600(void *course, void *work) {
    u8 *player;
    u8 *timer;
    u8 *counter;
    u8 participant;
    int outer;
    int inner;
    const u8 *sound;
    struct {
        s32 x;
        s32 y;
        s32 radius;
        VecFx32 pos;
    } local;

    participant = (u8)ov96_021E5F24(course);

    for (outer = 0; outer < 3; outer++) {
        ov96_021F6798(course, work, (u8)outer);
    }
    outer = 0;
    player = work;
    timer = work;
    counter = (u8 *)work + 0x90;
    sound = ov96_0221DC18;
    while (outer < 3) {
        void *object = PTR_AT(player, 0x90);
        ov96_021EB0A4(object, S32_AT(player, 0xac) / 4096, (0x120000 - (S32_AT(timer, 0xfb4) << 6)) / 4096, &local.y, &local.x);
        ov96_021EAF78(object, local.y << 12, local.x << 12, &local.pos.x, &local.pos.y, &local.radius);
        local.pos.z = 0;
        for (inner = 0; inner < 29; inner++) {
            if (ov96_021F75E0(PTR_AT(work, 0x8c), (u8)inner) != NULL) {
                void *item = ov96_021F75D4(PTR_AT(work, 0x8c), (u8)inner);
                if (ov96_021F6060(&local.pos, local.radius << 12, item, 0x8000)) {
                    u8 value;
                    ov96_021F75BC(PTR_AT(work, 0x8c), (u8)inner);
                    value = ov96_021F65D8((Ov96CourseCollisionCounter *)counter);
                    ov96_021F75E8(PTR_AT(work, 0x8c), (u8)inner, value);
                    U8_AT(work, 0x142) = 1;
                    PlaySEOnPlayer(0x88d, *sound);
                    GF_SndSetTrackPitchByPlayerNo(*sound, 0xffff, ov96_0221C110[value - 1]);
                    ov96_021E8228(course, participant, (u8)outer, 3, 1);
                }
            }
        }
        player += 0x38;
        sound++;
        timer += 0x1c;
        counter += 0x38;
        outer++;
    }

    for (outer = 0; outer < 3; outer++, work = (u8 *)work + 0x38) {
        if (U8_AT(work, 0xb6) != 0) {
            U8_AT(work, 0xb7)
            --;
            if (U8_AT(work, 0xb7) == 0) {
                U8_AT(work, 0xb7) = 0;
                U8_AT(work, 0xb6) = 0;
            }
        }
    }
}
