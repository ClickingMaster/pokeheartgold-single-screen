#include "global.h"
#include "pokeathlon/pokeathlon_save.h"
typedef struct {u32 stats[4][3][8];u32 total,extra;u8 pad[64],flagA[4],flagB[4];struct{u16 first:1,second:1,spare:14;}flags;} Data;
typedef struct{u32 reserved,type;}Control;
typedef struct{u8 prefix[0x1f8];Control*control;u8 middle[0x1dc];u32 awards[4];u8 rest[0x342];u8 count,pad;Data data;}Course;
extern int ov96_021E5F24(void *);
extern void ov96_021E7F98(u32,u32,u32 *),ov96_021E7F48(void *);
void ov96_021E7D6C(void *,Pokeathlon_UnkSubStruct_B00 *);
static inline void AccumulateStatRow(const u32 *row,Pokeathlon_UnkSubStruct_B00 *counters){
ov96_021E7F98(row[0],9999999,&counters->unk18);
ov96_021E7F98(row[1],9999999,&counters->unk1C);
ov96_021E7F98(row[2],9999999,&counters->unk20);
ov96_021E7F98(row[3],9999999,&counters->unk24);
ov96_021E7F98(row[4],9999999,&counters->unk28);
ov96_021E7F98(row[5],9999999,&counters->unk2C);
ov96_021E7F98(row[6],9999999,&counters->unk30);
ov96_021E7F98(row[7],9999999,&counters->unk34);
}
#pragma force_active on
void ov96_021E7D6C(void *course,Pokeathlon_UnkSubStruct_B00 *out){
 u32 selected;
 Course*c=course;Data *data=&c->data;u8 i,j;u32 *record;
 selected=(u8)ov96_021E5F24(course);
 if(c->control->type==1){
  ov96_021E7F98(1,9999999,&out->unk38);
  if(data->flags.first){ov96_021E7F98(1,9999999,&out->unk3C);ov96_021E7F48(course);}else if(data->flags.second)ov96_021E7F98(1,9999999,&out->unk40);
 }else{
  ov96_021E7F98(1,9999999,&out->unk4);
  if(data->flags.first){ov96_021E7F98(1,9999999,&out->unk8);ov96_021E7F48(course);}else if(data->flags.second)ov96_021E7F98(1,9999999,&out->unkC);
 }

 for(i=0;i<c->count;i++){
  if(data->flagA[i])ov96_021E7F98(1,9999999,&out->unk44[(u8)c->awards[i]]);
  else if(data->flagB[i])ov96_021E7F98(1,9999999,&out->unk6C);
 }
 for(j=0;j<3;j++){record=data->stats[selected][j];AccumulateStatRow(record,out);}
 ov96_021E7F98(data->total,9999999,&out->unk10);ov96_021E7F98(data->extra,9999999,&out->unk14);
}
