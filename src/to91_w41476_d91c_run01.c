#include "global.h"
#pragma force_active on
#include "battle/battle_system.h"
#include "message_format.h"
#include "msgdata.h"
#include "pokemon.h"
#include <stddef.h>
typedef struct {Pokemon*mon;u8 gap[12];u16 hp;u8 tail[62];} Slot;
typedef struct {u8 pad[8];BattleSystem*battle;void*other;u8 p10,index,special,p13,partyA,partyB;u8 gap[12];u16 battler,flag;u8 p26[2];u32 id;u8 party[6];u8 p32,ready,move;} Context;
typedef struct {Context*ctx;Slot slots[6];u8 gap[0x1fa8-4-6*sizeof(Slot)];MsgData *messages;MessageFormat *format;String *message;SpriteManager *spriteManager;u8 gap2[0x2076-0x1fb8];u8 selected;u8 gap3[0x208f-0x2077];u8 mode;} Work;
int ov08_0221DB24(void*,u32),ov08_0221D91C(void*);
u8 ov08_0221DAC4(u8*);
typedef char WorkSize[sizeof(Work)==0x2090?1:-1];
typedef char SlotSize[sizeof(Slot)==80?1:-1];
typedef char MsgOffset[offsetof(Work,messages)==0x1fa8?1:-1];
typedef char SelectOffset[offsetof(Work,selected)==0x2076?1:-1];
typedef char ModeOffset[offsetof(Work,mode)==0x208f?1:-1];
static inline void Nick(Work *s,Slot *slot,int id){String *t=NewString_ReadMsgData(s->messages,id);BufferBoxMonNickname(s->format,0,Mon_GetBoxMon(slot->mon));StringExpandPlaceholders(s->format,s->message,t);String_Delete(t);}
typedef Slot SlotBank[6];
typedef struct {u32 offset;SlotBank *bank;} SlotSelection;
#pragma opt_propagation off
int ov08_0221D91C(void*opaque){Work*s=opaque;Context*ctx;Slot*slot=&s->slots[s->ctx->index];if(ov08_0221DB24(s,s->ctx->index)==1){String*t=NewString_ReadMsgData(s->messages,80);int partner;ctx=s->ctx;partner=BattleSystem_GetBattlerIdPartner(ctx->battle,ctx->id);Trainer*trainer=BattleSystem_GetTrainer(s->ctx->battle,partner);BufferTrainerNameFromDataStruct(s->format,0,trainer);StringExpandPlaceholders(s->format,s->message,t);String_Delete(t);return 0;}if(slot->hp==0){Nick(s,slot,77);return 0;}ctx=s->ctx;{u8 index=ctx->party[ctx->index];if(ctx->partyA==index||ctx->partyB==index){Nick(s,slot,76);return 0;}}if(ov08_0221DAC4((u8*)s)==1){ReadMsgDataIntoString(s->messages,79,s->message);return 0;}ctx=s->ctx;{if(ctx->special!=6){u8 index=ctx->index;if(ctx->special==ctx->party[index]){{String*t=NewString_ReadMsgData(s->messages,93);BufferBoxMonNickname(s->format,0,Mon_GetBoxMon(s->slots[index].mon));StringExpandPlaceholders(s->format,s->message,t);String_Delete(t);}return 0;}}}if(ctx->flag){u8 index=s->selected;SlotSelection selected={index*sizeof(Slot),&s->slots};String*t=NewString_ReadMsgData(s->messages,78);BufferBoxMonNickname(s->format,0,Mon_GetBoxMon(*(Pokemon **)((u8 *)selected.bank+selected.offset)));StringExpandPlaceholders(s->format,s->message,t);String_Delete(t);return 0;}return 1;}
