#pragma force_active on
#include "global.h"
extern struct{u32 unused;u8*data;} _021D4124;
u16 WM_GetAllowedChannel(void);u16 WM_GetDispersionScanPeriod(void);int WM_StartScan(void(*)(void*),void*);void sub_02032858(int);void sub_02039AD8(int);void sub_02032D4C(void*);BOOL sub_02032C84(void);
extern void sub_02032844(int);extern int sub_02032E48(void);
#define H(p,o) (*(u16*)((p)+(o)))
#pragma opt_common_subs off
// @fn sub_02032C84
BOOL sub_02032C84(void){u16 channels=WM_GetAllowedChannel();if(channels==0x8000){sub_02032858(3);sub_02039AD8(1);return 0;}if(channels==0){sub_02032858(22);sub_02039AD8(1);return 0;}if(H(_021D4124.data,0x130c)==0){for(;;){H(_021D4124.data,0x12e4)++;if(H(_021D4124.data,0x12e4)>16)H(_021D4124.data,0x12e4)=1;if(channels&(1u<<(H(_021D4124.data,0x12e4)-1))){goto channelSelected;}else{continue;}}}else H(_021D4124.data,0x12e4)=H(_021D4124.data,0x130c);channelSelected:H(_021D4124.data,0x12e6)=WM_GetDispersionScanPeriod()/3;*(void**)(_021D4124.data+0x12e0)=_021D4124.data+0x1220;{int error=WM_StartScan(sub_02032D4C,_021D4124.data+0x12e0);if(error!=2){sub_02032858(error);return 0;}}return 1;}
#pragma opt_common_subs reset
