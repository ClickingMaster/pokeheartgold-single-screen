#pragma force_active on
#include "global.h"
#pragma force_active on
typedef union{u32 word;struct{u8 hour,minute,second,pad;}field;} Time;
void ov45_0222D524(const Time*,const Time*,Time*);
void ov45_0222D524(const Time*left,const Time*right,Time*out){Time a=*left,b=*right;a.field.second+=b.field.second;a.field.minute+=a.field.second/60;out->field.second=a.field.second%60;a.field.minute+=b.field.minute;a.field.hour+=a.field.minute/60;out->field.minute=a.field.minute%60;a.field.hour+=b.field.hour;out->field.hour=a.field.hour%24;}
