#pragma force_active on
#include "global.h"
#pragma force_active on
extern const int ov70_02245F5C[][2];
int ov70_02242364(u8 *,int);
int ov70_02242364(u8 *w,int value){
 int idx;
 if(value!=3){
 idx=*(s16 *)(w+60);
 if(idx<=8){if(value>=ov70_02245F5C[idx][0])value=-1;return value;}
 return value;
 }else return -2;
}
