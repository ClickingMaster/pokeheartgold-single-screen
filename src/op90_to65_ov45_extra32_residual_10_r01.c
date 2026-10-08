// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
#include "to90_ov45_input_private.h"

typedef struct Message { u32 pad[8]; Ov45Input48 *input; u32 state; } Message;
typedef struct List { int count; char **keys; int flag8; int flagc; } List;
typedef struct Node { u32 pad[4]; void *callback; u32 pad14; void *data; List *list; u32 id; u32 pad24; void *cb; void *userdata; } Node;
typedef struct Event { u32 success; char *id; u32 sender; int count; char **keys; char **values; } Event;
typedef struct Query { u32 type; char *key; u32 zero; } Query;
typedef struct Change { char *id; u32 sender; char *key; char *value; } Change;
extern const char ov45_022568B8[], ov45_022568C0[], ov45_022565D0[], ov45_022568A0[], ov45_022560C4[];
extern void __msl_assertion_failed(const char *, const char *, const char *, u32);
extern Node *ov45_02247E98(void *, char *);
extern void ov45_02244D30(void *, u32, void *, void *, void *, u32, char *, u32);
extern void *ov00_021EC3A8(u32);
extern void *ov00_021EC3BC(void *, u32);
extern void ov00_021EC3D8(void *);
extern char *ov45_0224D240(char *, int *);
extern void ov45_02249A0C(void *, Node *, Event *);
void ov45_0224D588(void *ctx, Message *msg);
#pragma force_active on
// @fn ov45_0224D588
void ov45_0224D588(void *ctx, Message *msg) {
    Query query;
    Node *record;
    Change change;
    Event event;
    Ov45Input48 *input;
    char *key;
    u32 sender;
    int count;
    char *id;
    char *cursor;
    Node *node;
    List *list;
    char **values;
    int i;
    int consumed;
    char *a;
    char *b;
    char *value;
    char saved;
    int size;
    char **newKeys;
    char **newValues;
    if (msg->state != 5) {
        __msl_assertion_failed(ov45_022568A0, ov45_022565D0, ov45_022560C4, 0xbfc);
    }
    if (msg->state != 5) {
        return;
    }
    input = msg->input;
    key = (char *)input->words[3];
    id = (char *)input->words[1];
    sender = input->words[2];
    cursor = (char *)input->words[4];
    if (strcmp(key, ov45_022568B8) == 0) {
        node = ov45_02247E98(ctx, id);
        if (node == NULL || node->cb == NULL) {
            return;
        }
        memset(&change, 0, sizeof(change));
        change.id = id;
        change.sender = sender;
        if (*cursor == 0) {
            return;
        }
        do {
            a = strstr(cursor, ov45_022568C0);
            cursor = a;
            while (*cursor != 0 && *cursor != '\\') {
                cursor++;
            }
            if (*cursor == 0) {
                return;
            }
            *cursor++ = 0;
            value = cursor;
            while (*cursor != 0 && *cursor != '\\') {
                cursor++;
            }
            saved = *cursor;
            *cursor = 0;
            change.key = a;
            change.value = value;
            ov45_02244D30(ctx, 28, node->cb, &change, node->userdata, 0, id, 16);
            *cursor = saved;
        } while (*cursor != 0);
        return;
    }
    memset(&query, 0, sizeof(query));
    query.type = 13;
    query.key = key;
    record = (Node *)ov45_022496AC(ctx, 1, (u32 *)&query);
    if (record == NULL) {
        return;
    }
    list = record->list;
    count = list->count;
    values = ov00_021EC3A8(count *4);
    if (values == NULL) {
        return;
    }
    for (i = 0; i < count; i++) {
        values[i] = ov45_0224D240(cursor, &consumed);
        if (values[i] != NULL) {
            cursor += consumed;
        }
    }
    if (list->flagc) {
        while ((a = ov45_0224D240(cursor, &consumed)) != NULL) {
            cursor += consumed;
            b = ov45_0224D240(cursor, &consumed);
            if (b != NULL) {
                size = (count + 1) *4;
                cursor += consumed;
                newKeys = ov00_021EC3BC(list->keys, size);
                if (newKeys != NULL) {
                    list->keys = newKeys;
                    newValues = ov00_021EC3BC(values, size);
                    if (newValues != NULL) {
                        values = newValues;
                        list->keys[count] = a;
                        values[count] = b;
                        count++;
                    } else {
                        ov00_021EC3D8(a);
                        ov00_021EC3D8(b);
                    }
                } else {
                    ov00_021EC3D8(a);
                    ov00_021EC3D8(b);
                }
            } else {
                ov00_021EC3D8(a);
                break;
            }
        }
        list->count = count;
    }
    event.success = 1;
    event.id = id;
    event.sender = sender;
    event.count = count;
    event.keys = list->keys;
    event.values = values;
    if (list->flag8 == 0) {
        ov45_02249A0C(ctx, record, &event);
    } else {
        ov45_02244D30(ctx, 30, record->callback, &event, record->data, record->id, 0, 24);
    }
    for (i = 0; i < count; i++) {
        ov00_021EC3D8(values[i]);
    }
    ov00_021EC3D8(values);
}
