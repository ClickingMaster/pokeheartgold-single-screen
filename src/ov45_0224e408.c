#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
#pragma force_active on
#include "to90_ov45_input_private.h"

typedef struct {
    char *first, *second, *third;
    int count;
    char **items;
} Payload4;

typedef struct {
    char *first;
} Payload7;

typedef struct {
    char *data;
    int used, capacity;
} Buffer;

typedef struct {
    int handle;
    u32 state;
    u8 pad[0x100];
    Buffer recv, send;
    int crypt;
    u8 context[0x102];
    u8 sendContext[1];
} Conn;

typedef struct Message {
    char *raw, *prefix, *name, *a, *b, *command, *target, *tail;
    char **argv;
    int argc;
} Input;

typedef struct Event {
    int type;
    u8 pad[12];
    void *callback, *callback0, *arg, *payload, *user;
} Result;

extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_022561B8[];
extern char ov45_02256904[];
extern char ov45_022568DC[];
extern char ov45_0225691C[];
extern char ov45_02256934[];
extern char ov45_02256950[];
void __msl_assertion_failed(const char *, const char *, const char *, int);
char *ov00_021FF3D4(const char *);
int ov45_02252F74(Conn *, const char *, ...);
void ov45_02249A0C(void *, Result *, void *);
void ov45_0224E408(u8 *, u8 *);

void ov45_0224E408(u8 *work, u8 *packet) {
    Input *input = (Input *)packet;
    u32 keys[6];
    Result *result;
    char *value;
    if (!work) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_022561B8, 3797);
    }
    if (input->argc != 6) {
        __msl_assertion_failed(ov45_02256904, ov45_022565D0, ov45_022561B8, 3803);
    }
    if (input->argc != 6) {
        return;
    }
    value = input->argv[1];
    memset(keys, 0, sizeof(keys));
    keys[0] = 4;
    keys[1] = (u32)value;
    keys[3] = 7;
    keys[4] = (u32)value;
    result = (Result *)ov45_022496AC(work, 2, keys);
    if (!result) {
        return;
    }
    if (result->type == 4) {
        char *second;
        char *third;
        Payload4 *out = result->payload;
        second = input->argv[5];
        third = input->argv[3];
        out->first = ov00_021FF3D4(input->argv[2]);
        if (!out->first) {
            return;
        }
        out->second = ov00_021FF3D4(second);
        if (!out->second) {
            return;
        }
        out->third = ov00_021FF3D4(third);
        if (!out->third) {
            return;
        }
    } else if (result->type == 7) {
        Payload7 *out = result->payload;
        if (!out) {
            __msl_assertion_failed(ov45_022568DC, ov45_022565D0, ov45_022561B8, 3869);
        }
        if (!out->first) {
            __msl_assertion_failed(ov45_0225691C, ov45_022565D0, ov45_022561B8, 3870);
        }
        if (!*(s8 *)out->first) {
            __msl_assertion_failed(ov45_02256934, ov45_022565D0, ov45_022561B8, 3870);
        }
        ov45_02252F74((Conn *)(work + 28), ov45_02256950, out->first, input->argv[3]);
        ov45_02249A0C(work, result, 0);
    }
}
