#pragma force_active on
#include "global.h"
#pragma force_active on
extern int ov45_0222D724(void*,u8),ov45_0222D6FC(void*,u8),ov45_0222D6B0(void*,u8),ov45_0222D6D4(void*,u8);int ov48_02259A68(void*,int,int);
int ov48_02259A68(void*w,int kind,int other){int i;for(i=0;i<50;i++){if(ov45_0222D724(w,(u8)i)){int flag=ov45_0222D6FC(w,(u8)i);u8 k=ov45_0222D6B0(w,(u8)i);int v=ov45_0222D6D4(w,(u8)i);if(k==kind&&v==other){if(flag==1)return 1;else return 0;}}}return 3;}
