#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    void *fields;
    u8 p[8];
    int multipart;
} Post;

typedef struct {
    u8 p[0x38];
    int result;
    u8 a[0x5c - 0x3c];
    int pending, sent;
    u8 b[0xfc - 0x64];
    int error;
    u8 c[0x13c - 0x100];
    Post *post;
    void *states;
    int index;
} Connection;

extern const char ov38_022259D0[];
extern const char ov38_02225644[];
extern const char ov38_02225520[];
extern const char ov38_02225750[];
extern const char ov38_022259DC[];
extern const char ov38_02225764[];
extern const char ov38_022259FC[];
extern const char ov38_02225A20[];
extern const char ov38_02225A70[];
extern const char ov38_02225A7C[];
extern const char ov38_02225AA8[];
void __msl_assertion_failed(const char *, const char *, const char *, int);
int ArrayLength(void *), ov38_0221C748(Connection *), ghiPostStateDoPosting(void *, Connection *, int), ov38_0221CE0C(Connection *, const char *, int);
void *ov00_021FE6EC(void *, int);
void ghiResetBuffer(void *);
int ghiPostDoPosting(Connection *);

int ghiPostDoPosting(Connection *c) {
    int count, status;
    if (!c) {
        __msl_assertion_failed(ov38_022259D0, ov38_02225644, ov38_02225520, 1028);
    }
    if (!c->post) {
        __msl_assertion_failed(ov38_02225750, ov38_02225644, ov38_02225520, 1029);
    }
    if (!c->states) {
        __msl_assertion_failed(ov38_022259DC, ov38_02225644, ov38_02225520, 1030);
    }
    count = ArrayLength(c->post->fields);
    if (count != ArrayLength(c->states)) {
        __msl_assertion_failed(ov38_02225764, ov38_02225644, ov38_02225520, 1031);
    }
    if (c->index < 0) {
        __msl_assertion_failed(ov38_022259FC, ov38_02225644, ov38_02225520, 1032);
    }
    if (c->index > ArrayLength(c->states)) {
        __msl_assertion_failed(ov38_02225A20, ov38_02225644, ov38_02225520, 1033);
    }
    count = ArrayLength(c->states);
    if (c->pending) {
        if (!ov38_0221C748(c)) {
            return 0;
        }
        if (c->sent < c->pending) {
            return 2;
        }
        ghiResetBuffer((u8 *)c + 0x50);
        if (c->index == count) {
            return 1;
        }
    }
    for (; c->index < count; c->index++) {
        void *state;
        state = ov00_021FE6EC(c->states, c->index);
        if (!state) {
            __msl_assertion_failed(ov38_02225A70, ov38_02225644, ov38_02225520, 1071);
        }
        status = ghiPostStateDoPosting(state, c, c->index == 0);
        if (status == 0) {
            if (!c->error || !c->result) {
                __msl_assertion_failed(ov38_02225A7C, ov38_02225644, ov38_02225520, 1083);
            }
            return 0;
        }
        if (status == 2) {
            return 2;
        }
    }
    if (c->post->multipart) {
        int length = strlen(ov38_02225AA8);
        if (!ov38_0221CE0C(c, ov38_02225AA8, length)) {
            return 0;
        }
    }
    return c->pending ? 2 : 1;
}
