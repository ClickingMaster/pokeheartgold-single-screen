#include "to57_overlay91_run036_private.h"

void ov91_0225E40C(u8 *base) {
    u8 *record = base;
    int i = 0;
    u8 *q = base;
    record += 0x4c;
    for (; i < 0x60; i++, q += 0x44, record += 0x44) {
        u8 state = q[0x4c];
        if ((u8)(state + 255) <= 1) {
            ov91_0225E45C(base, (Ov91R36Work *)record);
        } else if (state == 5) {
            ov91_0225E8BC(record, base);
        } else if (state == 3) {
            ov91_0225E9F4(record, base);
        }
    }
}

void ov91_0225E45C(void *outerArg, Ov91R36Work *work) {
    u8 *outer = outerArg;
    VecFx32 a;
    VecFx32 b;
    fx32 oldY;
    int result;
    int state = work->state;

    if (state != 1 && state != 2) {
        return;
    }
    oldY = work->position.y;
    ov91_0225E728(work, &a);
    if (work->state == 1) {
        result = ov91_0225DE8C(outer + 0x19cc, &work->position, &a, &b);
    } else {
        result = 0;
    }
    ov91_0225EA54(work, &a);
    if (result == 3 || result == 1) {
        ov91_0225EA54(work, &b);
    }
    work->counter++;
    if (result == 1) {
        *(s32 *)(outer + 0x18) += ov91_0225D600(outer);
        if (*(s32 *)(outer + 0x18) > 99999) {
            *(s32 *)(outer + 0x18) = 99999;
        }
        switch (*(s32 *)(outer + 0x1c)) {
        case 0:
            PlaySE(0x582);
            PlaySE(0x583);
            break;
        case 1:
            PlaySE(0x582);
            PlaySE(0x584);
            break;
        default:
            PlaySE(0x582);
            PlaySE(0x585);
            break;
        }
        ov91_0225E9E4(work, outer);
        ov91_0225D694(outer + 0x1e38, *(s32 *)(outer + 0x14), *(s32 *)(outer + 0x1c), *(void **)outer);
        if (*(s32 *)(outer + 0x1c) < 99) {
            (*(s32 *)(outer + 0x1c))++;
        }
        if (*(s32 *)(outer + 0x1c) == 3) {
            ov91_02261808(outer + 0x862c, outer + 0x1ab4, *(s32 *)(outer + 0x1c));
        } else {
            ov91_022618B0(outer + 0x862c, outer + 0x1ab4, *(s32 *)(outer + 0x1c));
        }
        return;
    }
    result = 0;
    if (ov91_0225DFB0(outer + 0x19cc, &work->position) == 1) {
        result = 1;
        if (state == 1) {
            PlaySE(0x722);
        }
        ov91_0225E7C4(work, outer, 1);
    } else if (work->position.y <= -0x32000) {
        if (!ov91_0225E990(work)) {
            result = 1;
            if (oldY <= -0x32000) {
                ov91_0225E3F4(outer, work, 0, *(s32 *)(outer + 0x1c));
            } else {
                ov91_0225E7C4(work, outer, 0);
            }
        } else if (work->position.y <= 0x400) {
            result = 1;
            ov91_0225E3F4(outer, work, 0, *(s32 *)(outer + 0x1c));
        }
    } else if (ov91_0225E9C0(work) == 1) {
        result = 1;
        VEC_Subtract(&work->position, &a, &work->position);
        ov91_0225E7E8(work, outer, 1, 0x400, 0);
    }
    if (result == 1 && state == 1) {
        *(s32 *)(outer + 0x1c) = 0;
        ov91_02261890(outer + 0x862c);
    }
}

void ov91_0225E62C(u8 *base) {
    int i = 0;
    u8 *p = base;
    p += 0x4c;
    for (; i < 0x60; i++, p += 0x44) {
        ov91_0225E67C(p, base);
    }
}

void ov91_0225E648(u8 *base) {
    u8 *q;
    int i;
    u8 *p;
    p = base;
    i = 0;
    q = base;
    p += 0x4c;
    for (; i < 0x60; i++, q += 0x44, p += 0x44) {
        if (q[0x4d] == 1) {
            q[0x4c] = 2;
        }
        ov91_0225E67C(p, base);
    }
}
