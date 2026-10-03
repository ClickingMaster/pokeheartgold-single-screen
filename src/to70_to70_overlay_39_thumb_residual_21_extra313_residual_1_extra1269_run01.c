#pragma force_active on
#include "global.h"
extern u32 ov00_021ECDC8(void);extern void ov39_02228AA8(int *),ov00_021EC210(void),ov00_021FA0D8(void),ov00_021ED9B4(void),sub_020399EC(void);
extern int ov00_021EC11C(int *,u32 *);
extern void ov00_021EC8D8(void);
int ov39_02228DA0(int *);
int ov39_02228DA0(int *w){int a;u32 b;switch(ov00_021ECDC8()){case 3:w[2]=5;break;case 0:case 4:case 5:ov39_02228AA8(w);w[5]=ov00_021EC11C(&a,&b);w[6]=a;w[7]=b;ov00_021EC210();ov00_021EC8D8();w[2]=6;switch(b){case 1:case 2:w[2]=6;break;case 4:ov00_021FA0D8();w[2]=6;break;case 6:w[2]=6;break;case 3:ov00_021ED9B4();w[2]=6;break;case 5:case 7:sub_020399EC();break;}if(a<-20000&&a>=-29999)w[2]=6;break;}return 0;}
