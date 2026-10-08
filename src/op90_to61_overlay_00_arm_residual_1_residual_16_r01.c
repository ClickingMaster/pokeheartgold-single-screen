#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 pad0[0x24];
    int state;
    u8 pad28[0x389 - 0x28];
    volatile u8 subState;
} DwcState;
typedef struct {
    void *from;
    u32 unk4;
    char *text;
} Message;
extern DwcState *ov00_0221A784;
extern const char ov00_022179F8[];
extern const char ov00_02217A00[];
int ov00_021F02DC(void *ctx, Message *msg, int arg);
char *strchr(const char *s, int c);
unsigned long strtoul(const char *s, char **end, int base);
void ov00_021F2E40(void *ctx, void *from, const char *text);
void ov00_021EECCC(void *ctx, Message *msg, int arg);
// @fn ov00_021EECCC
void ov00_021EECCC(void *ctx, Message *msg, int arg) {
    char version[12] = {0};
    char *text = msg->text;
    u32 len;
    if (ov00_021F02DC(ctx, msg, arg) != 0) {
        return;
    }
    if (memcmp(text, ov00_022179F8, strlen(ov00_022179F8)) != 0) {
        return;
    }
    text += strlen(ov00_022179F8);
    len = strchr(text, 'v') - text;
    strncpy(version, text, len);
    if (len > 10) {
        return;
    }
    if (strtoul(version, NULL, 10) != 3) {
        return;
    }
    text += len + 1;
    if (memcmp(text, ov00_02217A00, strlen(ov00_02217A00)) != 0) {
        return;
    }
    switch (ov00_0221A784->state) {
    case 5:
        break;
    case 6:
        if (ov00_0221A784->subState == 2 || ov00_0221A784->subState == 3) {
            break;
        }
    default:
        return;
    }
    ov00_021F2E40(ctx, msg->from, text + strlen(ov00_02217A00));
}
