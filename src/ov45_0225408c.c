#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma inlineintrinsics on
#include <string.h>

typedef struct State {
    int state;
    u8 p4[4];
    int result;
    u8 pc[0x4c];
    char *digest;
    u8 p5c[4];
    char *buffer;
    int length;
    u8 p68[8];
    int (*callback)(char *, int);
} State;

extern State ov45_0225762C;
extern const char ov45_02254E05[17];
extern const char ov45_0225765C[20];
extern const char ov45_022576CC[];
extern const char ov45_022576A0[];
extern const char ov45_022576D4[];
extern int strncmp(const char *, const char *, size_t);
extern void ov45_022545BC(void);
extern void ov00_021EC3D8(void *);
extern int ov45_02253ED8(char *, int);
extern void MATH_CalcSHA1(void *, const void *, u32);
void ov45_0225408C(char *, int, int);

void ov45_0225408C(char *buffer, int length, int error) {
    ov45_0225762C.result = -1;
    if (ov45_0225762C.state == 1) {
        return;
    }
    if (error == 0) {
        switch (ov45_0225762C.state) {
        case 5:
            if (length == 32) {
                struct {
                    char prefix[20];
                    char input[32];
                } block;

                char hex[17];
                u8 *digest = (u8 *)ov45_0225762C.digest + 20;
                int i, j;
                memcpy(hex, ov45_02254E05, 17);
                memcpy(block.prefix, ov45_0225765C, 20);
                memcpy(block.input, buffer, 32);
                MATH_CalcSHA1(digest, &block, 52);
                for (i = 0, j = 0; i < 20; i++, j += 2) {
                    ov45_0225762C.digest[j] = hex[digest[i] >> 4];
                    ov45_0225762C.digest[j + 1] = hex[digest[i] & 15];
                }
                *(ov45_0225762C.digest - strlen(ov45_022576CC)) = '&';
                strncpy((char *)ov45_022576A0, ov45_0225762C.digest, 41);
                ov45_0225762C.state = 6;
            } else {
                ov45_022545BC();
            }
            ov00_021EC3D8(buffer);
            break;
        case 7:
            if (strncmp(buffer, ov45_022576D4, strlen(ov45_022576D4)) == 0) {
                ov00_021EC3D8(buffer);
                ov45_022545BC();
                return;
            }
            if (ov45_0225762C.buffer) {
                ov00_021EC3D8(ov45_0225762C.buffer);
                ov45_0225762C.buffer = NULL;
            }
            if (!ov45_02253ED8(buffer, length)) {
                ov00_021EC3D8(buffer);
                ov45_022545BC();
                return;
            }
            if (ov45_0225762C.callback && !ov45_0225762C.callback(buffer, length)) {
                ov00_021EC3D8(buffer);
                ov45_022545BC();
                return;
            }
            ov45_0225762C.buffer = buffer;
            ov45_0225762C.length = length;
            ov45_0225762C.state = 8;
            break;
        }
    } else {
        ov45_022545BC();
    }
}
