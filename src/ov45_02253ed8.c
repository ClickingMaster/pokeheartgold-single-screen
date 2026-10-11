#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    char c[17];
} HexDigits;

typedef struct {
    u8 b[20];
} Salt;

typedef struct {
    u8 pad[0x68];
    const char *str;
    int len;
} AuthState;

extern HexDigits ov45_02254DF4;
extern Salt ov45_0225765C;
extern AuthState ov45_0225762C;
int ov45_02253D50(int len);
void *ov00_021EC2A8(int heap, int size);
void ov00_021EC2EC(int heap, void *ptr, int unk);
void ov00_021FFC18(const char *src, void *dst, int len, int mode);
int strncmp(const char *a, const char *b, size_t n);
BOOL ov45_02253ED8(const char *str, int len);

BOOL ov45_02253ED8(const char *str, int len) {
    char hexOut[41];
    u8 digest[20];
    HexDigits hex;
    u32 bodyLen;
    u8 *buf;
    int i;
    hex = ov45_02254DF4;
    if (len <= 40) {
        return FALSE;
    }
    bodyLen = len - 40;
    buf = ov00_021EC2A8(7, ov45_02253D50(bodyLen) + 41);
    if (buf == NULL) {
        return FALSE;
    }
    *(Salt *)buf = ov45_0225765C;
    ov00_021FFC18(str, buf + 20, bodyLen, 2);
    {
        int n = ov45_02253D50(bodyLen);
        u8 *p = buf + 20;
        p += n;
        *(Salt *)p = ov45_0225765C;
    }
    MATH_CalcSHA1(digest, buf, ov45_02253D50(bodyLen) + 40);
    ov00_021EC2EC(7, buf, 0);
    for (i = 0; i < 20; i++) {
        hexOut[i * 2] = hex.c[digest[i] >> 4];
        hexOut[i * 2 + 1] = hex.c[digest[i] & 0xf];
    }
    hexOut[40] = 0;
    if (strncmp(str + (len - 40), hexOut, 40) != 0) {
        return FALSE;
    }
    ov45_0225762C.str = str;
    ov45_0225762C.len = bodyLen;
    return TRUE;
}
