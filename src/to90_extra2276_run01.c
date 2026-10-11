#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x208f];u8 mode;}Work;BOOL BattlePartyMenu_IsMultiBattle(void*);int BattlePartyMenu_IsPartnerMon(void*,u32);
int BattlePartyMenu_IsPartnerMon(void*opaque,u32 bits){Work*w=opaque;if(BattlePartyMenu_IsMultiBattle(w)==1){if(w->mode==2){if(bits&1)return 1;}else{if(!(bits&1))return 1;}}return 0;}
