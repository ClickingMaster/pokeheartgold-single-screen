#pragma force_active on
#include "global.h"
int Comm_GetLocalNetId(void);void ov83_02240DE0(void*,int,int);void ov83_022414DC(int,int,u16*,u8*);
void ov83_022414DC(int id,int unused,u16*data,u8*p){if(id!=Comm_GetLocalNetId()){p[0x7FC]=data[1];ov83_02240DE0(p,p[0x7FC],1);}}
