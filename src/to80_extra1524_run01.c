#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[4];void*save;u8 gap[32];void*data;u8 tail[2392];struct W*object;}W;extern W*_022577C0;extern u8 ov45_02254F14[],ov45_02254F04[];extern void*sub_0202C08C(void*);extern W*ov45_022310C0(void*,void*,int,void(**)(void),void*,int,void*);extern void ov45_0222F74C(void*);void ov45_0222E810(int,int);
extern void ov45_0222FC44(void);
extern void ov45_0222FCE0(void);
extern void ov45_0222FD50(void);
extern void ov45_0222FDD4(void);
extern void ov45_0222FDD8(void);
extern void ov45_0222FE84(void);
extern void ov45_0222FEC4(void);
extern void ov45_0222FF40(void);
extern void ov45_0222FF7C(void);
extern void ov45_02230008(void);
extern void ov45_02230050(void);
extern void ov45_02230064(void);
extern void ov45_02230090(void);
extern void ov45_022300B0(void);
extern void ov45_022300DC(void);
extern void ov45_02230108(void);
extern void ov45_02230130(void);
void ov45_0222E810(int value,int mode){void(*callbacks[17])(void);void*save;if(_022577C0==0)GF_AssertFail();callbacks[0]=ov45_0222FC44;callbacks[1]=ov45_0222FCE0;callbacks[2]=ov45_0222FD50;callbacks[3]=ov45_0222FDD4;callbacks[4]=ov45_0222FDD8;callbacks[5]=ov45_0222FE84;callbacks[6]=ov45_0222FEC4;callbacks[7]=ov45_0222FF40;callbacks[8]=ov45_0222FF7C;callbacks[9]=ov45_02230008;callbacks[10]=ov45_02230050;callbacks[11]=ov45_02230064;callbacks[12]=ov45_02230090;callbacks[13]=ov45_022300B0;callbacks[14]=ov45_022300DC;callbacks[15]=ov45_02230108;callbacks[16]=ov45_02230130;save=sub_0202C08C(_022577C0->save);_022577C0->object=ov45_022310C0(ov45_02254F04,ov45_02254F14,mode,callbacks,save,value,_022577C0->data);ov45_0222F74C(_022577C0->object);}
