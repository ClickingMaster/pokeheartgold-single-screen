#pragma force_active on
#include "global.h"
#include "mystery_gift.h"
#include "save.h"
#include "save_arrays.h"
#pragma force_active on
typedef struct Ov74CardInfo {
 u8 pad00[0x48];u32 flags;u16 id;
 union {u8 raw;struct {u8 bit0:1;u8 bit1:1;u8 bit2:1;u8 bit3:1;u8 bit4:1;u8 bit5:1;u8 bit6:1;u8 bit7:1;} bits;} attr;
} Ov74CardInfo;
int ov74_0222FD98(SaveData*,Ov74CardInfo*);
int ov74_0222FD98(SaveData *save,Ov74CardInfo *gift){MysteryGiftSave *mg=Save_MysteryGift_Get(save);u32 mask=gift->flags;if(mask==~0U&&gift->id==0xffff){Save_MysteryGift_Init(mg);Save_NowWriteFile_AfterMGInit(save,0);OS_ResetSystem(0);}if(gift->id>=100&&gift->id<=152)mask|=0x1180;if(mask==0)mask=~0U;
#ifdef HEARTGOLD
if(!(mask&128))return 1;
#else
if(!(mask&256))return 1;
#endif
if(gift->attr.bits.bit0==1&&SaveMysteryGift_ReceivedFlagTest(mg,gift->id)==1)return 2;if(gift->attr.bits.bit2==1&&SaveMysteryGift_CardFindAvailable(mg)==0)return 4;if(SaveMysteryGift_FindAvailable(mg)==0)return 3;if(gift->attr.bits.bit5==1)return 5;return 0;}
