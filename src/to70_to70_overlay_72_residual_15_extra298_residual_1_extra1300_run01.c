#pragma force_active on
#include "global.h"
extern int ov72_02237B54(void),ov72_02237B74(void);
extern void sub_0202D5DC(void*,int,int),ov72_02238680(void*,int,int),ov72_0223A444(void*),sub_020399EC(void);
int ov72_02239A88(u8*);
int ov72_02239A88(u8 *w){int result;if(ov72_02237B54()){result=ov72_02237B74();*(int*)(w+4052)=0;switch(result){case 0:sub_0202D5DC(**(void***)w,5,2);ov72_02238680(w,39,28);break;case -1:*(int*)(w+3932)=result;*(int*)(w+28)=53;ov72_0223A444(w);break;case -2:*(int*)(w+3932)=result;*(int*)(w+28)=53;ov72_0223A444(w);break;case -3:*(int*)(w+3932)=result;*(int*)(w+28)=53;ov72_0223A444(w);break;case -6:*(int*)(w+3932)=result;*(int*)(w+28)=53;ov72_0223A444(w);break;case -7:case -4:*(int*)(w+3932)=result;*(int*)(w+28)=53;ov72_0223A444(w);break;case -5:*(int*)(w+3932)=result;*(int*)(w+28)=53;ov72_0223A444(w);sub_020399EC();break;}}else{(*(int*)(w+4052))++;if(*(int*)(w+4052)==3600)sub_020399EC();}return 3;}
