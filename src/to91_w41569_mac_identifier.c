#include "global.h"
#pragma force_active on
#include "message_format.h"
#include "font.h"
#include "bg_window.h"
typedef struct {u32 first[2],mode,visible;Window *canvas;MessageFormat *format;u8 remaining[0x3c];} WindowContext;
typedef struct{enum HeapID heap;BgConfig*bg;u8 p[8];PlayerProfile*profile;u8 q[0xdc];WindowContext window;u32 color;}W;
extern void ov74_02235414(void*,u32,u32,u32,u32,u32);extern void ov74_02235464(void*,u32,u32,u32);extern void ov74_0223546C(void*,u32,u32);extern u32 ov74_02235568(BgConfig*,void*,int,int,void*);extern void ov74_02235474(void*,u32,u32);
extern u32 ov74_022311F4(u8*);extern void ov74_02231238(u32,u32*,u32*);
void ov74_02229084(W*);
typedef struct{u32 code;}HardwareCode;
static inline HardwareCode DecodeHardwareCode(u8 *mac){HardwareCode decoded;decoded.code=ov74_022311F4(mac);return decoded;}
// @fn ov74_02229084
void ov74_02229084(W*w){MessageFormat*f=MessageFormat_New(w->heap);u32 a,b;u8 mac[6];HardwareCode value;
 ov74_02235414(&w->window,(u32)((u8*)w+56),0,247,1,1);ov74_02235464(&w->window,26,4,w->color);ov74_0223546C(&w->window,0,1);w->window.format=f;BufferPlayersName(f,0,w->profile);BufferIntegerAsString(f,1,PlayerProfile_GetTrainerID_VisibleHalf(w->profile),5,(PrintingMode)2,1);ov74_02235568(w->bg,&w->window,3,13,(void*)(u32)68);w->window.mode=0;ov74_02235474(&w->window,0,GetFontAttribute(0,1));ov74_02235568(w->bg,&w->window,-1,-1,(void*)(u32)70);
 OS_GetMacAddress(mac);value=DecodeHardwareCode(mac);ov74_02231238(value.code,&a,&b);BufferIntegerAsString(f,0,a,4,(PrintingMode)2,1);BufferIntegerAsString(f,1,b,4,(PrintingMode)2,1);w->window.visible=1;ov74_02235568(w->bg,&w->window,-1,-1,(void*)(u32)71);MessageFormat_Delete(f);}
