"""Finite genuine GNU ARM7 full-source producer then complete-object partition."""
from pathlib import Path
import argparse,hashlib,json,os,shlex,subprocess,sys,tempfile
sys.dont_write_bytecode=True
HOME=Path(__file__).resolve().parent
ROOT=next(p for p in HOME.parents if (p/'common.mk').is_file());SUB=ROOT/'sub'
sys.path.insert(0,str(HOME));import partition
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()

def need(x,m):
 if not x:raise ValueError(m)
def read(p):return json.loads(Path(p).read_text())
def pins(mapping):
 for p,h in mapping.items():q=ROOT/p;need(q.is_file() and not q.is_symlink() and sha(q)==h,'physical input drift '+p)
def config():
 c=read(HOME/'profile.json');pins(c['tool_sha256']);pins(c['C_header_pins'])
 for n,h in c['modules'].items():need(sha(HOME/n)==h,'production module drift '+n)
 return c

def dep_paths(md,cwd=SUB):
 raw=Path(md).read_text();flat=raw.replace('\\\n',' ').replace('\\\r\n',' ')
 need(': ' in flat,'MD target delimiter');target,tokens=flat.split(': ',1);paths=[]
 for t in tokens.split():
  t=t.replace('\\','/');t=t[2:] if t.startswith('Z:') else t;p=Path(t);p=p if p.is_absolute() else cwd/p;p=p.resolve()
  need(p.is_file(),'actual MD input absent '+str(p));paths.append(str(p))
 need(paths,'empty MD input closure');return {'raw_sha256':sha(md),'target':target,'occurrences':paths,'unique':list(dict.fromkeys(paths))}

def profile(tokens,kind,c):
 t=shlex.split(tokens) if isinstance(tokens,str)else list(tokens);expected=c[kind+'_base']
 need(len(t)>1 and t[:1]+t[2:]==expected[:1]+expected[2:] and Path(t[1]).resolve()==(ROOT/expected[1]).resolve(),'actual ordered GNU compiler profile mismatch');need(t[0]=='wine','native Wine launcher');exe=Path(t[1]).resolve()
 need(exe.is_relative_to(ROOT/'tools/mwccarm/2.0/sp2p3'),'truthful SDK2p3 tool path');pins(c['tool_sha256']);return t

def handler_proof(blob):
 import struct
 h,ss,ns,st,sy,names=partition.parse(blob);need(h[7]==0,'genuine assembler ELF flags')
 alloc=[i for i,s in enumerate(ss)if s[2]&2];need(len(alloc)==1,'one native handler section');i=alloc[0];s=ss[i];need(ns[i]=='.text' and s[1:4]==[1,6,0] and s[5:]==[16,0,0,4,0],'complete handler physical section attributes')
 need(blob[s[4]:s[4]+16]==bytes.fromhex('01402de9feffffeb0140bde81eff2fe1'),'actual four handler opcodes including BL condition')
 defs=[(n,y[1:5])for n,y in zip(names,sy)if y[5]==i];need(sorted(defs)==sorted([('$a',[0,0,2,0]),('.text',[0,16,3,0]),('OSi_AlarmHandler',[0,16,18,0])]),'all handler raw mapped symbols')
 rel=[]
 for rs in ss:
  if rs[1]!=4 or rs[7]!=i:continue
  for p in range(rs[4],rs[4]+rs[5],12):
   off,info,add=struct.unpack_from('<IIi',blob,p);j=info>>8;rel.append((off,info&255,names[j],sy[j][1:],add))
 need(rel==[(4,1,'OSi_ArrangeTimer',[0,0,16,0,0],-8)],'exact external PC24 endpoint/addend');return {'allocated_text':16,'raw_handler_inventory':defs,'relocation':rel,'natural_ELF_flags':0}

def produce(source,obj,role,game,gnu_profile):
 need(role in ('prefix','handler','suffix') and game in ('HEARTGOLD','SOULSILVER'),'finite role/game')
 c=config();row=c['roles'][role];source=Path(source);need(not source.is_symlink(),'source symlink');source=source.resolve();actual=ROOT/row['source'];fixture=HOME.parents[1]/row['source']
 need(source in (actual.resolve(),fixture.resolve()),'unregistered source pathname');need(sha(source)==row['sha256'],'canonical full source drift')
 obj=Path(obj).resolve();need(obj.suffix=='.o' and obj!=source,'object alias');kind='ASM'if role=='handler'else'C';base=profile(gnu_profile,kind,c)
 obj.parent.mkdir(parents=True,exist_ok=True);parent=obj.with_suffix('.producer');parent.mkdir(exist_ok=True);work=Path(tempfile.mkdtemp(prefix='full-',dir=parent));os.chmod(work,0o700);full=work/'full.o'
 argv=base+(['-c']if kind=='C'else[])+['-o',os.path.relpath(full,SUB),os.path.relpath(source,SUB)]
 env=dict(os.environ,WINEPREFIX=os.environ.get('WINEPREFIX',str(Path.home()/'.wine')),WINEDEBUG='-all',LM_LICENSE_FILE=str(ROOT/'tools/mwccarm/license.dat'))
 with (work/'compiler.log').open('wb')as log:r=subprocess.run(argv,cwd=SUB,env=env,stdout=log,stderr=subprocess.STDOUT,timeout=240)
 need(r.returncode==0 and full.is_file() and full.with_suffix('.d').is_file(),'actual private full producer failed')
 deps=dep_paths(full.with_suffix('.d'));expected={str((ROOT/h).resolve())for h in c['C_headers']}|{str(source)}if kind=='C'else{str(source)};need(set(deps['unique'])==expected,'complete genuine MD source/include closure changed')
 closure={p:sha(p)for p in deps['unique']};pins(closure);config();need(sha(source)==row['sha256'],'source changed while compiling')
 if kind=='C':
  children,reports=partition.split(full.read_bytes());proof=partition.prove(full.read_bytes(),children,reports);index=0 if role=='prefix'else 1;child=children[index];proof['selected_partition']=reports[index]
 else:child=full.read_bytes();proof=handler_proof(child)
 # Publish only after all complete raw, source and dependency checks succeeded.
 temp=obj.with_suffix('.o.tmp');need(not temp.exists(),'stale temporary target');temp.write_bytes(child);temp.replace(obj)
 md=full.with_suffix('.d').read_bytes();need(b': ' in md,'original MD delimiter');obj.with_suffix('.d').write_bytes(os.path.relpath(obj,SUB).encode()+b': '+md.split(b': ',1)[1])
 receipt={'schema':1,'role':role,'game':game,'source':str(source),'source_sha256':sha(source),'installed_source':source==actual.resolve(),'object':str(obj),'object_sha256':sha(obj),'full_producer':str(full),'full_sha256':sha(full),'full_MD':str(full.with_suffix('.d')),'full_MD_sha256':sha(full.with_suffix('.d')),'dependency_evidence':deps,'input_pins':closure,'argv':argv,'GNU_base':base,'GNU_profile_verified':True,'compiler_log':str(work/'compiler.log'),'compiler_log_sha256':sha(work/'compiler.log'),'profile_sha256':sha(HOME/'profile.json'),'proof':proof}
 (work/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');obj.with_suffix('.receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');return receipt
if __name__=='__main__':
 a=argparse.ArgumentParser();a.add_argument('--source',required=True);a.add_argument('--object',required=True);a.add_argument('--role',required=True);a.add_argument('--game',required=True);a.add_argument('--gnu-profile',required=True);x=a.parse_args();print(json.dumps(produce(x.source,x.object,x.role,x.game,x.gnu_profile),indent=2))
