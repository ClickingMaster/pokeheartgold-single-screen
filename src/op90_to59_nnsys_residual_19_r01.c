#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void SequenceEdgeHandle_(NNSG2dAnimController *);
BOOL NNS_G2dTickAnimCtrl(NNSG2dAnimController *,fx32);
static inline BOOL IsSpeedPlus(const NNSG2dAnimController *c){return c->speed>0;}
static inline BOOL IsSpeedPlus2(const NNSG2dAnimController *c){return c->speed>0?TRUE:FALSE;}
static inline BOOL IsForward(const NNSG2dAnimController *c){return IsSpeedPlus(c)^c->bReverse;}
static inline BOOL IsForward2(const NNSG2dAnimController *c){return IsSpeedPlus2(c)!=c->bReverse;}
static inline BOOL IsReachedEnd(const NNSG2dAnimController *c){
 if(IsForward(c)){return c->pCurrent>=c->pAnimSequence->pAnmFrameArray+c->pAnimSequence->numFrames;}
 else{return c->pCurrent<=c->pAnimSequence->pAnmFrameArray+(c->pAnimSequence->loopStartFrameIdx-1);}
}
static inline BOOL IsReachedEnd2(const NNSG2dAnimController *c){
 const NNSG2dAnimFrame *base=c->pAnimSequence->pAnmFrameArray;
 if(IsForward(c)){return c->pCurrent>=base+c->pAnimSequence->numFrames;}
 else{return c->pCurrent<=base+c->pAnimSequence->loopStartFrameIdx-1;}
}
static inline BOOL IsForward3(const NNSG2dAnimController *c){return c->bReverse^IsSpeedPlus(c);}
static inline BOOL IsReachedEnd3(const NNSG2dAnimController *c){
 const NNSG2dAnimFrame *cur=c->pCurrent;
 if(IsForward(c)){return cur>=c->pAnimSequence->pAnmFrameArray+c->pAnimSequence->numFrames;}
 else{return cur<=c->pAnimSequence->pAnmFrameArray+(c->pAnimSequence->loopStartFrameIdx-1);}
}
static inline BOOL IsReachedEnd4(const NNSG2dAnimController *c){
 const NNSG2dAnimFrame *cur=c->pCurrent;
 if(IsForward3(c)){return cur>=c->pAnimSequence->pAnmFrameArray+c->pAnimSequence->numFrames;}
 else{return cur<=c->pAnimSequence->pAnmFrameArray+(c->pAnimSequence->loopStartFrameIdx-1);}
}
static inline BOOL IsReachedEnd5(const NNSG2dAnimController *c){
 const NNSG2dAnimFrame *cur=c->pCurrent;const NNSG2dAnimSequence *seq;
 if(IsForward3(c)){seq=c->pAnimSequence;return cur>=seq->pAnmFrameArray+seq->numFrames;}
 else{seq=c->pAnimSequence;return cur<=seq->pAnmFrameArray+seq->loopStartFrameIdx-1;}
}
static inline void IncFrame3(NNSG2dAnimController *c){if(IsForward3(c))c->pCurrent++;else c->pCurrent--;}
static inline void IncFrame(NNSG2dAnimController *c){if(IsForward(c))c->pCurrent++;else c->pCurrent--;}
static inline BOOL IsActive(const NNSG2dAnimController *c){return c->bActive;}
static inline BOOL IsTimeOver(const NNSG2dAnimController *c){return IsActive(c)&&c->currentTime>=(c->pCurrent->frames<<12);}
static inline BOOL IsTimeOver2(const NNSG2dAnimController *c){if(c->bActive&&c->currentTime>=(c->pCurrent->frames<<12))return TRUE;return FALSE;}
static inline u16 CurFrame(const NNSG2dAnimController *c){return (u16)(c->pCurrent-c->pAnimSequence->pAnmFrameArray);}
static inline void Callback(NNSG2dAnimController *c){
 switch(c->callbackFunctor.type){
 case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM: if(CurFrame(c)==c->callbackFunctor.frameIdx)c->callbackFunctor.pFunc(c->callbackFunctor.param,CurFrame(c)); break;
 case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM: c->callbackFunctor.pFunc(c->callbackFunctor.param,CurFrame(c)); break;
 default: break;
 }
}
static inline BOOL Fw_a(const NNSG2dAnimController *c){return IsSpeedPlus(c)^c->bReverse;}
static inline void Inc_a(NNSG2dAnimController *c){if(Fw_a(c))c->pCurrent++;else c->pCurrent--;}
static inline BOOL End_a(const NNSG2dAnimController *c){const NNSG2dAnimFrame *cur=c->pCurrent;const NNSG2dAnimSequence *seq;
 if(Fw_a(c)){seq=c->pAnimSequence;return cur>=seq->pAnmFrameArray+seq->numFrames;}else{seq=c->pAnimSequence;return cur<=seq->pAnmFrameArray+seq->loopStartFrameIdx-1;}}
