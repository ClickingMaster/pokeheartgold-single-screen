#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 data[0x58];} Message;
typedef struct {u8 data[0x50];} Source;
typedef struct {int values[9];u8 unused_tail[4];} Descriptor;
typedef struct {u8 pad0[0x20];void *pic;u8 pad24[0x17c];int flag;void *narc;} Context;
typedef struct {void *system;Context *context;void *object8,*objectC,*object10;u8 template[0x10];u8 pad24[0x5c];u8 command,battler,type,state,facing,height;u16 species;u32 personality;u8 slot,nature;u16 palette;s8 value90,value91;u8 shiny,value93;u16 value94;u8 value96,form;u32 value98;} Work;
extern u32*ov12_0223A8DC(void*);
extern void *BattleSystem_GetPokepicManager(void*);
extern void *BattleSystem_GetSpriteSystem(void*);
extern void *BattleSystem_GetPaletteData(void*);
extern void *BattleSystem_GetPartyMon(void*,int,int);
extern void *ov12_0223B750(void*);
extern void NARC_ReadPokepicAnimScript(void*,void*,u16,u16);
extern void *ov12_022612A4(void*,void*,void*,int,int,int,int,int,int,int,int,void*,int);
extern void Pokepic_SetAttr(void*,int,int);
extern int Pokepic_GetAttr(void*,int);
extern void Pokepic_AddAttr(void*,int,int);
extern void Pokepic_StartPaletteFade(void*,int,int,int,int);
extern int Pokepic_IsAnimFinished(void*);
extern int Pokepic_ResumePaletteFade(void*);
extern void Pokepic_Delete(void*);
extern void sub_0200914C(void*,int);
extern void *ov07_02233DB8(Descriptor*);
extern int ov07_02233E88(void*);
extern void ov07_02233ECC(void*);
extern int ov07_0221C394(u8 *);
extern int ov07_0221C3B0(void*);
extern void ov07_0221C3C0(void*);
extern int sub_02017068(void*,int);
extern void ov12_02261F38(void*,int,int,void*,void*,int,int,int,u32);
extern void ov12_022643C8(void*,int,Message*,int,int,int,int,int);
extern void ov12_02261B80(void*,void*,void*,Message*);
extern void ov12_02261CA8(void*,void*,Source*,int);
extern void ov07_02234A20(Source*,int);
extern void ov07_0223494C(Source*,int);
extern void ov12_0226430C(void*,int,int);
extern void Heap_Free(void*);
extern void SysTask_Destroy(void*);
extern void PlaySE_SetPan(int,int);
extern const s16 ov07_022377F4[];
extern const u8 ov12_0226D120[];
extern const u16 ov12_0226D15A[];
typedef struct {void *system;Context *context;void *pic,*object;u8 source[0x54];u8 pad64[4];u8 command,battler,type,state,height,pad6d;u16 species;u32 personality;u32 comparison74;} SmallWork;
typedef struct {int values[6];} Table;
extern const Table ov12_0226D128;
void ov12_0225C9BC(void*,void*);
void ov12_0225C9BC(void *task,void *data) {
 u32 *engine;SmallWork *work=data;
 Message message1;Source source;Message message2;Descriptor first,second;Table table;
 u8 state;engine=ov12_0223A8DC(work->system);
 state=work->state;switch(state) {
 case 0:if(work->personality){ov12_022643C8(work->system,0,&message1,1,15,work->battler,work->battler,0);ov12_02261B80(work->system,work->context,engine,&message1);work->state++;}else work->state=4;break;
 case 2:ov12_02261CA8(work->system,work->source,&source,work->battler);ov07_0223494C(&source,5);ov12_022643C8(work->system,0,&message2,1,16,work->battler,work->battler,0);ov12_02261B80(work->system,work->context,engine,&message2);work->context->flag=0;work->state++;break;
 case 1:case 3:ov07_0221C394((u8 *)engine);if(ov07_0221C3B0(engine)==0){ov07_0221C3C0(engine);work->state++;}break;
 case 4:{int type=work->type;if(type&1) {
 first.values[0]=ov12_0226D120[type];first.values[1]=5;first.values[2]=5;first.values[3]=work->battler;first.values[4]=work->species;
 first.values[7]=(int)BattleSystem_GetSpriteSystem(work->system);first.values[8]=(int)BattleSystem_GetPaletteData(work->system);first.values[5]=1;first.values[6]=0;work->object=ov07_02233DB8(&first);
 }else {table=ov12_0226D128;second.values[0]=table.values[type];second.values[1]=5;second.values[2]=5;second.values[3]=work->battler;second.values[4]=work->species;
 second.values[7]=(int)BattleSystem_GetSpriteSystem(work->system);second.values[8]=(int)BattleSystem_GetPaletteData(work->system);second.values[5]=1;second.values[6]=0;work->object=ov07_02233DB8(&second);}
 work->state=5;Pokepic_StartPaletteFade(work->pic,0,16,0,ov12_0226D15A[work->species]);
 switch(work->type){case 0:case 2:case 4:PlaySE_SetPan(1798,-117);break;case 1:case 3:case 5:PlaySE_SetPan(1798,117);break;}
 }break;
 case 5:if(Pokepic_ResumePaletteFade(work->pic)==0)work->state++;break;
 case 6:Pokepic_AddAttr(work->pic,12,-32);Pokepic_AddAttr(work->pic,13,-32);sub_0200914C(work->pic,work->height);if(Pokepic_GetAttr(work->pic,12)<=0){Pokepic_Delete(work->pic);work->state=7;}break;
 case 7:if(ov07_02233E88(work->object)==0){ov07_02233ECC(work->object);work->state=8;}break;
 case 8:ov12_0226430C(work->system,work->battler,work->command);Heap_Free(work);SysTask_Destroy(task);break;
 }
}
