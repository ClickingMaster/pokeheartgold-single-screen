#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_controller.h"
#pragma force_active on
typedef struct{u8 command,gender;u16 species;u32 personality;u8 form,substitute,transformed,pad;u16 speciesAll[4];u8 genderAll[4],shinyAll[4],formAll[4];u32 personalityAll[4];}Packet;
void ov12_02262240(BattleSystem*,int,int,const void*,int);
void BattleController_EmitPlayFaintAnimation(BattleSystem*bs,BattleContext*ctx,int id){Packet p;p.command=26;p.species=ctx->battleMons[id].species;p.form=ctx->battleMons[id].form;p.substitute=(ctx->battleMons[id].status2&0x1000000)!=0;p.transformed=(ctx->battleMons[id].status2&0x200000)!=0;if(ctx->battleMons[id].status2&0x200000){p.gender=ctx->battleMons[id].unk88.transformGender;p.personality=ctx->battleMons[id].unk88.transformPersonality;}else{p.gender=ctx->battleMons[id].gender;p.personality=ctx->battleMons[id].personality;}for(int i=0;i<4;i++){p.speciesAll[i]=ctx->battleMons[i].species;p.shinyAll[i]=ctx->battleMons[i].shiny;p.formAll[i]=ctx->battleMons[i].form;if(ctx->battleMons[i].status2&0x200000){p.genderAll[i]=ctx->battleMons[i].unk88.transformGender;p.personalityAll[i]=ctx->battleMons[i].unk88.transformPersonality;}else{p.genderAll[i]=ctx->battleMons[i].gender;p.personalityAll[i]=ctx->battleMons[i].personality;}}ov12_02262240(bs,1,id,&p,48);}
