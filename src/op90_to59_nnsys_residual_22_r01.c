// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u16 sequence;s16 x,y;u16 flags;} CellPlacement;
typedef struct {u16 count,distinct;CellPlacement *placements;} MultiCellData;
typedef struct {NNSG2dCellAnimation animation;u32 assigned;} SharedAnimation;
typedef union {SharedAnimation *shared;NNSG2dNode *nodes;} InstanceArray;
typedef struct {MultiCellData *data;NNSG2dCellAnimBankData *bank;u32 sharing;InstanceArray instances;} MultiCell;
extern BOOL NNSi_G2dIsAnimCtrlLoopAnim(const NNSG2dAnimController*);
extern void NNSi_G2dSrtcSetTrans(NNSG2dSRTControl*,s16,s16);
void SetMCDataToMCInstanceImpl_(MultiCell*,MultiCellData*,u32);
#pragma force_active off
static inline u32 SequenceDuration(const NNSG2dAnimSequence *sequence){u32 duration=0,frame;for(frame=0;frame<sequence->numFrames;frame++)duration+=sequence->pAnmFrameArray[frame].frames;return duration;}
static inline void Advance(NNSG2dCellAnimation *animation,const NNSG2dAnimSequence *sequence,u32 elapsed)
{
 u32 duration=SequenceDuration(sequence);
 if(NNSi_G2dIsAnimCtrlLoopAnim(&animation->animCtrl))NNS_G2dTickCellAnimation(animation,(fx32)((elapsed%duration)<<12));
 else{if(elapsed<duration)duration=elapsed;NNS_G2dTickCellAnimation(animation,(fx32)(duration<<12));}
}
#pragma force_active on
void n2(MultiCell *instance,MultiCellData *data,u32 elapsed);
// @fn SetMCDataToMCInstanceImpl_
void SetMCDataToMCInstanceImpl_(MultiCell *instance,MultiCellData *data,u32 elapsed)
{
 instance->data=data;
 if(instance->sharing==1){
  SharedAnimation *animations=instance->instances.shared;CellPlacement *placement;SharedAnimation *target;const NNSG2dAnimSequence *sequence;u16 index;
  {u16 clear;for(clear=0;clear<data->distinct;clear++)animations[clear].assigned=0;}
  for(index=0;index<data->count;index++){
   placement=&data->placements[index];
   target=&animations[(u16)((placement->flags&0xff00)>>8)];
   if(!target->assigned){
    sequence=NNS_G2dGetAnimSequenceByIdx(instance->bank,placement->sequence);
    NNS_G2dSetCellAnimationSequence(&target->animation,sequence);target->animation.animCtrl.bActive=TRUE;
    if((placement->flags&15)==1)Advance(&target->animation,sequence,elapsed);
    target->assigned=1;
   }
  }
 }else{
  u16 index;u32 duration;NNSG2dNode *nodes=instance->instances.nodes;
  for(index=0;index<data->count;index++){
   CellPlacement *placement=&data->placements[index];
   NNSG2dCellAnimation *animation=nodes[index].pContent;
   const NNSG2dAnimSequence *sequence=NNS_G2dGetAnimSequenceByIdx(instance->bank,placement->sequence);
   NNS_G2dSetCellAnimationSequence(animation,sequence);animation->animCtrl.bActive=TRUE;
   if((placement->flags&15)==1){duration=SequenceDuration(sequence);if(NNSi_G2dIsAnimCtrlLoopAnim(&animation->animCtrl))NNS_G2dTickCellAnimation(animation,(fx32)((elapsed%duration)<<12));else{NNS_G2dTickCellAnimation(animation,(fx32)((elapsed<duration?elapsed:duration)<<12));}}
   nodes[index].bVisible=TRUE;NNSi_G2dSrtcSetTrans(&nodes[index].srtCtrl,placement->x,placement->y);
  }
 }
}
