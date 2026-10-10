#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>

#include "to90_ov45_input_private.h"
extern const char ov45_02256848[];
extern const char ov45_022565D0[];
extern const char ov45_022562EC[];
extern const char ov45_02256824[];
void __msl_assertion_failed(const char *, const char *, const char *, int);
void ov00_021EC3D8(void *);

typedef union {
    struct {
        u32 type;
        char *id;
        u32 extra;
    } record;

    u32 words[3];
} Query;

typedef struct {
    u8 prefix[32];
    Ov45Input48 *info;
    u32 kind;
} Message;

typedef struct {
    u32 words[3];
    int count;
    char **items;
} NameList;
typedef struct Node Node;

struct Node {
    int type;
    void *base, *a, *b;
    int c, d, e;
    void *data;
    u32 id;
    Node *next;
};

char *ov00_021FF3D4(const char *);
void *ov00_021EC3BC(void *, u32);
void ov45_0224E5EC(void *, Message *);

void ov45_0224E5EC(void *ctx, Message *message) {
    char *id;
    Node *result;
    NameList *list;
    char *token, *name;
    char **array;
    if (message->kind != 3) {
        __msl_assertion_failed(ov45_02256848, ov45_022565D0, ov45_022562EC, 3892);
    }
    if (message->kind != 3) {
        return;
    }
    id = (char *)message->info->words[1];
    Query query;
    memset(&query, 0, sizeof(query));
    query.record.type = 4;
    query.record.id = id;
    result = (Node *)ov45_022496AC(ctx, 1, query.words);
    if (!result) {
        return;
    }
    list = (NameList *)result->data;
    token = strtok((char *)message->info->words[2], ov45_02256824);
    if (!token) {
        return;
    }
    do {
        if (*token == '-') {
            token++;
        }
        if (*token == '@' || *token == '+') {
            token++;
        }
        name = ov00_021FF3D4(token);
        if (!name) {
            return;
        }
        array = ov00_021EC3BC(list->items, (list->count + 1) * 4);
        if (!array) {
            ov00_021EC3D8(name);
            return;
        }
        list->items = array;
        list->items[list->count] = name;
        list->count++;
        token = strtok(NULL, ov45_02256824);
    } while (token);
}
