#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 lo:4,hi:4;} Nibble;
int ov74_02231094(void),ov74_02231064(void),ov74_02230D80(void),ov74_02230DB8(int),ov74_02230E44(void);u8 *ov74_02231154(void),*ov74_0223115C(void);void ov74_02231584(void),ov74_02230DF4(int);
int ov74_02230D80(void){int mode=ov74_02231094();int count=0;if(mode==1){u8*p=ov74_02231154();if(((Nibble *)(p+451))->hi==1){u32 i=0;do{if(*(int *)p)count++;i++;p+=56;}while(i<8);}}return count;}
int ov74_02230DB8(int index){if(ov74_02231094()==1&&ov74_02231064()==7){u8*p=ov74_02231154();if(((Nibble *)(p+451))->hi==1&&*(int *)(p+index*56))return 1;}return 0;}
void ov74_02230DF4(int index){if(ov74_02231094()==1&&ov74_02231064()==7){u8*p=ov74_02231154();if(((Nibble *)(p+451))->hi==1&&*(int *)(p+index*56)){((Nibble *)(p+451))->lo=(u8)index;*(u16 *)(p+448)=120;ov74_02231584();}}}
int ov74_02230E44(void){int mode=ov74_02231094();int count=0;if(mode==2){u8*p=ov74_0223115C();if(p[96]==2){u32 i=0;do{if(*(int *)p&&p[10])count++;i++;p+=12;}while(i<8);}}return count;}
