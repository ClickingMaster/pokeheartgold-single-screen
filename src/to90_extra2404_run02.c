#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 id:9,rest:23;u32 a,b;} Data;
extern u16 ov99_021E94CC(void *,s8,u8,int);
extern void ov99_021E9464(void *,Data *,s8,int,int,int),PokeathlonUtil_SetMonIconSprite(void *,void *,Data *,int,int),ManagedSprite_SetDrawFlag(void *,int),ManagedSprite_SetAnimSpeed(void *,fx32);
typedef struct {void *profile;u8 pad[16];void *manager;void *sprites[37];s8 mode;u8 padad[3];int arg;} Work;
void ov99_021E8F50(void *);
void ov99_021E8F50(void *s){int row=0,col,base=0;for(;row<5;row++,base+=3){if(ov99_021E94CC(*(void **)s,*(s8 *)((u8 *)s+0xac),(u8)row,*(int *)((u8 *)s+0xb0))!=0xffff){for(col=0;col<3;col++){Data data={0};int index=col+base+22;ov99_021E9464(*(void **)s,&data,*(s8 *)((u8 *)s+0xac),row,col,*(int *)((u8 *)s+0xb0));if(data.id){Work *object=(Work*)((u8*)s+index*4);PokeathlonUtil_SetMonIconSprite(*(void **)((u8 *)s+20),object->sprites[0],&data,0,1);ManagedSprite_SetDrawFlag(object->sprites[0],1);if(row==0)ManagedSprite_SetAnimSpeed(object->sprites[0],0x1800);}}}}}
