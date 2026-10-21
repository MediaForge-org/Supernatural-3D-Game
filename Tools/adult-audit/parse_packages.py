#!/usr/bin/env python3
"""Extract numbered question packages (chat 35, messages 186-810) and the developer answers. Output: matrix.json. Run in the work directory."""
import json,re,sys
m=json.load(open('msgs35.json'))
msgs={x[0]:x for x in m}
PK={186:'R1',188:'R2',220:'R3',226:'R4',228:'R5',230:'R6',232:'R7',246:'R8',260:'R9A',268:'R9B.1',270:'R9B.2',272:'R9B.3',278:'R9C',
280:'R10A',282:'R10B',288:'KR1',290:'KR2',443:'R10S',457:'R11',463:'R11B',483:'R12',485:'R13',499:'R14',511:'R14B',
523:'R15A',525:'R15B',527:'R15C',529:'R15D',539:'R15E',541:'R15F',549:'R15G',551:'R15H',
573:'R16A',583:'R16B',589:'R16C',609:'R16D',611:'R16E',618:'R16F',624:'R16G',626:'R16H',642:'R16I',650:'R16J',652:'R16K',676:'R16L',678:'R16M',686:'R16N',692:'R16O',694:'R16P',706:'R16Q',712:'R16R',
742:'PA',748:'R17A',760:'R17B',766:'R17C',768:'R17D',770:'R17E',774:'R17F',778:'R17G',782:'R17H',784:'R17I',790:'R18A',794:'R18B',800:'R18C',804:'R19A',806:'R19B',810:'R20A'}
order=sorted(PK)
qre=re.compile(r'^\s*(?:#{1,4}\s*)?(?:\*\*)?\s*(A)?(\d{1,3})\s*[.)](?:\*\*)?[:\s]*(.*)$')
def clean(t):
    t=re.sub(r'\*\*|`|#+','',t); t=re.sub(r'\s+',' ',t).strip(); return t
def extract(b,series):
    lines=b.split('\n'); items=[]
    for i,ln in enumerate(lines):
        mm=qre.match(ln)
        if not mm: continue
        a,num,txt=mm.group(1),int(mm.group(2)),mm.group(3).strip()
        mi=re.match(r'^\*\*(\d{1,3})\.\*\*\s*(.*)$',txt)
        if mi: num,txt=int(mi.group(1)),mi.group(2)
        if (series=='A')!=bool(a): continue
        if not txt:
            for j in range(i+1,min(i+4,len(lines))):
                if lines[j].strip(): txt=lines[j].strip(); break
        items.append((num,clean(txt)))
    runs=[];cur=[]
    for it in items:
        if cur and it[0]==cur[-1][0]+1: cur.append(it)
        elif cur and it[0]==cur[-1][0]: pass
        else:
            if cur: runs.append(cur)
            cur=[it]
    if cur: runs.append(cur)
    runs=[r for r in runs if len(r)>=5]
    # last long run that is a question package: choose longest, ties -> last
    best=max(runs,key=lambda r:(len(r),runs.index(r)))
    return best
ans_re=re.compile(r'^\s*[aA]?(\d{1,3})(?:\s*[-–]\s*[aA]?(\d{1,3}))?\s*[.:)]?\s+(.+)$|^\s*[aA]?(\d{1,3})(?:\s*[-–]\s*[aA]?(\d{1,3}))?\s*[.:)]?\s*$')
SPLIT=re.compile(r',\s*(?=[aA]?\d{1,3}(?:\s*[-–]\s*[aA]?\d{1,3})?(?:\s|$|[.:)]))')
def expand(text):
    outl=[]
    for ln in text.split('\n'):
        parts=SPLIT.split(ln)
        if len(parts)>1 and all(re.match(r'^\s*[aA]?\d',p) for p in parts):
            # inherit text for number-only fragments
            fr=[]
            for p in parts:
                fr.append(p.strip())
            res=[]
            nxt=''
            for p in reversed(fr):
                mm=re.match(r'^([aA]?\d{1,3}(?:\s*[-–]\s*[aA]?\d{1,3})?)\s*[.:)]?\s*(.*)$',p)
                if not mm: res.append(p); continue
                t=mm.group(2).strip()
                if t: nxt=t
                res.append(mm.group(1)+' '+(t or nxt))
            outl.extend(reversed(res))
        else: outl.append(ln)
    return '\n'.join(outl)
def parse_answers(text,lo,hi,series):
    out={}  # num-> text
    cur=None
    text=expand(text)
    for ln in text.split('\n'):
        if ln.strip()=='---': continue
        mm=ans_re.match(ln)
        is_a = series=='A'
        if mm and (bool(re.match(r'^\s*[aA]\d',ln))==is_a or not is_a and not re.match(r'^\s*[aA]\d',ln)):
            a=mm.group(1) or mm.group(4); b=mm.group(2) or mm.group(5); t=(mm.group(3) or '').strip()
            a=int(a); b=int(b) if b else a
            if a>b: cur=None; continue
            if lo<=a<=hi or lo<=b<=hi:
                cur=list(range(max(a,lo),min(b,hi)+1))
                for k in cur: out[k]=(out.get(k,'')+' '+t).strip()
                continue
            cur=None
        elif cur and ln.strip():
            for k in cur[-1:]: out[k]=(out.get(k,'')+' '+ln.strip()).strip()
    return out
data=[]
for idx,n in enumerate(order):
    label=PK[n]; series='A' if label=='PA' else 'N'
    qs=extract(msgs[n][3],series)
    lo,hi=qs[0][0],qs[-1][0]
    nxt=order[idx+1] if idx+1<len(order) else 813
    ans={}  # num -> list of (msg,text)
    for k in range(n+1,nxt):
        x=msgs.get(k)
        if not x or x[1]!='user': continue
        pa=parse_answers(x[3],lo,hi,series)
        for q,t in pa.items(): ans.setdefault(q,[]).append((k,t))
    data.append(dict(label=label,msg=n,date=msgs[n][2][:10],lo=lo,hi=hi,qs=qs,ans={str(q):v for q,v in ans.items()},nxt=nxt))
json.dump(data,open('matrix.json','w'),ensure_ascii=False)
tot=0;ans=0
for d in data:
    a=len(d['ans']); q=len(d['qs']); tot+=q; ans+=a
    print(d['label'],d['msg'],d['lo'],d['hi'],'q',q,'answered',a)
print(tot,ans)
