#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35220Track { u8 active:1; u8 flag1:1; u8 mute:1; u8 other:5; u8 mod; u8 b2,b3; u8 volume,volume2; s8 bend; u8 range; s8 pan,pan2; s16 extvol,pitch; u8 rest[10]; u16 mod1,mod2,mod3; u8 rest2[30]; struct W35220Channel *channels; } W35220Track;
typedef struct W35220Player { u8 active:1; u8 flags:7; u8 number,b2,b3,b4,volume; s16 extvol; u8 tracks[16]; } W35220Player;
typedef struct W35220Channel { u8 b0,b1,type,b3,mod,key; s16 extvol; u8 note,velocity; s8 pan,pan2; s16 volume,pitch; u8 rest[18]; u8 release; u8 rest2[5]; u16 mod1,mod2,mod3; u8 rest3[6]; void *callback; u8 rest4[24]; struct W35220Channel *next; } W35220Channel;
extern u8 SNDi_Work[];
extern void SND_FreeExChannel(W35220Channel *);
void ChannelCallback(W35220Channel *ch,int event,W35220Track *t);
// @fn ChannelCallback
void ChannelCallback(W35220Channel *ch,int event,W35220Track *t) { W35220Channel *p; if(event==1){ch->release=0;SND_FreeExChannel(ch);} p=t->channels; if(p==ch){t->channels=ch->next;return;} while(p->next){if(p->next==ch){p->next=ch->next;break;}p=p->next;} }
