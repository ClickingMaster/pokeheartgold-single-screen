#pragma force_active on
#include "global.h"
extern void sub_02019B1C(void*,int,s8*,s8*),sub_020198FC(void*,int,int,int,u8);
void ov14_021E7FB8(void*);void ov14_021E7FEC(void*);
void ov14_021E7FB8(void*p){struct{s8 y,x;}v;sub_02019B1C(p,1,&v.x,&v.y);if(v.y!=24)sub_020198FC(p,1,0,1,(u8)(24-v.y));}
void ov14_021E7FEC(void*p){struct{s8 y,x;}v;sub_02019B1C(p,1,&v.x,&v.y);if(v.y!=6)sub_020198FC(p,1,0,-1,(u8)(v.y-6));}
