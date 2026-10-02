#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 a,b,c,d;u32 e;} Packet;
extern Packet ov112_021FFA18;
extern u8 ov112_021FFA20[];
extern int ov112_021E5900(void*,int);
extern void CARD_SpiWaitWriteRange(void*,int);
typedef struct {u32 pad,a,b,c,d;void(*callback)(int);u32 p[5],enabled;} State;
extern State _021FF9E0;
extern u8 _021FF500;
extern void ov112_021E594C(void);
void ov112_021E59B4(u8 *src,int len,u8 a,u8 b,u32 value);
void ov112_021E5A14(int result);
void ov112_021E5A14(int result){if(_021FF9E0.enabled && !_021FF9E0.b && !result)ov112_021E59B4(0,0,0xf4,_021FF500,_021FF9E0.a);if(_021FF9E0.callback)_021FF9E0.callback(result);ov112_021E594C();}
