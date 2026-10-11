#pragma force_active on
#include "global.h"
#include "sound.h"
#include <nitro/fx/fx_trig.h>

typedef struct Particle {
    void *shadow;
    void *body;
    void *effect;
    u8 unk0C[0x42 - 0x0C];
    u8 dead;
    u8 unk43[0x48 - 0x43];
} Particle;

typedef struct Slot {
    void *sprite;
    u8 unk04[0x1C - 0x04];
    u16 timer;
    u16 latched;
} Slot;

typedef struct Scene {
    u8 unk000[0x64];
    void *local[3];
    void *remote[9];
    void *actors[9];
    Particle particles[12];
    Slot slots[12];
    u8 unk598[0x5E0 - 0x598];
    void *screen;
    u8 unk5E4[0x5EC - 0x5E4];
    u8 stage[3];
    u8 flash[3];
} Scene;

typedef struct InputState {
    u16 header[4];
    u16 entries[12];
    s32 flags;
} InputState;

extern const VecFx32 ov96_0221C98C[];
extern const VecFx32 ov96_0221C7C4;

void PlaySE(u32 seq);
BOOL IsSEPlaying(u32 seq);
void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *mtx, VecFx32 *dst);
void MTX_RotY43_(MtxFx43 *mtx, fx32 sinVal, fx32 cosVal);
void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
u8 *PokeathlonCourse_GetDataCopyArea(void *course);
u8 *ov96_021E8A20(void *ptr);
int ov96_021E5F24(void *course);
void ov96_022038A0(Scene *scene, int time);
void ov96_02203BD0(void *screen, u16 value);
void ov96_021EB52C(void *sprite, int a, int b);
void ov96_021EB564(void *sprite, int value);
void ov96_021EB588(void *sprite, VecFx32 *pos);
VecFx32 *ov96_021EB594(void *sprite);
void ov96_021EB5A0(void *sprite, VecFx32 *scale, int type);
void ov96_02204134(f32 x, f32 z, int *sx, int *sy);
void ov96_02203BC0(void *screen, u8 index, int value);
void ov96_02203CA4(void *screen, u8 index, int value);
BOOL ov96_02203CC4(void *screen, u8 index);
void ov96_02203CD4(void *screen);
void PokeathlonCourseMonObject_SetAnim(void *sprite, int value);
BOOL PokeathlonCourseMonObject_IsAnimated(void *sprite);
void ov96_02203970(Scene *scene, u8 current);
void PokeathlonCourse_UpdateTimeUpCountdown(void *course, int value);
void ov96_02202958(void *course);

