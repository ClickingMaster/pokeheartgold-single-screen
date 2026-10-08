#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern u8 wmspW[];
#define STATE P(wmspW+0x1000,0x550)
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void*);
extern void __VENEER_MI_CpuCopy8(const void*,void*,u32);
extern void __VENEER_MIi_CpuCopy16(const void*,void*,u32);
extern void __VENEER_MI_CpuFill8(void*,u32,u32);
extern void __VENEER_MIi_CpuClear16(u32,void*,u32);
extern u16 *WMSP_WL_DevGetStationState(void*);
extern u16 *WMSP_WL_DevClass1(void*);
extern u16 *WMSP_WL_MlmePowerManagement(void*,u16,u16,u16);
extern u16 *WMSP_WL_MlmeScan(void*,u32,void*,u16,void*,u16,void*,u16);
extern int WMSP_SetAllParams(u16,void*);
extern void WmspError_06003ED4(u16,u16,int);
extern u32 WMSP_GetRssi8_06003988(u32);
extern u32 WMSP_GetLinkLevel(u32);
extern void WMSP_AddRssiToRandomPool(u32);
extern u16 *WMSP_WL_ParamSetPreambleType(void*,u16);
extern u32 __VENEER__u32_div_f(u32,u32);
extern u16 *WMSP_WL_ParamSetSsidMask(void*,void*);
typedef struct{u16 prefix[8];void*list[16];u16 levels[16];} w37009_Callback;
typedef struct {u16 state;u8 a[0xc4];u16 power;u32 roles;u8 b[0x1a];u16 role;u8 c[0x9c];void*output;u8 d[8];u16 channel;u8 e[0x5c];u16 preamble;u8 f[4];u16 allowed;} w37009_Status;
static inline int w37009_HasSsid(void*p){u16 len=H(p,10);int i;if(len==0)return 0;if(len>32)return 0;for(i=0;i<len;i++)if(*((u8*)p+12+i)!=0)return 1;return 0;}
typedef struct{u16 command,channels;void*output;u16 capacity,duration;u8 bssid[6];u16 mode,length;u8 ssid[32];u16 matchlen;}ScanRequest;
void WMSP_StartScanEx(ScanRequest *req);
#pragma opt_lifetimes off
// @fn WMSP_StartScanEx
void WMSP_StartScanEx(ScanRequest *req){
 u32 bufArr[278];u8 ssid[32];u8 mask[32];u8 channels[16];u16 bssid[3];
 u32 *buffer=bufArr;w37009_Status*s=STATE;int bits;struct{u16 custom,matchlen;} options;u16 mode,maxduration,capacity,ssidlen,count,j;u16*r,*c;int i;u32 rssi;u8*entry,*dest;u16 bytes;
 if(s->state!=2&&s->state!=3&&s->state!=5){c=__VENEER_WMSP_GetBuffer4Callback2Wm9();c[0]=38;c[1]=3;c[4]=4;__VENEER_WMSP_ReturnResult2Wm9(c);return;}
 s->output=req->output;bits=req->channels;s->channel=bits;maxduration=req->duration;__VENEER_MI_CpuCopy8(req->bssid,bssid,6);mode=req->mode;options.matchlen=req->matchlen;
 switch(mode){case 2:options.custom=1;mode=0;break;case 3:options.custom=1;mode=1;break;default:options.custom=0;break;}
 ssidlen=req->length;__VENEER_MI_CpuCopy8(req->ssid,ssid,32);capacity=req->capacity;if(bssid[0]!=65535&&(bssid[0]&1))bssid[0]&=~1;
 bits=s->allowed&(u16)(bits<<1);
 if(bits && (!(s->roles&1)||mode==1) && req->output && !((u32)req->output&3) && req->capacity>=64) goto valid_scan;
{c=__VENEER_WMSP_GetBuffer4Callback2Wm9();c[0]=38;c[1]=6;c[4]=4;__VENEER_WMSP_ReturnResult2Wm9(c);return;}

valid_scan:
 {u16 role=2;s->role=role;}
 r=WMSP_WL_DevGetStationState(buffer);if(!r[2]) goto station_ready;WmspError_06003ED4(0x308,r[2],1);return;
station_ready:;
 if(r[3]==16){if(!WMSP_SetAllParams(38,buffer))return;r=WMSP_WL_DevClass1(buffer);if(r[2]){WmspError_06003ED4(0x303,r[2],1);return;}s->state=3;r=WMSP_WL_MlmePowerManagement(buffer,1,0,1);if(r[2]){WmspError_06003ED4(1,r[2],1);return;}s->power=1;}
 if(mode==0){if(s->preamble==1){r=WMSP_WL_ParamSetPreambleType(buffer,0);if(r[2]){WmspError_06003ED4(0x20e,r[2],1);return;}s->preamble=0;}}else if(s->preamble==0){r=WMSP_WL_ParamSetPreambleType(buffer,1);if(r[2]){WmspError_06003ED4(0x20e,r[2],1);return;}s->preamble=1;}
 if(options.custom!=1)goto mask_ready;{__VENEER_MI_CpuFill8(mask,255,32);if(options.matchlen<=32)__VENEER_MI_CpuFill8(mask,0,options.matchlen);r=WMSP_WL_ParamSetSsidMask(buffer,mask);if(r[2]){WmspError_06003ED4(0x20d,r[2],1);return;}}
mask_ready:
 count=0;s->state=5;__VENEER_MI_CpuFill8(channels,0,16);for(j=1;j<15;j++)if(bits&(1u<<j))channels[count++]=j;
 r=WMSP_WL_MlmeScan(buffer,capacity-2*__VENEER__u32_div_f((u32)(capacity-64),66)+94,bssid,ssidlen,ssid,mode,channels,maxduration);if(r[2]){WmspError_06003ED4(2,r[2],1);return;}
 c=__VENEER_WMSP_GetBuffer4Callback2Wm9();if(r[4]==0){c[0]=38;c[1]=0;c[4]=4;c[7]=0;c[5]=bits>>1;}else{
 dest=s->output;entry=(u8*)r+10;__VENEER_MIi_CpuClear16(0,dest,capacity);
 for(i=0;i<r[4];i++){bytes=(u16)(H(entry,0)*2);__VENEER_MI_CpuCopy8(entry,dest,bytes);if(ssidlen!=0&&!w37009_HasSsid(dest)){H(dest,10)=ssidlen;__VENEER_MI_CpuCopy8(ssid,dest+12,32);}((w37009_Callback*)c)->list[i]=dest;rssi=WMSP_GetRssi8_06003988(H(entry,2)&255);((w37009_Callback*)c)->levels[i]=WMSP_GetLinkLevel(rssi);WMSP_AddRssiToRandomPool(rssi);dest+=bytes;entry+=bytes;if((u32)dest&2)dest=(u8*)(((u32)dest+2)&~3u);}
 c[0]=38;c[1]=0;c[4]=5;c[7]=r[4];c[5]=bits>>1;}
 __VENEER_WMSP_ReturnResult2Wm9(c);
}
