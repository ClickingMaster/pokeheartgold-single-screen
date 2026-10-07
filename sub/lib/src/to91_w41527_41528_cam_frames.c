#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W40827Frame{u16 type,index;}W40827Frame;
typedef struct W40827Entry{u8 reserved[22];u16 frameCount,timeout,lifetime;}W40827Entry;
typedef struct W40827Work{u8 reserved0[0x31c];W40827Entry*entries;u8 reserved1[0x350-0x320];u16 role;u8 reserved2[0x534-0x352];u16 powerSave;}W40827Work;
#define W40827_ROOT (*(W40827Work**)0x0380fff4)
extern u32 __VENEER_OS_DisableIrqMask(u32),__VENEER_OS_EnableIrqMask(u32);extern void CAM_SetTIMElementBitmap(u32);
typedef char W40827AssertEntry[(sizeof(W40827Entry)==28)?1:-1];
void CAM_IncFrameCount(void*);
typedef struct {u16 words[11];u16 frames;u16 rest[2];} Station;
typedef struct {u8 prefix[0x31c];Station *stations;u8 gap[0x350-0x320];u16 mode;} State;
typedef struct {u16 first,index;} Frame;
#define ROOT (*(State **)0x0380fff4)
void CAM_ClrTIMElementBitmap(u32);
void CAM_DecFrameCount(void *);
#pragma force_active off
static inline void RemoveStationFrame09(Station *entry,u32 index){if(ROOT->mode==1&&entry->frames==1)CAM_ClrTIMElementBitmap(index);entry->frames--;}
#pragma force_active on
void CAM_IncFrameCount(void*raw){u32 irq,index;W40827Entry*entry;W40827Frame*frame;frame=raw;index=frame->index;entry=&W40827_ROOT->entries[index];irq=__VENEER_OS_DisableIrqMask(0x1000000);if(W40827_ROOT->role==1&&entry->frameCount==0)CAM_SetTIMElementBitmap(index);entry->frameCount++;__VENEER_OS_EnableIrqMask(irq);if(!(W40827_ROOT->powerSave&(1U<<index)))entry->timeout=entry->lifetime;}

void CAM_DecFrameCount(void *raw){u32 mask,index;Station *entry;Frame *frame;frame=raw;index=frame->index;entry=ROOT->stations+index;mask=__VENEER_OS_DisableIrqMask(0x1000000);RemoveStationFrame09(entry,index);__VENEER_OS_EnableIrqMask(mask);}
