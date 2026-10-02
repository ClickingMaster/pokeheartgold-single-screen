#pragma force_active on
#include "global.h"
typedef struct {u16 species;u8 pad[0x24];u8 form:5;u8 gender:1;u8 unused:2;u8 pad2[0x41];u32 personality;u8 pad3[4];u32 flags;u8 pad4[0xa];u8 variant:4;u8 extra:4;u8 pad5[0x25];u32 alternate;u8 pad6[0x12];u16 alternateVariant;u8 pad7[4];}Mon;
typedef struct{u8 pad[0x2d40];Mon mon[4];}Ctx;
typedef struct{u8 pad[0x30];Ctx*ctx;}System;
typedef struct{u8 command,arg,hidden,pad;u16 species[4];u8 variant[4],gender[4],form[4];u32 personality[4];}Packet;
extern void ov12_02262240(System*,int,int,void*,int);
void BattleController_EmitToggleVanish(System*,int,u8);
void BattleController_EmitToggleVanish(System*s,int battler,u8 arg){Packet p;int i;p.command=29;p.arg=arg;p.hidden=(s->ctx->mon[battler].flags&(1<<24))?1:0;for(i=0;i<4;i++){p.species[i]=s->ctx->mon[i].species;p.gender[i]=s->ctx->mon[i].gender;p.form[i]=s->ctx->mon[i].form;if(s->ctx->mon[i].flags&(1<<21)){p.variant[i]=s->ctx->mon[i].alternateVariant;p.personality[i]=s->ctx->mon[i].alternate;}else{p.variant[i]=s->ctx->mon[i].variant;p.personality[i]=s->ctx->mon[i].personality;}}ov12_02262240(s,1,battler,&p,40);}
