#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Node {
    u8 pad[0x24];
    struct Node *next;
} Node;

typedef struct {
    u8 pad[0x814];
    Node *head, *tail;
} List;

extern const char ov45_022565C0[];
extern const char ov45_022565D0[];
extern const char ov45_02255F74[];
extern const char ov45_02256644[];
void __msl_assertion_failed(const char *, const char *, const char *, int);
void ov45_02249904(Node *);
void ov45_02249958(List *, Node *);

typedef struct {
    int type, unused;
    void *arg, *arg2;
} Request;

void ov45_02249A0C(List *, Request *, void *);
void ov45_0224A14C(List *, Request *);
extern const char ov45_02256654[];
extern const char ov45_02256010[];
extern const char ov45_0225665C[];
extern const char ov45_02256694[];
extern const char ov45_02256690[];

void ov45_0224A14C(List *list, Request *r) {
    BOOL valid;
    if (!r) {
        __msl_assertion_failed(ov45_02256654, ov45_022565D0, ov45_02256010, 0x268);
    }
    valid = FALSE;
    if (r->type >= 0 && r->type < 17) {
        valid = TRUE;
    }
    if (!valid) {
        __msl_assertion_failed(ov45_0225665C, ov45_022565D0, ov45_02256010, 0x269);
    }
    if (r->type == 0) {
        u32 data[5];
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 1) {
        u32 data[3];
        data[0] = 0;
        data[1] = (u32)7;
        data[2] = (u32)r->arg;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 2) {
        u32 data[3];
        data[0] = 0;
        data[1] = (u32)r->arg;
        data[2] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 3) {
        u32 data[5];
        data[0] = 0;
        data[1] = (u32)r->arg;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 4) {
        u32 data[7];
        data[0] = 0;
        data[1] = (u32)r->arg;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 0;
        data[6] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 10) {
        u32 data[4];
        data[0] = 0;
        data[1] = (u32)r->arg;
        data[2] = 0;
        data[3] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 11) {
        u32 data[5];
        data[0] = 0;
        data[1] = (u32)r->arg;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 5) {
        u32 data[3];
        data[0] = 0;
        data[1] = (u32)r->arg;
        data[2] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 6) {
        u32 data[4];
        data[0] = 0;
        data[1] = (u32)r->arg2;
        data[2] = (u32)r->arg;
        data[3] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 7) {
        ov45_02249A0C(list, r, NULL);
        return;
    }
    if (r->type == 8) {
        u32 data[4];
        data[1] = (u32)r->arg;
        data[2] = 0;
        data[3] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 9) {
        u32 data[3];
        data[0] = 0;
        data[1] = (u32)r->arg;
        data[2] = (u32)r->arg2;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 12) {
        u32 data[5];
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 13) {
        u32 data[6];
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 14) {
        u32 data[6];
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 0;
        ov45_02249A0C(list, r, data);
        return;
    }
    if (r->type == 15) {
        ov45_02249958(list, (Node *)r);
        return;
    }
    if (r->type == 16) {
        u32 data[2];
        data[0] = 0;
        data[1] = (u32)ov45_02256694;
        ov45_02249A0C(list, r, data);
        return;
    }
    __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256010, 0x311);
}
