#pragma force_active on
#include "global.h"
typedef struct{volatile int state;s32 result;}Control;typedef struct{u8 p[3];volatile u8 code;u32 q;u8*kind;}Response;extern Control _0223B820;extern Response ov72_0223B920;
extern int ov38_0221BF48(void),ov38_0221C020(void),ov72_02237D88(void);extern void ov38_0221C014(void),ov38_0221BFEC(void);
void ov72_022378DC(void);
void ov72_022378DC(void){switch(_0223B820.state){case 0:case 1:case 12:break;
case 2:switch(ov38_0221BF48()){case 1:_0223B820.state=12;ov38_0221C014();_0223B820.result=ov72_02237D88();ov38_0221BFEC();break;case 7:_0223B820.state=12;if(ov38_0221C020()>0)_0223B820.result=ov72_0223B920.code;else _0223B820.result=-2;ov38_0221BFEC();break;}break;
case 4:switch(ov38_0221BF48()){case 1:_0223B820.state=12;ov38_0221C014();_0223B820.result=ov72_02237D88();ov38_0221BFEC();break;case 7:_0223B820.state=12;if(ov38_0221C020()==2616)_0223B820.result=0;else switch(*ov72_0223B920.kind){case 2:_0223B820.result=-3;break;case 5:_0223B820.result=-2;break;default:_0223B820.result=-5;break;}ov38_0221BFEC();break;}break;
case 6:switch(ov38_0221BF48()){case 1:_0223B820.state=12;ov38_0221C014();_0223B820.result=ov72_02237D88();ov38_0221BFEC();break;case 7:_0223B820.state=12;switch(ov72_0223B920.code){case 1:_0223B820.result=0;break;case 2:_0223B820.result=-3;break;case 4:_0223B820.result=-1;break;case 5:_0223B820.result=-2;break;default:_0223B820.result=-5;break;}ov38_0221BFEC();break;}break;
case 8:switch(ov38_0221BF48()){case 1:_0223B820.state=12;ov38_0221C014();_0223B820.result=ov72_02237D88();ov38_0221BFEC();break;case 7:_0223B820.state=12;switch(ov72_0223B920.code){case 1:_0223B820.result=0;break;case 3:_0223B820.result=1;break;case 4:_0223B820.result=2;break;case 5:_0223B820.result=-2;break;default:_0223B820.result=-5;break;}ov38_0221BFEC();break;}break;
case 10:switch(ov38_0221BF48()){case 1:_0223B820.state=12;ov38_0221C014();_0223B820.result=ov72_02237D88();ov38_0221BFEC();break;case 7:_0223B820.state=12;if(ov38_0221C020()==8)_0223B820.result=0;else switch(ov72_0223B920.code){case 1:_0223B820.result=0;break;case 3:_0223B820.result=1;break;case 4:_0223B820.result=2;break;case 5:_0223B820.result=-2;break;default:_0223B820.result=-5;break;}ov38_0221BFEC();break;}break;
case 3:case 5:case 7:case 9:case 11:_0223B820.state=12;_0223B820.result=-4;ov38_0221BFEC();break;}}
