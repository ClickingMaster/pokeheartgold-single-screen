#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 _02108FB8[];
typedef void *(*Allocate)(int);
typedef void (*Release)(void *);

typedef struct {
    Allocate allocate;
    u32 reserved;
} AllocatorSlot;

typedef struct {
    Release release;
    u32 reserved[3];
} ReleaseSlot;

extern AllocatorSlot gCpsAlloc;
extern ReleaseSlot gCpsFree;
extern void CPSi_TcpWrite2Raw(void *, int, int, int, void *);
extern void set_random(void *, int);
extern void CPSi_big_from_char(u16 *, void *, int, int);
extern int enter_computebound(void);
extern void exit_computebound(int);
extern void CPSi_big_power(u16 *, u16 *, u16 *, int, u16 *);
extern void update_digest(void *, void *, int);
void send_client_key_exchange(void *);

typedef struct {
    u8 major, minor, random[46];
} Secret;

typedef struct {
    Secret secret;
    u8 gap30[0x425];
    u8 error, gap456[0x3e], modulus[256];
    int bytes;
    u8 exponent[8];
    int exponentBytes;
    u8 gap5a4[8], sendHeader;
} Session;

typedef struct {
    u8 reserved[12];
    Session *session;
} Connection;

typedef struct {
    u8 type, major, minor, lengthHigh, lengthLow, handshake, sizeHigh, sizeMiddle, sizeLow;
} RecordHeader;

void send_client_key_exchange(void *ctx) {
    Connection *connection = ctx;
    Session *s = connection->session;
    u16 *work;
    u16 *modulus;
    u8 *p;
    int old;
    u8 *out;
    int i;
    int limbs;
    u8 *encoded;
    RecordHeader *header;
    u16 *plain;
    int bytes;
    u16 *exponent;
    if (s->sendHeader) {
        CPSi_TcpWrite2Raw(_02108FB8, 7, 0, 0, ctx);
    }
    s->secret.major = 3;
    s->secret.minor = 0;
    set_random(s->secret.random, 46);
    bytes = s->bytes;
    limbs = (s32)((u32)bytes * 2) / 2;
    encoded = gCpsAlloc.allocate(bytes);
    if (!encoded) {
        s->error = 9;
        return;
    }
    encoded[0] = 0;
    encoded[1] = 2;
    set_random(encoded + 2, (s32)((u32)bytes - 51));
    encoded[(s32)((u32)bytes - 49)] = 0;
    MI_CpuCopy8(&s->secret, encoded + bytes - 48, 48);
    work = gCpsAlloc.allocate((s32)((u32)limbs << 3));
    if (!work) {
        gCpsFree.release(encoded);
        s->error = 9;
        return;
    }
    plain = work + limbs;
    exponent = plain + limbs;
    modulus = exponent + limbs;
    CPSi_big_from_char(plain, encoded, bytes, limbs);
    CPSi_big_from_char(exponent, s->exponent, s->exponentBytes, limbs);
    CPSi_big_from_char(modulus, s->modulus, bytes, limbs);
    old = enter_computebound();
    CPSi_big_power(work, plain, exponent, limbs, modulus);
    exit_computebound(old);
    out = gCpsAlloc.allocate((s32)((u32)bytes + 73));
    if (!out) {
        gCpsFree.release(encoded);
        gCpsFree.release(work);
        s->error = 9;
        return;
    }
    header = (RecordHeader *)out;
    header->type = 22;
    header->major = 3;
    header->minor = 0;
    header->lengthHigh = (bytes + 4) >> 8;
    header->lengthLow = bytes + 4;
    header->handshake = 16;
    header->sizeHigh = bytes >> 16;
    header->sizeMiddle = bytes >> 8;
    header->sizeLow = bytes;
    p = out + 9;
    if (bytes & 1) {
        out[9] = work[bytes / 2];
        p = out + 10;
    }
    for (i = bytes / 2 - 1; i >= 0; i--) {
        p[0] = work[i] >> 8;
        p[1] = work[i];
        p += 2;
    }
    CPSi_TcpWrite2Raw(out, (s32)((u32)bytes + 9), 0, 0, ctx);
    update_digest(s, out + 5, (s32)((u32)bytes + 4));
    gCpsFree.release(out);
    gCpsFree.release(work);
    gCpsFree.release(encoded);
}
