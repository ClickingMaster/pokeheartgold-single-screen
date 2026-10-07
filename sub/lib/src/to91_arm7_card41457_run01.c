#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct Common {u8 prefix[0x20];u32 len,dma,request;s32 retry;u32 mode;u32 reserved[2];void (*callback)(void *);void *argument;void (*task)(struct Common *);u8 thread[0xec-0x48];void *wake;u32 priority,queue[2];volatile u32 flags;} Common;
typedef struct CARDRomStat {void (*read_func)(void *);u32 ctrl;u8 *cache_page;u32 dummy[5];u8 cache_buf[512];} CARDRomStat;
extern Common cardi_common;extern CARDRomStat rom_stat;extern u32 cardi_rom_base;
void CARDi_ReadCard(void *);void CARDi_InitCommon(void);void CARD_InitPulledOutCallback(void);
void CARD_Init(void);
void CARD_Init(void){Common *p=&cardi_common;if(p->flags==0){p->flags=1;p->request=0;p->dma=0;p->len=0;p->retry=-1;p->callback=0;p->argument=0;cardi_rom_base=0;CARDi_InitCommon();rom_stat.read_func=CARDi_ReadCard;CARD_InitPulledOutCallback();}}
