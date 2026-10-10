#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include "to90_login_callback_private.h"

typedef struct Friend {
    u32 v[3];
} Friend;

typedef struct FriendWork {
    u32 pad0;
    void *connection;
    u8 pad8[20];
    u8 index, changed, state;
} FriendWork;

extern struct {
    u32 a, b;
    FriendWork *work;
} ov00_0221A918;

typedef struct Status {
    u32 profile;
    u8 rest[524];
} Status;

extern int ov00_022003D0(void *, int *);
extern int ov00_02200400(void *, int, Status *);
extern int ov00_022005FC(void *, u32);
extern int ov00_02200540(void *, u32, int *);
extern void ov00_021F0C48(int);
extern u32 ov00_021F03BC(int);
extern int DWC_IsBuddyFriendData(Friend *);
extern void DWC_SetGsProfileId(Friend *, u32);
extern void DWCi_SetBuddyFriendData(Friend *);
extern int ov00_021F09A4(Friend *, int, u32);
extern void ov00_021F0B58(u32);
extern void *ov00_021EF3A4(void);
extern int DWC_GetGsProfileId(void *, Friend *);
extern void DWC_LoginIdToUserName(void *, Friend *, char *);
extern void ov00_021F0CBC(GPConnection *, void *, void *);
extern int ov00_021FFF58(GPConnection *, int, int, int, int, int, int, int, GPCallback, void *);
void ov00_021F06A8(Friend *, int);

void ov00_021F06A8(Friend *list, int length) {
    struct {
        int i;
        volatile int count;
    } local;

    char username[24];
    Status status;
    if (!ov00_0221A918.work->state) {
        ov00_021F0C48(ov00_022003D0(ov00_0221A918.work->connection, (int *)&local.count));
        for (local.i = 0; local.i < local.count; local.i++) {
            int j;
            ov00_021F0C48(ov00_02200400(ov00_0221A918.work->connection, *(volatile int *)&local.i, &status));
            for (j = 0; j < length; j++) {
                if (status.profile == ov00_021F03BC(j)) {
                    if (DWC_IsBuddyFriendData(&list[j])) {
                        break;
                    }
                    DWC_SetGsProfileId(&list[j], status.profile);
                    DWCi_SetBuddyFriendData(&list[j]);
                    ov00_0221A918.work->changed = 1;
                    break;
                }
            }
            if (j == length) {
                ov00_021F0C48(ov00_022005FC(ov00_0221A918.work->connection, status.profile));
                local.count--;
                local.i--;
            }
        }
        ov00_0221A918.work->state = 1;
    }
    while (ov00_0221A918.work->index < length) {
        u32 profile = ov00_021F03BC(ov00_0221A918.work->index);
        if (profile) {
            if (!ov00_021F09A4(list, ov00_0221A918.work->index, profile)) {
                ov00_021F0C48(ov00_02200540(ov00_0221A918.work->connection, profile, (int *)&local.i));
                if (local.i == -1) {
                    ov00_021F0B58(profile);
                }
            }
        } else if (DWC_GetGsProfileId(ov00_021EF3A4(), &list[ov00_0221A918.work->index]) == -1) {
            DWC_LoginIdToUserName(ov00_021EF3A4(), &list[ov00_0221A918.work->index], username);
            ov00_021FFF58(ov00_0221A918.work->connection, 0, 0, 0, 0, (int)(u32)username, 0, 0, ov00_021F0CBC, (void *)(u32)ov00_0221A918.work->index);
            ov00_0221A918.work->state = 2;
            return;
        }
        ov00_0221A918.work->index++;
    }
}
