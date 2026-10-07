#pragma force_active on
#include <nitro/types.h>
#include <nitro/SND_bank_shared.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void SNDi_LockMutex(void),SNDi_UnlockMutex(void);
int SND_ReadInstData(struct SNDBankData*,int,int,struct SNDInstData*);
int SND_ReadInstData(struct SNDBankData *bank,int index,int key,struct SNDInstData *out){int region;u8 high,low;u32 offset,word;const struct SNDDrumSet *drum;const struct SNDKeySplit *split;
if(index<0)return 0;SNDi_LockMutex();if((u32)index>=bank->instCount){SNDi_UnlockMutex();return 0;}word=bank->instOffsets[index];out->type=word;offset=word>>8;
switch(out->type){
case 1:case 2:case 3:case 4:case 5:out->param=*(const struct SNDInstParam *)((const u8*)bank+offset);break;
case 16:{drum=(const struct SNDDrumSet *)((const u8*)bank+offset);low=drum->minKey;high=drum->maxKey;if(key<low||key>high){SNDi_UnlockMutex();return 0;}*out=drum->instruments[key-low];}break;
case 17:{region=0;split=(const struct SNDKeySplit *)((const u8*)bank+offset);while(key>split->key[region]){if(++region>=8){SNDi_UnlockMutex();return 0;}}*out=split->instruments[region];}break;
case 0:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:default:SNDi_UnlockMutex();return 0;}SNDi_UnlockMutex();return 1;}
