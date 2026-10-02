#pragma force_active on
#include "global.h"
#include "unk_02019BA4.h"
extern TouchscreenHitbox ov81_02243668[],ov81_02243688[];
extern DpadMenuBox ov81_02243750[],ov81_02243718[];
extern GridCallbacks ov81_02243648;
extern int ov81_02242704(void *);

GridInputHandler *ov81_022432AC(void *p,int pos);
GridInputHandler *ov81_022432DC(void *p,int pos);
void ov81_02243314(GridInputHandler *p,int mode,void *w);

GridInputHandler *ov81_022432AC(void *p,int pos){return GridInputHandler_Create(ov81_02243668,ov81_02243750,&ov81_02243648,p,1,(u8)pos,(enum HeapID)100);}
GridInputHandler *ov81_022432DC(void *p,int pos){GridInputHandler *r=GridInputHandler_Create(ov81_02243688,ov81_02243718,&ov81_02243648,p,1,(u8)pos,(enum HeapID)100);GridInputHandler_ClearEnabledFlag(r,2);return r;}
void ov81_02243314(GridInputHandler *p,int mode,void *w){if(mode==1){GridInputHandler_SetAllEnabled(p);GridInputHandler_ClearEnabledFlag(p,3);}else{GridInputHandler_SetAllEnabled(p);GridInputHandler_ClearEnabledFlag(p,4);GridInputHandler_ClearEnabledFlag(p,5);GridInputHandler_ClearEnabledFlag(p,6);}if(ov81_02242704(w)==1)GridInputHandler_ClearEnabledFlag(p,2);}
