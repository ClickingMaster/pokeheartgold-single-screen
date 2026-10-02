#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Record {u16 species:11,form:5;u16 item,moves[4];u32 ot,pid;u32 hp:5,attack:5,defense:5,speed:5,spAttack:5,spDefense:5,nickname:1,flag:1;u8 evs[6],ppUps,language,ability,unknown;u16 name[11];} Record;
typedef struct Param {u8 pp,value;u16 move;} Param;
extern void ZeroMonData(void*);
extern void CreateMon(void*,u16,int,u32,int,u32,int,int);
extern void SetMonData(void*,int,const void*);
extern u32 GetMonData(void*,int,void*);
extern void CalcMonLevelAndStats(void*);
extern void *NewMsgDataFromNarc(int,int,int,int),*NewString_ReadMsgData(void*,int);
extern void String_Delete(void*),DestroyMsgData(void*);
void ov80_0222A140(Record*,void*,int);
typedef struct Source {u16 species,moves[4];u8 mask,nature;u16 item,form;} Source;
extern const u16 _0223B620[];
extern void ov80_02229EF4(Source*,u32,int);
extern u16 LCRandom(void);
extern int GetNatureFromPersonality(u32),CalcShininessByOtIdAndPersonality(u32,u32);
extern u32 MaskOfFlagNo(int);
extern u32 GetMonBaseStat(u16,int);
extern void GetSpeciesNameIntoArray(u16,int,u16*);
u32 ov80_02229F6C(Record*,u32,u32,u32,u8,u8,int,int,int);
u32 ov80_02229F6C(Record*r,u32 id,u32 ot,u32 pid,u8 ivs,u8 item,int useItem,int heap,int source){Source s;int found;MI_CpuFill8(r,0,56);ov80_02229EF4(&s,id,source);r->species=s.species;r->form=s.form;if(useItem){if(item>=4)item=item&3;r->item=_0223B620[item];}else r->item=s.item;found=255;for(int i=0;i<4;i++){r->moves[i]=s.moves[i];if(s.moves[i]==218)found=0;}r->ot=ot;if(pid==0){do{u32 lo=LCRandom();pid=lo|(LCRandom()<<16);}while(GetNatureFromPersonality(pid)!=s.nature||CalcShininessByOtIdAndPersonality(ot,pid)==1);r->pid=pid;}else r->pid=pid;r->hp=ivs;r->attack=ivs;r->defense=ivs;r->speed=ivs;r->spAttack=ivs;r->spDefense=ivs;int count=0;for(int i=0;i<6;i++)if(MaskOfFlagNo(i)&s.mask)count++;int value=510/count;if(value>255)value=255;int i=0;u8 ev=value;for(;i<6;i++)if(MaskOfFlagNo(i)&s.mask)r->evs[i]=ev;r->ppUps=0;r->language=gGameLanguage;u32 ability=GetMonBaseStat(r->species,25);if(ability){if(r->pid&1)r->ability=ability;else r->ability=GetMonBaseStat(r->species,24);}else r->ability=GetMonBaseStat(r->species,24);r->unknown=found;GetSpeciesNameIntoArray(r->species,heap,r->name);return pid;}
void ov80_0222A140(Record*r,void*mon,int level){u32 ivs,ot;Param p;Record *iter;ZeroMonData(mon);if(level==120)level=50;else if(level==121)level=100;ivs=*(u32*)((u8*)r+0x14)&0x3fffffff;CreateMon(mon,r->species,level,ivs,1,r->pid,2,0);SetMonData(mon,175,&ivs);CalcMonLevelAndStats(mon);p.value=r->form;SetMonData(mon,112,&p.value);SetMonData(mon,6,&r->item);iter=r;int i=0;int shift=0;for(;i<4;i++,iter=(Record*)((u8*)iter+2),shift+=2){p.move=iter->moves[0];SetMonData(mon,i+54,&p.move);p.value=(r->ppUps>>shift)&3;SetMonData(mon,i+62,&p.value);p.pp=GetMonData(mon,i+66,0);SetMonData(mon,i+58,&p.pp);}ot=r->ot;SetMonData(mon,7,&ot);p.value=r->evs[0];SetMonData(mon,13,&p.value);p.value=r->evs[1];SetMonData(mon,14,&p.value);p.value=r->evs[2];SetMonData(mon,15,&p.value);p.value=r->evs[3];SetMonData(mon,16,&p.value);p.value=r->evs[4];SetMonData(mon,17,&p.value);p.value=r->evs[5];SetMonData(mon,18,&p.value);SetMonData(mon,10,&r->ability);SetMonData(mon,9,&r->unknown);if(r->nickname){void *msg=NewMsgDataFromNarc(1,27,237,4);void *name=NewString_ReadMsgData(msg,r->species);SetMonData(mon,119,name);String_Delete(name);DestroyMsgData(msg);}else SetMonData(mon,117,r->name);SetMonData(mon,12,&r->language);CalcMonLevelAndStats(mon);}
