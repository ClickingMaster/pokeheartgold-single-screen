#pragma force_active on
#include "global.h"

typedef struct Packet {
    u16 x[4];
    u8 y[4];
    u8 packed[4];
    u8 flags[4];
    s32 status;
    u16 unk18;
    u16 cursorX;
    u8 cursorY;
    u8 unk1D[0x28 - 0x1D];
} Packet;

typedef struct Shared {
    Packet local;
    Packet previous;
    Packet participants[4];
    Packet global;
} Shared;

typedef struct Actor {
    void *sprites[3];
    u8 unk0C[0x6C - 0x0C];
    void *sprite6C;
    u8 unk70[0x78 - 0x70];
    void *sprite78;
    u8 unk7C[0xA6 - 0x7C];
    u8 status;
    u8 anim;
    u8 unkA8[0xC8 - 0xA8];
    u32 inactive;
    u8 unkCC[0xD2 - 0xCC];
    u8 pace;
    u8 latch;
} Actor;

typedef struct State {
    u8 unk000[0x3C3];
    u8 frame;
    u8 unk3C4[0x3CC - 0x3C4];
    void *cursors[4];
    void *unk3DC;
    void *screen;
} State;

u8 *ov96_021E8A20(void *ptr);
void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
u8 PokeathlonCourse_GetParticipantCount(void *course);
int ov96_021E5F24(void *course);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *sprite, VecFx32 *pos);
void PokeathlonCourseSpritePoolSprite_Show(void *sprite, int a, int b);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *sprite, int value);
BOOL PokeathlonCourseSpritePoolSprite_TryChangeAnim(void *sprite, int value);
void PlaySEOnPlayer(u32 seqNo, u32 playerNo);
u32 sub_02006190(u32 a);
void ov96_021FEECC(void *screen, Actor *actor, VecFx32 *pos, Packet *packet, u8 index, u8 target, Packet *info, int a7, int a8, int a9);
void ov96_02200950(void *screen, u8 index, u8 target, u8 hide, int status);
void PokeathlonCourseMonObject_SetDrawFlag(void *sprite, int value);
void PokeathlonCourseObject_SetGroundPosition(void *sprite, int x, int y);
void ov96_021FFB7C(Actor *actor, int y, int flag);
void *PokeathlonCourse_GetPerformance(void *course, int participant, int member);
void ov96_021FFAEC(void *mon, VecFx32 *pos, void *sprite);
void PokeathlonCourseMonObject_SetAnim(void *sprite, int value);
void ov96_021FFE38(int x, int seqNo, int flag);
u8 ov96_02200BC8(void *screen, int index);
void ov96_02200A18(void *screen, u8 a, u8 b);
void ov96_021FE550(void *course, int selected, Actor *actor, Shared *shared);

