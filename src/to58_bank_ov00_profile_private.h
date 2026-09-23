#include "global.h"

#define U32(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define PTR(p, n) ((void *)((u8 *)(p) + (n)))

typedef void (*Ov00Callback)(void *, u16);
typedef struct {
    u8 pad_fb4[0xfb4];
    Ov00Callback callback;
    void *callbackArg;
    u8 pad_fbc[0x1098 - 0xfbc];
    u32 value;
} Ov00SysFollowup;
extern Ov00SysFollowup *_0221A680;
extern BOOL DWC_CheckHasProfile(void *);
extern void DWC_CreateExchangeToken(void *, void *);
extern int DWC_GetFriendDataType(void *);
extern BOOL DWC_IsBuddyFriendData(void *);
extern BOOL ov00_021E7AA4(void);

void ov00_021E6FE0(int);
void ov00_021E700C(void);
BOOL ov00_021E7078(void);
