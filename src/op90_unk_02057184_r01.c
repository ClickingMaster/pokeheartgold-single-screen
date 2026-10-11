#pragma force_active on
#include "global.h"
extern void*sub_02034818(int),*MapObjectManager_GetFirstActiveObjectByID(void*,int),*PlayerAvatar_GetMapObject(void*);
extern int Comm_GetLocalNetId(void),PlayerProfile_GetVersion(void*),PlayerProfile_GetTrainerGender(void*);
extern void MapObject_Remove(void*),MapObject_SetID(void*,int);
extern void*PlayerAvatar_CreateWithParams(void*,u32,u32,u32,s32,u32,u32,void*);
typedef struct{u16 x,y;s8 dir;u8 pad[3];}Position;
typedef struct {u8 prefix[0x3c];void *objects;} Field;
typedef struct{int a;void*avatars[8];u8 flags[12];Field*field;u8 p[0x40];Position positions[15];u8 q[2];u8 blocked;}Work;
extern Work*_021D41C4;
void sub_02057184(int);
// @fn sub_02057184
void sub_02057184(int n){void*profile,*avatar,*obj;int version,game;if(_021D41C4->avatars[n]||_021D41C4->blocked)return;profile=sub_02034818(n);if(!profile)return;if(n!=Comm_GetLocalNetId()){obj=MapObjectManager_GetFirstActiveObjectByID(_021D41C4->field->objects,n+256);if(obj)MapObject_Remove(obj);} version=2;game=PlayerProfile_GetVersion(profile);if(game==0)version=1;else if(game==12)version=0;avatar=PlayerAvatar_CreateWithParams(_021D41C4->field->objects,_021D41C4->positions[n].x,_021D41C4->positions[n].y,_021D41C4->positions[n].dir,0,PlayerProfile_GetTrainerGender(profile),version,0);if(!avatar)GF_AssertFail();_021D41C4->avatars[n]=avatar;MapObject_SetID(PlayerAvatar_GetMapObject(avatar),n+256);_021D41C4->flags[n]=1;}
