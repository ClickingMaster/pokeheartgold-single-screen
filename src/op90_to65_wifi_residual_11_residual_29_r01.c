#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 x, y, s[256];
} RC4;

typedef struct Session {
    u8 master[48], resumed, accepted;
    u16 cipher;
    u8 clientRandom[32], serverRandom[32], keyBlock[72];
    u8 *clientMac, *clientKey;
    u8 padc4[4];
    RC4 clientRc4;
    u8 pad1ca[2], clientSequence[8];
    u8 *serverMac, *serverKey;
    u8 pad1dc[4];
    RC4 serverRc4;
    u8 pad2e2[2], serverSequence[8];
    u32 handshakeSha[23], scratchSha[23], handshakeMd5[22], scratchMd5[22];
    u8 server;
    u8 rest[0x834 - 0x455];
} Session;

typedef struct {
    u8 prefix;
    u8 digest[20];
} DigestStage;

typedef struct {
    int key, hash;
    u8 *out;
} KeyStage;

extern void CPSi_sha1_init(u32 *);
extern void CPSi_sha1_calc(u8 *, u8 *, u32);
extern void CPSi_sha1_result(u8 *, u8 *);
extern void CPSi_md5_init(u32 *);
extern void CPSi_md5_calc(u8 *, u8 *, u32);
extern void CPSi_md5_result(u8 *, u8 *);
extern void CPSi_rc4_init(RC4 *, const u8 *, int);
void create_key_block(Session *);
typedef char CheckSession[(sizeof(Session) == 0x834) ? 1 : -1];
typedef char CheckRc4[(sizeof(RC4) == 258) ? 1 : -1];
typedef char CheckDigest[(sizeof(DigestStage) == 21) ? 1 : -1];
void create_key_block(Session *);
#pragma opt_arithtransformation off
#pragma opt_propagation off

void create_key_block(Session *s) {
    struct {
        int key, hash;
    } len;

    u8 prefix[21];
    int iv;
    int n;
    int i;
    u8 *out;
    u8 *base;
    int j;
    int off;
    u8 *sha;
    switch (s->cipher) {
    case 4:
        len.hash = 16;
        len.key = 16;
        iv = 0;
        break;
    case 5:
        len.hash = 20;
        len.key = 16;
        iv = 0;
        break;
    }
    int total = (len.hash + len.key + iv) * 2;
    i = 0;
    if (total > 0) {
        out = s->keyBlock;
        sha = (u8 *)s->scratchSha;
        off = 0;
        do {
            CPSi_sha1_init((u32 *)sha);
            prefix[0] = 'A' + i;
            j = 0;
            n = i + 1;
            if (n > 0) {
                do {
                    CPSi_sha1_calc(sha, prefix, 1);
                    j++;
                } while (j < n);
            }
            CPSi_sha1_calc(sha, s->master, 48);
            CPSi_sha1_calc(sha, s->serverRandom, 32);
            CPSi_sha1_calc(sha, s->clientRandom, 32);
            CPSi_sha1_result(sha, prefix + 1);
            CPSi_md5_init(s->scratchMd5);
            CPSi_md5_calc((u8 *)s->scratchMd5, s->master, 48);
            CPSi_md5_calc((u8 *)s->scratchMd5, prefix + 1, 20);
            CPSi_md5_result((u8 *)s->scratchMd5, out + off);
            off += 16;
            i++;
        } while (off < total);
    }
    base = s->keyBlock;
    if (s->server) {
        s->serverMac = base;
        s->serverKey = base + len.hash * 2;
        s->clientMac = base + len.hash;
        s->clientKey = base + len.hash + len.hash + len.key;
    } else {
        s->clientMac = base;
        s->clientKey = base + len.hash * 2;
        s->serverMac = base + len.hash;
        s->serverKey = base + len.hash + len.hash + len.key;
    }
    CPSi_rc4_init(&s->serverRc4, s->serverKey, 16);
    CPSi_rc4_init(&s->clientRc4, s->clientKey, 16);
}

#pragma opt_arithtransformation reset
