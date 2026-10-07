#include "global.h"
#include "math_util.h"
#pragma force_active on
typedef struct{u16 values[4];}Choices;
extern const u16 ov87_021E8358[];void ov87_021E734C(void*,int);
// @fn ov87_021E734C
void ov87_021E734C(void*owner,int selection){u8*w=owner;Choices choices;struct {int i,j;u16 selected;u16*current;} progress;for(progress.i=0;progress.i<4;progress.i++)choices.values[progress.i]=255;progress.selected=(int)LCRandom()%4;progress.current=choices.values;for(progress.i=0;progress.i<4;progress.i++,progress.current++,w+=2){if(progress.i==progress.selected)*(u16*)(w+0x36a)=92;else{do{*progress.current=(u32)LCRandom()%23;for(progress.j=0;progress.j<progress.i;progress.j++){u16 previous=choices.values[progress.j];if(previous==*progress.current)break;}}while(progress.j!=progress.i);*(u16*)(w+0x36a)=ov87_021E8358[*progress.current];}}}
