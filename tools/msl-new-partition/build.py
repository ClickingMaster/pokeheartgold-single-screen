"""Finite scalar/array-new source producer; raw compiler product retained, child is last commit record."""
from pathlib import Path
import sys,os,json,hashlib,shlex,subprocess,signal,tempfile,importlib.util
sys.dont_write_bytecode=True
HOME=Path(__file__).resolve().parent
ROOT=next(p for p in HOME.parents if (p/'common.mk').is_file())
sys.path.insert(0,str(HOME));import shape
def selector_for(key):
 spec=importlib.util.spec_from_file_location('selector_'+key,HOME/('select-'+key+'.py'));selector=importlib.util.module_from_spec(spec);spec.loader.exec_module(selector);return selector
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def config(key):
 c=json.loads((HOME/'profile.json').read_text())
 for rel,pin in c['modules'].items():assert digest(HOME/rel)==pin,('tool drift',rel)
 for rel,pin in c['headers'].items():assert digest(ROOT/rel)==pin,('header drift',rel)
 assert digest(ROOT/c['compiler'])==c['compiler_sha256'],'compiler drift'
 entry=c['producers'][key];entry.update(headers=c['headers'],modules=c['modules'],compiler=c['compiler'],compiler_sha256=c['compiler_sha256']);return entry
def paths(obj):return obj.with_suffix('.compiler-full.o'),obj.with_suffix('.compiler-full.d'),obj.with_suffix('.d'),obj.with_suffix('.receipt.json')
def remove(path):
 try:path.unlink()
 except FileNotFoundError:pass
def atomic(path,data):
 fd,name=tempfile.mkstemp(prefix=path.name+'.',suffix='.tmp',dir=path.parent)
 try:
  with os.fdopen(fd,'wb')as f:f.write(data);f.flush();os.fsync(f.fileno())
  os.replace(name,path)
 finally:
  if Path(name).exists():Path(name).unlink()
def dependencies(md):
 text=md.read_text();target,tail=text.split(': ',1)
 # Colon in Win32 include paths is not the target separator. Exact original dependency closure is required.
 names=[x.strip().removesuffix('\\').strip().replace('\\','/')for x in tail.splitlines()]
 result=[]
 for x in names:
  if not x:continue
  if x.startswith('Z:'):x=x[2:]
  result.append(str(Path(x).resolve()))
 return target,sorted(result)
def publish(source,obj,game,nodep,command,c,key):
 selector=selector_for(key)
 full,md,dep,receipt=paths(obj);raw=full.read_bytes()
 expected=json.loads((HOME/(key+'-shape.json')).read_text())
 assert shape.inventory(raw,source)==expected,'complete producer section/symbol/relocation/payload drift'
 child,report=selector.select(raw,source)
 # Independent retained payload/endpoint checks and full child rederivation are mandatory per build.
 assert selector.select(raw,source)==(child,report)
 sh,ss,sn,st,sy,yn=shape.parse(child)
 assert [(sn[i],s[5])for i,s in enumerate(ss)if s[2]&2]==[('.text',size)for name,size in c['functions']]
 assert [(n,y[1],y[2],y[3],y[4])for n,y in zip(yn,sy)if y[5]and y[3]>>4==1]==[(name,0,size,18,0)for name,size in c['functions']]
 if nodep:
  assert not md.exists(),'NODEP contains a stale MD';depbytes=None;mdhash=None;closure={'source':digest(source),**c['headers']}
 else:
  assert md.is_file(),'compiler did not produce physical MD'
  target,names=dependencies(md);want=sorted([str(source),*[str(ROOT/r)for r in c['headers']]])
  assert names==want,('MD closure drift',names,want)
  assert target.replace('\\','/')==os.path.relpath(full,ROOT),'MD full-producer target drift'
  mdhash=digest(md);closure={p:digest(p)for p in names}
  # Preserve original compiler MD unchanged; only the derived normal target is normalized.
  depbytes=(os.path.relpath(obj,ROOT)+': '+(' '+chr(92)+'\n\t').join(names)+'\n').encode()
 assert digest(source)==c['source_sha256'],'source changed during producer'
 config(key)
 record={'schema':1,'source':str(source),'source_sha256':digest(source),'game':game,'NODEP':nodep,'command':command,'full_producer':str(full),'full_sha256':digest(full),'original_MD':None if nodep else str(md),'original_MD_sha256':mdhash,'dependency_closure':closure,'derived_dependency_sha256':None if depbytes is None else hashlib.sha256(depbytes).hexdigest(),'object':str(obj),'child_sha256':hashlib.sha256(child).hexdigest(),'profile_sha256':digest(HOME/'profile.json'),'modules':c['modules'],'selector':report,'mapped_credit_bytes':0,'commit_invariant':'child published last after complete receipt and physical derived dependency; target always reconstructed by named phony prerequisite'}
 if depbytes is not None:atomic(dep,depbytes)
 else:remove(dep)
 atomic(receipt,(json.dumps(record,indent=2)+'\n').encode())
 atomic(obj,child)
 return record

def build(source,obj,game,nodep,command,key):
 source=Path(source).resolve();obj=Path(obj).resolve();obj.parent.mkdir(parents=True,exist_ok=True)
 full,md,dep,receipt=paths(obj)
 # Remove the last commit record before validation/compilation; no stale pair may survive a failure.
 remove(obj);remove(receipt);remove(dep);remove(md)
 try:
  c=config(key);assert source==(ROOT/c['source']).resolve() and digest(source)==c['source_sha256'],'unregistered source'
  assert obj==(ROOT/'build'/f'{game}.us'/'src'/(source.stem+'.o')).resolve(),'unregistered object or dishonest basename'
  tokens=shlex.split(command);expected=c['GNU_CPP_tokens'][game]
  assert tokens==expected+([]if nodep else['-gccdep','-MD'])+['-c','-o'],'production command profile drift'
  args=tokens+[os.path.relpath(full,ROOT),os.path.relpath(source,ROOT)]
  env=dict(os.environ,WINEPREFIX=str(Path.home()/'.wine'),WINEDEBUG='-all')
  with full.with_suffix('.log').open('w')as log:
   process=subprocess.Popen(args,cwd=ROOT,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
   try:status=process.wait(timeout=265)
   except BaseException:
    if process.poll() is None:
     os.killpg(process.pid,signal.SIGTERM)
     try:process.wait(timeout=2)
     except subprocess.TimeoutExpired:
      os.killpg(process.pid,signal.SIGKILL);process.wait(timeout=1)
    raise
  assert status==0,('compiler failure',status)
  return publish(source,obj,game,nodep,args,c,key)
 except BaseException:
  remove(obj);remove(receipt);remove(dep)
  raise
if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('--producer',choices=['scalar','array'],required=True);p.add_argument('--source',required=True);p.add_argument('--object',required=True);p.add_argument('--game',choices=['heartgold','soulsilver'],required=True);p.add_argument('--nodep',default='');p.add_argument('--command',required=True);a=p.parse_args();build(a.source,a.object,a.game,bool(a.nodep),a.command,a.producer)
