#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>

typedef struct Input {
    u32 pad, id;
    char *key, *value;
} Input;

typedef struct Message {
    u32 pad[8];
    Input *input;
    u32 state;
} Message;

typedef struct List {
    int count;
    char **keys;
    int flag8, flagc;
} List;

typedef struct Node {
    u32 pad[4];
    void *callback;
    u32 pad14;
    void *data;
    List *list;
    u32 id;
    u32 pad24;
    void *cb;
    void *userdata;
} Node;

typedef struct Event {
    u32 success, id, sender;
    int count;
    char **keys, **values;
} Event;

typedef struct Query {
    u32 type;
    char *key;
    u32 zero;
} Query;

typedef struct Change {
    u32 id, sender;
    char *key, *value;
} Change;

extern const char ov45_022568B8[];
extern const char ov45_022568C0[];
extern const char ov45_022565D0[];
extern void __msl_assertion_failed(const char *, const char *, const char *, u32);
extern Node *ov45_02247E98(void *, u32);
extern Node *ov45_022496AC(void *, int, Query *);
extern void ov45_02244D30(void *, u32, void *, void *, void *, u32, u32, u32);
extern void *ov00_021EC3A8(u32);
extern void *ov00_021EC3BC(void *, u32);
extern void ov00_021EC3D8(void *);
extern char *ov45_0224D240(char *, int *);
extern void ov45_02249A0C(void *, Node *, Event *);
extern const char ov45_0225680C[];
extern const char ov45_02256200[];
void ov45_0224D9F0(void *, Message *);

void ov45_0224D9F0(void *ctx, Message *message) {
    Node *found;
    Input *input;
    u32 id;
    char *key, *cursor;
    int consumed;
    Node *node;
    Query query;
    Change change;
    Event event;
    if (message->state != 4) {
        __msl_assertion_failed(ov45_0225680C, ov45_022565D0, ov45_02256200, 0xce1);
    }
    if (message->state != 4) {
        return;
    }
    input = message->input;
    id = input->id;
    key = input->key;
    cursor = input->value;
    if (strcmp(key, ov45_022568B8) == 0) {
        node = ov45_02247E98(ctx, id);
        if (!node || !node->cb) {
            return;
        }
        memset(&change, 0, sizeof(change));
        change.id = id;
        change.sender = 0;
        if (!*cursor) {
            return;
        }
        do {
            char *start = strstr(cursor, ov45_022568C0);
            char *value;
            s8 saved;
            cursor = start;
            while (*cursor && *cursor != '\\') {
                cursor++;
            }
            if (!*cursor) {
                return;
            }
            *cursor++ = 0;
            value = cursor;
            while (*cursor && *cursor != '\\') {
                cursor++;
            }
            saved = *cursor;
            *cursor = 0;
            change.key = start;
            change.value = value;
            ov45_02244D30(ctx, 28, node->cb, &change, node->userdata, 0, id, 16);
            *cursor = saved;
        } while (*cursor);
        return;
    }
    memset(&query, 0, sizeof(query));
    query.type = 14;
    query.key = key;
    found = ov45_022496AC(ctx, 1, &query);
    if (!found) {
        return;
    }
    {
        List *list = found->list;
        int count = list->count, i;
        char **values;
        char *a, *b;
        if (count) {
            values = ov00_021EC3A8(count * 4);
            if (!values) {
                return;
            }
            for (i = 0; i < count; i++) {
                values[i] = ov45_0224D240(cursor, &consumed);
                if (values[i]) {
                    cursor += consumed;
                }
            }
            if (list->flag8) {
                a = ov45_0224D240(cursor, &consumed);
                while (a) {
                    char **newkeys, **newvalues;
                    u32 size;
                    cursor += consumed;
                    b = ov45_0224D240(cursor, &consumed);
                    if (b) {
                        size = (count + 1) * 4;
                        cursor += consumed;
                        newkeys = ov00_021EC3BC(list->keys, size);
                        if (newkeys) {
                            list->keys = newkeys;
                            newvalues = ov00_021EC3BC(values, size);
                            if (newvalues) {
                                values = newvalues;
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
                    a = ov45_0224D240(cursor, &consumed);
                }
                list->count = count;
            }
        } else {
            char **keys = NULL;
            values = NULL;
            count = 0;
            while (1) {
                char **newkeys, **newvalues;
                u32 size;
                a = ov45_0224D240(cursor, &consumed);
                if (!a) {
                    break;
                }
                cursor += consumed;
                b = ov45_0224D240(cursor, &consumed);
                if (!b) {
                    ov00_021EC3D8(a);
                    break;
                }
                size = (count + 1) * 4;
                cursor += consumed;
                newkeys = ov00_021EC3BC(keys, size);
                newvalues = ov00_021EC3BC(values, size);
                if (!newkeys || !newvalues) {
                    ov00_021EC3D8(a);
                    ov00_021EC3D8(b);
                    while (count--) {
                        ov00_021EC3D8(keys[count]);
                        ov00_021EC3D8(values[count]);
                    }
                    if (newkeys) {
                        ov00_021EC3D8(newkeys);
                    } else {
                        ov00_021EC3D8(keys);
                    }
                    if (newvalues) {
                        ov00_021EC3D8(newvalues);
                    } else {
                        ov00_021EC3D8(values);
                    }
                }
                newkeys[count] = a;
                newvalues[count] = b;
                keys = newkeys;
                values = newvalues;
                count++;
            }
            list->count = count;
            list->keys = keys;
        }
        event.success = 1;
        event.id = id;
        event.sender = 0;
        event.count = count;
        event.keys = list->keys;
        event.values = values;
        ov45_02249A0C(ctx, found, &event);
        for (i = 0; i < count; i++) {
            ov00_021EC3D8(values[i]);
        }
        ov00_021EC3D8(values);
    }
}