// @fn ov96_02202958
void ov96_02202958(void *course) {
    VecFx32 pos1;
    MtxFx43 mtx1;
    VecFx32 in1;
    VecFx32 out1;
    VecFx32 pos2;
    MtxFx43 mtx2;
    VecFx32 in2;
    VecFx32 out2;
    VecFx32 scale;
    int sx1;
    int sy1;
    int sx2;
    int sy2;
    int cellX;
    int cellZ;
    Scene *scene;
    InputState *input;
    u8 time;
    u8 state;
    u8 flash;
    u8 phase;
    int partX;
    int partZ;
    u8 current;
    int owner;
    int rot;
    const fx16 *sc;
    int y;
    int member;
    int k;
    u8 slot;
    u8 mode;
    u8 stage;
    int x;
    int z;
    int i;
    int ready;
    f32 factor;

    scene = PokeathlonCourse_GetHeapAllocPtr4(course);
    input = (InputState *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    current = ov96_021E5F24(course);
    time = (input->flags >> 25) & 0x7F;
    ov96_022038A0(scene, time == 0 ? 0 : (u8)(time - 1));
    ov96_02203BD0(scene->screen, input->header[current] & 0x3FF);
    for (i = 0; i < 12; i++) {
        if ((u8)((input->entries[i] >> 13) & 1) && current == i / 3) {
            ov96_021EB52C(scene->local[(u8)(i % 3)], 1, 1);
            ov96_021EB564(scene->local[(u8)(i % 3)], 6);
            x = input->entries[i] & 0x3F;
            z = (input->entries[i] >> 6) & 0x3F;
            x++;
            z++;
            if (current != 0) {
                rot = 4 - current;
                in1.x = x << FX32_SHIFT;
                in1.z = z << FX32_SHIFT;
                in1.y = 0;
                in1.x -= FX32_CONST(33);
                in1.z -= FX32_CONST(33);
                sc = &FX_SinCosTable_[(((u8)rot * 0x4000) >> 4) << 1];
                MTX_RotY43_(&mtx1, sc[0], sc[1]);
                MTX_MultVec43(&in1, &mtx1, &out1);
                out1.x += FX32_CONST(33);
                out1.z += FX32_CONST(33);
                x = out1.x / FX32_ONE;
                z = out1.z / FX32_ONE;
            }
            ov96_02204134(x, z, &sx1, &sy1);
            pos1.x = sx1 << FX32_SHIFT;
            pos1.y = sy1 << FX32_SHIFT;
            pos1.z = 0;
            pos1.y = (sy1 - 8 - sy1 / 20) << FX32_SHIFT;
            ov96_021EB588(scene->local[(u8)(i % 3)], &pos1);
            PlaySE(0x88F);
        }
    }
    for (k = 0; k < 3; k++) {
        slot = current * 3 + k;
        state = (input->entries[slot] >> 14) & 3;
        if (state == 1) {
            ov96_021EB52C(scene->slots[slot].sprite, 1, 1);
            ov96_02203BC0(scene->screen, k, 0);
        } else if (state == 2) {
            ov96_021EB52C(scene->slots[slot].sprite, 1, 0);
            ov96_02203BC0(scene->screen, k, 1);
        } else {
            ov96_021EB52C(scene->slots[slot].sprite, 1, 0);
            ov96_02203BC0(scene->screen, k, 0);
        }
        flash = (input->entries[slot] >> 12) & 1;
        phase = (input->flags >> (slot * 2)) & 3;
        if (state == 1) {
            ov96_02203CA4(scene->screen, k, 10);
            scene->slots[slot].timer = 10;
            if (!IsSEPlaying(0x890)) {
                PlaySE(0x890);
            }
            if (flash) {
                PlaySE(0x8B0);
            }
        } else if (flash) {
            ov96_02203CA4(scene->screen, k, 7);
            scene->slots[slot].timer = 7;
            PlaySE(0x8B0);
            ov96_02203CD4(scene->screen);
        } else if (phase == 1 && scene->slots[slot].latched == 0) {
            if (state == 1) {
                GF_AssertFail();
            }
            ov96_02203CA4(scene->screen, k, 4);
            scene->slots[slot].timer = 4;
            scene->slots[slot].latched = 1;
            if (scene->flash[k] == 0) {
                PlaySE(0x8AC);
            }
        } else {
            ready = FALSE;
            if (scene->slots[slot].timer == 10) {
                ready = TRUE;
            } else if (!ov96_02203CC4(scene->screen, k)) {
                ready = TRUE;
            }
            if (ready) {
                ov96_02203CA4(scene->screen, k, 1);
                scene->slots[slot].timer = 1;
            }
        }
        if (phase != 1) {
            scene->slots[slot].latched = 0;
        }
    }
    for (k = 0; k < 9; k++) {
        slot = ((u8)(((current + 1) * 3) % 12) + k) % 12;
        state = (input->entries[slot] >> 14) & 3;
        if (state == 1) {
            ov96_021EB52C(scene->slots[slot].sprite, 1, 1);
            ov96_021EB52C(scene->remote[k], 1, 0);
        } else if (state == 2) {
            ov96_021EB52C(scene->slots[slot].sprite, 1, 0);
            ov96_021EB52C(scene->remote[k], 1, 1);
        } else {
            ov96_021EB52C(scene->slots[slot].sprite, 1, 0);
            ov96_021EB52C(scene->remote[k], 1, 0);
        }
        flash = (input->entries[slot] >> 12) & 1;
        phase = (input->flags >> (slot * 2)) & 3;
        cellX = input->entries[slot] & 0x3F;
        cellZ = (input->entries[slot] >> 6) & 0x3F;
        if (state == 1) {
            PokeathlonCourseMonObject_SetAnim(scene->actors[k], 20);
            scene->slots[slot].timer = 20;
            if (!IsSEPlaying(0x890)) {
                PlaySE(0x890);
            }
        } else if (flash) {
            PokeathlonCourseMonObject_SetAnim(scene->actors[k], 22);
            scene->slots[slot].timer = 22;
        } else if (phase == 1 && cellX + 1 == ov96_0221C98C[slot].x / FX32_ONE && cellZ + 1 == ov96_0221C98C[slot].z / FX32_ONE) {
            if (state == 1) {
                GF_AssertFail();
            }
            PokeathlonCourseMonObject_SetAnim(scene->actors[k], 16);
            scene->slots[slot].timer = 16;
        } else {
            ready = FALSE;
            if (scene->slots[slot].timer == 20) {
                ready = TRUE;
            } else if (!PokeathlonCourseMonObject_IsAnimated(scene->actors[k])) {
                ready = TRUE;
            }
            if (ready) {
                PokeathlonCourseMonObject_SetAnim(scene->actors[k], 0);
                scene->slots[slot].timer = 0;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        mode = (input->flags >> (i * 2)) & 3;
        if (mode == 1 || mode == 2) {
            scene->particles[i].dead = FALSE;
            if (mode != 2) {
                ov96_021EB52C(scene->particles[i].body, 1, 1);
            } else {
                ov96_021EB52C(scene->particles[i].body, 1, 0);
            }
            ov96_021EB52C(scene->particles[i].shadow, 1, 1);
            owner = i / 3;
            member = i % 3;
            stage = ((u8)((input->header[i / 3] >> 10) & 0x3F) >> (member * 2)) & 3;
            partX = (input->entries[i] & 0x3F) + 1;
            partZ = ((input->entries[i] >> 6) & 0x3F) + 1;
            if (current != 0) {
                rot = 4 - current;
                in2.x = partX << FX32_SHIFT;
                in2.z = partZ << FX32_SHIFT;
                in2.y = 0;
                in2.x -= FX32_CONST(33);
                in2.z -= FX32_CONST(33);
                sc = &FX_SinCosTable_[(((u8)rot * 0x4000) >> 4) << 1];
                MTX_RotY43_(&mtx2, sc[0], sc[1]);
                MTX_MultVec43(&in2, &mtx2, &out2);
                out2.x += FX32_CONST(33);
                out2.z += FX32_CONST(33);
                partX = out2.x / FX32_ONE;
                partZ = out2.z / FX32_ONE;
            }
            ov96_02204134(partX, partZ, &sx2, &sy2);
            pos2.x = sx2 << FX32_SHIFT;
            pos2.y = sy2 << FX32_SHIFT;
            pos2.z = 0;
            ov96_021EB588(scene->particles[i].body, &pos2);
            y = sy2;
            pos2.y = (y - 8 - y / 20) << FX32_SHIFT;
            ov96_021EB588(scene->particles[i].shadow, &pos2);
            scale = ov96_0221C7C4;
            factor = ((f32)sy2 + 312.0) / 752.0 * (1.0 + (f32)stage / 3.0);
            scale.x = 4096.0f * factor;
            scale.y = 4096.0f * factor;
            ov96_021EB5A0(scene->particles[i].body, &scale, 2);
            ov96_021EB5A0(scene->particles[i].shadow, &scale, 2);
            ov96_021EB5A0(scene->particles[i].effect, &scale, 2);
            if (owner == current && stage != scene->stage[member]) {
                PlaySE(0x8B1);
                scene->stage[member] = stage;
            }
        } else {
            ov96_021EB52C(scene->particles[i].body, 1, 0);
            ov96_021EB52C(scene->particles[i].shadow, 1, 0);
            if (mode == 3 && !scene->particles[i].dead) {
                scene->particles[i].dead = TRUE;
                ov96_021EB588(scene->particles[i].effect, ov96_021EB594(scene->particles[i].shadow));
                ov96_021EB564(scene->particles[i].effect, 3);
                ov96_021EB52C(scene->particles[i].effect, 1, 1);
                if (i / 3 == current) {
                    PlaySE(0x8B3);
                    scene->flash[i % 3] = 0;
                    scene->stage[i % 3] = 0;
                }
            }
        }
    }
    ov96_02203970(scene, current);
    PokeathlonCourse_UpdateTimeUpCountdown(course, time * 30);
}
