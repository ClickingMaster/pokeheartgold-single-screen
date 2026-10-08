#pragma force_active on
#include "global.h"
typedef struct Rot49 {
    u32 unk00;
    u8 unk04[8];
} Rot49;
typedef struct Pos49 {
    u32 unk00;
    u8 unk04[0x24];
} Pos49;
typedef struct Scene49 {
    s16 state;
    u8 unk02[2];
    u16 count;
    u8 unk06[2];
    Pos49 pos[4];
    Rot49 rots[4];
    void *renders[4];
} Scene49;
void ov49_022655F4(Rot49 *rot, int a1, int a2, int a3);
void ov49_02265628(Rot49 *rot);
void ov49_02265434(Pos49 *pos, int state);
void ov49_02265660(Rot49 *rot, fx32 *y);
void ov49_022655E0(Pos49 *pos, fx32 *x, fx32 *y, fx32 *z);
void sub_020182A8(void *render, fx32 x, fx32 y, fx32 z);
BOOL ov49_02268230(Scene49 *scene);
// @fn ov49_02268230
BOOL ov49_02268230(Scene49 *scene) {
    int i;
    Rot49 *rot;
    Pos49 *pos;
    fx32 z;
    fx32 y;
    fx32 x;
    fx32 dy;
    i = 0;
    if (i < scene->count) {
        rot = scene->rots;
        pos = scene->pos;
        do {
            if (scene->state == 14) {
                ov49_022655F4(rot, 0, 0xE38, 0xC000);
            } else {
                ov49_02265628(rot);
            }
            ov49_02265434(pos, scene->state);
            ov49_02265660(&scene->rots[i], &dy);
            ov49_022655E0(&scene->pos[i], &x, &y, &z);
            y += dy;
            sub_020182A8(scene->renders[i], x, y, z);
            rot++;
            pos++;
        } while (++i < scene->count);
    }
    return scene->state >= 22;
}
