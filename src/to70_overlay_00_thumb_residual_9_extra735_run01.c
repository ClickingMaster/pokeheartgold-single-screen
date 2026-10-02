#pragma force_active on
#include "global.h"
extern int DWC_IsBuddyFriendData(void*),ov00_021EFDCC(void*,void*,int*);
typedef struct{u32 a,b,c;}Friend;
typedef struct{u8 p[0x100];Friend*friends;u8 q[0xf40];u8 status[32];u8*data;int stride,index;}Work;
extern Work*_0221A680;
void ov00_021E6CE8(void);
void ov00_021E6CE8(void){int result;if(_0221A680->data){int n=_0221A680->index&31;if(DWC_IsBuddyFriendData(&_0221A680->friends[n])){_0221A680->status[n]=ov00_021EFDCC(&_0221A680->friends[n],_0221A680->data+_0221A680->stride*n,&result);}_0221A680->index=(_0221A680->index+1)&31;}}
