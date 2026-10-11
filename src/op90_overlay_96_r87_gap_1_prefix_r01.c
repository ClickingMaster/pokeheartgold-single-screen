#pragma force_active on
#include "global.h"

typedef struct CoursePlayer {
    void *sprites[4];
    u8 unk10[0x38];
    void *secondary;
    void *effects[2];
    void *flash;
    u8 unk58[0x18];
    s16 offsetY;
    u8 flashTimer;
    u8 effectCount;
    u8 unk74[4];
    u16 nextEffect;
    u8 unk7A[0x29];
    u8 active;
    u8 unkA4[0xA];
    u16 count;
    u8 unkB0[8];
} CoursePlayer;

typedef struct CoursePacket {
    u32 words[6];
    u16 attributes[4];
} CoursePacket;

typedef struct CourseNetData {
    u8 local[0x50];
    u8 remote[4][0x28];
    u8 packet[0x28];
} CourseNetData;

typedef struct CourseView {
    u8 unk0[0x34C];
    void *marks1[4];
    void *marks2[4];
    u8 unk36C[4];
    void *ui;
    u8 unk374[0x1A4];
    u8 markIdx1;
    u8 unk519;
    u8 markIdx2;
    u8 unk51B[9];
    u32 markX1[4];
    u32 markY1[4];
    u32 markX2[4];
    u32 markY2[4];
} CourseView;

extern u32 ov96_021E5F24(void *course);
extern u8 *ov96_021E8A20(void *ptr);
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
extern u8 PokeathlonCourse_GetParticipantCount(void *course);
extern u8 *ov96_021E60C0(void *course, int index, u16 mode);
extern void PlaySEOnPlayer(u32 seqNo, u32 playerNo);
extern u32 sub_02006190(u32 playerNo);
extern void ov96_021EB52C(void *obj, int a, int b);
extern void ov96_021EB564(void *obj, int anim);
extern void ov96_021EB588(void *obj, VecFx32 *pos);
extern void ov96_021EAB38(void *sprite, int visible);
extern void ov96_021EAF94(void *sprite, int x, int y);
extern void ov96_021EAC5C(void *sprite, int anim);
extern void ov96_021EAC08(void *sprite, u8 width);
extern void ov96_021EAC0C(void *sprite, u8 width);
extern void ov96_02207C38(int x, int y, int seq, int a3);
extern void ov96_022076E4(CourseView *view);
extern void ov96_02206DEC(void *ui, CoursePlayer *player, CoursePacket *packet, u8 index, u16 mode, u8 *info, int newflag, int self, int remote);
extern void ov96_02208658(void *ui, u8 index, u16 mode, u8 hidden, u8 active);
extern void ov96_02207718(CoursePlayer *player, int y);
extern void ov96_02208840(void *ui, int index, u8 effect);
extern void ov96_02208374(void *ui, u16 count);
extern void ov96_02208864(void *ui, int index);
void ov96_02206380(void *course, int viewer, CoursePlayer *players, CourseNetData *data);

