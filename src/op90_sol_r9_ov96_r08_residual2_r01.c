#pragma force_active on
#include "global.h"

typedef struct Packet {
    u8 x;
    u8 y;
    u8 unk2[2];
    u8 unk4_0 : 6;
    u8 kind : 2;
    u8 unk5_0 : 5;
    u8 mode : 2;
    u8 unk5_7 : 1;
} Packet;

typedef struct State {
    u8 unk0[0xe4];
    u32 unkE4_0 : 8;
    u32 sub : 4;
    u32 mode : 2;
    u32 unkE4_14 : 18;
} State;

void *PokeathlonCourse_GetDataCopyArea(void *course);
u32 PokeathlonCourse_GetParticipantCount(void *course);
void *ov96_021E8A20(void *data);
u32 ov96_021E5F24(void *course);
void ov96_0221996C(void *a, u32 index, u32 kind, int arg);
void ov96_02216770(State *s, s16 *point, void *a, u32 index, void *value, int flag, int special);
void ov96_02216934(State *s, s16 *point, void *a, u32 index, void *value, int flag, int special);
void ov96_02216A54(State *s, s16 *point, void *value, int flag, int special);
void ov96_02216AA4(Packet *in, State *s, void *a, void *course, u32 index);

// @fn ov96_02216AA4
void ov96_02216AA4(Packet *in, State *s, void *a, void *course, u32 index) {
    void *value;
    BOOL valid;
    BOOL changed;
    BOOL flag;
    s16 point[2];
    u8 *data;
    BOOL host;
    BOOL special;
    BOOL either;

    changed = FALSE;
    data = PokeathlonCourse_GetDataCopyArea(course);
    point[0] = in->x;
    point[1] = in->y;
    if (PokeathlonCourse_GetParticipantCount(course) <= index) {
        valid = TRUE;
        value = ov96_021E8A20(data + 0x50 + index * 0x28);
    } else {
        valid = FALSE;
        value = ov96_021E8A20(data);
    }
    host = ov96_021E5F24(course) == 0 ? TRUE : FALSE;
    if (host && valid) {
        flag = TRUE;
    } else {
        flag = FALSE;
    }
    special = index == ov96_021E5F24(course) ? TRUE : FALSE;
    either = flag | special;
    if (in->mode != s->mode) {
        s->sub = 0;
        s->mode = in->mode;
        changed = TRUE;
    }
    switch (s->mode) {
    case 1:
        if (changed) {
            ov96_0221996C(a, index, in->kind, 1);
        }
        ov96_02216770(s, point, a, index, value, flag, special);
        break;
    case 2:
        ov96_02216934(s, point, a, index, value, either, special);
        break;
    case 0:
        if (changed) {
            ov96_02216A54(s, point, value, either, special);
            ov96_0221996C(a, index, in->kind, 0);
        }
        break;
    }
}
