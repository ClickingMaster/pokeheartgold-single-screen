#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35217Track { u8 active:1; u8 flag1:1; u8 mute:1; u8 other:5; u8 mod; u8 b2,b3; u8 volume,volume2; s8 bend; u8 range; s8 pan,pan2; s16 extvol,pitch; u8 rest[10]; u16 mod1,mod2,mod3; u8 rest2[30]; struct W35217Channel *channels; } W35217Track;
typedef struct W35217Player { u8 active:1; u8 flags:7; u8 number,b2,b3,b4,volume; s16 extvol; u8 tracks[16]; } W35217Player;
typedef struct W35217Channel { u8 b0,b1,type,b3,mod,key; s16 extvol; u8 note,velocity; s8 pan,pan2; s16 volume,pitch; u8 rest[18]; u8 release; u8 rest2[5]; u16 mod1,mod2,mod3; u8 rest3[6]; void *callback; u8 rest4[24]; struct W35217Channel *next; } W35217Channel;
extern u8 SNDi_Work[];
W35217Track *GetPlayerTrack(W35217Player *p,int i);
// @fn GetPlayerTrack
W35217Track *GetPlayerTrack(W35217Player *p,int i) { int n; if(i>15)return 0; n=p->tracks[i]; if(n==255)return 0; return (W35217Track *)(SNDi_Work+0x780)+n; }
