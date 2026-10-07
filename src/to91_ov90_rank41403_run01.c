#include "global.h"
#pragma force_active on
typedef struct {u32 scores[4];u8 ranks[4];} Scores;
void ov90_02258AA8(Scores *,u32);
// @fn ov90_02258AA8
void ov90_02258AA8(Scores *w,u32 count) {
 u8 order[4];int i,j;
 for(i=0;i<count;i++) {
  for(j=i;j>0;j--) {
   if(w->scores[i]<=w->scores[order[j-1]])break;
   order[j]=order[j-1];
  }
  order[j]=i;
 }
 for(i=0;i<count;i++) {
  int rank=i;
  if(i>0 && w->scores[order[i]]==w->scores[order[i-1]])rank=w->ranks[order[i-1]];
  w->ranks[order[i]]=rank;
 }
}
