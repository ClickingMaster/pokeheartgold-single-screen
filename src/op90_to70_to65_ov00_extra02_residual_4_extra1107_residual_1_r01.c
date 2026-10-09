#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct { u8 id; u8 used; u16 count; char *name; u32 value; } Entry;
extern Entry ov00_0221A948[];
u32 ov00_021F7A98(void);
void ov00_021EC2EC(int a, void *p, u32 unused);
void *ov00_021EC2A8(int a, u32 n);
void ov00_021F2EE4(int kind, int code);
void ov00_02212008(u32 id, const char *name);
u32 ov00_021F13A4(u32 id, const char *name, u32 value);
// @fn ov00_021F13A4
u32 ov00_021F13A4(u32 id, const char *name, u32 value) {
    u32 idx;
    if (name == NULL || value == 0) {
        return 0;
    }
    if (id >= 100 && ov00_0221A948[idx = id - 100].id != 0) {
        if (ov00_0221A948[idx].name != NULL && strcmp(ov00_0221A948[idx].name, name) != 0) {
            return 0;
        }
    } else {
        id = ov00_021F7A98();
        if (id == 0) {
            return 0;
        }
    }
    idx = id - 100;
    ov00_0221A948[idx].id = id;
    ov00_0221A948[idx].used = 1;
    ov00_0221A948[idx].count = 0;
    if (ov00_0221A948[idx].name != NULL) {
        ov00_021EC2EC(4, ov00_0221A948[idx].name, 0);
    }
    ov00_0221A948[idx].name = ov00_021EC2A8(4, strlen(name) + 1);
    if (ov00_0221A948[idx].name == NULL) {
        ov00_021F2EE4(9, -80001);
        return 0;
    }
    strcpy(ov00_0221A948[idx].name, name);
    ov00_0221A948[idx].value = value;
    ov00_02212008(id, ov00_0221A948[idx].name);
    return id;
}
