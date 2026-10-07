#pragma force_active on
#include "global.h"
extern u32 BattleSystem_GetBattleType(void *);extern int BattleHpBar_Util_GetBarTypeFromBattlerSide(u8,u32);
extern void ov12_0226498C(void *,int,int),ov12_0226430C(void *,u8,u8);
typedef struct {u8 a,b;s16 value;u16 size;u8 c;u8 low:5,secondary:2,last:1;u32 d,e,f;} Input;
void ov12_0225AF74(void *,void *,void *);
#pragma force_active on
void ov12_0225AF74(void *battle,void *argument,void *inputArgument){
 u8 *parent=argument,*bar=parent+0x28,*input=inputArgument;
 MIi_CpuClearFast(0,(u32 *)bar,1);*(void **)(bar+12)=battle;bar[0x24]=parent[0x194];bar[0x25]=(u8)BattleHpBar_Util_GetBarTypeFromBattlerSide(parent[0x195],BattleSystem_GetBattleType(battle));
 bar[0x4c]=input[0];*(s32 *)(bar+0x28)=*(s16 *)(input+2);*(u32 *)(bar+0x2c)=*(u16 *)(input+4);bar[0x48]=input[1];bar[0x49]=(((Input *)input)->secondary);*(u32 *)(bar+0x30)=0;*(u32 *)(bar+0x38)=*(u32 *)(input+8);*(u32 *)(bar+0x3c)=*(u32 *)(input+12);bar[0x26]=input[6];bar[0x4a]=((Input *)input)->low;bar[0x4b]=((Input *)input)->last;bar[0x27]=(u8)*(u32 *)(input+16);ov12_0226498C(bar,*(s32 *)(bar+0x28),-33);ov12_0226430C(*(void **)(bar+12),bar[0x24],bar[0x4c]);
}
