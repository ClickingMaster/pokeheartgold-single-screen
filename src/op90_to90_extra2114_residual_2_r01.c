#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 pad0[0x1008];
    int error;
    char code[4];
    char f1010[0xf];
    char f101f[0x33];
    char f1052[0x12d];
    char f117f[9];
    char f1188[0x2b];
    u8 f11b3;
    u8 pad11b4[0x1314 - 0x11b4];
    void *handle;
} AuthWork;
typedef struct {
    int state;
    int mode;
    AuthWork *work;
    u64 userId;
} AuthState;
extern AuthState ov00_0221A694;
extern int errno;
extern const char ov00_022164C0[];
extern const char ov00_022164C8[];
extern const char ov00_022164D4[];
extern const char ov00_022164E0[];
extern const char ov00_022164E8[];
extern const char ov00_022164F0[];
extern const char ov00_022164FC[];
extern const char ov00_02216508[];
extern const char ov00_02216514[];
extern const char ov00_02216520[];
extern const char ov00_02216528[];
extern const char ov00_02216530[];
extern const char ov00_02216538[];
extern const char ov00_0221653C[];
extern const char ov00_02216544[];
extern u8 ov00_0221A6A0[];
extern u32 ov00_0221A6BC;
char *ov00_021EA1CC(void *handle, const char *key);
void ov00_021EB5B0(char *value);
int ov00_021EA224(void *handle, const char *key, char *buf, int size);
int ov00_021EA278(void *handle, const char *key, char *buf, int size);
long atol(const char *s);
long strtol(const char *s, char **end, int base);
int ov00_021E99A4(void *req, const char *key, const void *value, u32 len);
BOOL ov00_021E89AC(u8 *info, u8 *src);
BOOL ov00_021E8C5C(void *req, u8 *info, u32 arg);
u32 wcslen(const u16 *s);
int ov00_021E84D4(void);
int ov00_021E879C(void *req, const char *str, const u16 *wstr);
// @fn ov00_021E84D4
int ov00_021E84D4(void) {
    char *end = NULL;
    long code;
    long value;
    AuthWork *work;
    end = ov00_021EA1CC(ov00_0221A694.work->handle, ov00_022164C0);
    if (end != NULL) {
        ov00_021EB5B0(end);
    }
    end = NULL;
    errno = 0;
    code = atol(ov00_021EA1CC(ov00_0221A694.work->handle, ov00_022164C8));
    if (errno == 0x22) {
        ov00_0221A694.work->error = 20101;
        return 12;
    }
    if (code != 200) {
        ov00_0221A694.work->error = code + 23000;
        return 18;
    }
    if (ov00_021EA224(ov00_0221A694.work->handle, ov00_022164D4, ov00_0221A694.work->code, 4) <= 0) {
        ov00_0221A694.work->error = 20101;
        return 14;
    }
    value = strtol(ov00_0221A694.work->code, &end, 10);
    work = ov00_0221A694.work;
    if (end != work->code + strlen(work->code)) {
        work->error = 20101;
        return 12;
    }
    work->error = value + 20000;
    if (value < 100) {
        ov00_0221A694.work->f1052[0] = 0;
        ov00_0221A694.work->f101f[0] = 0;
        ov00_0221A694.work->f117f[0] = 0;
        ov00_0221A694.work->f1010[0] = 0;
        ov00_0221A694.work->f1188[0] = 0;
        ov00_021EA224(ov00_0221A694.work->handle, ov00_022164E0, ov00_0221A694.work->f1052, 0x12d);
        ov00_021EA224(ov00_0221A694.work->handle, ov00_022164E8, ov00_0221A694.work->f101f, 0x33);
        ov00_021EA224(ov00_0221A694.work->handle, ov00_022164F0, ov00_0221A694.work->f117f, 9);
        ov00_021EA224(ov00_0221A694.work->handle, ov00_022164FC, ov00_0221A694.work->f1010, 0xf);
        ov00_021EA278(ov00_0221A694.work->handle, ov00_02216508, ov00_0221A694.work->f1188, 0x41);
        ov00_0221A694.work->f11b3 = 0;
        if (value != 40) {
            ov00_0221A694.state = 1;
        } else {
            ov00_0221A694.state = 2;
        }
    }
    return 0;
}

// @fn ov00_021E879C
u32 wcslen(const u16 *s);
int ov00_021E879C(void *req, const char *str, const u16 *wstr) {
    u8 info[0x94];
    if (ov00_0221A694.userId == 0) {
        if (ov00_021E99A4(req, ov00_02216520, ov00_02216514, strlen(ov00_02216514)) != 0) {
            return 8;
        }
    } else {
        if (ov00_021E99A4(req, ov00_02216520, ov00_02216528, strlen(ov00_02216528)) != 0) {
            return 8;
        }
        if (ov00_021E99A4(req, ov00_02216530, str, strlen(str)) != 0) {
            return 8;
        }
    }
    if (!ov00_021E89AC(info, ov00_0221A6A0)) {
        return 5;
    }
    if (!ov00_021E8C5C(req, info, ov00_0221A6BC)) {
        return 8;
    }
    if (ov00_0221A694.mode == 1) {
        if (ov00_021E99A4(req, ov00_0221653C, ov00_02216538, strlen(ov00_02216538)) != 0) {
            return 8;
        }
    }
    if (wcslen(wstr) != 0) {
        if (ov00_021E99A4(req, ov00_02216544, wstr, wcslen(wstr) * 2) != 0) {
            return 8;
        }
    }
    return 0;
}
