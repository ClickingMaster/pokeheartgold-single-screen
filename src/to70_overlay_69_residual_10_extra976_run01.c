#pragma force_active on
#include "global.h"

void ov69_021E75A0(int*p);

void ov69_021E75A0(int*p){if(p[0]>=0)p[0]%=65535;else p[0]+=((p[0]<0?-p[0]:p[0])/65535+1)*65535;if(p[1]>=0)p[1]%=65535;else p[1]+=((p[1]<0?-p[1]:p[1])/65535+1)*65535;}
