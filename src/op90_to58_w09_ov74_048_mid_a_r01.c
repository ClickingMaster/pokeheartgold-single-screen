#include "global.h"
typedef struct {void *buffer;u16 channel,period;u8 address[6];u16 padding;} Scan;typedef struct {u8 prefix[0xf00];u8 buffer[192];Scan scan;u8 gap[0x1150-0xfd0];s32 state;u32 pad;u16 mode;u8 gap2[0x1170-0x115a];u8 address[6];u16 index;} Work;
extern Work *ov74_0223E2FC;int WM_GetAllowedChannel(void),WM_StartScan(void *,void *);void ov74_022361B8(int),ov74_02236354(void);int ov74_02236258(void);
#pragma force_active on
#pragma push
#pragma opt_rotateloops off
#pragma opt_common_subs off
#pragma opt_propagation off
#pragma opt_dead_assignments off
// @fn ov74_02236258
int ov74_02236258(void){int channels,index,i;(void)((ov74_0223E2FC->state==2)||(OS_Terminate(),0));channels=WM_GetAllowedChannel();if(channels==0x8000){ov74_022361B8(3);return 0;}if(channels==0){ov74_022361B8(22);return 0;}index=ov74_0223E2FC->index;if((u32)index>=16){index-=16;for(i=0;i<13;i++){index=(index+1)%13;if(channels&(1<<index))break;}ov74_0223E2FC->index=index+16;ov74_0223E2FC->scan.channel=index+1;}else ov74_0223E2FC->scan.channel=index;ov74_0223E2FC->scan.period=220;ov74_0223E2FC->scan.buffer=ov74_0223E2FC->buffer;if(ov74_0223E2FC->mode==3)for(i=0;i<6;i++)ov74_0223E2FC->scan.address[i]=ov74_0223E2FC->address[i];int result=WM_StartScan(ov74_02236354,&ov74_0223E2FC->scan);if(result!=2){ov74_022361B8(result);return 0;}return 1;}

#pragma pop
