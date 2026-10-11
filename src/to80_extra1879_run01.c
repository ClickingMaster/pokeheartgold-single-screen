#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct{u8 flags:4;u8 unused:4;u8 p[3];void *obj,*aux;u8 p2[0x78];u8 state,byte;u16 val;u32 result;} Work;
typedef struct{u16 a,b;} Pair;
extern int ov42_02228188(void *,int),ov45_02230DC4(u8);
extern u8 BillboardObject_GetSequence(void *);
extern u32 BillboardObject_GetSequenceFrame(void *),ov42_022282F4(void *);
extern void ov45_022308C0(Work *,Pair *);
extern void (*const ov45_02254F28[])(Work *);
void ov45_02230ACC(Work *);
void ov45_02230ACC(Work *w){int state;u32 val;struct {volatile Pair a;Pair b;} pair;u32 packed;if(w->flags){state=ov42_02228188(w->obj,5);val=ov42_02228188(w->obj,8);if(w->val>val||w->state!=state){if(ov45_02230DC4(w->state)==1){w->byte=BillboardObject_GetSequence(w->aux);w->result=BillboardObject_GetSequenceFrame(w->aux);}w->state=state;}{void (*cb)(Work *)=ov45_02254F28[state];w->val=val;cb(w);}packed=ov42_022282F4(w->obj);pair.a.a=packed;pair.a.b=packed>>16;pair.b=pair.a;ov45_022308C0(w,&pair.b);}}
