#ifndef TO40_OVERLAY_72_PRIVATE_H
#define TO40_OVERLAY_72_PRIVATE_H
#include "global.h"

typedef struct Ov72GlobalState {
    s32 state;
    s32 result;
} Ov72GlobalState;
typedef struct Ov72NetState {
    u8 pad00[3];
    u8 mode;
    u8 pad04[4];
    u8 *response;
} Ov72NetState;

extern s32 _0223B820[];
extern u8 ov72_0223B920[];
int GameStats_Update(void);
void GameStats_GetError(void);
s32 GameStats_GetResponseLength(void);
void GameStats_Shutdown(void);
s32 ov72_02237D88(void);
void ov72_022378DC(void);
BOOL ov72_02237B54(void);

#endif
