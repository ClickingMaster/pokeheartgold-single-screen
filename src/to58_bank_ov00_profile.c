#include "to58_bank_ov00_profile_private.h"

void ov00_021E700C(void) {
    u8 profileToken[12];
    u8 newToken[12];
    int i;
    int offset;

    if (!DWC_CheckHasProfile((void *)U32(_0221A680, 0xf10))) {
        DWC_CreateExchangeToken((void *)U32(_0221A680, 0xf10), profileToken);
    } else {
        DWC_CreateExchangeToken((void *)U32(_0221A680, 0xf10), newToken);
    }
    for (i = 0, offset = 0; i < 32; i++, offset += 12) {
        DWC_GetFriendDataType(PTR((void *)U32(_0221A680, 0x100), offset));
        DWC_IsBuddyFriendData(PTR((void *)U32(_0221A680, 0x100), offset));
    }
}

BOOL ov00_021E7078(void) {
    return ov00_021E7AA4();
}
