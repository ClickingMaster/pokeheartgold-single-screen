#pragma force_active on
#include "global.h"
typedef struct {u32 active;u32 pad;u32 buffer;u16 start,count,color;u8 q;u8 target;u8 value;s8 step;s8 delay;volatile s8 timer;void*palette;}Work;
extern void PaletteData_BlendPalette(void*,u32,u16,u16,u8,u16);
void ov07_02222E74(void*,Work*);
void ov07_02222E74(void*arg,Work*w){if(w->active){w->timer++;if(w->timer>=w->delay){int sum;s8 next;w->timer=0;PaletteData_BlendPalette(w->palette,w->buffer,w->start,w->count,w->value,w->color);if(w->value==w->target){w->active=0;return;}sum=w->value+w->step;next=sum;if(w->step>0){if(next>w->target)w->value=w->target;else w->value=sum;}else{if(next<w->target)w->value=w->target;else w->value=sum;}}}}
