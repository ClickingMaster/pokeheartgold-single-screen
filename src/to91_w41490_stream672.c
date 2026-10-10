#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
extern char ov45_02257334[];
extern char ov45_02257344[];
extern char ov45_022573C4[];
extern char ov45_022573E4[];
extern char ov45_02257408[];
extern char ov45_02257438[];
extern char ov45_0225745C[];
extern char ov45_0225749C[];
extern char ov45_022574BC[];
extern char ov45_022574E0[];
extern char ov45_02257514[];
extern char ov45_02257538[];
extern char ov45_022575D8[];
extern char ov45_02257258[];
extern char ov45_02257220[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02252530(void *, int *, int *, int *);
extern void ov45_022495DC(u8 *, int, u8 *);
extern int ov45_02251C50(void *, int);
extern int ov00_021FF92C(int, void *, int, int);

typedef struct {
    u8 *data;
    s32 used, capacity;
} W40897_Buffer;

typedef struct {
    int socket;
    u32 state;
    u8 reserved[0x100];
    W40897_Buffer input, output;
    u32 cipherEnabled;
    u8 receiveCipher[258], sendCipher[258], tail[40];
} W40897_Stream;

#include <stddef.h>
typedef char W40897BufferPosition[(offsetof(W40897_Stream, input) == 0x108) ? 1 : -1];
typedef char W40897OutputPosition[(offsetof(W40897_Stream, output) == 0x114) ? 1 : -1];
typedef char StreamExtent[sizeof(W40897_Stream) == 0x350 ? 1 : -1];
void ov45_02252800(void *);

void ov45_02252800(void *parent) {
    W40897_Stream *s = parent;
    int ready, n, count;
    u8 *ptr;
    if (!s) {
        __msl_assertion_failed(ov45_02257334, ov45_02257258, ov45_02257220, 378);
    }
    if (s->state > 2) {
        __msl_assertion_failed(ov45_02257344, ov45_02257258, ov45_02257220, 378);
    }
    if (!((u32)s + 0x108)) {
        __msl_assertion_failed(ov45_022573C4, ov45_02257258, ov45_02257220, 378);
    }
    if ((s32)s->input.capacity < 0) {
        __msl_assertion_failed(ov45_022573E4, ov45_02257258, ov45_02257220, 378);
    }
    if ((s32)s->input.capacity % 8192 != 0) {
        __msl_assertion_failed(ov45_02257408, ov45_02257258, ov45_02257220, 378);
    }
    if ((s32)s->input.used < 0) {
        __msl_assertion_failed(ov45_02257438, ov45_02257258, ov45_02257220, 378);
    }
    if ((s32)s->input.used > (s32)s->input.capacity) {
        __msl_assertion_failed(ov45_0225745C, ov45_02257258, ov45_02257220, 378);
    }
    if (!((u32)s + 0x114)) {
        __msl_assertion_failed(ov45_0225749C, ov45_02257258, ov45_02257220, 378);
    }
    if ((s32)s->output.capacity < 0) {
        __msl_assertion_failed(ov45_022574BC, ov45_02257258, ov45_02257220, 378);
    }
    if ((s32)s->output.capacity % 8192 != 0) {
        __msl_assertion_failed(ov45_022574E0, ov45_02257258, ov45_02257220, 378);
    }
    if ((s32)s->output.used < 0) {
        __msl_assertion_failed(ov45_02257514, ov45_02257258, ov45_02257220, 378);
    }
    if ((s32)s->output.used > (s32)s->output.capacity) {
        __msl_assertion_failed(ov45_02257538, ov45_02257258, ov45_02257220, 378);
    }
    if (s->state != 1) {
        __msl_assertion_failed(ov45_022575D8, ov45_02257258, ov45_02257220, 379);
    }
    for (;;) {
        ov45_02252530((void *)s->socket, &ready, 0, 0);
        if (!ready) {
            return;
        }
        if (!ov45_02251C50((u8 *)s + 0x108, 4096)) {
            return;
        }
        ptr = s->input.data;
        count = s->input.used;
        n = ov00_021FF92C(s->socket, ptr + count, 4096, 0);
        if (n <= 0) {
            s->state = 2;
            return;
        }
        if (s->cipherEnabled) {
            ov45_022495DC(ptr + count, n, s->receiveCipher);
        }
        s->input.used += n;
        s->input.data[s->input.used] = 0;
    }
}
