#ifndef TO50_OVERLAY70_ENTRY_PRIVATE_H
#define TO50_OVERLAY70_ENTRY_PRIVATE_H
#include "global.h"
typedef struct Ov70GlobalState { s32 state; s32 result; } Ov70GlobalState;
typedef struct Ov70NetworkWork { u8 pad00[0x1c]; u8 mode; } Ov70NetworkWork;
typedef struct Ov70SharedWork { u8 pad00[0x40]; u8 *response; } Ov70SharedWork;
extern volatile Ov70GlobalState _02246800;
extern Ov70SharedWork ov70_02246900;
extern volatile Ov70NetworkWork ov70_02246920;
int GameStats_Update(void);
void GameStats_GetError(void);
u32 GameStats_GetResponseLength(void);
void GameStats_Shutdown(void);
s32 ov70_02238398(void);
void ov70_022378DC(void);
#endif
