#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <nnsys/g3d/binres/res_struct.h>
// clang-format on
#pragma force_active on
extern void *NNS_G3dGetResDataByName(const NNSG3dResDict*,const NNSG3dResName*);
extern int NNS_G3dGetResDictIdxByName(const NNSG3dResDict*,const NNSG3dResName*);
static inline const NNSG3dResName *GetName(const NNSG3dResDict*,u32);
static inline const NNSG3dResName *GetName(const NNSG3dResDict *dict,u32 idx){if(dict&&idx<dict->numEntry){NNSG3dResDictEntryHeader *h=(NNSG3dResDictEntryHeader*)((u8*)dict+dict->ofsEntry);return (const NNSG3dResName*)((u8*)h+h->ofsName)+idx;}return 0;}
static inline void *GetData(const NNSG3dResDict*,u32);
static inline void *GetData(const NNSG3dResDict *d,u32 i){if(d&&i<d->numEntry){NNSG3dResDictEntryHeader *h=(NNSG3dResDictEntryHeader*)((u8*)d+d->ofsEntry);return h->data+h->sizeUnit*i;}return 0;}
void *NNS_G3dGetResDataByName(const NNSG3dResDict *dict,const NNSG3dResName *name){u32 idx;if(!name)return 0;if(dict->numEntry<16){u32 key0=name->val[0],key1=name->val[1],key2=name->val[2],key3=name->val[3];for(idx=0;idx<dict->numEntry;idx++){const NNSG3dResName *found=GetName(dict,idx);if(found->val[0]==key0&&found->val[1]==key1&&found->val[2]==key2&&found->val[3]==key3)return GetData(dict,idx);}}else{const NNSG3dResDictTreeNode *base=dict->node,*root=base,*node,*next;u32 child=root->idxLeft;if(child){next=base+child;while(root->refBit>next->refBit){s32 bit=next->refBit;u32 value=(name->val[bit>>5]>>(bit&31))&1;node=next;child=*((const u8*)((u32)node+value)+1);next=base+child;root=node;}idx=next->idxEntry;{const NNSG3dResName *found=GetName(dict,idx);if(found->val[0]==name->val[0]&&found->val[1]==name->val[1]&&found->val[2]==name->val[2]&&found->val[3]==name->val[3])return GetData(dict,idx);}}}return 0;}
int NNS_G3dGetResDictIdxByName(const NNSG3dResDict *dict,const NNSG3dResName *name){u32 idx;if(!name)return -1;if(dict->numEntry<16){u32 key0=name->val[0],key1=name->val[1],key2=name->val[2],key3=name->val[3];for(idx=0;idx<dict->numEntry;idx++){const NNSG3dResName *found=GetName(dict,idx);if(found->val[0]==key0&&found->val[1]==key1&&found->val[2]==key2&&found->val[3]==key3)return idx;}}else{const NNSG3dResDictTreeNode *base=dict->node,*root=base,*node,*next;u32 child=root->idxLeft;if(child){next=base+child;while(root->refBit>next->refBit){s32 bit=next->refBit;u32 value=(name->val[bit>>5]>>(bit&31))&1;node=next;child=*((const u8*)((u32)node+value)+1);next=base+child;root=node;}idx=next->idxEntry;{const NNSG3dResName *found=GetName(dict,idx);if(found->val[0]==name->val[0]&&found->val[1]==name->val[1]&&found->val[2]==name->val[2]&&found->val[3]==name->val[3])return idx;}}}return -1;}
