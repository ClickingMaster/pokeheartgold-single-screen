#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct E{void*data;u8 pad[8];}E;typedef struct W{u8 pad[12];E entries[3];}W;typedef struct R{u8 pad[16];}R;extern void ov41_02249A50(void*,R*),ov41_02248B20(W*,void*,int,int);void ov41_022486C4(W*,int,int,void**);
void ov41_022486C4(W*w,int kind,int slot,void**object){ov41_02249A50(object,((R*)w->entries[kind].data)+slot);ov41_02248B20(w,*object,kind,slot);}
