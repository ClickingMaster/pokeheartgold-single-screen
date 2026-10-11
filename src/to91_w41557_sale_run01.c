#include "global.h"
#include "bag_types_def.h"
#include "player_data.h"
#include "bg_window.h"
#include "text.h"
#pragma force_active on
typedef struct {u8 beforeWindow[0x34];Window window;u8 beforeArgs[0x234-0x44];BagView *args;u32 opaque238;PlayerProfile *profile;u8 beforePrinter[0x616-0x240];u8 printer;u8 beforeQuantity[0x680-0x617];s16 quantity;u16 opaque682;u32 price;u8 remaining[0x94c-0x688];} SaleWork;
typedef char AssertSale[(sizeof(SaleWork)==0x94c)?1:-1];
extern BOOL PlaySE(u16);
extern BOOL Pocket_TakeItem(ItemSlot *,u32,u16,u16,u32);
extern void BagApp_LoadPocketItemNames(void *);
s32 BagApp_State_CompleteSale(void *);
s32 BagApp_State_CompleteSale(void *arg){SaleWork *w=arg;if(TextPrinterCheckActive(w->printer))return 23;PlaySE(0x643);PlayerProfile_AddMoney(w->profile,(u32)(s32)w->quantity*w->price);if(w->quantity==1){if(w->args->unk75!=255)w->args->unk75++;}else w->args->unk75=2;Pocket_TakeItem(w->args->pockets[w->args->unk64].slots,w->args->pockets[w->args->unk64].padding2[0],w->args->itemId,(u16)w->quantity,6);BagApp_LoadPocketItemNames(w);ScheduleWindowCopyToVram(&w->window);return 24;}
