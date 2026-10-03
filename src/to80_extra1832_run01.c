#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0xc];void*manager;}Work;extern void PokepicManager_HandleLoadImgAndOrPltt(void*);
void ov59_0223C374(Work*w);
void ov59_0223C374(Work*w){PokepicManager_HandleLoadImgAndOrPltt(w->manager);}
