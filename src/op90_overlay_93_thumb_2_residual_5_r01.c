#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "math_util.h"

typedef struct {
    u8 pad00[0x2C];
    u8 ids[4];
    u8 count;
} Info;
typedef struct {
    Info *info;
    u8 pad04[0x20];
    SpriteSystem *ss;
    SpriteManager *sm;
} Sys;
typedef struct {
    ManagedSprite *sprite;
    fx32 x;
    fx32 y;
    fx32 angle;
    fx32 speed;
    u16 life;
    u16 pad16;
} Particle;
typedef struct {
    u32 active;
    Particle a[36];
    Particle b[8];
    Particle c[3];
} Out;
typedef struct {
    s32 pad00;
    s32 type;
    s32 n1;
    s32 pad0c;
    s32 n2;
    s32 pad14;
    s32 weights[4];
} Data;
extern const ManagedSpriteTemplate ov93_02262E00;
extern const u16 ov93_02262D7C[][4];
extern const u16 _02262C6C[];
s32 WobbuffetPop_GetPlayerSeat(Sys *, s32);
void ov93_02260314(Sys *w, Out *out, Data *data);

#pragma optimization_level 2
#pragma opt_common_subs off
// @fn ov93_02260314
void ov93_02260314(Sys *w, Out *out, Data *data) {
    ManagedSpriteTemplate tmpl;
    s32 totals[4];
    s32 n2;
    s32 n1;
    int anim;
    int i;
    int j;
    int count;
    int id;
    Particle *p;
    ManagedSprite *s;
    Particle *q;
    ManagedSprite *t;

    count = 0;
    tmpl = ov93_02262E00;
    n1 = data->n1;
    n2 = data->n2;
    for (i = 0; i < w->info->count; i++) {
        id = WobbuffetPop_GetPlayerSeat(w, w->info->ids[i]);
        totals[i] = 36 * data->weights[id] / (n1 + n2);
        anim = ov93_02262D7C[w->info->count][id];
        for (j = 0; j < totals[i]; j++) {
            p = &out->a[count];
            s = SpriteSystem_NewSprite(w->ss, w->sm, &tmpl);
            if (s == NULL) {
                break;
            }
            ManagedSprite_SetPositionXYWithSubscreenOffset(s, 128, 96, 22 << 16);
            ManagedSprite_SetAnim(s, anim);
            Sprite_TickFrame(s->sprite);
            p->x = LCRandom() % 0x2000 + 0x2000;
            p->y = LCRandom() % 0x4000 + 0x2000;
            p->angle = (LCRandom() % 360) << 12;
            p->speed = LCRandom() % (5 << 14) + (10 << 12);
            p->life = LCRandom() % 15 + 20;
            out->a[count].sprite = s;
            count++;
        }
    }
    tmpl.drawPriority = 14;
    tmpl.pal = 0;
    for (i = 0; i < 8; i++) {
        q = &out->b[i];
        t = SpriteSystem_NewSprite(w->ss, w->sm, &tmpl);
        if (t == NULL) {
            break;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(t, 128, 96, 22 << 16);
        ManagedSprite_SetAnim(t, LCRandom() % 3 + 28);
        Sprite_TickFrame(t->sprite);
        q->x = LCRandom() % 0x2000 + 0x2000;
        q->y = LCRandom() % 0x4000 + 0x2000;
        q->angle = (LCRandom() % 360) << 12;
        q->speed = LCRandom() % (5 << 14) + (1 << 16);
        q->life = LCRandom() % 15 + 20;
        out->b[i].sprite = t;
    }
    tmpl.drawPriority = 13;
    tmpl.pal = _02262C6C[data->type];
    for (i = 0; i < 3; i++) {
        q = &out->c[i];
        t = SpriteSystem_NewSprite(w->ss, w->sm, &tmpl);
        if (t == NULL) {
            break;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(t, 128, 96, 22 << 16);
        ManagedSprite_SetAnim(t, 11);
        Sprite_TickFrame(t->sprite);
        q->x = LCRandom() % 0x2000 + 0x2000;
        q->y = LCRandom() % 0x4000 + 0x2000;
        q->angle = (LCRandom() % 360) << 12;
        q->speed = LCRandom() % (5 << 14) + (1 << 16);
        q->life = LCRandom() % 15 + 20;
        out->c[i].sprite = t;
    }
}
#pragma opt_common_subs reset
#pragma optimization_level reset
