#pragma force_active on
#include "global.h"
typedef union {u32 raw;struct {u32 pad:26;u32 flag26:1;u32 pad2:1;u32 flag28:1;u32 rest:3;} bits;} Flags;
extern u8 *PokeathlonCourse_GetHeapAllocPtr4(void *);
void ov96_02215EE8(void *,int,s16 *);void ov96_02215F2C(void *,int);
void ov96_02215EE8(void *course,int n,s16 *pos){u8 *w=PokeathlonCourse_GetHeapAllocPtr4(course);Flags *flags=(Flags *)(w+0x204);u32 offset=n*168;GF_ASSERT(((Flags *)((u8 *)flags+offset))->bits.flag28==0);((Flags *)((u8 *)flags+offset))->raw|=1<<30;*(s16 *)(w+offset+0x1c0)=pos[0];*(s16 *)(w+offset+0x1c2)=pos[1];}
void ov96_02215F2C(void *course,int n){u8 *w=PokeathlonCourse_GetHeapAllocPtr4(course);Flags *flags=(Flags *)(w+0x204);u32 offset=n*168;GF_ASSERT(((Flags *)((u8 *)flags+offset))->bits.flag28==0);GF_ASSERT(((Flags *)((u8 *)flags+offset))->bits.flag26==0);((Flags *)((u8 *)flags+offset))->raw|=1<<29;}
