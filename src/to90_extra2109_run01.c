#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 x,y,endX,endY;void *payload;} Hit;
typedef struct {u8 pad[13];u8 count;u8 pade[0x4676];int selection;u8 pad4688[12];u32 y,x;int mode;u32 pad46a0;Hit *hits;Hit current;} View;
void sub_02093E7C(View*,u8);
void sub_02094F5C(View*,int);
void sub_020956B8(View*);
void sub_02093F84(View*,u8,u8);
int sub_02093CE4(View*);
int sub_02093CE4(View *v){
    if(v->mode==1){
        int i=0,count;count=v->count;
        for(;i<count;i++){
            if(!v->hits[i].payload){
                u32 x=v->x;u32 t=v->hits[i].y,l=v->hits[i].x,r=v->hits[i].endX,b=v->hits[i].endY;
                if((u32)(v->y-t)<(u32)(b-t) && (u32)(x-l)<(u32)(r-l)){sub_02093E7C(v,(u8)i);return 1;}
            }else{
                u32 x=v->x;u32 t=v->hits[i].y,l=v->hits[i].x,r=v->hits[i].endX,b=v->hits[i].endY;
                if((u32)(v->y-t)<(u32)(b-t) && (u32)(x-l)<(u32)(r-l)){sub_02094F5C(v,i);sub_02093E7C(v,(u8)i);return 1;}
            }
        }
    }else if(v->mode==2){
        u32 x=v->x;u32 t=v->current.y,l=v->current.x,r=v->current.endX;u32 y=v->y;u32 b=v->current.endY;
        if((u32)(y-t)<(u32)(b-t) && (u32)(x-l)<(u32)(r-l)){sub_02094F5C(v,v->selection);sub_020956B8(v);return 2;}
        else{
            int i,count=v->count;
            for(i=0;i<count;i++){
                if(!v->hits[i].payload){
                    u32 t=v->hits[i].y,l=v->hits[i].x,r=v->hits[i].endX,b=v->hits[i].endY;
                    if((u32)(y-t)<(u32)(b-t) && (u32)(x-l)<(u32)(r-l)){sub_02093F84(v,(u8)v->selection,(u8)i);return 3;}
                }
            }
        }
    }else GF_AssertFail();
    return 0;
}
