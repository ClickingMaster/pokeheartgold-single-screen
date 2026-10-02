#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work{u8 pad0[0x1f];u8 count;u8 list[0x24];void*msg;u8 pad48[8];void*string;u8 pad54[0x24];u8 windows[2][16];u8 pad98[0x2c];void*sprites[14];u8 padfc[0x14];u8 top[16];u8 pad120[32];u8 bottom[16];}Work;
extern void ov113_021E6238(Work*,void*,int),ManagedSprite_SetDrawFlag(void*,int),ov113_021E6AE8(Work*,int,u8),ReadMsgDataIntoString(void*,int,void*),ov113_021E629C(void*,void*,u16,u16);void ov113_021E6084(Work*,int);
void ov113_021E6084(Work*w,int page){ov113_021E6238(w,w->bottom,6);ov113_021E6238(w,w->top,2);void*right;void*left;int offset;Work*walk;int i;int y;offset=14*page;right=w->windows[1];i=0;left=w;walk=w;y=0;left=(u8*)left+0x78;for(;i<14;i++,walk=(Work*)((u8*)walk+4),y+=24){int index=offset+i;if(index>=w->count)ManagedSprite_SetDrawFlag(walk->sprites[0],0);else{ManagedSprite_SetDrawFlag(walk->sprites[0],1);ov113_021E6AE8(w,i,w->list[index]);ReadMsgDataIntoString(w->msg,w->list[index]+20,w->string);if(i<7)ov113_021E629C(left,w->string,48,(u16)y);else ov113_021E629C(right,w->string,48,(u16)(y-168));}}}
