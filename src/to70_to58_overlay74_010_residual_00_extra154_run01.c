#pragma force_active on
#include "global.h"
#pragma force_active on
#include "config.h"
int ov74_02231764(int);void ov74_02231790(int*);int PmAgbCartridge_GetVersionInternal(void);
int ov74_02231764(int n){if(n<=0)return 1;if(n<=251)return 0;if(n<277)return 1;if(n>411)return 1;return 0;}
void ov74_02231790(int*p){switch(PmAgbCartridge_GetVersionInternal()){case 0:*p=VERSION_RUBY;break;case 1:*p=VERSION_SAPPHIRE;break;case 2:*p=VERSION_LEAFGREEN;break;case 3:*p=VERSION_FIRERED;break;case 4:*p=VERSION_EMERALD;break;default:*p=-1;break;}}
