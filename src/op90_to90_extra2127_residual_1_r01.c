// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {CARDResult result;CARDBackupType type;u32 pad[4],capacity,sector,extra,page,address,timing,program,programmax,erase,erasemax,sectorerase,sectormax,field48,field4c,max;u8 status,padding[3];u32 capabilities,last;} Cmd;
void CARDi_IdentifyBackupCore(CARDBackupType type);
// @fn CARDi_IdentifyBackupCore
void CARDi_IdentifyBackupCore(CARDBackupType type) {
 Cmd *p=(Cmd *)cardi_common.cmd; u32 capacity,kind,variant,exponent;
 MI_CpuFill8((u8*)p+0x18,0,0x48);p->type=type;p->capabilities=0x203f;
 if(type==0)return;
 exponent=(type>>8)&255;capacity=1U<<exponent;kind=type&255;variant=(type>>16)&255;p->capacity=capacity;p->status=255;
 if(kind==1){switch(capacity){
 case 0x200:p->page=16;p->address=1;p->timing=5;p->status=0xf0;break;
 case 0x2000:p->page=32;p->address=2;p->timing=5;p->status=0;break;
 case 0x10000:p->page=128;p->address=2;p->timing=10;p->status=0;break;
 case 0x20000:p->page=256;p->address=3;p->timing=5;p->status=0;break;
 default:goto error;}
 p->sector=p->page;p->capabilities|=0x40;p->capabilities|=0x4300;return;}
 else if(kind==2){switch(capacity){
 case 0x40000:case 0x80000:case 0x100000:p->program=25;p->programmax=300;p->max=300;p->sectorerase=5000;p->capabilities|=0x480;break;
 case 0x200000:p->program=23;p->programmax=300;p->sectorerase=500;p->sectormax=5000;p->erase=10000;p->erasemax=60000;p->status=0;p->capabilities|=0x80;p->capabilities|=0x5400;break;
 case 0x400000:p->sectorerase=600;p->sectormax=p->sectorerase+2400;p->field48=70;p->field4c=150;p->erase=23000;p->erasemax=800000;p->status=0;p->extra=4096;p->capabilities|=0xd000;break;
 case 0x800000:if(variant==0){p->sectorerase=1000;p->sectormax=p->sectorerase+2000;p->erase=68000;p->erasemax=160000;p->status=0;p->capabilities|=0x5000;}else if(variant==1){p->sectorerase=1000;p->sectormax=p->sectorerase+2000;p->erase=68000;p->erasemax=160000;p->status=0x84;p->capabilities|=0x5000;}break;
 default:goto error;}
 p->sector=0x10000;p->page=256;p->address=3;p->timing=5;p->capabilities|=0xb40;return;}
 else if(kind==3){if(capacity!=0x2000&&capacity!=0x8000)goto error;p->page=capacity;p->sector=capacity;p->address=2;p->status=0;p->capabilities|=0x40;p->capabilities|=0x4300;return;}
 error:p->capacity=p->type=CARD_BACKUP_TYPE_NOT_USE;cardi_common.cmd->result=CARD_RESULT_UNSUPPORTED;
}
