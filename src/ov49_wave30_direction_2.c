#include "ov49_wave30_direction_private.h"
typedef struct{u32 a;u8 state,timer,kind,flag;} R;

void *ov49_0225F2FC(void*w,void*a){u32 v[3];void*x=WiFiPlazaObj_CreateOwnPlayer(WiFiPlazaRoom_GetObjManager(w),a);void*y=ov49_0225A008(w);WiFiPlazaObj_GetPosition(x,v);WiFiPlazaCamera_SetTargetWithOffset(y,v[0],v[1],v[2]);return x;}

void ov49_0225F334(void*w,void*a,void*b,void*c,void*d){void*x=WiFiPlazaRoom_GetObjManager(w);void*y=ov49_0225A008(w);void*z=WiFiPlazaObj_CreateOwnPlayerAt(x,a,b,c);WiFiPlazaObj_QueueCommand(x,z,0,d);WiFiPlazaCamera_SetFollowObject(y,z);}

void ov49_0225F374(void*p){R*r=p;r->a=0;r->state=0;r->timer=1;r->kind=MTRandom()&3;r->flag=1;}
