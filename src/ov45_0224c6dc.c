#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    int capacity, count;
    char **names;
    int *modes;
} List;

typedef struct {
    int type;
    u8 pad[24];
    List *list;
} Result;

typedef struct {
    u8 pad[8];
    void *channel;
    char *text;
} Message;

typedef struct {
    u8 pad[32];
    Message *message;
    int type;
} Input;

typedef struct {
    int type;
    void *channel;
    int pad;
} Record;

typedef struct {
    Record items[4];
} Params;

extern char ov45_0225680C[];
extern char ov45_022565D0[];
extern char ov45_02256088[];
extern char ov45_02256824[];
extern char ov45_02256828[];
extern char ov45_02256838[];
extern char ov45_02256690[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern Result *ov45_022496AC(void *, int, Params *);
extern void *ov00_021EC3BC(void *, u32);
extern void *ov00_021EC3A8(u32);
extern void ov45_02248888(void *, char *, void *, int, int, int);
void ov45_0224C6DC(void *, Input *);

void ov45_0224C6DC(void *ctx, Input *input) {
    u32 result;
    void *channel;
    List *list = 0;
    if (input->type != 4) {
        __msl_assertion_failed(ov45_0225680C, ov45_022565D0, ov45_02256088, 2351);
    }
    if (input->type != 4) {
        return;
    }
    channel = input->message->channel;
    char *text = input->message->text;
    Params params = { 0 };
    params.items[0].type = 1;
    params.items[0].channel = channel;
    params.items[1].type = 15;
    params.items[1].channel = channel;
    params.items[2].type = 3;
    params.items[2].channel = channel;
    params.items[3].type = 3;
    result = (u32)ov45_022496AC(ctx, 4, &params);
    if (!result) {
        return;
    }
    if (((Result *)result)->type != 1) {
        list = ((Result *)result)->list;
    }
    char *token = strtok(text, ov45_02256824);
    if (!token) {
        return;
    }
    do {
        if (!*token) {
            __msl_assertion_failed(ov45_02256828, ov45_022565D0, ov45_02256088, 2385);
        }
        int mode;
        if (*token == '@') {
            if (!token[1]) {
                __msl_assertion_failed(ov45_02256838, ov45_022565D0, ov45_02256088, 2391);
            }
            mode = 2;
            token++;
        } else if (*token == '+') {
            if (!token[1]) {
                __msl_assertion_failed(ov45_02256838, ov45_022565D0, ov45_02256088, 2397);
            }
            mode = 1;
            token++;
        } else {
            mode = 0;
        }
        if (((Result *)result)->type != 1) {
            if (list->count == list->capacity) {
                void *newNames = ov00_021EC3BC(list->names, (list->capacity + 100) * 4);
                if (!newNames) {
                    __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256088, 2413);
                    return;
                }
                list->names = newNames;
                void *newModes = ov00_021EC3BC(list->modes, (list->capacity + 100) * 4);
                if (!newModes) {
                    __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256088, 2420);
                    return;
                }
                list->modes = newModes;
                list->capacity += 100;
            }
            u32 length = strlen(token);
            char *copy = ov00_021EC3A8(length + 1);
            if (!copy) {
                __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256088, 2433);
                return;
            }
            memcpy(copy, token, length + 1);
            list->names[list->count] = copy;
            list->modes[list->count] = mode;
            list->count++;
        }
        if (((Result *)result)->type == 1 || ((Result *)result)->type == 15) {
            ov45_02248888(ctx, token, channel, mode, 0, 0);
        }
        token = strtok(0, ov45_02256824);
    } while (token);
}
