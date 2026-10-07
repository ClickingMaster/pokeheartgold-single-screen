#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 form:5;u8 upper:3;} Form;
typedef struct {u8 data[0x1e4a4];} State;
extern void ov112_021E9C10(State *,int),ov112_021E9A78(void *, int),FillBgTilemapRect(void *,int,int,int,int,int,int,int),ov112_021EA688(State *,int);
extern int ov112_021EFAD4(void *,void *);
extern void ov112_021E7CA4(State *,int,int),ov112_021E9FD8(State *,int,void *,int),ov112_021EA6B8(State *,int,int,int),ov112_021EA670(State *,int),BufferSpeciesName(void *,int,int),ov112_021E9F5C(State *,int,int,int),ov112_021EAB78(State *,int,int,int),BeginNormalPaletteFade(int,int,int,int,int,int,int);
int ov112_021EFB14(State *);
int ov112_021EFB14(State *s){
 int i;u8 *form,*record;
 ov112_021E9C10(s,4);ov112_021E9A78(s,5);
 FillBgTilemapRect(*(void **)(s->data+24),1,0,0,0,32,24,16);
 if(!ov112_021EFAD4(s->data+0x9d70,s->data+0xaabc)){ov112_021EA688(s,9);return 17;}
 ov112_021E7CA4(s,2,15);
 ov112_021E9FD8(s,1,*(void **)(s->data+0x1e4a0),0);ov112_021E9FD8(s,1,*(void **)(s->data+0x1e4a0),0);
 for(i=0;i<4;i++)ov112_021EA688(s,i+9);
 ov112_021EA6B8(s,9,40,64);ov112_021EA6B8(s,10,144,64);ov112_021EA6B8(s,11,40,96);ov112_021EA6B8(s,12,144,96);
 for(i=0,record=s->data,form=s->data+0x9d89;i<3;i++,record+=16,form+=16){
  if(*(u16 *)(record+0x9d7c)!=0){
   ov112_021EA670(s,i+9);BufferSpeciesName(*(void **)(s->data+0x1e448),i+8,*(u16 *)(record+0x9d7c));
   ov112_021E9F5C(s,i+3,i+0x43,0x30400);ov112_021EAB78(s,i,*(u16 *)(record+0x9d7c),((Form *)form)->form);
  }
 }
 if(((u32)s->data[0xaabc]<<26)>>31){
  record=s->data+0xad00;
  if(*(u16 *)record!=0){
   ov112_021EA670(s,12);BufferSpeciesName(*(void **)(s->data+0x1e448),11,*(u16 *)record);
   ov112_021E9F5C(s,6,0x46,0x30400);ov112_021EAB78(s,3,*(u16 *)record,((Form *)(record+13))->form);
  }
 }
 BeginNormalPaletteFade(0,1,1,0,6,1,154);return 13;
}
