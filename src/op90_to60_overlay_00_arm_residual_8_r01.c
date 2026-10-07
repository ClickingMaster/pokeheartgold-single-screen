#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

extern const char ov00_02219868[];
extern const char ov00_02219778[];
extern const char ov00_02219754[];
extern const char ov00_022197C8[];
extern const char ov00_022197D4[];
extern const char ov00_02219878[];
void __msl_assertion_failed(const char *expr, const char *file, const char *func, int line);
void ov00_0220A86C(void *conn, int type, const char *msg);
void ov00_02202990(void *conn, int a, int b);
int ov00_0220A69C(void *conn, const char *buf, int *pos, char *key, char *value);

#define PARSE_ERROR(conn)                          \
    {                                              \
        ov00_0220A86C(conn, 1, ov00_02219878);     \
        ov00_02202990(conn, 3, 1);                 \
        return 3;                                  \
    }
// @fn ov00_0220A69C
int ov00_0220A69C(void *conn, const char *buf, int *pos, char *key, char *value) {
    const char *start;
    char c;
    int i;
    int j;
    if (buf == NULL) {
        __msl_assertion_failed(ov00_02219868, ov00_02219778, ov00_02219754, 0x199);
    }
    if (key == NULL) {
        __msl_assertion_failed(ov00_022197C8, ov00_02219778, ov00_02219754, 0x19a);
    }
    if (value == NULL) {
        __msl_assertion_failed(ov00_022197D4, ov00_02219778, ov00_02219754, 0x19b);
    }
    if (buf[*pos] != '\\') {
        PARSE_ERROR(conn);
    }
    start = buf + *pos;
    buf = start + 1;
    c = *buf++;
    i = 0;
    while (c != '\\') {
        if (c == 0) {
            PARSE_ERROR(conn);
        }
        if (i == 0x1ff) {
            PARSE_ERROR(conn);
        }
        *key++ = c;
        c = *buf++;
        i++;
    }
    *key = 0;
    j = 0;
    while ((c = *buf++) != '\\' && c != 0) {
        if (j == 0x1ff) {
            PARSE_ERROR(conn);
        }
        *value++ = c;
        j++;
    }
    *value = 0;
    *pos += (buf - start) - 1;
    return 0;
}
