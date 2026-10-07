#pragma force_active on
#include "global.h"
#pragma force_active on
int ov13_02226D0C(char*,int);
int ov13_02226D0C(char*p,int value){char*cursor=p;int count;u8 byte=value;int digit=(byte&240)>>4;count=0;int next=byte&15;do{if(digit<=9)digit+=48;else digit+=55;count++;*cursor=digit;cursor++;digit=next;}while(count<2);*cursor=0;return cursor-p;}
