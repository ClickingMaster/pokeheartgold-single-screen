#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
struct Control {void (**vtable)(void *);int count,weak;void *pointer;};
static inline void Dispose(struct Control *x) {x->vtable[3](x);}
struct Deleter {u8 value;};
struct Pair {void *pointer;struct Control *control;};
struct Move {void *pointer;struct Deleter *deleter;};
extern void *_Znwm(u32);
extern u8 _02111540[],_02111558[];
void *_ZN10Metrowerks8move_ptrIcRNSt6detail14default_deleteIA0_cEEED1Ev(void *);
struct Pair *_ZNSt3tr110shared_ptrIcEC1IcNSt6detail14default_deleteIA0_cEEEEPT_T0_(struct Pair *,void *,struct Deleter);
extern void _ZdlPv(void *);
extern void _ZdaPv(void *);
extern int _ZNKSt9type_infoeqERKS_(const void *,const void *);
extern u8 _02111514[];
extern void *_ZNSt3tr110shared_ptrIcED1Ev(void *);
void *_ZNSt9exceptionD2Ev(void *x);
void *_ZNSt9exceptionD0Ev(void *x);
void *_ZNSt9exceptionD1Ev(void *x);
void *_ZNKSt3tr16detail18shared_ptr_deleterIcNSt6detail14default_deleteIA0_cEEE11get_deleterERKSt9type_info(void *x,const void *type);
void _ZNSt3tr16detail18shared_ptr_deleterIcNSt6detail14default_deleteIA0_cEEE7disposeEv(struct Control *x);
void *_ZN10Metrowerks8move_ptrIcRNSt6detail14default_deleteIA0_cEEED1Ev(void *x);
void *_ZNSt3tr16detail18shared_ptr_deleterIcNSt6detail14default_deleteIA0_cEEED0Ev(void *x);
void *_ZNSt3tr16detail18shared_ptr_deleterIcNSt6detail14default_deleteIA0_cEEED1Ev(void *x);
void *_ZNSt3tr16detail25shared_ptr_deleter_commonD2Ev(struct Control *x);
void *_ZNSt11logic_errorD1Ev(void *x);
void *_ZNSt11logic_errorD0Ev(void *x);
void *_ZNSt12length_errorD1Ev(void *x);
void *_ZNSt12length_errorD0Ev(void *x);
void _ZNSt3tr16detail25shared_ptr_deleter_common7releaseEv(struct Control *x);
void _ZNSt3tr16detail25shared_ptr_deleter_common12release_weakEv(struct Control *x);
void *_ZNSt9exceptionD2Ev(void *x) {return x;}
struct Pair *_ZNSt3tr110shared_ptrIcEC1IcNSt6detail14default_deleteIA0_cEEEEPT_T0_(struct Pair *self,void *pointer,struct Deleter deleter) {struct Move temp;struct Control *control;self->pointer=pointer;temp.pointer=pointer;temp.deleter=&deleter;control=_Znwm(16);if(control) {control->vtable=(void (**)(void *))_02111540;control->count=1;control->weak=1;control->vtable=(void (**)(void *))_02111558;control->pointer=pointer;}self->control=control;temp.pointer=0;_ZN10Metrowerks8move_ptrIcRNSt6detail14default_deleteIA0_cEEED1Ev(&temp);return self;}
void *_ZNSt9exceptionD0Ev(void *x) {_ZdlPv(x);return x;}
void *_ZNSt9exceptionD1Ev(void *x) {return x;}
void *_ZNKSt3tr16detail18shared_ptr_deleterIcNSt6detail14default_deleteIA0_cEEE11get_deleterERKSt9type_info(void *x,const void *type) {return _ZNKSt9type_infoeqERKS_(type,_02111514)?(u8 *)x+12:0;}
void _ZNSt3tr16detail18shared_ptr_deleterIcNSt6detail14default_deleteIA0_cEEE7disposeEv(struct Control *x) {_ZdaPv(x->pointer);}
