#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct {
    void *obj;
    void *subs[3];
    void *text;
    u8 pad14[0x10];
    int y;
} Ent;
typedef struct {
    int heapId;
    void *mgr;
    u8 pad8[4];
    u8 xc[0x18];
    Ent ents[4];
} Work;
Sprite *ov96_021EB408(void *mgr, int priority, int screen, u16 resId, u8 x8);
void *ov96_021EB4F4(void *mgr, u8 a, u8 b);
void ov96_021EB52C(void *obj, int a, int b);
Sprite *ov96_021EB5B8(void *obj);
void *PokeathlonCourse_GetPlayerProfileFromData(void *course, int index);
void *PlayerProfile_GetPlayerName_NewString(void *profile, int heapId);
void ov96_021F9134(Work *work, void *a, void *str, int b, int c, int d, Sprite *sprite, void **text);
void String_Delete(void *str);
void TextOBJ_SetSpritesDrawFlag(void *obj, BOOL draw);
void ov96_021F8F94(Work *work, void *course, int y);
// @fn ov96_021F8F94
void ov96_021F8F94(Work *work, void *course, int y) {
    int i;
    int j;
    Sprite *sprite;
    VecFx32 pos;
    void *name;
    Ent *ent;
    for (i = 0; i < 4; i++) {
        sprite = ov96_021EB408(work->mgr, 0, 2, 0x68, 3);
        pos.z = 0;
        pos.x = (i * 0x40) << FX32_SHIFT;
        pos.y = (y + 0x10) << FX32_SHIFT;
        Sprite_SetMatrix(sprite, &pos);
        Sprite_SetAnimActiveFlag(sprite, TRUE);
        Sprite_SetAnimCtrlSeq(sprite, 0);
        Sprite_SetPalIndexRespectVramOffset(sprite, i);
        Sprite_SetDrawPriority(sprite, 2);
        for (j = 0; j < 3; j++) {
            sprite = ov96_021EB408(work->mgr, 0, 2, 0x68, 4);
            pos.z = 0;
            pos.x = (i * 0x40 + 8 + j * 0x10) << FX32_SHIFT;
            pos.y = (y + 0x18) << FX32_SHIFT;
            Sprite_SetMatrix(sprite, &pos);
            Sprite_SetAnimActiveFlag(sprite, TRUE);
            Sprite_SetAnimCtrlSeq(sprite, 1);
            Sprite_SetDrawPriority(sprite, 1);
        }
    }
    for (i = 0; i < 4; i++) {
        ent = &work->ents[i];
        ent->obj = ov96_021EB4F4(work->mgr, 0x68, 3);
        ov96_021EB52C(ent->obj, 1, 1);
        for (j = 0; j < 3; j++) {
            ent->subs[j] = ov96_021EB4F4(work->mgr, 0x68, 4);
            ov96_021EB52C(ent->subs[j], 1, 1);
        }
        name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, i), work->heapId);
        sprite = ov96_021EB5B8(work->ents[i].obj);
        ov96_021F9134(work, work->xc, name, 2, 0, -16, sprite, &work->ents[i].text);
        String_Delete(name);
        TextOBJ_SetSpritesDrawFlag(work->ents[i].text, TRUE);
        work->ents[i].y = y + 0x10;
    }
}
