"""Finite alarm child GNU rule: full actual-path C compilation before physical selection."""
from pathlib import Path
import sys,os,json,hashlib,tempfile,shlex,struct
sys.dont_write_bytecode=True
HOME=Path(__file__).resolve().parent;sys.path.insert(0,str(HOME));import compile as cc,partition,assemble
ROOT=next(p for p in HOME.parents if (p/'common.mk').is_file())
cc.ROOT=ROOT;assemble.R=ROOT
def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def config():
 # The closure pins every input the compiler reads (sources, headers, compiler executables).
 # Makefiles are not pinned as files: their effect is the command line, which build() checks
 # token by token against GNU_*_tokens, so any flag change in common.mk etc. is still rejected.
 c=json.loads((HOME/'profile.json').read_text())
 for rel,pin in c['closure'].items():assert digest(ROOT/rel)==pin,('production closure drift',rel)
 for rel,pin in c['modules'].items():assert digest(HOME/rel)==pin,('build module drift',rel)
 return c

def deps(path):
 out=[]
 for line in Path(path).read_text().splitlines()[1:]:
  name=line.strip().removesuffix('\\').strip().replace('\\','/')
  if name.startswith('Z:'):name=name[2:]
  if name:out.append(str(Path(name).resolve()))
 return sorted(out)
def validate_source(source,role,c):
 assert role in('prefix','suffix','handler')
 assert not Path(source).is_symlink(),'source symlink is not enrolled'
 source=Path(source).resolve();row=c['roles'][role];assert digest(source)==row['sha256'],'actual source content drift'
 # Scratch proof may use exact payload paths; GNU enrollment only exact registered installed paths.
 actual=ROOT/row['source'];fixture=HOME.parents[1]/row['source']
 assert source in(actual.resolve(),fixture.resolve()),('unregistered source pathname',source)
 return source

def build(source,obj,role,game,gnu_profile=None):
 assert game in('hg','ss');c=config();source=validate_source(source,role,c);obj=Path(obj).resolve();before=digest(source);assert obj!=source and obj.suffix=='.o','object/source alias or invalid target'
 if gnu_profile is not None:
  expected=c['GNU_'+('ASM'if role=='handler'else'C')+'_tokens']
  tokens=shlex.split(gnu_profile);assert Path(tokens[0]).resolve()==(ROOT/expected[0]).resolve() and tokens[1:]==expected[1:],'GNU target production profile mismatch'
 obj.parent.mkdir(parents=True,exist_ok=True)
 proof_parent=obj.with_suffix('.producer');proof_parent.mkdir(exist_ok=True)
 work=Path(tempfile.mkdtemp(prefix='full-',dir=proof_parent));os.chmod(work,0o700)
 full=work/'full.o'
 if role=='handler':
  command=assemble.assemble(source,full);child=full.read_bytes();h,ss,ns,st,yy,yn=partition.parse(child)
  assert h[7]==0 and [(ns[i],s[1],s[2],s[5],s[8])for i,s in enumerate(ss)if s[2]&2]==[('.text',1,6,16,4)]
  assert [(n,y[1:5])for n,y in zip(yn,yy)if y[5]<len(ss)and y[5] and y[3]>>4==1]==[('OSi_AlarmHandler',[0,16,18,0])]
  result={'role':'natural-MWAS-handler','ELF_flags':0,'only_live_text':16}
 else:
  assert cc.compile_c(source,full,game,'2.0/sp2p3',True,('-gccdep','-MD'))==0
  child_objects,reports=partition.split(full.read_bytes());result=partition.prove(full.read_bytes(),child_objects,reports)
  child=child_objects[0 if role=='prefix'else 1];command=c['GNU_C_tokens'];result['partition']=reports[0 if role=='prefix'else 1]
 actual_deps=deps(full.with_suffix('.d'))
 expected=sorted(str((ROOT/r).resolve())for r in c['C_dependencies'])if role!='handler'else [str(source)]
 assert actual_deps==expected,('exact MD include closure',role,actual_deps,expected)
 closure={p:digest(p)for p in actual_deps};assert digest(source)==before;config()
 temporary=obj.with_suffix('.o.tmp');assert not temporary.exists();temporary.write_bytes(child);temporary.replace(obj)
 # Root standard fixdep will normalize this derived dependency file; retain untouched full .d in receipt.
 dep=full.with_suffix('.d').read_bytes();target=str(obj.relative_to(ROOT)).replace('/','\\') if obj.is_relative_to(ROOT)else str(obj)
 lines=dep.split(b': ',1);assert len(lines)==2;obj.with_suffix('.d').write_bytes(target.encode()+b': '+lines[1])
 receipt={'schema':1,'role':role,'game':game,'source':str(source),'source_sha256':before,'full_producer':str(full),'full_sha256':digest(full),'full_dependency':str(full.with_suffix('.d')),'full_dependency_sha256':digest(full.with_suffix('.d')),'object':str(obj),'object_sha256':digest(obj),'command_profile':command,'include_closure':closure,'profile_sha256':digest(HOME/'profile.json'),'GNU_profile_verified':gnu_profile is not None,'proof':result}
 (work/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');obj.with_suffix('.receipt.json').write_text(json.dumps(receipt,indent=2)+'\n')
 return receipt
if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('--source',required=True);p.add_argument('--object',required=True);p.add_argument('--role',required=True);p.add_argument('--game',required=True);p.add_argument('--gnu-profile');a=p.parse_args();build(a.source,a.object,a.role,a.game,a.gnu_profile)
