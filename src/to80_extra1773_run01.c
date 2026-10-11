#pragma force_active on
#include "global.h"
typedef struct{u8 p[52];u8 *data;}Host;void PCBox_ShowYesNoPrompt(Host*,int);int PCBox_ShowYesNo(Host*,int);
int PCBox_ShowYesNo(Host*h,int state){if(state==1)PCBox_ShowYesNoPrompt(h,1);else PCBox_ShowYesNoPrompt(h,0);*(u16*)(h->data+0x438)=state;return 7;}
