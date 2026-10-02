#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
struct File {u32 handle;struct {unsigned pad:2,access:3,pad5:2,kind:3,orientation:2,rest:20;}mode;struct {unsigned io:3,flag:1,rest:28;}state;u8 padC,error;u8 padE[10];u32 count;u8 pad1C[12];u32 left;u8 pad2C[24];int (*close)(u32);};
extern int __flush_all(void);
extern int __flush_buffer(struct File *,void *);
int fclose(struct File *);
int fflush(struct File *);
int fclose(struct File *file) {int flush,close;if(!file)return -1;if(file->mode.kind==0)return 0;flush=fflush(file);close=file->close(file->handle);file->mode.kind=0;file->handle=0;if(file->state.flag)return -1;return -(flush!=0 || close!=0);}
int fflush(struct File *file) {if(!file)return __flush_all();if(file->error || file->mode.kind==0)return -1;if(file->mode.access==1)return 0;if(file->state.io>=3)file->state.io=2;if(file->state.io==2)file->left=0;if(file->state.io!=1){file->state.io=0;return 0;}if(__flush_buffer(file,0)!=0){file->error=1;file->left=0;return -1;}file->state.io=0;file->count=0;file->left=0;return 0;}
