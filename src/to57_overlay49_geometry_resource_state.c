#include "to57_overlay49_geometry_resource_state_private.h"

#pragma require_prototypes off

typedef struct Ov49S10 {
    u8 b[0x10];
} Ov49S10;

extern void ov49_0225D528(void *p, void *a, u32 b, void *c);
extern void ov45_0222D740(void *p);
extern void ov49_0225D574(void *p);
extern void ov49_0225D57C(void *p, void *a, fx32 b);
extern void sub_020180BC(void *p, void *a, void *b, u32 c, void *d, void *e);
extern void sub_020180F8(void *p, void *a);
extern void sub_020181B0(void *p, void *a);
extern void sub_020182A0(void *p, u32 a);
extern void sub_020181D4(void *p, void *a);
extern void sub_020181E0(void *p, void *a);
extern void sub_02018198(void *p, u32 a);

typedef struct X49 {
    Ov49S10 obj[2];
    u8 anim[5][0x14];
    u32 flags[5];
} X49;

typedef struct R49 {
    u8 pad[0x180];
    u32 first[2];
    u32 pad188;
    u32 ids[5];
} R49;

void ov49_0225D5FC(X49 *p, void *a, R49 *r, void *c, void *e) {
    int i;
    for (i = 0; i < 2; i++) {
        ov49_0225D528(&p->obj[i], a, r->first[i], c);
        ov45_0222D740((void *)U32_AT(&p->obj[i], 0));
    }
    for (i = 0; i < 5; i++) {
        if (r->first[0] == r->ids[i]) {
            p->flags[i] = 0;
        } else {
            p->flags[i] = 1;
            if (i != 3) {
                sub_020180BC(p->anim[i], &p->obj[0], a, r->ids[i], c, e);
            } else {
                sub_020180BC(p->anim[i], &p->obj[1], a, r->ids[i], c, e);
            }
        }
    }
}

void ov49_0225D6AC(X49 *p, void *a) {
    int i;
    for (i = 0; i < 5; i++) {
        if (p->flags[i]) {
            sub_020180F8(p->anim[i], a);
            p->flags[i] = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        ov49_0225D574(&p->obj[i]);
    }
}

typedef struct Y49 {
    u32 enabled;
    u8 object[2][0x78];
    u32 active[5];
    u32 frame[5];
} Y49;

void ov49_0225D6F0(Y49 *p, X49 *a) {
    int i;
    p->enabled = 1;
    for (i = 0; i < 2; i++) {
        sub_020181B0(p->object[i], &a->obj[i]);
        sub_020182A0(p->object[i], 1);
    }
    for (i = 0; i < 4; i++) {
        if (a->flags[i] == 1) {
            p->active[i] = 1;
            if (i != 3) {
                sub_020181D4(p->object[0], a->anim[i]);
            } else {
                sub_020181D4(p->object[1], a->anim[i]);
            }
        }
    }
}

void ov49_0225D76C(Y49 *p, X49 *a) {
    int i;
    for (i = 0; i < 4; i++) {
        if (p->active[i] == 1) {
            p->active[i] = 0;
            if (i != 3) {
                sub_020181E0(p->object[0], a->anim[i]);
            } else {
                sub_020181E0(p->object[1], a->anim[i]);
            }
        }
    }
    p->enabled = 0;
}

void ov49_0225D7B8(void *p, void *a) {
    void *A;
    void *B;
    void *C;
    void *D;
    volatile int i;
    int next;
    A = p;
    i = 0;
    B = (u8 *)a + 0x20;
    C = (u8 *)p + 0x108;
    D = B;
    do {
        if (U32_AT(A, 0xF4)) {
            ov49_0225D57C(C, B, 0x1000);
            sub_02018198(D, U32_AT(A, 0x108));
        }
        next = i + 1;
        A = (u8 *)A + 4;
        B = (u8 *)B + 0x14;
        C = (u8 *)C + 4;
        D = (u8 *)D + 0x14;
        i = next;
    } while (next < 5);
}
