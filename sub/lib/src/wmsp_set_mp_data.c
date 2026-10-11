#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 wmspW[];
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void __VENEER_MI_CpuFill8(void *, int, u32);
extern void __VENEER_MIi_CpuClear16(int, void *, u32);
extern int WMSP_PutSendQueue(u16, u16, u16, u32, void *, u16, u32, u32);

typedef struct Request40789 {
    u32 kind;
    void *data;
    u32 size, dest, port, priority, a, b;
} Request40789;

typedef struct Callback40789 {
    u16 kind, error, reserved0[2], api, port, dest, failed, retries, reserved1;
    void *data;
    u16 size, index;
    u32 a, b;
    u16 first, second;
} Callback40789;

void WMSP_SetMPData(void *);

void WMSP_SetMPData(void *raw) { /*D*/
    void *data;
    u16 size;
    u32 dest;
    u32 a;
    u32 b;
    u32 err;
    u16 connected;
    u16 port;
    u16 priority;
    u8 *s;
    Request40789 *r = raw; /*E*/
    s = P(wmspW + 0x1000, 0x550);
    err = H(s, 0x188);
    connected = H(s, 0x182);
    size = r->size;
    port = r->port;
    priority = r->priority;
    data = r->data;
    dest = r->dest;
    a = r->a;
    b = r->b;
    if (err) {
        dest = 1;
    }
    if (!W(s, 12)) {
        err = 3;
    } else if (!(dest & connected)) {
        err = 0;
    } else {
        err = WMSP_PutSendQueue(connected, priority, port, dest, data, size, a, b);
    }
    if (err != 2) {
        Callback40789 *p = (Callback40789 *)__VENEER_WMSP_GetBuffer4Callback2Wm9();
        p->kind = 129;
        p->error = err;
        p->api = 20;
        p->port = port;
        p->dest = dest;
        p->failed = err == 10 ? (dest & connected) : 0;
        p->retries = 0;
        p->size = size;
        p->data = data;
        p->a = a;
        p->b = b;
        p->index = 0xffff;
        {
            u16 first = H(s, 0x30), second = H(s, 0x32);
            p->first = H(s, 0x188) == 0 ? first : second;
            if (H(s, 0x188) != 0) {
                second = first;
            }
            p->second = second;
        }
        __VENEER_WMSP_ReturnResult2Wm9(p);
    }
}
