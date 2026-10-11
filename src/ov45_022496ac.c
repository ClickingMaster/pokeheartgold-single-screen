#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_02255EB4[];
extern char ov45_022565E0[];
extern char ov45_022565F0[];
extern char ov45_022565F8[];
void __msl_assertion_failed(const char *cond, const char *file, const char *func, int line);
int strnicmp(const char *a, const char *b);
u32 ov00_021FF368(void);

typedef struct {
    int type;
    const char *s1;
    const char *s2;
} QueryItem;

typedef struct Entry {
    int type;
    u32 expire;
    const char *s1;
    const char *s2;
    u8 pad[0x14];
    struct Entry *next;
} Entry;

typedef struct {
    u8 pad[0x814];
    Entry *head;
} Ctx;

#define ASSERT_AT(cond, str, line) ((cond) ? (void)0 : __msl_assertion_failed(str, ov45_022565D0, ov45_02255EB4, line))
Entry *ov45_022496AC(Ctx *ctx, int count, QueryItem *items);

Entry *ov45_022496AC(Ctx *ctx, int count, QueryItem *items) {
    int i;
    const char *s1;
    const char *s2;
    Entry *e;
    ASSERT_AT(ctx != NULL, ov45_022565C0, 0x15B);
    ASSERT_AT(count > 0, ov45_022565E0, 0x15D);
    ASSERT_AT(items != NULL, ov45_022565F0, 0x15E);
    for (e = ctx->head; e != NULL; e = e->next) {
        for (i = 0; i < count; i++) {
            QueryItem *q = &items[i];
            ASSERT_AT(q->type >= 0 && q->type < 17, ov45_022565F8, 0x164);
            if (e->type != q->type) {
                continue;
            }
            s1 = q->s1;
            s2 = q->s2;
            if (!((s1 == NULL && e->s1 == NULL) || (s1 != NULL && e->s1 != NULL && strnicmp(s1, e->s1) == 0))) {
                continue;
            }
            if (!((s2 == NULL && e->s2 == NULL) || (s2 != NULL && e->s2 != NULL && strnicmp(s2, e->s2) == 0))) {
                continue;
            }
            e->expire = ov00_021FF368() + 60000;
            return e;
        }
    }
    return NULL;
}
