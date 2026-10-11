#pragma force_active on
#include "global.h"
typedef struct State {s32 speed,phase;u16 inc,angle;s8 dir,count,unused,kind;} State;
typedef struct Vec {s32 x,y,z;} Vec;
extern s32 *_0210FACC[];
State *sub_0205F3E4(void *);
void MapObject_OffsetPositionInDirection(void *,int,int);
void MapObject_UpdateHeight(void *);
void MapObject_MoveCoordsInDirection(void *,int);
void MapObject_SetFlagsBits(void *,u32);
void MapObject_SetFacingVector(void *,Vec *);
void MapObject_SetPreviousCoordsToCurrent(void *);
void sub_0205F484(void *);
void sub_0205F328(void *,int);
void MapObject_IncrementMovementStep(void *);
void PlaySE(int);
int MapObject_CheckVisible(void *);
int MapObjectMovementCmd092_Step1(void *);
// @fn MapObjectMovementCmd092_Step1
int MapObjectMovementCmd092_Step1(void *obj){
    State *s=sub_0205F3E4(obj);
    Vec face;
    if(s->speed){
        MapObject_OffsetPositionInDirection(obj,s->dir,s->speed);
        MapObject_UpdateHeight(obj);
        if(s->phase>=65536){
            s->phase=0;
            MapObject_MoveCoordsInDirection(obj,s->dir);
            MapObject_SetFlagsBits(obj,4);
        }
        s->phase+=(s->speed<0)?-s->speed:s->speed;
    }
    s->angle+=s->inc;
    if(s->angle>3840) s->angle=3840;
    {
        int kind;
        int angle;
        u16 idx;
        s32 *row;
        kind = s->kind;
        angle = s->angle;
        idx = (u16)(angle / 256);
        row = _0210FACC[kind];
        face.x = 0;
        face.y = row[idx];
        face.z = 0;
    }
    MapObject_SetFacingVector(obj,&face);
    s->count--;
    if(s->count>0) return 0;
    {Vec zero={0,0,0};MapObject_SetFacingVector(obj,&zero);}
    MapObject_SetFlagsBits(obj,0x20028);
    MapObject_SetPreviousCoordsToCurrent(obj);
    sub_0205F484(obj);
    sub_0205F328(obj,0);
    MapObject_IncrementMovementStep(obj);
    if(!MapObject_CheckVisible(obj)) PlaySE(1606);
    return 1;
}
