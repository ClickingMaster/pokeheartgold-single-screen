#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "bg_window.h"

typedef struct {
    float v[6];
} ScaleTable;

typedef struct {
    int v[6];
} OffsetTable;

typedef struct {
    u16 v[8];
} TileRect;

typedef struct {
    u16 width;
    u16 height;
    u16 colorMode;
    u16 format;
    u32 size;
    u8 data[1];
} ScreenData;

typedef struct {
    u8 x[12];
    u8 y[12];
    s32 seqBits;
    s32 statusBits;
    s32 flagBits;
} Packet;

typedef struct {
    void *primary;
    void *shadow;
    void *marker;
    fx32 x;
    fx32 y;
    u8 unk14[0x34];
    u8 styleA[32];
    u8 styleB[32];
    u8 unk88[8];
} Actor;

typedef struct {
    u16 state;
    u16 tick;
} Counter;

typedef struct {
    u8 unk0[3];
    u8 active;
    u8 unk4[32];
    s16 pitch;
    u8 unk26[26];
} Batch;

typedef struct {
    BgConfig *bg;
    u8 unk04[0x1C];
    Actor actors[4][3];
    u8 unk6E0[0x50];
    u16 group;
    u8 unk732[6];
    ScreenData *screen;
    u8 flags[12];
    u8 unk748[0x24];
    void *manager;
    u32 unk770;
    void *ui;
    Counter counters[12];
    u32 alarm;
    Batch batch;
} CourseWork;

extern const TileRect ov96_0221BC8C;
extern const ScaleTable ov96_0221BC9C;
extern const OffsetTable ov96_0221BCB4;

void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void *PokeathlonCourse_GetDataCopyArea(void *course);
void *ov96_021E8A20(void *area);
int ov96_021E5F24(void *course);
void ov96_021F2B68(Batch *batch, u8 oldGroup, u8 group, void *course);
BOOL PlaySE(u16 seq);
BOOL IsSEPlaying(u16 seq);
void ov96_021F295C(int time, BgConfig *bg);
void ov96_021F4364(void *ui, int level);
void *ov96_021EAA20(void *obj);
Sprite *ov96_021E8BAC(void *obj);
void ov96_021EB06C(void *obj, int x, int y, int *outX, int *outY);
void ov96_021EABE0(void *obj, int flag);
void ov96_021EAB38(void *obj, int flag);
void ov96_021EB52C(void *obj, int a, int b);
void ov96_021EAB74(void *obj, int flag);
void ov96_021EB01C(void *obj, int x, int y, int flag);
void ov96_021EAF60(void *manager, u8 index, int flag, void *style);
Sprite *ov96_021EB5B8(void *obj);
u8 *ov96_021E60C0(void *course, int row, int col);
void *ov96_021EAA04(void *manager, u8 index);
void ov96_021EAC0C(void *obj, int anim);
u8 *ov96_021E60D8(void *course, int row, int col);
void ov96_021EAED4(void *obj, int a, int pitch, int anim, int b);
void ov96_021F2E80(void *course, Batch *batch, void *obj, u8 index, int anim);
void ov96_021F4390(void *ui, u8 row, int effect);
void ov96_021F2E2C(Batch *batch, u8 index);
BOOL ov96_021F3180(u8 group, u16 x, u16 y, u8 *result);
void ov96_021E658C(void *course, u8 col, int flag);
void ov96_021F2FEC(CourseWork *work, Packet *packet);
void ov96_021F2D98(Batch *batch);
void ov96_021F4688(void *ui, int arg, int mode);
void ov96_021F45F4(void *ui, int active, u8 *flags);


