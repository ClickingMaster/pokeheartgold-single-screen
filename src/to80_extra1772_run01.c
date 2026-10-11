#pragma force_active on
#include "global.h"
void PCBox_SetBlendEnabled(int);
void PCBox_SetBlendEnabled(int enabled){if(enabled==1)G2x_SetBlendAlpha_(0x04000050,0,42,6,10);else *(volatile u16*)0x04000050=0;}
