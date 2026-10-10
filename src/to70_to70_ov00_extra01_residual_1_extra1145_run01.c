#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
long atol(const char *);
typedef void *(*Alloc)(char *, int);
typedef void (*Free)(char *, void *, int);

typedef struct {
    u8 p[0x1010];
    Alloc alloc;
    Free free;
    u8 q[12];
    char url[256];
    char *host, *path;
    int unused, mode;
    u16 port;
} Ctx;

typedef struct {
    char *key, *value;
} Pair;

typedef struct {
    Pair *items;
    int limit, count;
} List;

extern char ov00_022167F0[];
extern char ov00_022167F8[];
extern char ov00_02216804[];
extern char ov00_02216808[];
extern char ov00_0221680C[];
extern char ov00_0221682C[];
extern char ov00_0221684C[];
extern char ov00_02216868[];
int ov00_021E9C84(Ctx *, char *);
int ov00_021E9DD8(Ctx *, List *, char *, char *);

int ov00_021E9DD8(Ctx *c, List *l, char *k, char *v) {
    Alloc alloc = c->alloc;
    Free free = c->free;
    if (l->count > l->limit) {
        return 0;
    }
    l->items[l->count].key = alloc(ov00_0221680C, strlen(k) + 1);
    if (l->items[l->count].key) {
        l->items[l->count].value = alloc(ov00_0221682C, strlen(v) + 1);
        if (l->items[l->count].value) {
            strcpy(l->items[l->count].key, k);
            strcpy(l->items[l->count].value, v);
            l->count++;
            return 1;
        }
    }
    if (l->items[l->count].key) {
        free(ov00_0221684C, l->items[l->count].key, 0);
        l->items[l->count].key = 0;
    }
    if (l->items[l->count].value) {
        free(ov00_02216868, l->items[l->count].value, 0);
        l->items[l->count].value = 0;
    }
    return 0;
}
