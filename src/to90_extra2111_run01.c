#pragma force_active on
#include "global.h"

#include "to90_login_callback_private.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 p0[0x10];
    void *text;
    u8 p1[8];
    void *account;
    u32 p20;
    void *alloc;
    u64 time;
    u8 p30[0x48 - 0x30];
    char user[256], pass[256];
    u8 p248[9];
    char realm[1];
} Work;

typedef struct {
    u32 arg;
    Work *w;
    void (*callback)(char *, char *, u32);
} Control;

typedef struct {
    u32 error;
    u8 p[0x4a - 4];
    char user[0x177 - 0x4a], pass[0x1c4 - 0x177];
} Result;

typedef struct {
    u8 text[0x34];
    char realm[12];
    void (*alloc)(void);
    void (*free)(void);
} Request;

extern Control ov00_0221A90C;
extern int ov00_021E7E5C(void);
extern int DWCi_Acc_IsAuthentic(void *);
extern void ov00_021E7EA8(Result *);
extern void ov00_021E7DD4(void);
extern void ov00_021EC2EC(u32, void *, u32);
extern void ov00_021EF508(GPConnection *, void *, void *);
extern void ov00_021EF658(const char *, const char *, GPCallback, int);
extern void ov00_021EF3C0(int, u32);
extern void ov00_021EC2A8(void);
extern void ov00_021E7B70(Request *, void *);
extern u32 ov00_021ED704(void *);
void ov00_021EF8C0(void);

void ov00_021EF8C0(void) {
    Result success, failure;
    Request request;
    if (ov00_021E7E5C() == 21) {
        ov00_021E7EA8(&success);
        strcpy(ov00_0221A90C.w->user, success.user);
        strcpy(ov00_0221A90C.w->pass, success.pass);
        ov00_021E7DD4();
        ov00_021EC2EC(0, ov00_0221A90C.w->alloc, 0);
        ov00_0221A90C.w->alloc = 0;
        if (DWCi_Acc_IsAuthentic(ov00_0221A90C.w->account)) {
            ov00_0221A90C.callback(ov00_0221A90C.w->user, ov00_0221A90C.w->pass, ov00_0221A90C.arg);
            return;
        }
        ov00_021EF658(ov00_0221A90C.w->user, ov00_0221A90C.w->pass, ov00_021EF508, 3);
        return;
    }
    if (ov00_021E7E5C() == 0) {
        return;
    }
    if (((OS_GetTick() - ov00_0221A90C.w->time) << 6) / 33514 > 10000) {
        ov00_021E7EA8(&failure);
        ov00_021E7DD4();
        ov00_021EC2EC(0, ov00_0221A90C.w->alloc, 0);
        ov00_0221A90C.w->alloc = 0;
        ov00_021EF3C0(2, failure.error);
        return;
    }
    ov00_021E7DD4();
    MI_CpuFill8(&request, 0, sizeof(request));
    MI_CpuCopy8(ov00_0221A90C.w->text, request.text, ov00_021ED704(ov00_0221A90C.w->text) * 2 + 2);
    strcpy(request.realm, ov00_0221A90C.w->realm);
    request.alloc = ov00_021EC2A8;
    request.free = (void (*)(void))ov00_021EC2EC;
    ov00_021E7B70(&request, ov00_0221A90C.w->alloc);
}
