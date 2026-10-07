from pathlib import Path
import os,subprocess,tempfile,fcntl,time,hashlib
R=next(p for p in Path(__file__).resolve().parents if (p/'common.mk').is_file())
FLAGS=['-DSDK_ARM9','-DSDK_CODE_ARM','-DSDK_FINALROM','-proc','arm5te','-g','-gccinc','-i','.','-i','./include','-i','./asm/include','-i','./files','-i','./lib/asm/include','-i','./lib/NitroDWC/asm/include','-i','./lib/MSL_C/asm/include','-i','./lib/NitroSDK/asm/include','-i','./lib/syscall/asm/include','-i','./asm','-i','./files/msgdata','-I./lib/include','-DSDK_ASM','-gccdep','-MD']
def assemble(src,out):
 src,out=Path(src).resolve(),Path(out).resolve();before=src.read_bytes();held=None;deadline=time.monotonic()+900
 while held is None:
  for i in range(4 if (R/'.local-tools/to80/full-build-active').exists() else 12):
   f=(R/'.local-tools/to80/compiler-slots'/str(i)).open('a')
   try:fcntl.flock(f,fcntl.LOCK_EX|fcntl.LOCK_NB);held=f;break
   except BlockingIOError:f.close()
  if held is None:assert time.monotonic()<deadline;time.sleep(.2)
 try:
  cmd=['wine',str(R/'tools/mwccarm/2.0/sp2p3/mwasmarm.exe'),*FLAGS,'-o',os.path.relpath(out,R),os.path.relpath(src,R)]
  with tempfile.TemporaryFile(mode='w+')as log:
   r=subprocess.run(cmd,cwd=R,env=dict(os.environ,WINEPREFIX=str(Path.home()/'.wine'),WINEDEBUG='-all',LM_LICENSE_FILE=str(R/'tools/mwccarm/license.dat')),stdout=log,stderr=subprocess.STDOUT,timeout=300)
   if r.returncode:log.seek(0);raise AssertionError(log.read())
 finally:held.close()
 assert src.read_bytes()==before
 return cmd
