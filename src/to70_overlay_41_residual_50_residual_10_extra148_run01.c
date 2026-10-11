#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work{u8 pad0[0x2c];void*a;u32 pad30;void*b;u8 small[0x68],large[0x94];void*c;u32 flags;u8 tail[12];}Work;
extern void ov41_0224AD7C(Work*),ov41_0224AD84(void*),ov41_0224AFD4(void*,void*),ov41_0224B21C(void*,void*);
void DressUpHud_Free(Work*);
void DressUpHud_Free(Work*w){if(w->flags&2){ov41_0224AD7C(w);w->flags&=~2;}if(w->flags&4){ov41_0224AD84(w->a);w->flags&=~4;}if(w->flags&8){ov41_0224AFD4(w->small,w->b);w->flags&=~8;}if(w->flags&16){ov41_0224B21C(w->large,w->b);w->flags&=~16;}if(w->flags&32){ov41_0224AD84(w->c);w->flags&=~32;}memset(w,0,0x148);}
