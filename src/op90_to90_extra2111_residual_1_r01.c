#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u32 w[3];
} LoginId;
typedef struct {
    u32 unk0;
    LoginId consoleId;
    LoginId loginId;
    u8 pad1c[8];
    u32 userId;
} AccData;
typedef struct {
    u8 pad0[0xc];
    u32 userId;
    u16 *name;
    u8 pad14[8];
    AccData *acc;
    u8 pad20[4];
    void *heap;
    u64 startTick;
    u8 pad30[0xc];
    LoginId loginId;
    u8 pad48[0x248 - 0x48];
    char userName[9];
    char password[0x20];
} FriendCtx;
typedef struct {
    int b;
    FriendCtx *ctx;
    int a;
} FriendGlobal;
typedef struct {
    u16 name[0x1a];
    char password[0xc];
    void *(*alloc)(int, u32);
    void (*free)(int, void *, u32);
} LoginParams;
extern FriendGlobal ov00_0221A90C;
BOOL DWCi_Acc_IsAuthentic(AccData *acc);
void DWCi_Acc_LoginIdToUserName(LoginId *id, u32 userId, char *out);
BOOL DWCi_Acc_IsValidLoginId(LoginId *id);
BOOL DWCi_Acc_CheckConsoleUserId(LoginId *id);
void DWCi_Acc_CreateTempLoginId(LoginId *id);
void DWCi_Acc_SetPlayerId(LoginId *id, u32 value);
int ov00_021ED704(const u16 *s);
void *ov00_021EC2A8(int a, u32 n);
void ov00_021EC2EC(int a, void *p, u32 unused);
void *ov00_021EC2B8(int a, u32 n, int align);
void ov00_021E7B70(LoginParams *params, void *heap);
void ov00_021EF6F4(int a, int b);
// @fn ov00_021EF6F4
void ov00_021EF6F4(int a, int b) {
    LoginParams params;
    void *heap;
    MI_CpuFill8(&params, 0, sizeof(params));
    ov00_0221A90C.a = a;
    ov00_0221A90C.b = b;
    if (DWCi_Acc_IsAuthentic(ov00_0221A90C.ctx->acc)) {
        DWCi_Acc_LoginIdToUserName(&ov00_0221A90C.ctx->acc->loginId, ov00_0221A90C.ctx->acc->userId, ov00_0221A90C.ctx->userName);
    } else {
        if (!DWCi_Acc_IsValidLoginId(&ov00_0221A90C.ctx->loginId)) {
            if (DWCi_Acc_CheckConsoleUserId(&ov00_0221A90C.ctx->acc->consoleId)) {
                ov00_0221A90C.ctx->loginId = ov00_0221A90C.ctx->acc->consoleId;
            } else {
                DWCi_Acc_CreateTempLoginId(&ov00_0221A90C.ctx->loginId);
            }
        } else {
            DWCi_Acc_SetPlayerId(&ov00_0221A90C.ctx->loginId, (u32)((OS_GetTick() * 0x5D588B656C078965 + 0x269EC3) >> 32));
        }
        DWCi_Acc_LoginIdToUserName(&ov00_0221A90C.ctx->loginId, ov00_0221A90C.ctx->userId, ov00_0221A90C.ctx->userName);
    }
    MI_CpuCopy8(ov00_0221A90C.ctx->name, params.name, ov00_021ED704(ov00_0221A90C.ctx->name) * 2 + 2);
    strcpy(params.password, ov00_0221A90C.ctx->password);
    params.alloc = ov00_021EC2A8;
    params.free = ov00_021EC2EC;
    heap = ov00_021EC2B8(0, 0x1c14, 4);
    ov00_0221A90C.ctx->heap = heap;
    ov00_0221A90C.ctx->startTick = OS_GetTick();
    ov00_021E7B70(&params, heap);
}
