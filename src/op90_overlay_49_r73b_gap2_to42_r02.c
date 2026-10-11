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
void ov49_02265628(Rot49 *rot);
int ov49_02265434(Pos49 *pos, int state);
void ov49_02265660(Rot49 *rot, fx32 *y);
void ov49_022655E0(Pos49 *pos, fx32 *x, fx32 *y, fx32 *z);
void ModelInstance_SetPosition(void *render, fx32 x, fx32 y, fx32 z);
int ov49_022680B4(Scene49 *scene);
// @fn ov49_022680B4
int ov49_022680B4(Scene49 *scene) {
    int i;
    int result;
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
            ov49_02265628(rot);
            result = ov49_02265434(pos, scene->state);
            ov49_02265660(&scene->rots[i], &dy);
            ov49_022655E0(&scene->pos[i], &x, &y, &z);
            x += dy;
            ModelInstance_SetPosition(scene->renders[i], x, y, z);
            rot++;
            pos++;
        } while (++i < scene->count);
    }
    return result;
}