// @fn ov96_02206380
void ov96_02206380(void *course, int viewer, CoursePlayer *players, CourseNetData *data) {
    u32 baseX;
    u32 baseY;
    CourseView *view;
    CoursePacket *packet;
    void *sprite;
    u8 width;
    u8 hidden;
    u8 active;
    int curY;
    int x;
    long px;
    u8 local;
    u8 mode;
    long py;
    int newflag;
    int y;
    int curX;
    int i;
    int j;
    u8 *info;
    int remote;
    u8 effect;
    u16 count;


    local = ov96_021E5F24(course);
    packet = (CoursePacket *)ov96_021E8A20(data->packet);
    view = PokeathlonCourse_GetHeapAllocPtr4(course);
    baseX = packet->words[viewer] & 0x1FF;
    baseY = (packet->words[viewer] >> 9) & 0x1FF;

    if ((u8)((packet->words[5] >> 18) & 1)) {
        x = packet->words[5] & 0x1FF;
        y = (packet->words[5] >> 9) & 0x1FF;
        view->markX1[view->markIdx1] = x;
        view->markY1[view->markIdx1] = y;
        ov96_021EB52C(view->marks1[view->markIdx1], 1, 1);
        ov96_021EB564(view->marks1[view->markIdx1], 7);
        view->markIdx1 = (view->markIdx1 + 1) % 4;
        ov96_02207C38(x + 128 - baseX, y + 96 - baseY, 0x8A3, 1);
    }
    if ((u8)(((u8)((packet->words[5] >> 19) & 0xF) >> local) & 1)) {
        PlaySEOnPlayer(0x8CE, 3);
    }
    if ((u8)((packet->words[4] >> 18) & 1)) {
        x = packet->words[4] & 0x1FF;
        y = (packet->words[4] >> 9) & 0x1FF;
        view->markX2[view->markIdx2] = x;
        view->markY2[view->markIdx2] = y;
        ov96_021EB52C(view->marks2[view->markIdx2], 1, 1);
        ov96_021EB564(view->marks2[view->markIdx2], 4);
        view->markIdx2 = (view->markIdx2 + 1) % 4;
        ov96_02207C38(x + 128 - baseX, y + 96 - baseY, 0x8CD, 1);
    }

    for (i = 0; i < 4; i++) {
        VecFx32 pos1 = {0, 0, 0};
        y = (view->markY1[i] + 96 - baseY) << 12;
        x = (view->markX1[i] + 128 - baseX) << 12;
        pos1.x = x;
        pos1.y = y;
        ov96_021EB588(view->marks1[i], &pos1);
    }
    for (i = 0; i < 4; i++) {
        VecFx32 pos2 = {0, 0, 0};
        y = (view->markY2[i] + 96 - baseY) << 12;
        x = (view->markX2[i] + 128 - baseX) << 12;
        pos2.x = x;
        pos2.y = y;
        ov96_021EB588(view->marks2[i], &pos2);
    }
    ov96_022076E4(view);

    newflag = viewer == 0 ? TRUE : FALSE;

    for (i = 0; i < 4; i++, players++) {
        CoursePlayer *p = players;
        VecFx32 position;
        curX = packet->words[i] & 0x1FF;
        curY = (packet->words[i] >> 9) & 0x1FF;
        mode = (packet->attributes[i] >> 5) & 3;
        width = (u8)((packet->attributes[i] >> 7) & 3) + 1;
        active = (packet->attributes[i] >> 9) & 1;
        hidden = ((u8)((packet->words[4] >> 19) & 0xF) >> i) & 1;
        sprite = p->sprites[mode];
        if (PokeathlonCourse_GetParticipantCount(course) <= i) {
            remote = TRUE;
            info = ov96_021E8A20(data->remote[i]);
        } else {
            remote = FALSE;
            info = ov96_021E8A20(data->local);
        }
        if (viewer == i) {
            ov96_02206DEC(view->ui, p, packet, i, mode, info, newflag, TRUE, FALSE);
        } else {
            ov96_02206DEC(view->ui, p, packet, i, mode, info, newflag, FALSE, remote);
        }
        px = curX - baseX + 128;
        py = curY - baseY + 96;
        py += p->offsetY;
        position.x = px << 12;
        position.y = py << 12;
        position.z = 0;
        ov96_02208658(view->ui, i, mode, hidden, active);
        if (!hidden) {
            if ((px >= -32 && px <= 288) || (py >= -32 && py <= 224)) {
                ov96_021EAB38(sprite, TRUE);
            } else {
                ov96_021EAB38(sprite, FALSE);
            }
        }
        for (j = 0; j < 3; j++) {
            ov96_021EAF94(p->sprites[j], px, py);
        }
        {
        VecFx32 secondary = {0, 0, 0};
        secondary.x = px << 12;
        secondary.y = py << 12;
        if (ov96_021E60C0(course, i, mode)[5]) {
            secondary.y -= 0x18000;
        } else {
            secondary.y -= 0x10000;
        }
        ov96_021EB588(p->secondary, &secondary);
        }
        ov96_02207718(p, py);
        if (!hidden) {
            remote = FALSE;
            if (active == TRUE) {
                if (active != p->active) {
                    ov96_021EB52C(p->secondary, 1, 1);
                }
                ov96_021EAC5C(sprite, 21);
                p->flashTimer = 0;
                remote = TRUE;
                if (local == i) {
                    if (sub_02006190(3) == 0) {
                        ov96_02207C38(position.x / FX32_ONE, position.y / FX32_ONE, 0x890, 1);
                    }
                } else if (sub_02006190(4) == 0) {
                    ov96_02207C38(position.x / FX32_ONE, position.y / FX32_ONE, 0x890, 0);
                }
            } else {
                ov96_021EB52C(p->secondary, 1, 0);
            }
            if (!remote) {
                ov96_021EAC08(sprite, width);
                if (p->flashTimer) {
                    ov96_021EAC5C(sprite, 26);
                    p->flashTimer--;
                } else {
                    ov96_021EAC5C(sprite, 0);
                }
            }
        } else {
            ov96_021EB52C(p->secondary, 1, 0);
            ov96_021EAC0C(sprite, width);
        }
        p->active = active;
        effect = (packet->words[i] >> 28) & 0xF;
        ov96_02208840(view->ui, i, effect);
        if (p->effectCount < effect) {
            p->flashTimer = 10;
            ov96_021EB564(p->flash, 8);
            ov96_021EB52C(p->flash, 1, 1);
            if (local == i) {
                ov96_02207C38(position.x / FX32_ONE, position.y / FX32_ONE, 0x5E2, 1);
            } else {
                ov96_02207C38(position.x / FX32_ONE, position.y / FX32_ONE, 0x5EB, 0);
            }
        } else if (!hidden && p->effectCount > effect) {
            ov96_021EB564(p->flash, 9);
            ov96_021EB52C(p->flash, 1, 1);
        }
        p->effectCount = effect;
        ov96_021EB588(p->flash, &position);
        count = (packet->words[i] >> 18) & 0x3FF;
        if (viewer == i) {
            ov96_02208374(view->ui, count);
        }
        if (p->count < count) {
            ov96_021EB564(p->effects[p->nextEffect], 3);
            ov96_021EB52C(p->effects[p->nextEffect], 1, 1);
            p->nextEffect = (p->nextEffect + 1) % 2;
            ov96_02208864(view->ui, i);
            if (local == i) {
                ov96_02207C38(position.x / FX32_ONE, position.y / FX32_ONE, 0x88F, 1);
            } else {
                ov96_02207C38(position.x / FX32_ONE, position.y / FX32_ONE, 0x88F, 0);
            }
        }
        for (effect = 0; effect < 2; effect++) {
            ov96_021EB588(p->effects[effect], &position);
        }
        p->count = count;
    }
}
