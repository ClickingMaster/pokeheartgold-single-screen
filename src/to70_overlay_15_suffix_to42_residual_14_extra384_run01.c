#pragma force_active on
#include "global.h"
extern u8 ov15_02200B0C[];
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void *,s16,s16,int),ManagedSprite_SetDrawFlag(void *,int),ov15_021FF8F0(void *,int,int),ov15_022000F4(void *);
typedef struct {u8 pad[0x254];void *sprites[6];u8 pad2[0x6a4-0x26c];u16 values[1];} State;
typedef struct {u8 pad[6];s16 index;} Param;

void ov15_02200140(State *s,Param *p,int count,int update);
void ov15_022001C4(State *s,Param *p,int value);

void ov15_02200140(State *s,Param *p,int count,int update){int i;State *cursor;u8 *pos=ov15_02200B0C;for(i=0,cursor=s;i<6;i++,pos+=0x34,cursor=(State *)((u8 *)cursor+4)){ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[0],*(s16 *)(pos+0x34),*(s16 *)(pos+0x36),0x100000);if(i<count){if(update)ov15_021FF8F0(s,i,s->values[p->index+i]);ManagedSprite_SetDrawFlag(cursor->sprites[0],1);}else ManagedSprite_SetDrawFlag(cursor->sprites[0],0);}ov15_022000F4(s);}
void ov15_022001C4(State *s,Param *p,int value){int selected=-1;int i;u8 *pos;State *cursor;if(p->index==(value/6)*6)selected=value%6;pos=ov15_02200B0C;for(i=0,cursor=s;i<6;i++,pos+=0x34,cursor=(State *)((u8 *)cursor+4)){ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[0],*(s16 *)(pos+0x34),*(s16 *)(pos+0x36),0x100000);if(i==selected)ManagedSprite_SetDrawFlag(cursor->sprites[0],1);else ManagedSprite_SetDrawFlag(cursor->sprites[0],0);}ov15_022000F4(s);}
