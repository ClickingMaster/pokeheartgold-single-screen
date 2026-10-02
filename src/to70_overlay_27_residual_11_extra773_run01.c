#pragma force_active on
#include "global.h"
typedef struct{u8 p[12];void *save;u8 q[8];int a;u8 r[0x70-0x1c];int b;}Work;
extern void *Save_VarsFlags_Get(void*);extern int FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(Work*),Save_VarsFlags_CheckSafariSysFlag(void*),Save_VarsFlags_CheckBugContestFlag(void*),Save_VarsFlags_CheckPalParkSysFlag(void*);int ov27_0225BD50(Work*);
int ov27_0225BD50(Work*w){void*flags=Save_VarsFlags_Get(w->save);if(FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(w))return 6;if(Save_VarsFlags_CheckSafariSysFlag(flags))return 1;if(Save_VarsFlags_CheckBugContestFlag(flags))return 2;if(Save_VarsFlags_CheckPalParkSysFlag(flags))return 3;if(w->a==3)return 4;if(w->b==3)return 5;return 0;}
