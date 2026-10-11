#include "to55_bank_w16_private.h"

#define S8(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef struct Vec3 {
    int x, y, z;
} Vec3;

typedef struct Rec {
    int tag;
    Vec3 v;
} Rec;

int ov07_02221664(void *p) {
    int ret = 1;
    int a = BattleAnimUtil_GetBattlerType((void *)U32(p, 0), U32(p, 0x24));
    int b = BattleAnimUtil_GetBattlerType((void *)U32(p, 0), U32(p, 0x28));
    switch (a) {
    case 0:
        break;
    case 1:
        ret -= 2;
        break;
    case 2:
        break;
    case 3:
        ret -= 2;
        break;
    case 4:
        break;
    case 5:
        ret -= 2;
        break;
    }
    return ret;
}

void ov07_022216A8(void *p, int q, Vec3 *out) {
    Vec3 v = { 0 };
    Rec rec;
    Rec temp;
    int i;
    for (i = 0; i < 6; i++) {
        if (i == BattleAnimUtil_GetBattlerType((void *)U32(p, 0), q)) {
            BattleAnimSystem_ReadScriptArgs((void *)U32(p, 0), &rec, 4);
        } else {
            BattleAnimSystem_ReadScriptArgs((void *)U32(p, 0), &temp, 4);
        }
    }
    S8(p, 8) = ov07_02221664(p);
    v.x = rec.v.x;
    v.y = rec.v.y;
    v.z = rec.v.z;
    if (rec.tag == 1) {
        S8(p, 8) = 1;
    }
    v.x *= S8(p, 8);
    v.y *= S8(p, 8);
    v.z *= S8(p, 8);
    out->x += v.x;
    out->y += v.y;
    out->z += v.z;
}

void ov07_02221734(void *p, Vec3 *out) {
    Rec b;
    Vec3 a = { 0 };
    BattleAnimSystem_ReadScriptArgs((void *)U32(p, 0), &b, 4);
    S8(p, 8) = ov07_02221664(p);
    a.x = b.v.x;
    a.y = b.v.y;
    a.z = b.v.z;
    if (b.tag == 1) {
        S8(p, 8) = 1;
    }
    a.x *= S8(p, 8);
    a.y *= S8(p, 8);
    a.z *= S8(p, 8);
    out->x += a.x;
    out->y += a.y;
    out->z += a.z;
}
