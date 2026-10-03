#pragma force_active on
#include "global.h"
typedef struct {u16 id;u16 pad;u32 data[7];} Entry;
extern int ov96_021E7D18(u16,u16,void*);
void ov96_021E7C04(void*arg,Entry*src,Entry*entries);
void ov96_021E7C04(void*arg,Entry*src,Entry*entries){int i;Entry*p;Entry temp;if(ov96_021E7D18(entries[4].id,src->id,arg)){entries[4]=*src;i=4;p=&entries[4];goto loop;loop: {if(!ov96_021E7D18(p[-1].id,p->id,arg))goto finished;temp=p[-1];p[-1]=*p;*p=temp;i--;p--;if(i>0)goto loop;}}finished:; }