static inline BOOL Fw_b(const NNSG2dAnimController *c){return c->bReverse^IsSpeedPlus(c);}
static inline void Inc_b(NNSG2dAnimController *c){if(Fw_b(c))c->pCurrent++;else c->pCurrent--;}
static inline BOOL End_b(const NNSG2dAnimController *c){const NNSG2dAnimFrame *cur=c->pCurrent;const NNSG2dAnimSequence *seq;
 if(Fw_b(c)){seq=c->pAnimSequence;return cur>=seq->pAnmFrameArray+seq->numFrames;}else{seq=c->pAnimSequence;return cur<=seq->pAnmFrameArray+seq->loopStartFrameIdx-1;}}
static inline BOOL Fw_c(const NNSG2dAnimController *c){return IsSpeedPlus(c)!=c->bReverse;}
static inline void Inc_c(NNSG2dAnimController *c){if(Fw_c(c))c->pCurrent++;else c->pCurrent--;}
static inline BOOL End_c(const NNSG2dAnimController *c){const NNSG2dAnimFrame *cur=c->pCurrent;const NNSG2dAnimSequence *seq;
 if(Fw_c(c)){seq=c->pAnimSequence;return cur>=seq->pAnmFrameArray+seq->numFrames;}else{seq=c->pAnimSequence;return cur<=seq->pAnmFrameArray+seq->loopStartFrameIdx-1;}}
static inline BOOL Fw_d(const NNSG2dAnimController *c){return c->bReverse!=IsSpeedPlus(c);}
static inline void Inc_d(NNSG2dAnimController *c){if(Fw_d(c))c->pCurrent++;else c->pCurrent--;}
static inline BOOL End_d(const NNSG2dAnimController *c){const NNSG2dAnimFrame *cur=c->pCurrent;const NNSG2dAnimSequence *seq;
 if(Fw_d(c)){seq=c->pAnimSequence;return cur>=seq->pAnmFrameArray+seq->numFrames;}else{seq=c->pAnimSequence;return cur<=seq->pAnmFrameArray+seq->loopStartFrameIdx-1;}}
static inline BOOL Fw_e(const NNSG2dAnimController *c){return IsSpeedPlus2(c)^c->bReverse;}
static inline void Inc_e(NNSG2dAnimController *c){if(Fw_e(c))c->pCurrent++;else c->pCurrent--;}
static inline BOOL End_e(const NNSG2dAnimController *c){const NNSG2dAnimFrame *cur=c->pCurrent;const NNSG2dAnimSequence *seq;
 if(Fw_e(c)){seq=c->pAnimSequence;return cur>=seq->pAnmFrameArray+seq->numFrames;}else{seq=c->pAnimSequence;return cur<=seq->pAnmFrameArray+seq->loopStartFrameIdx-1;}}
static inline BOOL Fw_f(const NNSG2dAnimController *c){return (BOOL)(IsSpeedPlus(c)^c->bReverse);}
static inline void Inc_f(NNSG2dAnimController *c){if(Fw_f(c))c->pCurrent++;else c->pCurrent--;}
static inline BOOL End_f(const NNSG2dAnimController *c){const NNSG2dAnimFrame *cur=c->pCurrent;const NNSG2dAnimSequence *seq;
 if(Fw_f(c)){seq=c->pAnimSequence;return cur>=seq->pAnmFrameArray+seq->numFrames;}else{seq=c->pAnimSequence;return cur<=seq->pAnmFrameArray+seq->loopStartFrameIdx-1;}}
