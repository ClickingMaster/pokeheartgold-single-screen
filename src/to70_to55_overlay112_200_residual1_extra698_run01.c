#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[96];} Entry;
extern Entry *Save_TrainerHouse_Get(void *);extern int TrainerHouseSet_CheckHasData(void *),ov112_021F35A4(Entry *),ov112_021F35C8(Entry *,void *,int *);extern u16 GF_CalcCRC16(void *,int);extern void ov112_021F3608(void *,u8 *),ov112_021F3630(void *);
void ov112_021F328C(void *,u8 *);
void ov112_021F328C(void *save,u8 *incoming){u8 ids[12];volatile struct {int remembered,replaced;} locals;int count,index,shift;locals.remembered=0;locals.replaced=0;shift=locals.remembered;Entry *entries=Save_TrainerHouse_Get(save);if(TrainerHouseSet_CheckHasData(incoming)==0){ov112_021F35A4(entries);return;}if(*(u16*)(incoming+498)==GF_CalcCRC16(incoming,498)){count=ov112_021F35A4(entries);ov112_021F3608(save,ids);index=ov112_021F35C8(entries,incoming,(int*)&locals.replaced);if(count>=10||locals.replaced!=0){shift=1;count--;locals.remembered=ids[index];}if(shift&&index<count){for(;index<count;index++){entries[index]=entries[index+1];ids[index]=ids[index+1];}}MI_CpuCopy8(incoming,&entries[count],384);ids[count]=locals.remembered;ov112_021F3630(save);}}
