#pragma force_active on
#include "global.h"
#pragma force_active on
#include <stdarg.h>
typedef struct {u32 values[9];} Payload;typedef struct {u8 opcode,pad[3];Payload payload;} Buffer;
extern void *BattleSystem_GetBattleContext(void *);extern void BattleBuffer_Clear(void *,int);extern void ov12_02262240(void *,int,int,void *,int);void ov12_022639B8(void *,int,Payload);
void ov12_022639B8(void *battle,int battler,Payload payload){Buffer buffer;BattleBuffer_Clear(BattleSystem_GetBattleContext(battle),battler);buffer.opcode=37;buffer.payload=payload;ov12_02262240(battle,1,battler,&buffer,40);}
