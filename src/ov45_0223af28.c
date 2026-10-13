#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 state, pad[3];
    s64 start;
    u8 q[0x20];
    u32 index, count;
    u32 **chunks;
    u32 base, pad2, mod;
} W;

typedef struct {
    u8 p[0x30];
    void (*cb)(u32);
    u8 q[0x84];
    u32 text[3];
} Owner;

extern int ov45_0223B104(W *);
extern Owner *PPWLobby_GetWork(void);
extern void ov45_022320B4(void *, void *);
extern void LobbyChat_SetChannelMode(char *, void *);
extern void LobbyString_Destroy(void *);
extern u8 ov45_02254D9C[];
extern void PPWLobby_SendMessage(int, int, int, int, int, void *, int);

typedef struct {
    u8 v;
} Byte;

extern void ov45_0223B1C8(W *);
extern void ov45_0223B81C(u32 *, int, Byte);
extern void DWC_GetDateTime(RTCDate *, RTCTime *);

typedef union {
    struct {
        u32 flag : 1, rest : 31;
        u32 pad;
        char *ptr;
    } s;

    u8 bytes[12];
} Text;

void ov45_0223AF28(W *);

void ov45_0223AF28(W *w) {
    Text text;
    RTCTime time;
    RTCDate date;
    u32 zero;
    u32 *entry;
    s64 now;
    switch (w->state) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        if (!ov45_0223B104(w)) {
            return;
        }
        ov45_022320B4(&text, PPWLobby_GetWork()->text);
        LobbyChat_SetChannelMode(text.s.flag == 0 ? (char *)&text.bytes[1] : text.s.ptr, ov45_02254D9C);
        LobbyString_Destroy(&text);
        zero = 0;
        PPWLobby_SendMessage(-1, 0, 1, 0, 0, &zero, 4);
        ov45_0223B1C8(w);
        return;
    case 5:
        if (!w->count) {
            return;
        }
        entry = w->chunks[(w->base + (w->index >> 7)) % w->mod] + (w->index & 127) * 2;
        DWC_GetDateTime(&date, &time);
        now = RTC_ConvertDateTimeToSecond(&date, &time);
        if ((s32)entry[0] > now - w->start) {
            return;
        }
        do {
            Byte byte;
            PPWLobby_GetWork()->cb(entry[1]);
            memset(&byte, 0, sizeof byte);
            ov45_0223B81C(&w->index, 1, byte);
            if (!w->count) {
                return;
            }
            {
                u32 *next = w->chunks[(w->base + (w->index >> 7)) % w->mod] + (w->index & 127) * 2;
                entry[0] = next[0];
                entry[1] = next[1];
            }
        } while ((s32)entry[0] <= now - w->start);
        break;
    }
}
