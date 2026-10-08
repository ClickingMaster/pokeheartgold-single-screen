#pragma force_active on
#include "global.h"
typedef struct MapObject MapObject;
extern int MapObject_GetFacingDirection(MapObject *);extern u32 MapObject_GetFlagsBitsMask(MapObject *,u32);extern void MapObject_SetFacingDirection(MapObject *,int);extern void MapObject_SetFlagsBits(MapObject *,u32);extern int _020FD7E0[][4];
void sub_0206207C(MapObject *,s8 *);
// @fn sub_0206207C
void sub_0206207C(MapObject *obj,s8 *p){int facing=MapObject_GetFacingDirection(obj);int i=0;while(i<4&&facing!=_020FD7E0[p[1]][i])i++;GF_ASSERT(i<4);p[0]=facing;i=(i+1)%4;facing=_020FD7E0[p[1]][i];if(MapObject_GetFlagsBitsMask(obj,128))p[2]=1;else p[2]=0;MapObject_SetFacingDirection(obj,facing);MapObject_SetFlagsBits(obj,128);}