// @fn ov96_021FE550
void ov96_021FE550(void *course, int selected, Actor *actor, Shared *shared) {
    int x;
    int dx;
    int sx;
    State *s;
    Packet *packet;
    void *sprite;
    u8 hide;
    u8 anim;
    int status;
    u8 packed;
    u8 target;
    int i;
    int isFirst;
    int own;
    int d;
    u32 px;
    int j;
    Packet *info;
    int dummy;
    BOOL animated;
    BOOL hurt;
    u8 first;
    u8 second;
    s32 bits;

    packet = (Packet *)ov96_021E8A20(&shared->global);
    s = PokeathlonCourse_GetHeapAllocPtr4(course);
    x = packet->x[selected];
    {
    VecFx32 cursor = { 0, 0, 0 };
    if ((u8)((packet->status >> 25) & 1)) {
        d = x - packet->cursorX;
        if (d >= 768.0) {
            d = 80.0 + (1024.0 - d);
        } else {
            d = 80 - d;
        }
        cursor.x = d << 12;
        cursor.y = packet->cursorY << 12;
        PokeathlonCourseSpritePoolSprite_SetPosition(s->cursors[s->frame], &cursor);
        PokeathlonCourseSpritePoolSprite_Show(s->cursors[s->frame], 1, 1);
        PokeathlonCourseSpritePoolSprite_SetAnim(s->cursors[s->frame], 9);
        s->frame = (s->frame + 1) % 4;
        if (d >= 0 && d < 256) {
            PlaySEOnPlayer(0x8A3, 5);
        }
    }
    }
    isFirst = selected == 0;
    for (i = 0; i < 4; i++) {
        VecFx32 world;
        VecFx32 displaced;

        own = selected == i;
        packed = packet->packed[i];
        target = packed & 3;
        px = packet->x[i];
        dx = (int)px - x;
        if (x > 724.0) {
            if (px < 300) {
                dx = (1024.0 - x) + px;
            }
        } else if (x < 100 && px > 924.0) {
            dx = 0.0 - (x + (1024.0 - px));
        }
        sx = dx + 80;
        hide = (packet->flags[i] >> 7) & 1;
        status = (packet->status >> ((u8)((u32)target + i * 3) * 2)) & 3;
        anim = (packed >> 2) & 3;
        sprite = actor->sprites[target];
        world.x = sx << 12;
        world.y = packet->y[i] << 12;
        world.z = 0;
        if (PokeathlonCourse_GetParticipantCount(course) <= i) {
            dummy = 1;
            info = (Packet *)ov96_021E8A20(&shared->participants[i]);
        } else {
            dummy = 0;
            info = (Packet *)ov96_021E8A20(shared);
        }
        if (selected == i) {
            ov96_021FEECC(s->screen, actor, &world, packet, i, (u8)target, info, isFirst, 1, 0);
        } else {
            ov96_021FEECC(s->screen, actor, &world, packet, i, (u8)target, info, isFirst, 0, dummy);
        }
        ov96_02200950(s->screen, i, (u8)target, hide, status);
        if (actor->inactive == 0 && sx >= -32 && sx <= 288) {
            PokeathlonCourseMonObject_SetDrawFlag(sprite, 1);
        } else {
            PokeathlonCourseMonObject_SetDrawFlag(sprite, 0);
        }
        for (j = 0; j < 3; j++) {
            PokeathlonCourseObject_SetGroundPosition(actor->sprites[j], sx, packet->y[i]);
        }
        ov96_021FFB7C(actor, packet->y[i], i == ov96_021E5F24(course));
        ov96_021FFAEC(PokeathlonCourse_GetPerformance(course, i, target), &world, actor->sprite6C);
        displaced = world;
        displaced.y -= FX32_CONST(24);
        PokeathlonCourseSpritePoolSprite_SetPosition(actor->sprite78, &displaced);
        if (!hide) {
            animated = FALSE;
            hurt = FALSE;
            switch (anim) {
            case 2:
                if (anim != actor->anim) {
                    PokeathlonCourseSpritePoolSprite_Show(actor->sprite78, 1, 1);
                    PokeathlonCourseSpritePoolSprite_SetAnim(actor->sprite78, 4);
                }
                PokeathlonCourseMonObject_SetAnim(sprite, 8);
                animated = TRUE;
                if (actor->pace <= 1 && actor->latch == 0) {
                    ov96_021FFE38(sx, 0x8A7, own);
                    actor->pace = 2;
                    actor->latch = TRUE;
                }
                break;
            case 3:
                if (anim != actor->anim) {
                    PokeathlonCourseSpritePoolSprite_Show(actor->sprite78, 1, 1);
                    PokeathlonCourseSpritePoolSprite_SetAnim(actor->sprite78, 5);
                }
                PokeathlonCourseMonObject_SetAnim(sprite, 8);
                animated = TRUE;
                if (actor->pace <= 1 && actor->latch == 0) {
                    ov96_021FFE38(sx, 0x8A7, own);
                    actor->pace = 3;
                    actor->latch = TRUE;
                }
                break;
            default:
                PokeathlonCourseSpritePoolSprite_Show(actor->sprite78, 1, 0);
                actor->pace = anim;
                actor->latch = FALSE;
                break;
            }
            switch (status) {
            case 1:
                PokeathlonCourseSpritePoolSprite_Show(actor->sprite6C, 1, 1);
                PokeathlonCourseSpritePoolSprite_TryChangeAnim(actor->sprite6C, 6);
                break;
            case 2:
                if (status != actor->status) {
                    PokeathlonCourseSpritePoolSprite_Show(actor->sprite6C, 1, 1);
                    PokeathlonCourseSpritePoolSprite_SetAnim(actor->sprite6C, 7);
                }
                PokeathlonCourseMonObject_SetAnim(sprite, 21);
                hurt = TRUE;
                if (own) {
                    if (!sub_02006190(3)) {
                        ov96_021FFE38(sx, 0x890, own);
                    }
                } else {
                    if (!sub_02006190(4)) {
                        ov96_021FFE38(sx, 0x890, own);
                    }
                }
                break;
            default:
                PokeathlonCourseSpritePoolSprite_Show(actor->sprite6C, 1, 0);
                break;
            }
            if (!animated && !hurt) {
                PokeathlonCourseMonObject_SetAnim(sprite, 0);
            }
        } else {
            PokeathlonCourseSpritePoolSprite_Show(actor->sprite78, 1, 0);
            PokeathlonCourseSpritePoolSprite_Show(actor->sprite6C, 1, 0);
        }
        actor->anim = anim;
        actor->status = status;
        actor++;
    }
    first = ov96_02200BC8(s->screen, 0);
    second = ov96_02200BC8(s->screen, 1);
    bits = packet->status;
    ov96_02200A18(s->screen, (bits >> ((u8)(first + selected * 3) * 2)) & 3, (bits >> ((u8)(second + selected * 3) * 2)) & 3);
}
