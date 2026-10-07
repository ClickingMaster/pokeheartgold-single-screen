#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
int __throw_catch_compare(const unsigned char*,const unsigned char*,int*);
int __throw_catch_compare(const unsigned char*thrown,const unsigned char*caught,int*adjust){const unsigned char*cursor=caught;int n;*adjust=0;if(caught==0)return 1;if(*caught=='P'){cursor++;if(*cursor=='V')cursor++;if(*cursor=='K')cursor++;if(*cursor=='v'&&(*thrown=='P'||*thrown=='*'))return 1;cursor=caught;}
if(*thrown=='!'||*thrown=='*'){if(*thrown++!=*cursor++)return 0;for(;;){if(*thrown==*cursor++){if(*thrown++=='!'){n=0;while(*thrown!='!'){n=n*10+*thrown++-'0';}*adjust=n;return 1;}}else{while(*thrown++!='!'){}while(*thrown++!='!'){}if(*thrown==0)return 0;cursor=caught+1;}}}
while((*thrown=='P'||*thrown=='R')&&*thrown==*cursor){thrown++;cursor++;if(*cursor=='K'){if(*thrown=='K')thrown++;cursor++;}if(*thrown=='K')return 0;if(*cursor=='V'){if(*thrown=='V')thrown++;cursor++;}if(*(const volatile unsigned char*)thrown=='V')return 0;}
while(*thrown==*cursor){if(!*thrown)return 1;thrown++;cursor++;}return 0;}
