#pragma force_active on
#include "global.h"
#pragma force_active on
typedef void (*Status)(int);typedef void (*Error)(void*,u32,u32);
typedef struct{u8*state;Error error;} Global;extern Global ov74_0223E2FC;extern u8 ov74_0223D018[];
extern void ov74_022361B8(int),ov74_02236168(int),ov74_022366E8(void*),ov74_022365FC(void);extern int ov74_0223648C(void),ov74_02236768(void*),ov74_02236258(void);
#define U(o) (*(u32*)(ov74_0223E2FC.state+(o)))
#define S(o) (*(u16*)(ov74_0223E2FC.state+(o)))
#define CB (*(Status*)(ov74_0223E2FC.state+0x117c))
#define H(o) (*(u16*)(packet+(o)))
void ov74_02236354(u8*);
void ov74_02236354(u8*packet){if(H(2)){ov74_022361B8(H(2));ov74_02236168(9);if(CB)CB(4);return;}if(U(0x1150)!=2){if(ov74_0223648C()==0){ov74_02236168(9);if(CB)CB(4);}return;}switch(H(8)){case 3:return;case 4:break;case 5:DC_InvalidateRange(ov74_0223E2FC.state+0xf00,192);if(H(0x36)<8||*(u32*)(packet+0x3c)!=0x400318){if(ov74_0223E2FC.error)ov74_0223E2FC.error(ov74_0223D018,*(u32*)(packet+0x3c),0x400318);break;}U(0x116c)=H(0x12);if(S(0x1158)==2){int i;ov74_022366E8(packet+0x48);for(i=0;i<6;i++)ov74_0223E2FC.state[0x1170+i]=packet[10+i];S(0x1176)-=15;if(CB)CB(1);}if(ov74_02236768(packet+0x48)){S(0x1158)=4;ov74_022365FC();return;}break;default:break;}if(!ov74_02236258()){ov74_02236168(9);if(CB)CB(4);}}
