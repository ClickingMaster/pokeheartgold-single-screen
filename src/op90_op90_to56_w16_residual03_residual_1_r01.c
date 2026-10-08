#pragma force_active on
#include "global.h"
#include <string.h>

typedef struct Entry {
    u32 len;
    char name[32];
    u8 pad[4];
    u8 address[6];
    u16 mode;
} Entry;

typedef struct List {
    u32 count;
    Entry entries[];
} List;

typedef struct ApInfo {
    u8 pad0[4];
    u8 address[6];
    u16 len;
    char name[32];
    u16 flags;
    u8 rest[0xC0 - 0x2E];
} ApInfo;

typedef struct Work {
    u8 pad0[0x10];
    int cancelled;
    u32 pad14;
    u32 selection;
    u8 pad1C[0x1C];
    int mode;
    u8 pad3C[0x20];
    ApInfo *list;
    u8 pad60[0x10];
    int count;
} Work;

typedef struct Limits {
    u8 pad[12];
    u32 limit;
} Limits;

extern Work ov13_0224DF30;
extern Limits ov13_02245A58;
extern char ov13_0224E440[];
extern u8 ov13_0224E3E0[6];

void *ov13_02226CD8(int kind, u32 size);
void ov13_02226CFC(void *ptr);
u32 ov13_02226CBC(void);
int ov13_02223B28(int a, int b, int c, u32 d);
int ov13_02223E6C(void);
int ov13_02223AE4(ApInfo *list, u32 count);
int ov13_02224164(const List *a, const List *b, u32 *out);
void ov13_02223ED4(void *arg);
void ov13_02226D40(char *buf, const u8 *mac);
void ov13_02226F3C(void);
void OS_Sleep(u32 msec);
int ov13_02224330(void);

#pragma optimization_level 3
// @fn ov13_02224330
int ov13_02224330(void) {
    char text[32];
    OSAlarm alarm;
    u32 selected;
    int attempt;
    int i;
    int total;
    u32 size;
    List *out;
    List *previous;
    int result;
    int running;
    int event;
    int found;
    ApInfo *ap;

    result = -1;
    previous = NULL;
    selected = 0;
    size = sizeof(Entry) * ov13_0224DF30.count + sizeof(List) + sizeof(Entry);
    out = ov13_02226CD8(1, size);
    if (out == NULL) {
        goto done;
    }
    previous = ov13_02226CD8(1, size);
    if (previous == NULL) {
        goto done;
    }
    for (attempt = 0; attempt < 30 && !ov13_0224DF30.cancelled; attempt++) {
        if (ov13_02226CBC() >= ov13_02245A58.limit) {
            break;
        }
        if (!ov13_02223B28(0, 0, 0, 0x30BFFE)) {
            result = -2;
            goto done;
        }
        OS_CreateAlarm(&alarm);
        OS_SetAlarm(&alarm, 0xFFB10, (OSAlarmHandler)ov13_02223ED4, (void *)19);
        running = TRUE;
        result = 0;
        do {
            OS_Sleep(10);
            if (ov13_02226CBC() >= ov13_02245A58.limit || ov13_0224DF30.cancelled) {
                break;
            }
            while ((event = ov13_02223E6C()) != 0) {
                switch (event) {
                case 19:
                    running = FALSE;
                    break;
                case 5:
                    found = ov13_02223AE4(ov13_0224DF30.list, ov13_0224DF30.count);
                    if (found > result) {
                        result = found;
                        OS_CancelAlarm(&alarm);
                        OS_SetAlarm(&alarm, 0xFFB10, (OSAlarmHandler)ov13_02223ED4, (void *)19);
                    }
                    break;
                case 10:
                    running = FALSE;
                    break;
                case 4:
                case 8:
                case 18:
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                default:
                    running = FALSE;
                    break;
                }
            }
        } while (running);
        OS_CancelAlarm(&alarm);
        while (ov13_02223E6C() != 0) {
        }
        if (ov13_0224DF30.cancelled) {
            break;
        }
        total = 0;
        if (result >= ov13_0224DF30.count) {
            result = -6;
            goto done;
        }
        ap = ov13_0224DF30.list;
        i = 0;
        if (result > 0) {
            for (; i < result; i++) {
                memcpy(out->entries[total].name, ap->name, 32);
                out->entries[total].len = ap->len;
                out->entries[total].name[ap->len] = 0;
                out->entries[total].mode = (ap->flags & 0x10) ? 1 : 0;
                memcpy(out->entries[total].address, ap->address, 6);
                ap++;
                total++;
            }
        }
        out->count = result;
        if (ov13_0224DF30.mode != 1 && ov13_02224164(out, previous, &selected)) {
            Entry *entry = &out->entries[selected];
            ov13_0224DF30.selection = selected;
            strcpy(ov13_0224E440, entry->name);
            memcpy(ov13_0224E3E0, entry->address, 6);
            ov13_02226D40(text, ov13_0224E3E0);
            break;
        }
        memcpy(previous, out, size);
        ov13_0224DF30.mode = 2;
        ov13_02226F3C();
    }
    if (attempt >= 30 || ov13_02226CBC() > ov13_02245A58.limit) {
        result = -3;
    } else if (ov13_0224DF30.cancelled) {
        result = -8;
    } else {
        result = 1;
    }
done:
    if (out != NULL) {
        ov13_02226CFC(out);
    }
    if (previous != NULL) {
        ov13_02226CFC(previous);
    }
    return result;
}
#pragma optimization_level reset
