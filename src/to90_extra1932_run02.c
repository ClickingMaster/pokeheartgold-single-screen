#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int id,value;u8 pad[12];int index;u8 gap[8];} Record;
int ov96_021F39F0(const Record*,const Record*,int);
#pragma opt_loop_invariants off
int ov96_021F39F0(const Record *records,const Record *current,int kind){int result,i;int index;if(current->id==-1)return 0;index=current->index;result=0;for(i=0;i<12;i++,records++){if(i!=index&&kind==records->id){if(current->value<records->value){if(index/3==records->index/3)result--;else result-=2;}else result--;}}return result;}
