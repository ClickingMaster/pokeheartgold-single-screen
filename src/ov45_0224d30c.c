#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 type;
    char *p;
    u32 pad;
    char *cursor;
} Input;

typedef struct {
    u8 pad[0x20];
    Input *input;
    int state;
} A;

typedef struct {
    int count, number, async;
} Info;

typedef struct {
    u8 pad[16];
    u32 id;
    u32 pad2;
    void *p18;
    Info *info;
    void *p20;
} Obj;

typedef struct {
    u32 type;
    char *p;
    u32 unused;
} Query;

typedef struct {
    u32 type;
    char *p;
    int count, number;
    char **values;
} Result;

extern char ov45_0225680C[];
extern char ov45_022565D0[];
extern char ov45_02256060[];
extern char ov45_02256708[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02249A0C(void *, Obj *, Result *);
extern void ov45_02244D30(void *, int, u32, Result *, void *, void *, int, int);
extern void ov00_021EC3D8(void *);
extern Obj *ov45_022496AC(void *, int, Query *);
extern void *ov00_021EC3A8(int);
extern char *ov45_0224D240(char *, int *);
extern char *ov00_021FF3D4(char *);
void ov45_0224D30C(void *, A *);

void ov45_0224D30C(void *ctx, A *a) {
    char *p;
    Obj *obj;
    char *cursor, *query;
    Info *info;
    int count;
    char **values;
    int i;
    if (a->state != 4) {
        __msl_assertion_failed(ov45_0225680C, ov45_022565D0, ov45_02256060, 0xb76);
    }
    if (a->state != 4) {
        return;
    }
    p = a->input->p;
    query = (char *)a->input->pad;
    cursor = a->input->cursor;
    {
        Query q = { 0, 0, 0 };
        Result result;
        int used;
        q.type = 12;
        q.p = query;
        obj = ov45_022496AC(ctx, 1, &q);
        if (!obj) {
            return;
        }
        info = obj->info;
        count = info->count;
        values = ov00_021EC3A8(count * 4);
        if (!values) {
            return;
        }
        for (i = 0; i < count; i++) {
            char *value = ov45_0224D240(cursor, &used);
            if (value) {
                values[i] = value;
                cursor += used;
            } else {
                values[i] = ov00_021FF3D4(ov45_02256708);
            }
        }
        result.type = 1;
        result.p = p;
        result.count = count;
        result.number = info->number;
        result.values = values;
        if (info->async == 0) {
            ov45_02249A0C(ctx, obj, &result);
        } else {
            ov45_02244D30(ctx, 29, obj->id, &result, obj->p18, obj->p20, 0, 20);
        }
        for (i = 0; i < count; i++) {
            ov00_021EC3D8(values[i]);
        }
        ov00_021EC3D8(values);
    }
}