static inline void UpdateActorAnim(void *course, CourseWork *work, Actor *actor, int index, int row, u8 player, int anim, int x, int y, VecFx32 *pos, u8 active, Sprite *sprite) {
    void *obj;

    switch (ov96_021E60C0(course, index / 3, index % 3)[7]) {
    case 1:
        pos->y -= 0x8000;
        break;
    case 2:
        pos->y -= 0x8000;
        break;
    case 3:
        pos->y -= 0x10000;
        break;
    default:
        GF_AssertFail();
        break;
    }
    Sprite_SetMatrix(ov96_021EB5B8(actor->marker), pos);
    obj = ov96_021EAA04(work->manager, index);
    ov96_021EAC0C(obj, anim);
    ov96_021EB01C(obj, x, y, 1);
    if (row == player && ov96_021E60D8(course, player, index % 3)[2]) {
        ov96_021EAED4(obj, 0, work->batch.pitch, anim, 0);
    }
    if (active) {
        void *obj2 = ov96_021EAA04(work->manager, index);
        Sprite_SetAnimActiveFlag(sprite, FALSE);
        work->batch.active = TRUE;
        ov96_021F2E80(course, &work->batch, obj2, index, anim);
    } else {
        Sprite_SetAnimActiveFlag(sprite, TRUE);
    }
}

