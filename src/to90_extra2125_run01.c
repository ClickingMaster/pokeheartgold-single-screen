#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u16 values[5];} Params;
typedef struct {u8 type,pad;Params params;} Inst;
typedef struct {u8 pad[0x38];u32 count;u32 entries[1];} Bank;
typedef struct {u32 index,sub;} Position;
int SND_GetNextInstData(Bank *,Inst *,Position *);
int SND_GetNextInstData(Bank *bank,Inst *out,Position *pos){
 while(pos->index<bank->count){u32 entry=bank->entries[pos->index];out->type=entry&0xff;u32 offset=entry>>8;
 switch(out->type){
 case 1:case 2:case 3:case 4:case 5:{Params *p=(Params*)((u8*)bank+offset);out->params=*p;pos->index++;return 1;}
 case 16:{u8 *p=(u8*)bank+offset;while(pos->sub<(u32)(p[1]-p[0]+1)){*out=((Inst*)(p+2))[pos->sub];pos->sub++;return 1;}break;}
 case 17:{u8 *p=(u8*)bank+offset;while(pos->sub<8){if(!p[pos->sub])break;*out=((Inst*)(p+8))[pos->sub];pos->sub++;return 1;}break;}
 }pos->index++;pos->sub=0;
 }return 0;
}
