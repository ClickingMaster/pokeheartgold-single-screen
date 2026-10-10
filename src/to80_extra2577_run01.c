#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_02255EF4[];
extern char ov45_0225671C[];
extern char ov45_02256750[];
extern char ov45_02256760[];

typedef struct Event {
    char *raw, *prefix, *name, *a, *b, *command, *target, *tail;
    char **names;
    int kind;
} Event;

#include "to90_ov45_input_private.h"
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern int strcmp(const char *, const char *);
extern int ov45_02247AE0(void *, char *);
extern int ov45_02248238(void *, char *);
extern int ov45_0224861C(void *, char *);
extern void *ov45_02247E98(void *, char *);
extern void ov45_02248454(void *, char *, char *);
extern void ov45_02248888(void *, char *, char *, int, char *, char *);
extern int ov45_02244D30(u8 *, int, void *, u32 *, int, int, char *, int);

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

extern int ov45_02252F74(Conn *, const char *, ...);

void ov45_0224B8D0(u8 *, u8 *);
#pragma force_active on

void ov45_0224B8D0(u8 *c, u8 *packet) {
    Event *event = (Event *)packet;
    char *id;
    char *name;
    char *a, *b;
    void *member;
    int mode;
    Ov45Key12 key;
    u32 change[3];
    u32 saved;
    u8 *node;
    if (!c) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255EF4, 1750);
    }
    if (event->kind != 1) {
        __msl_assertion_failed(ov45_0225671C, ov45_022565D0, ov45_02255EF4, 1756);
    }
    if (event->kind != 1) {
        return;
    }
    name = event->name;
    id = *event->names;
    a = event->a;
    b = event->b;
    if (*name == '@') {
        name++;
        mode = 2;
        if (!*name) {
            __msl_assertion_failed(ov45_02256750, ov45_022565D0, ov45_02255EF4, 1771);
        }
    } else if (*name == '+') {
        name++;
        mode = 1;
        if (!*name) {
            __msl_assertion_failed(ov45_02256750, ov45_022565D0, ov45_02255EF4, 1777);
        }
    } else {
        mode = 0;
    }
    if (!strcmp(name, (char *)c + 0x36c)) {
        if (!ov45_02247AE0(c, id)) {
            return;
        }
        memset(&key, 0, sizeof(key));
        key.words[0] = 1;
        key.words[1] = (u32)id;
        node = ov45_022496AC(c, 1, key.words);
        if (!node) {
            return;
        }
        member = P(node, 28);
        ov45_02247B94(c, id, &((Ov45Payload84 *)member)->input);
        ov45_02248454(c, id, ((Ov45Payload84 *)member)->text);
        ((Ov45Payload84 *)member)->flag = 1;
        ov45_02252F74((Conn *)(c + 28), ov45_02256760, id);
        return;
    }
    if (ov45_02248238(c, id)) {
        ov45_02248888(c, name, id, mode, a, b);
    }
    if (!ov45_0224861C(c, id)) {
        return;
    }
    member = ov45_02247E98(c, id);
    if (!member) {
        return;
    }
    Ov45Input48 *remote = (Ov45Input48 *)member;
    if (remote->words[2]) {
        change[0] = (u32)id;
        change[1] = (u32)name;
        change[2] = mode;
        ov45_02244D30(c, 6, (void *)remote->words[2], change, remote->words[11], 0, id, 12);
    }
    if (!remote->words[8]) {
        return;
    }
    saved = (u32)id;
    ov45_02244D30(c, 12, (void *)remote->words[8], &saved, remote->words[11], 0, id, 4);
}
