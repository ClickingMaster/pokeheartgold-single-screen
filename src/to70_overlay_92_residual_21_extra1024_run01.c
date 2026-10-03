#pragma force_active on
#include "global.h"
extern fx32 FX_Sqrt(fx32);extern float ov92_02263218(void*);

float ov92_0226325C(void*w);

float ov92_0226325C(void*w){float value;if(ov92_02263218(w)>0.0f)value=4096.0f*ov92_02263218(w)+0.5f;else value=4096.0f*ov92_02263218(w)-0.5f;return (float)FX_Sqrt((int)value)/4096.0f;}
