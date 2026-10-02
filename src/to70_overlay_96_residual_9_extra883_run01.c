#pragma force_active on
#include "global.h"
void ov96_021E8340(u32,u32,u32*);
void ov96_021E8340(u32 which,u32 amount,u32*data){u32*p;switch(which){case 1:p=data;break;case 2:p=data+1;break;case 3:p=data+2;break;case 4:p=data+3;break;case 5:p=data+4;break;case 6:p=data+5;break;case 7:p=data+6;break;case 8:p=data+7;break;default:GF_AssertFail();return;}*p+=amount;if(*p>9999)*p=9999;}
