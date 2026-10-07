#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u16 count,last;u32 unused;NNSG2dAnimationPlayMode mode;u8 *frames;} Seq;
typedef struct {u8 *frame;int pad4,reverse,active,time,pad14;NNSG2dAnimationPlayMode mode;Seq *seq;int cbkind,cbarg;void (*callback)(int,int);} Ctrl;

void SequenceEdgeHandle_(Ctrl *);
static inline NNSG2dAnimationPlayMode Mode(Ctrl *c){return c->mode?c->mode:c->seq->mode;}
static inline void Clamp(Ctrl *c){Seq*seq=c->seq;u8*frame=c->frame;u8*base=seq->frames;u8 *last=base+seq->count*8-8;if(frame>last)c->frame=last;else if(frame<base)c->frame=base;}
void SequenceEdgeHandle_(Ctrl *c){NNSG2dAnimationPlayMode mode=Mode(c);if((s32)mode==3||(u32)mode==4){c->reverse^=1;if(c->frame<=c->seq->frames+c->seq->last*8-8){if(c->cbkind==1)c->callback(c->cbarg,c->time);mode=Mode(c);if(mode==2||mode==4)NNS_G2dResetAnimCtrlState((NNSG2dAnimController *)c);else c->active=0;}}else{if(c->cbkind==1)c->callback(c->cbarg,c->time);mode=Mode(c);if(mode==2||mode==4)NNS_G2dResetAnimCtrlState((NNSG2dAnimController *)c);else c->active=0;}Clamp(c);}
