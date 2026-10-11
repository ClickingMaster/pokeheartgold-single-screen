#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 bytes[0x834];
} State;

typedef struct {
    u8 p[12];
    State *state;
} Socket;

typedef struct {
    u8 init;
    u8 p[3];
    u32 count;
} RandomState;

typedef struct {
    u64 state, mul, add;
} Generator;

extern RandomState _021D5E60;
extern Generator _021D4688;
extern u8 _021D5FD8[20];
extern u8 _021D5FEB[];
extern u8 *(*gCpsAlloc)(u32);
extern void (*gCpsFree)(void *);
extern u8 *CPSi_TcpReadRaw(u32 *, Socket *);
extern void CPSi_SocConsumeRaw(u32, Socket *);
extern void client_hello_v2(State *, void *);
extern void update_digest(State *, void *, u32);
extern void parse_record_in_buf(State *, u8 *);
extern void CPS_SslAddRandomSeed(void *, int);
extern void CPSi_sha1_init(void *);
extern void CPSi_sha1_calc(void *, void *, int);
extern void CPSi_sha1_result_prng(void *, void *);
extern int tcp_read_raw_nbytes(void *, u32, Socket *);
int parse_record(Socket *);
void set_random(void *, int);
#pragma force_active on

void set_random(void *dest, int count) {
    u8 sha[92], digest[20];
    u32 seed;
    int written, index;
    if (!_021D5E60.init) {
        _021D4688.state = _021D4688.mul * _021D4688.state + _021D4688.add;
        seed = _021D4688.state >> 32;
        CPS_SslAddRandomSeed(&seed, 4);
    }
    for (written = 0, index = 20; written < count; index++) {
        if (index == 20) {
            OSIntrMode irq;
            int i;
            u32 carry;
            CPSi_sha1_init(sha);
            irq = OS_DisableInterrupts();
            CPSi_sha1_calc(sha, _021D5FD8, 20);
            CPSi_sha1_result_prng(sha, digest);
            carry = 1;
            {
                u8 *p = _021D5FEB, *d;
                u32 sum = 0;
                i = 19;
                d = digest + 19;
                do {
                    sum = *p + *d-- + carry;
                    *p-- = sum;
                    carry = sum >> 8;
                } while (--i >= 0);
                seed = sum;
            }
            OS_RestoreInterrupts(irq);
            index = 0;
        }
        if (digest[index]) {
            ((u8 *)dest)[written++] = digest[index];
        }
    }
}
