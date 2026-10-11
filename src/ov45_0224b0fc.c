#pragma force_active on
#include "global.h"
#pragma force_active on

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    char *sender, *message;
} Pair;

typedef struct {
    u8 pad8[8];
    int channel;
    u8 padC[20];
    Pair *pair;
    int kind;
} Event;

typedef struct {
    u8 pad[0x36c];
    char name[0x800 - 0x36c];
    void *callback;
    u32 pad804;
    void *arg;
} Context;

typedef struct {
    void *callback;
    u8 pad4[40];
    void *arg;
} Handler;

typedef struct {
    int channel;
    char *message;
    int action;
} Local;

typedef struct {
    char *sender;
    int channel;
    char *message;
    int action;
} Remote;

extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_02255FE8[];
extern char ov45_022566F0[];
extern char ov45_02256708[];
extern char ov45_0225670C[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02244D30(Context *, int, void *, void *, void *, int, void *, int);
extern int strnicmp(const char *, const char *);
extern Handler *ov45_02247E98(Context *, const char *);
extern char *strchr(const char *, int);
void ov45_0224B0FC(Context *, Event *);

void ov45_0224B0FC(Context *ctx, Event *event) {
    Local local;
    Remote remote;
    char *command, *sender;
    int channel, action = 0;
    char *message;
    int length;
    if (!ctx) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255FE8, 0x57c);
    }
    if (event->kind != 2) {
        __msl_assertion_failed(ov45_022566F0, ov45_022565D0, ov45_02255FE8, 0x582);
    }
    if (event->kind != 2) {
        return;
    }
    sender = event->pair->sender;
    message = event->pair->message;
    channel = event->channel;
    length = strlen(message);
    command = ov45_02256708;
    if ((s8)message[0] == 1 && (((s8)message[1] >= 'A' && (s8)message[1] <= 'Z') || ((s8)message[1] >= 'a' && (s8)message[1] <= 'z')) && (s8)message[length - 1] == 1) {
        char *space;
        message[length - 1] = 0;
        space = strchr(message, 32);
        if (space) {
            *space = 0;
            command = message + 1;
            message = space + 1;
        }
    }
    if (!strcmp(command, ov45_0225670C)) {
        action = 1;
    } else if ((s8)*command) {
        return;
    }
    if (!strnicmp(sender, ctx->name)) {
        if (!ctx->callback) {
            return;
        }
        if (action) {
            local.action = 1;
        } else {
            local.action = 0;
        }
        local.channel = channel;
        local.message = message;
        ov45_02244D30(ctx, 2, ctx->callback, &local, ctx->arg, 0, 0, 12);
        return;
    }
    {
        Handler *handler = ov45_02247E98(ctx, sender);
        if (!handler || !handler->callback) {
            return;
        }
        if (action) {
            remote.action = 1;
        } else {
            remote.action = 0;
        }
        remote.sender = sender;
        remote.channel = channel;
        remote.message = message;
        ov45_02244D30(ctx, 4, handler->callback, &remote, handler->arg, 0, sender, 16);
    }
}
