#pragma force_active on
#include "global.h"
int Comm_GetLocalNetId(void);void ov83_02244D0C(void*,int,int);void ov83_02245220(int,int,u16*,u8*);
void ov83_02245220(int id,int unused,u16*data,u8*p){if(id!=Comm_GetLocalNetId()){p[0x5B4]=data[1];ov83_02244D0C(p,p[0x5B4],1);}}
