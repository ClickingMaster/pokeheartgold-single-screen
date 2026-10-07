#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 _021E36B0[];
typedef struct {s32 xOffset,xUnused,xScale,yOffset,yUnused,yScale;} Calibration;
extern Calibration _021E36CC;
void TP_GetCalibratedPoint(TPData *out,const TPData *raw){Calibration *c;if(!*(u16*)(_021E36B0+0x34)){*out=*raw;return;}out->touch=raw->touch;c=&_021E36CC;out->validity=raw->validity;if(!raw->touch){out->x=0;out->y=0;return;}out->x=(u16)((((s64)((s32)raw->x<<2)-c->xOffset)*c->xScale)>>22);if((s16)out->x<0)out->x=0;else if((s16)out->x>255)out->x=255;out->y=(u16)((((s64)((s32)raw->y<<2)-c->yOffset)*c->yScale)>>22);if((s16)out->y<0)out->y=0;else if((s16)out->y>191)out->y=191;}
