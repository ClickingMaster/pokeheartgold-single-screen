#pragma force_active on
#include "global.h"
#pragma force_active on
void Pokepic_Delete(void*);void ov41_022496E8(u8*);
void ov41_022496E8(u8*p){u32 i;Pokepic_Delete(*(void**)p);i=16;do{*p=0;p++;}while(--i != 0);}
