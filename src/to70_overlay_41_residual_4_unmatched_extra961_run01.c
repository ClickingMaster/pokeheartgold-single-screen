#pragma force_active on
#include "global.h"
typedef struct {u16 a,b;} Unsigned;typedef struct {s16 a,b;} Signed;typedef struct {int a;void*obj;} Work;
extern u32 sub_02015FCC(void*),sub_02015FE8(void*);
void ov41_022460DC(Work*,u8*);
void ov41_022460DC(Work*w,u8*out){volatile Signed pos,dim;volatile Unsigned position,size;u32 packed=sub_02015FCC(w->obj);position.a=packed;position.b=packed>>16;pos.a=position.a;pos.b=position.b;packed=sub_02015FE8(w->obj);size.a=packed;size.b=packed>>16;dim.a=size.a;dim.b=size.b;{s16 y=pos.b,x;out[0]=y;out[1]=y+dim.b;x=pos.a;out[2]=x;out[3]=x+dim.a;}}
