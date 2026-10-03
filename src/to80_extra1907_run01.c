#pragma force_active on
#include "global.h"
#pragma force_active on
extern int PlayerAvatar_GetUnk24(void*);extern void PlayerAvatar_SetMoveState(void*,int);extern u8 PlayerAvatar_GetFacingDirection(void*);extern int PlayerAvatar_GetMoveState(void*);
int sub_0205D7AC(void*,int);
static inline BOOL SameFacing(u8 actual,int desired){return actual==desired;}
int sub_0205D7AC(void*w,int dir) {int v=PlayerAvatar_GetUnk24(w);if(dir==-1){if(v<2){PlayerAvatar_SetMoveState(w,0);return 0;}PlayerAvatar_SetMoveState(w,1);return 3;}if(!SameFacing(PlayerAvatar_GetFacingDirection(w),dir) && PlayerAvatar_GetMoveState(w)!=1){if(v<2){PlayerAvatar_SetMoveState(w,2);return 2;}PlayerAvatar_SetMoveState(w,1);return 3;}PlayerAvatar_SetMoveState(w,1);return 1;}
