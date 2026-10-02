#pragma force_active on
#include "global.h"
typedef struct {u8 bytes[7];} Seven;extern Seven ov13_02242680;extern u8 ov13_0224E3D8[],ov13_0224E464[];extern u8 *ov13_02245A58;extern struct {u8 p[0x50];int x;} ov13_0224DF30;extern u8 *ov13_022247DC(u8*,int,void*,int);extern void ov13_02224718(void*,int,void*,int,int);void ov13_02224988(void*);
void ov13_02224988(void*arg){struct {u16 x;Seven data;} buf;u8*ptr;buf.x=256;buf.data=ov13_02242680;ptr=ov13_022247DC(ov13_02245A58,1,&buf,2);ptr=ov13_022247DC(ptr,2,&buf,2);if(ov13_0224DF30.x)ptr=ov13_022247DC(ptr,5,&buf,2);ptr=ov13_022247DC(ptr,3,&buf.data,7);if(ov13_0224DF30.x)ptr=ov13_022247DC(ptr,4,ov13_0224E3D8,6);ov13_02224718(arg,2,ov13_0224E464,ptr-ov13_02245A58+8,0);}
