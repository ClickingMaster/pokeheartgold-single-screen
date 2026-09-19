#ifndef TO50_OV59_GAP8_PRIVATE_H
#define TO50_OV59_GAP8_PRIVATE_H
#include "global.h"
typedef struct Ov59Gap8Work {
    u8 raw[0x254];
} Ov59Gap8Work;
int MenuInputStateMgr_GetState(void *);
void MenuInputStateMgr_SetState(void *, int);
void *Save_PlayerData_GetOptionsAddr(void *);
int Options_GetTextFrameDelay(void *);
int Options_GetFrame(void *);
void *Save_ApricornBox_Get(void *);
void sub_02031CEC(void *, void *, void *);
void sub_02031DA0(void *, void *);
void ov59_0223AE20(Ov59Gap8Work *);
void ov59_0223AE88(Ov59Gap8Work *);
void ov59_0223A9E4(Ov59Gap8Work *);
void ov59_0223AA6C(Ov59Gap8Work *);
#endif
