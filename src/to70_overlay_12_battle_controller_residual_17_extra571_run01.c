#pragma force_active on
#include "global.h"
typedef struct {u16 species;u8 p[0x26-2];u8 x:5;u8 gender:1;u8 y:2;u8 q[0x68-0x27];u32 personality;u32 other;u32 flags;u8 r[0x7e - 0x74];u8 form:4;u8 z:4;u8 s[0xa4-0x7f];u32 transformedPersonality;u8 t[0xba-0xa8];u16 transformedForm;u8 u[4];}Mon;
typedef struct {u8 p[0x2d40];Mon mon[4];}Context;
typedef struct {u8 p[0x30];Context*ctx;}System;
typedef struct {u8 cmd,form;u16 species;u8 altform,gender;u16 pad;u32 personality;}Message;
extern void ov12_02262240(void*,int,int,void*,int);
void BattleController_EmitChangeForm(System*,int);
void BattleController_EmitChangeForm(System*sys,int battler){Message m;m.cmd=45;m.species=sys->ctx->mon[battler].species;m.gender=sys->ctx->mon[battler].gender;if(sys->ctx->mon[battler].flags&0x200000){m.altform=sys->ctx->mon[battler].transformedForm;m.personality=sys->ctx->mon[battler].transformedPersonality;}else{m.altform=sys->ctx->mon[battler].form;m.personality=sys->ctx->mon[battler].personality;}m.form=sys->ctx->mon[battler].x;ov12_02262240(sys,1,battler,&m,12);}
