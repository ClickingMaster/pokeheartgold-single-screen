#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Course Course;
typedef int (*Step)(Course *,u8 *);
typedef struct {Step *steps;u8 pad,state,index,pad2;} Info;
typedef struct {void *a,*b,*c;void (*exit)(Course *,int);} Callbacks;
struct Course {u8 pad[0x1e0];Callbacks *callbacks;void *heap4;u8 pad2[0x1ed-0x1e8];u8 flag;u8 pad3[0x3b4-0x1ee];Info *current;u8 pad4[8];int value;Info first,next;};
extern Callbacks ov96_0221A8BC;extern Step ov96_0221DA68[];
extern void PokeathlonCourse_InitStateInfo(Step *,Info *),PokeathlonCourse_SetField1F4(Course *,int),PokeathlonCourse_SetStateField07(Course *,int),GF_AssertFail(void);extern int PokeathlonCourse_RunSubStateLoop(Course *);
int ov96_021E6870(Course *,u8 *);
int ov96_021E6870(Course *s,u8 *state){switch(*state){case 0:s->callbacks=&ov96_0221A8BC;s->flag=0;PokeathlonCourse_InitStateInfo(ov96_0221DA68,&s->next);(*state)++;break;case 1:if(PokeathlonCourse_RunSubStateLoop(s))(*state)++;break;case 2:if(s->next.steps==0){(*state)++;break;}if(s->current==&s->first){s->current=&s->next;s->value=0;}if(s->next.steps[s->next.index](s,&s->next.state)){PokeathlonCourse_SetField1F4(s,0);s->current=&s->first;s->value=0;(*state)++;}break;case 3:s->callbacks->exit(s,0);if(s->heap4)GF_AssertFail();PokeathlonCourse_SetStateField07(s,3);break;}return 0;}
