#pragma force_active on
#include "global.h"
typedef struct {u16 id,pad;u16 low:10;u16 index:6;} E;
extern E ov01_022074A8[];extern u8 ov01_02207318[];
void *sub_021FA248(u32);
void *sub_021FA248(u32 id){E *p=ov01_022074A8;do{if(p->id==id){u32 index=p->index;if(index==255){GF_ASSERT(index!=255);return ov01_02207318;}return ov01_02207318+index*8;}p++;}while(p->id!=65535);GF_AssertFail();return 0;}
