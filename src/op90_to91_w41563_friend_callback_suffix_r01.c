#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 data[12];
} FriendData;
typedef struct {
    u8 pad0[0x14];
    int count;
    FriendData *list;
} FriendState;
typedef struct {
    u8 pad0[0x24];
    u32 version;
} UserData;
typedef struct {
    int status;
    int profileId;
    u8 pad8[0x8e - 8];
    char nick[9];
    u8 version[4];
} BuddyRequest;
extern struct {
    u32 pad[2];
    FriendState *state;
} ov00_0221A918;
extern const char ov00_02217A4C[];
int DWC_GetFriendDataType(FriendData *f);
UserData *ov00_021EF3A4(void);
void DWC_LoginIdToUserName(UserData *user, FriendData *f, char *out);
void ov00_022002C0(void *conn, int profileId);
void DWC_SetGsProfileId(FriendData *f, int profileId);
int DWC_GetGsProfileId(UserData *user, FriendData *f);
void ov00_021F0B58(int profileId);
void ov00_02200310(void *conn, int profileId);
void ov00_021F0EF8(void *conn, BuddyRequest *req);
// @fn ov00_021F0EF8
void ov00_021F0EF8(void *conn, BuddyRequest *req) {
    char name[21];
    char version[5];
    int i;
    BOOL found = FALSE;
    if (req->status != 0) {
        return;
    }
    for (i = 0; i < ov00_0221A918.state->count; i++) {
        if (DWC_GetFriendDataType(&ov00_0221A918.state->list[i]) == 1) {
            DWC_LoginIdToUserName(ov00_021EF3A4(), &ov00_0221A918.state->list[i], name);
            if (strcmp(name, req->nick) == 0) {
                ov00_022002C0(conn, req->profileId);
                DWC_SetGsProfileId(&ov00_0221A918.state->list[i], req->profileId);
                found = TRUE;
            }
        } else if (DWC_GetFriendDataType(&ov00_0221A918.state->list[i]) == 3 || DWC_GetFriendDataType(&ov00_0221A918.state->list[i]) == 2) {
            u32 v = ov00_021EF3A4()->version;
            OS_SNPrintf(version, 5, ov00_02217A4C, (v >> 24) & 0xff, (v >> 16) & 0xff, (v >> 8) & 0xff, v & 0xff);
            if (req->profileId == DWC_GetGsProfileId(ov00_021EF3A4(), &ov00_0221A918.state->list[i]) && memcmp(version, req->version, 4) == 0) {
                ov00_022002C0(conn, req->profileId);
                found = TRUE;
            }
        }
    }
    if (found) {
        ov00_021F0B58(req->profileId);
    } else {
        ov00_02200310(conn, req->profileId);
    }
}
