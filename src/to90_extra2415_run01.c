#pragma force_active on
#include "global.h"
#pragma force_active on

typedef struct Bits {u32 active:1;u32 phase:8;u32 cycles:7;u32 frame:16;} Bits;
typedef struct Work {u8 gap[0xac];u8 sprite[0x9c];Bits bits;} Work;
extern const s8 ov112_021FF4C8[];extern void ov112_021F1F54(void*,s16,s16,int),ov112_021F1F3C(void*,int),ov112_021F1EFC(void*,int);
void ov112_021F238C(Work*);
void ov112_021F238C(Work*w){if(w->bits.active){switch(w->bits.phase){case 0:ov112_021F1F54(w->sprite,152,(s16)(72-ov112_021FF4C8[w->bits.frame]),1);w->bits.frame++;if(ov112_021FF4C8[w->bits.frame]==111){w->bits.frame=0;w->bits.cycles++;if(w->bits.cycles>=2){ov112_021F1F3C(w->sprite,3);w->bits.cycles=0;w->bits.phase++;}}break;case 1:{s16 x=(s16)(152+w->bits.frame*8);w->bits.frame++;if(w->bits.frame>=5)ov112_021F1EFC(w->sprite,(w->bits.frame&1)==1);if(w->bits.frame>=7){ov112_021F1EFC(w->sprite,0);w->bits.frame=0;w->bits.phase++;}else ov112_021F1F54(w->sprite,x,72,0);break;}case 2:break;default:GF_ASSERT(0);}}}
