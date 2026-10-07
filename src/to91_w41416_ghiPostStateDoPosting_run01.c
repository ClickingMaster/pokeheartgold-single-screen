// clang-format off
#include "global.h"
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
extern int sprintf(char*,const char*,...);
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{int kind;char*name;u32 p8;char*diskName,*typeName,*memoryType;}Item;
typedef struct{Item*item;int state;}Post;
typedef struct{u8 p[12];int multipart;}Config;
typedef struct{u8 p[0x13c];Config*config;}Connection;
extern char ov38_022258A8[],ov38_02225644[],ov38_0222555C[],ov38_022258C8[],ov38_022258CC[],ov38_022258D4[],ov38_022258FC[],ov38_02225928[],ov38_02225958[],ov38_02225640[],ov38_022259AC[];
extern int ov38_0221CE0C(Connection*,const char*,int),ghiPostStringStateDoPosting(Post*,Connection*),ghiPostFileDiskStateDoPosting(Post*,Connection*),ghiPostFileMemoryStateDoPosting(Post*,Connection*);
int ghiPostStateDoPosting(Post*,Connection*,int);
int ghiPostStateDoPosting(Post*post,Connection*connection,int first){char buffer[2048];int status;if(post->state==-1){post->state=0;if(connection->config->multipart==0){if(post->item->kind!=0)__msl_assertion_failed(ov38_022258A8,ov38_02225644,ov38_0222555C,937);if(first)sprintf(buffer,ov38_022258C8,post->item->name);else sprintf(buffer,ov38_022258CC,post->item->name);}else if(post->item->kind==0){sprintf(buffer,ov38_02225928,first?ov38_022258D4:ov38_022258FC,post->item->name);}else if(post->item->kind==1||post->item->kind==2){char*filename,*type;if(post->item->kind==1){filename=post->item->diskName;type=post->item->typeName;}else{filename=post->item->typeName;type=post->item->memoryType;}sprintf(buffer,ov38_02225958,first?ov38_022258D4:ov38_022258FC,post->item->name,filename,type);}else{__msl_assertion_failed(ov38_02225640,ov38_02225644,ov38_0222555C,988);}status=ov38_0221CE0C(connection,buffer,strlen(buffer));if(status==0)return 0;if(status==2)return 2;}if(post->item->kind==0)return ghiPostStringStateDoPosting(post,connection);if(post->item->kind==1)return ghiPostFileDiskStateDoPosting(post,connection);if(post->item->kind!=2)__msl_assertion_failed(ov38_022259AC,ov38_02225644,ov38_0222555C,1013);return ghiPostFileMemoryStateDoPosting(post,connection);}
