#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x18];u8 gender[4];u8 kind;} Trainer;
typedef struct{u8 pad[0x80];Trainer trainer;} Record;
typedef struct{u8 pad[0x110];Record*records[10];} State;
extern int _020FC7FC[],_020FC814[][2],_020FC8D4[][2],_020FC804[][2],_020FC8B4[][2],_020FC8F4[][2],_020FC914[][2];
extern int sub_0205A51C(int);
extern void*sub_02035798(int);
extern int PlayerProfile_GetTrainerGender(void*);
extern u16 LCRandom(void);
int sub_0205A544(State*,int);
int sub_0205A544(State*s,int slot){int index,gender;Trainer*t;void*profile;if(slot>9){index=sub_0205A51C(slot);GF_ASSERT(slot!=-1);}else index=slot;if(s->records[index]==0)return 40;profile=sub_02035798(index);t=(Trainer*)((u8*)s->records[index]+0x50);t=(Trainer*)((u8*)t+0x30);if(profile==0)return 40;if(slot>9)gender=t->gender[(slot-10)%4]>>7;else gender=PlayerProfile_GetTrainerGender(profile);switch(t->kind){case 4:case 11:return _020FC7FC[gender];case 5:return _020FC814[(s32)LCRandom()%2][gender];case 6:return _020FC8D4[(s32)LCRandom()%4][gender];case 7:return _020FC804[(s32)LCRandom()%2][gender];case 1:case 8:return _020FC8B4[(s32)LCRandom()%4][gender];case 2:case 9:return _020FC8F4[(s32)LCRandom()%4][gender];case 3:case 10:case 12:case 13:return _020FC914[(s32)LCRandom()%4][gender];}return 40;}
