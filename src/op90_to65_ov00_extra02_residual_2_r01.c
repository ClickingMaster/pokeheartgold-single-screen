#pragma force_active on

#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u32 unk0;
    void *table;
} FriendState;
extern FriendState *ov00_0221A918[];
BOOL ov00_021EFC6C(void);
void *ov00_021EF3A4(void);
int DWC_GetGsProfileId(void *userData, void *friendData);
int ov00_02200540(void *table, int profileId, int *index);
int ov00_02200400(void *table, int index, void *out);
enum { RES_NG, RES_OK };
BOOL ov00_021F0B8C(void *friendData, void *out);

// @fn ov00_021F0B8C
BOOL ov00_021F0B8C(void *friendData, void *out) {
    int index = 0;
    int profileId;
    if (ov00_0221A918[2] == NULL || !ov00_021EFC6C()) {
        return FALSE;
    }
    profileId = DWC_GetGsProfileId(ov00_021EF3A4(), friendData);
    if (profileId > 0 && ov00_02200540(ov00_0221A918[2]->table, profileId, &index) != 0) {
        return FALSE;
    }
    if (profileId <= 0 || index == -1) {
        return FALSE;
    }
    return ov00_02200400(ov00_0221A918[2]->table, index, out) != 0 ? RES_NG : RES_OK;
}
