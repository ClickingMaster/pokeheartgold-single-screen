#include "global.h"
#include "party.h"
#include "pokemon.h"
#pragma force_active on
typedef struct{u8 prefix[0x11];u8 count;u8 beforeParties[0x3c0-0x12];Party *party,*otherParty;u16 selected[6];u16 *selectedOutput;u8 beforePacket[8];u16 packet[60];u8 remaining[0x48c-0x458];}Owner;
typedef char OwnerSize[(sizeof(Owner)==0x48c)?1:-1];
void ov81_0224102C(void*,int,void*);
void ov81_0224102C(void *argument,int kind,void *packetArgument){Owner *w=argument;int i=0,n=i;for(;i<60;i++)w->packet[i]=0;w->packet[0]=w->count;n++;for(i=0;i<w->count;i++)w->packet[n+i]=(u16)GetMonData(Party_GetMonByIndex(w->party,w->selected[i]),MON_DATA_SPECIES,NULL);n+=w->count;for(i=0;i<w->count;i++)w->packet[n+i]=(u16)GetMonData(Party_GetMonByIndex(w->party,w->selected[i]),MON_DATA_GENDER,NULL);}
