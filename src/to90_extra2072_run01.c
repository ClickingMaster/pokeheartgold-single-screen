#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 form:5;u8 upper:3;} Form;
typedef struct {u8 data[0x1e4a4];} State;
extern void PokewalkerApp_RemoveWindowBank(State *,int),PokewalkerApp_AddWindowBank(void *, int),FillBgTilemapRect(void *,int,int,int,int,int,int,int),PokewalkerApp_HideSprite(State *,int);
extern int ov112_021EFAD4(void *,void *);
extern void PokewalkerApp_LoadTilemap(State *,int,int),PokewalkerApp_PrintStringDefaultColor(State *,int,void *,int),PokewalkerApp_SetSpritePosition(State *,int,int,int),PokewalkerApp_ShowSprite(State *,int),BufferSpeciesName(void *,int,int),PokewalkerApp_PrintMessage(State *,int,int,int),PokewalkerApp_LoadMonIcon(State *,int,int,int),BeginNormalPaletteFade(int,int,int,int,int,int,int);
int ov112_021EFB14(State *);
int ov112_021EFB14(State *s){
 int i;u8 *form,*record;
 PokewalkerApp_RemoveWindowBank(s,4);PokewalkerApp_AddWindowBank(s,5);
 FillBgTilemapRect(*(void **)(s->data+24),1,0,0,0,32,24,16);
 if(!ov112_021EFAD4(s->data+0x9d70,s->data+0xaabc)){PokewalkerApp_HideSprite(s,9);return 17;}
 PokewalkerApp_LoadTilemap(s,2,15);
 PokewalkerApp_PrintStringDefaultColor(s,1,*(void **)(s->data+0x1e4a0),0);PokewalkerApp_PrintStringDefaultColor(s,1,*(void **)(s->data+0x1e4a0),0);
 for(i=0;i<4;i++)PokewalkerApp_HideSprite(s,i+9);
 PokewalkerApp_SetSpritePosition(s,9,40,64);PokewalkerApp_SetSpritePosition(s,10,144,64);PokewalkerApp_SetSpritePosition(s,11,40,96);PokewalkerApp_SetSpritePosition(s,12,144,96);
 for(i=0,record=s->data,form=s->data+0x9d89;i<3;i++,record+=16,form+=16){
  if(*(u16 *)(record+0x9d7c)!=0){
   PokewalkerApp_ShowSprite(s,i+9);BufferSpeciesName(*(void **)(s->data+0x1e448),i+8,*(u16 *)(record+0x9d7c));
   PokewalkerApp_PrintMessage(s,i+3,i+0x43,0x30400);PokewalkerApp_LoadMonIcon(s,i,*(u16 *)(record+0x9d7c),((Form *)form)->form);
  }
 }
 if(((u32)s->data[0xaabc]<<26)>>31){
  record=s->data+0xad00;
  if(*(u16 *)record!=0){
   PokewalkerApp_ShowSprite(s,12);BufferSpeciesName(*(void **)(s->data+0x1e448),11,*(u16 *)record);
   PokewalkerApp_PrintMessage(s,6,0x46,0x30400);PokewalkerApp_LoadMonIcon(s,3,*(u16 *)record,((Form *)(record+13))->form);
  }
 }
 BeginNormalPaletteFade(0,1,1,0,6,1,154);return 13;
}
