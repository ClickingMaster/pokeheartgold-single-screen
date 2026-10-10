#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

#include "to90_ov45_input_private.h"
#pragma force_active on

typedef struct Words {
    u32 w[8];
} Words;

typedef struct {
    volatile u32 tag;
    u32 value;
    char *text;
} Entry;

typedef struct Event {
    int type;
    u8 pad[12];
    void *callback, *callback0, *arg, *payload, *user;
} Event;

typedef struct Arg {
    u8 pad[32];
    Ov45Input48 *payload;
    int size;
} Arg;

typedef struct Node {
    u8 pad[24];
    void *callback;
    u8 pad1c[16];
    void *arg;
} Node;

typedef struct Record {
    int a;
    char *b;
    void *p;
} Record;

extern char ov45_022568EC[];
extern char ov45_022565D0[];
extern char ov45_022562D0[];
void __msl_assertion_failed(const char *, const char *, const char *, int);
Entry *ov45_0224AD24(u32, u32 *, int);
void ov45_0224AFD8(u32 *, Entry *);
int ov45_02248288(void *, char *, void *);
void ov45_02248374(void *, char *, void *);
void *ov45_02247E98(void *, char *);
int ov45_02244D30(u8 *, int, void *, u32 *, int, int, char *, int);
void ov45_02249A0C(void *, Event *, void *);
void ov00_021EC3D8(void *);
void ov45_0224E288(void *, Arg *);

void ov45_0224E288(void *ctx, Arg *arg) {
    char *id;
    Entry *data;
    void *found;
    if (arg->size < 3) {
        __msl_assertion_failed(ov45_022568EC, ov45_022565D0, ov45_022562D0, 0xe93);
    }
    if (arg->size < 3) {
        return;
    }
    id = (char *)arg->payload->words[1];
    data = ov45_0224AD24(arg->payload->words[2], arg->payload->words + 3, arg->size - 3);
    if (!data) {
        return;
    }
    Words input, output;

    union {
        Record record;
        u32 words[3];
    } event;

    Record reply;

    union {
        struct {
            char *a;
            Words *b;
        } record;

        u32 words[2];
    } notify;

    memset(&input, 0, sizeof(input));
    ov45_0224AFD8(input.w, data);
    if (!ov45_02248288(ctx, id, &output)) {
        Node *node;
        ov45_02248374(ctx, id, &input);
        node = ov45_02247E98(ctx, id);
        if (node && node->callback) {
            notify.record.a = id;
            notify.record.b = &input;
            ov45_02244D30(ctx, 10, node->callback, notify.words, (int)node->arg, 0, id, 8);
        }
    }
    memset(&event, 0, sizeof(event));
    event.record.a = 5;
    event.record.b = id;
    found = ov45_022496AC(ctx, 1, event.words);
    if (found) {
        reply.a = 1;
        reply.b = id;
        reply.p = &input;
        ov45_02249A0C(ctx, (Event *)found, &reply);
    }
    ov00_021EC3D8(data);
}
