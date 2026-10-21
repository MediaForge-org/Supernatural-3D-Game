#!/usr/bin/env python3
import os
import json,re,glob,unicodedata
def norm(t):
    t=t.lower().replace('ß','ss')
    t=unicodedata.normalize('NFKD',t)
    t=re.sub(r'[^a-z0-9 ]',' ',t); return re.sub(r'\s+',' ',t).strip()
corpus=''
for k in ('35','37','23','28'):
    for x in json.load(open(f'msgs{k}.json')): corpus+=' '+norm(x[3])
import glob
extra=''
for f in glob.glob(os.environ['SPN_MASTER']+'/02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/**/*.md',recursive=True):
    extra+=' '+norm(open(f,encoding='utf8',errors='ignore').read())

for _d in [x for x in os.environ.get('SPN_EXTRA_SOURCES','').split(':') if x]:
    for _f in glob.glob(_d+'/**/*',recursive=True):
        if _f.endswith(('.md','.txt')) and os.path.isfile(_f):
            extra+=' '+norm(open(_f,encoding='utf8',errors='ignore').read())
corpus_all=corpus+extra
files=sorted(glob.glob(os.environ['SPN_ADULT_DOCS']+'/*.md'))
miss=[];tot=0
for f in files:
    if any(f.endswith(x) for x in ('36-','37-','38-','39-','40-')) : continue
    name=f.split('/')[-1]
    if name[:2] in ('36','37','38','39','40'): continue
    for ln,line in enumerate(open(f,encoding='utf8'),1):
        for q in re.findall(r'„([^“]{12,160})“',line):
            n=norm(q); 
            if len(n)<10: continue
            tot+=1
            # try exact, else try first 5 words / last 5 words
            w=n.split()
            ok = n in corpus_all or (len(w)>=6 and (' '.join(w[:6]) in corpus_all or ' '.join(w[-6:]) in corpus_all))
            if not ok: miss.append((name,ln,q))
print(tot,len(miss))
for m in miss: print(m)

# fuzzy stage
msgs=[]
for k in ('35','37','23','28'):
    for x in json.load(open(f'msgs{k}.json')): msgs.append(norm(x[3]))
import glob
docs=[]
for f in glob.glob(os.environ['SPN_MASTER']+'/02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/**/*.md',recursive=True):
    docs.append(norm(open(f,encoding='utf8',errors='ignore').read()))
allt=msgs+docs
sets=[set(t.split()) for t in allt]
STOP=set('der die das und oder ein eine einen einem einer ist sind war wird kann man nur auch aber mit von zu im in an auf für den dem des es sich nicht dass wie wenn so ja als bei aus nach noch schon dann'.split())
def best(q):
    w=[x for x in norm(q).split() if x not in STOP]
    if not w: return 1.0
    ws=set(w); b=0
    for s in sets:
        r=len(ws&s)/len(ws)
        if r>b: b=r
    return b
res=[]
for name,ln,q in miss:
    res.append((best(q),name,ln,q))
res.sort()
import json as j
j.dump(res,open('quote_fuzzy.json','w'),ensure_ascii=False)
low=[r for r in res if r[0]<0.8]
print('fuzzy <0.8:',len(low),'of',len(res))
for r in low: print(round(r[0],2),r[1],r[2],r[3])
