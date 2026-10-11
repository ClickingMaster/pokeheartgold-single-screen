#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x664];int host,count,peers[16];u8 pad2[3];u8 select;u8 pad3[6];u8 enabled;} Link;
typedef struct {u32 a,b;Link *link;} Global;
extern Global _021D4140;
extern u8 _0210F900[];
extern int sub_0203993C(void);
extern u16 Comm_GetLocalNetId(void);
extern int sub_02034084(int),sub_02034044(int),sub_02036630(Link *),sub_02036E60(void *),sub_02036FA8(void),sub_02033FC4(u16),Comm_IsNetIdConnected(u16),sub_02033298(void);
extern int WifiConnection_SendToAll(Link *,int),WifiConnection_SendToHost(Link *,int);
extern void sub_02036AD8(void);
void sub_02036298(void);
void sub_02036298(void){
 int i,n;
 if(sub_02034084(sub_0203993C())){
  if(_021D4140.link->enabled){
  if(_021D4140.link->host){
   if(!sub_02036630(_021D4140.link))return;
   if(_0210F900[1]==4){sub_02036E60(_021D4140.link);_0210F900[1]=2;}
  }else if(_0210F900[1]==4){if(!sub_02036E60(_021D4140.link))return;_0210F900[1]=2;}
  if(sub_02036FA8())return;
  if(WifiConnection_SendToAll(_021D4140.link,38)){
  n=sub_02033FC4((u16)sub_0203993C())+1;
  for(i=0;i<n;i++)if(Comm_IsNetIdConnected((u16)i))_021D4140.link->peers[i]++;
  _0210F900[1]=4;
 }
 }
 }else if(sub_02034044(sub_0203993C())){
  if(!_021D4140.link->enabled)return;
  if(_021D4140.link->host){
   if(_021D4140.link->count>3)return;
   if(_0210F900[1]==4){sub_02036E60(_021D4140.link);_0210F900[1]=2;}
  }else if(_0210F900[1]==4){if(!sub_02036E60(_021D4140.link))return;_0210F900[1]=2;}
  if(sub_02036FA8())return;
  if(!WifiConnection_SendToHost(_021D4140.link,38))return;
  _0210F900[1]=4;_021D4140.link->count++;
 }else if(sub_02033298()==4){
  if(!Comm_IsNetIdConnected(Comm_GetLocalNetId()))return;
  if(_0210F900[1]==4&&_021D4140.link->count<=3){
   sub_02036E60((u8 *)_021D4140.link+_021D4140.link->select*64);
   sub_02036E60((u8 *)_021D4140.link+(1-_021D4140.link->select)*64);
   _0210F900[1]=0;
  }
  sub_02036AD8();
 }
}