void ov96_021F1614(void *course);
// @fn ov96_021F1614
void ov96_021F1614(void *course) {
    s32 chosen;
    u8 sound;
    u8 active;
    u8 mode;
    u8 arg;
    u8 level;
    Packet *packet;
    u8 seq;
    u8 status;
    u8 bit;
    Sprite *sprite;
    int offset;
    u8 player;
    u8 effect;
    s32 y;
    s32 x;
    int col;
    s32 i;
    int group;
    u8 time;
    CourseWork *work;
    u8 flags[12];

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    packet = ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    player = ov96_021E5F24(course);
    chosen = (packet->statusBits >> 28) & 0xF;
    group = (packet->statusBits >> 24) & 0xF;
    time = packet->seqBits >> 24;
    sound = (packet->flagBits >> 13) & 1;
    active = (packet->flagBits >> 14) & 1;
    effect = (packet->flagBits >> 15) & 7;
    mode = (packet->flagBits >> 26) & 3;
    arg = packet->flagBits >> 18;
    level = (u8)((packet->flagBits >> 28) & 0xF) + 1;

    if (!active) {
        ((u8 *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course)))[8] = 0;
    }
    {
        u8 oldGroup = work->group;
        u8 newGroup = group;
        if (oldGroup != newGroup) {
            work->group = newGroup;
            for (i = 0; i < 12; i++) {
                work->flags[i] = 0;
            }
            ov96_021F2B68(&work->batch, oldGroup, newGroup, course);
        }
    }
    if (time == 0) {
        BG_LoadScreenTilemapData(work->bg, 1, work->screen->data, work->screen->size);
        ScheduleBgTilemapBufferTransfer(work->bg, 1);
        work->alarm = 0;
    } else if (time < 43) {
        if (work->alarm == 0) {
            if (level <= 1 && time >= 32) {
                PlaySE(0x6D7);
                work->alarm = 1;
            } else if (level > 1 && time >= 28) {
                PlaySE(0x6D7);
                work->alarm = 1;
            } else if (level > 3 && time >= 21) {
                PlaySE(0x6D7);
                work->alarm = 1;
            }
        }
        ov96_021F295C(time, work->bg);
    } else {
        TileRect rect = ov96_0221BC8C;
        LoadRectToBgTilemapRect(work->bg, 1, &rect, 14, 2, 4, 2);
        ov96_021F295C(42, work->bg);
    }
    if (sound) {
        PlaySE(0x8CF);
    }
    ov96_021F4364(work->ui, level);

    for (i = 0; i < 12; i++) {
        Actor *actor;
        col = i % 3;
        actor = &work->actors[(i / 3)][col];
        sprite = ov96_021E8BAC(ov96_021EAA20(actor->primary));
        status = (packet->statusBits >> (i * 2)) & 3;
        bit = (packet->flagBits >> i) & 1;
        if (status == 0) {
            offset = 0;
            if (work->counters[i].state == 0) {
                work->counters[i].state = 1;
            } else if (work->counters[i].state == 1) {
                VecFx32 scale;
                ScaleTable scales = ov96_0221BC9C;
                OffsetTable offsets = ov96_0221BCB4;
                int outX, outY;
                scale.x = 4096.0f * scales.v[work->counters[i].tick / 2];
                scale.y = 4096.0f * scales.v[work->counters[i].tick / 2];
                scale.z = 4096.0f * scales.v[work->counters[i].tick / 2];
                ov96_021EB06C(actor->primary, packet->x[i], packet->y[i], &outX, &outY);
                if (outY >= 176) {
                    offset = offsets.v[work->counters[i].tick / 2];
                }
                work->counters[i].tick++;
                ov96_021EABE0(actor->primary, 1);
                Sprite_SetAffineScale(sprite, &scale);
                if (work->counters[i].tick >= 12) {
                    work->counters[i].state = 2;
                }
            } else {
                ov96_021EAB38(actor->primary, 0);
            }
            ov96_021EB52C(actor->marker, 1, 0);
            ov96_021EAB74(actor->primary, 0);
            ov96_021EB01C(actor->primary, packet->x[i], packet->y[i] + offset, 1);
            flags[i] = 1;
        } else {
            VecFx32 scale;
            work->counters[i].state = 0;
            work->counters[i].tick = 0;
            scale.x = FX32_ONE;
            scale.y = FX32_ONE;
            scale.z = FX32_ONE;
            ov96_021EABE0(actor->primary, 0);
            if (bit) {
                ov96_021EB52C(actor->marker, 1, 1);
                if ((i / 3) == player && !IsSEPlaying(0x890)) {
                    PlaySE(0x890);
                }
            } else {
                ov96_021EB52C(actor->marker, 1, 0);
            }
            Sprite_SetAffineScale(sprite, &scale);
            ov96_021EAB38(actor->primary, 1);
            if (status == 2 && work->flags[i] == 0) {
                work->flags[i] = 1;
                ov96_021EAF60(work->manager, i, 1, actor->styleB);
            } else if (status == 1 && work->flags[i] == 1) {
                work->flags[i] = 0;
                ov96_021EAF60(work->manager, i, 1, actor->styleA);
            }
            flags[i] = 0;
            x = packet->x[i];
            y = packet->y[i];
            seq = (packet->seqBits >> (i * 2)) & 3;
            actor->x = x << FX32_SHIFT;
            actor->y = y << FX32_SHIFT;
            VecFx32 pos = {0, 0, 0};
            pos.x = actor->x;
            pos.y = actor->y;
            Sprite_SetMatrix(ov96_021EB5B8(actor->shadow), &pos);
            VecFx32 pos2 = {0, 0, 0};
            pos2.x = actor->x;
            pos2.y = actor->y;
            UpdateActorAnim(course, work, actor, i, (i / 3), player, (u8)(seq + 1), x, y, &pos2, active, sprite);
            if (chosen == i) {
                if (effect >= 1 && effect <= 4) {
                    Sprite *shadow = ov96_021EB5B8(actor->shadow);
                    ov96_021EB52C(actor->shadow, 1, 1);
                    Sprite_SetAnimCtrlSeq(shadow, effect + 2);
                    if (effect <= 2) {
                        PlaySE(0x88D);
                    } else if (effect == 3) {
                        PlaySE(0x88E);
                    } else {
                        PlaySE(0x88F);
                    }
                    ov96_021F4390(work->ui, (i / 3), effect);
                    ov96_021F2E2C(&work->batch, i);
                } else {
                    GF_AssertFail();
                }
            }
            if ((i / 3) == player) {
                int outX, outY;
                u8 result;
                ov96_021EB06C(actor->primary, x, y, &outX, &outY);
                if (status == 1 && ov96_021F3180(work->group, outX, outY, &result)) {
                    ov96_021E658C(course, col, 1);
                } else {
                    ov96_021E658C(course, col, 0);
                }
            }
        }
    }
    ov96_021F2FEC(work, packet);
    if (work->batch.active && !active) {
        ov96_021F2D98(&work->batch);
    }
    ov96_021F4688(work->ui, arg, mode);
    ov96_021F45F4(work->ui, active, flags);
}
