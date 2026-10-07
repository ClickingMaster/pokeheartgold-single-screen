#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 *BattleSystem_GetRecvBufferPtr(void*),*BattleSystem_GetSendBufferPtr(void*);
extern u16 *ov12_0223A984(void*),*ov12_0223A990(void*),*ov12_0223A960(void*),*ov12_0223A96C(void*);
typedef struct {u8 type,id;u16 size;} Header;
void BattleController_SendData(void*battle,int type,int id,u8*data,u8 size);
void BattleController_SendData(void*battle,int type,int id,u8*data,u8 size){u8*buffer;u16*pos,*limit;Header h;u32 i;int j;u8 *bytes;
 if(type==1){buffer=BattleSystem_GetRecvBufferPtr(battle);pos=ov12_0223A984(battle);limit=ov12_0223A990(battle);}else{buffer=BattleSystem_GetSendBufferPtr(battle);pos=ov12_0223A960(battle);limit=ov12_0223A96C(battle);}
 if(*pos+5u+size>4096){*limit=*pos;*pos=0;}h.type=type;h.id=id;h.size=size;
 bytes=(u8*)&h;for(i=0;i<4;i++){buffer[*pos]=bytes[i];(*pos)++;}for(j=0;j<size;j++){buffer[*pos]=data[j];(*pos)++;}}