static inline BOOL Fw_g(const NNSG2dAnimController *c){BOOL r=IsSpeedPlus(c);return r^c->bReverse;}
static inline void Inc_g(NNSG2dAnimController *c){if(Fw_g(c))c->pCurrent++;else c->pCurrent--;}
static inline BOOL End_g(const NNSG2dAnimController *c){const NNSG2dAnimFrame *cur=c->pCurrent;const NNSG2dAnimSequence *seq;
 if(Fw_g(c)){seq=c->pAnimSequence;return cur>=seq->pAnmFrameArray+seq->numFrames;}else{seq=c->pAnimSequence;return cur<=seq->pAnmFrameArray+seq->loopStartFrameIdx-1;}}
static inline void CbA(NNSG2dAnimController *c){u16 f=(u16)(((u32)c->pCurrent-(u32)c->pAnimSequence->pAnmFrameArray)/sizeof(NNSG2dAnimFrame));
 switch(c->callbackFunctor.type){case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM:if(f==c->callbackFunctor.frameIdx)c->callbackFunctor.pFunc(c->callbackFunctor.param,f);break;case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM:c->callbackFunctor.pFunc(c->callbackFunctor.param,f);break;default:break;}}
static inline u16 GetFrame(const NNSG2dAnimController *c){return (u16)(((u32)c->pCurrent-(u32)c->pAnimSequence->pAnmFrameArray)/sizeof(NNSG2dAnimFrame));}
static inline void CbB(NNSG2dAnimController *c,u16 f){
 switch(c->callbackFunctor.type){case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM:if(f==c->callbackFunctor.frameIdx)c->callbackFunctor.pFunc(c->callbackFunctor.param,f);break;case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM:c->callbackFunctor.pFunc(c->callbackFunctor.param,f);break;default:break;}}
static inline void CbC(NNSG2dAnimCallBackFunctor *cb,u16 f){
 switch(cb->type){case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM:if(f==cb->frameIdx)cb->pFunc(cb->param,f);break;case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM:cb->pFunc(cb->param,f);break;default:break;}}
static inline BOOL HasCb(const NNSG2dAnimCallBackFunctor *cb){return cb->type!=NNS_G2D_ANMCALLBACKTYPE_NONE;}
static inline void CbD(NNSG2dAnimCallBackFunctor *cb,u16 f){if(cb->type!=NNS_G2D_ANMCALLBACKTYPE_NONE){switch(cb->type){case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM:if(f==cb->frameIdx)cb->pFunc(cb->param,f);break;case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM:cb->pFunc(cb->param,f);break;default:break;}}}
static inline void CbE(NNSG2dAnimController *c){NNSG2dAnimCallBackFunctor *cb=&c->callbackFunctor;u16 f;if(cb->type==NNS_G2D_ANMCALLBACKTYPE_NONE)return;f=GetFrame(c);switch(cb->type){case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM:if(f==cb->frameIdx)cb->pFunc(cb->param,f);break;case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM:cb->pFunc(cb->param,f);break;default:break;}}
// @fn NNS_G2dTickAnimCtrl
BOOL NNS_G2dTickAnimCtrl(NNSG2dAnimController *ctrl,fx32 frames){
 BOOL changed=FALSE;
 if(ctrl->bActive!=TRUE)return FALSE;
 ctrl->currentTime+=abs(FX_Mul(ctrl->speed,frames));
 while(IsTimeOver2(ctrl)){
  changed=TRUE;
  ctrl->currentTime-=ctrl->pCurrent->frames<<12;
  Inc_g(ctrl);
  if(End_g(ctrl))SequenceEdgeHandle_(ctrl);
  if(ctrl->pCurrent->frames!=0)ctrl->pActiveCurrent=ctrl->pCurrent;
  if(HasCb(&ctrl->callbackFunctor))CbB(ctrl,GetFrame(ctrl));
 }
 return changed;
}
