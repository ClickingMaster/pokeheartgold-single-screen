#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    int count;
    char **items;
} List;

typedef struct {
    u8 p[4];
    int id;
    char *text;
} Info;

typedef struct {
    u8 p[0x20];
    Info *info;
    int kind;
} Packet;

typedef struct {
    u8 p[0x1c];
    List *list;
} Result;

typedef struct {
    int kind, id, arg;
} Request;

extern char ov45_02256964[];
extern char ov45_022565D0[];
extern char ov45_022560EC[];
extern char ov45_022568DC[];
extern char ov45_0225697C[];
extern void __msl_assertion_failed(char *, char *, char *, int);
extern Result *ov45_022496AC(void *, int, Request *);
extern void *ov00_021EC3BC(void *, u32);
extern void *ov00_021EC3A8(u32);
void ov45_0224E81C(void *, Packet *);

void ov45_0224E81C(void *owner, Packet *p) {
    int id;
    char *text, *copy;
    Result *r;
    List *list;
    Request req;
    if (p->kind < 3) {
        __msl_assertion_failed(ov45_02256964, ov45_022565D0, ov45_022560EC, 3997);
    }
    if (p->kind < 3) {
        return;
    }
    id = p->info->id;
    text = p->info->text;
    memset(&req, 0, sizeof(req));
    req.kind = 8;
    req.id = id;
    r = ov45_022496AC(owner, 1, &req);
    if (r == 0) {
        return;
    }
    list = r->list;
    if (list == 0) {
        __msl_assertion_failed(ov45_022568DC, ov45_022565D0, ov45_022560EC, 4015);
    }
    if (list->count < 0) {
        __msl_assertion_failed(ov45_0225697C, ov45_022565D0, ov45_022560EC, 4016);
    }
    copy = ov00_021EC3BC(list->items, (list->count + 1) * 4);
    if (copy == 0) {
        return;
    }
    list->items = (char **)copy;
    id = strlen(text);
    copy = ov00_021EC3A8(id + 1);
    if (copy == 0) {
        return;
    }
    memcpy(copy, text, id + 1);
    list->items[list->count] = copy;
    list->count++;
}
