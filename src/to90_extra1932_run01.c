#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int x,y;} Vec;typedef struct{int id,value;u16*flag;Vec*coords;int*type;int participant;} Info;
extern int ov96_021F3A5C(void*,int,int),ov96_021F3AB0(void*,u8),ov96_021F39F0(void*,Info*,int);
int ov96_021F3930(void*,void*,int,Info*,int,int,int*);
int ov96_021F3930(void*list,void*world,int index,Info*info,int x,int y,int*out){int score=0;if(index<0)return score;*out=0;if(*info->flag!=0&&*info->type==1){if(ov96_021F3A5C(world,x<<12,y<<12))score+=5;if(ov96_021F3AB0(list,(u8)index)){score+=4;*out=1;}score+=ov96_021F39F0(list,info,index);}else{int adjustment;if(ov96_021F3A5C(world,x<<12,y<<12))score+=5;if(ov96_021F3AB0(list,(u8)index)){score+=4;*out=1;}adjustment=ov96_021F39F0(list,info,index);if(adjustment==0)score+=2;if(info->id==index)score+=2;if(adjustment!=0)score=0;if(ov96_021F3A5C(world,info->coords->x,info->coords->y))score+=4;}return score;}
