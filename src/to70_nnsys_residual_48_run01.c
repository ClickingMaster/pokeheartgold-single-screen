#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Player {u8 data[0x24];} Player;
extern Player _021DFDC4[];
void *NNS_FndGetNextListObject(void*,void*);void NNSi_SndPlayerPause(void*,int);void NNS_SndPlayerPauseByPlayerNo(int,int);
void NNS_SndPlayerPauseByPlayerNo(int id,int pause){void *handle=NNS_FndGetNextListObject(&_021DFDC4[id],0);while(handle){void *next=NNS_FndGetNextListObject(&_021DFDC4[id],handle);NNSi_SndPlayerPause(handle,pause);handle=next;}}
