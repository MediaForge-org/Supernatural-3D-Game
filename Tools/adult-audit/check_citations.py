#!/usr/bin/env python3
import os
import json,re,sys,glob
M={}
for k in ('35','37','23','28'):
    M[k]={x[0]:x for x in json.load(open(f'msgs{k}.json'))}
bad=[];tot=0
files=sorted(glob.glob(os.environ['SPN_ADULT_DOCS']+'/*.md'))
pat=re.compile(r'(35|37|23|28)#(\d+)([AEK]?)(?:\s*[–-]\s*(\d+)([AEK]?))?')
for f in files:
    for ln,line in enumerate(open(f,encoding='utf8'),1):
        for m in pat.finditer(line):
            c,a,suf,b,suf2=m.groups(); a=int(a); tot+=1
            if a not in M[c]: bad.append((f.split('/')[-1],ln,m.group(0),'nicht vorhanden')); continue
            if b:
                b=int(b)
                if b not in M[c]: bad.append((f.split('/')[-1],ln,m.group(0),'Ende nicht vorhanden'))
                elif b<a: bad.append((f.split('/')[-1],ln,m.group(0),'Bereich verkehrt'))
            role=M[c][a][1]
            if suf=='E' and role!='user': bad.append((f.split('/')[-1],ln,m.group(0),'E aber '+role))
            if suf=='A' and role!='assistant': bad.append((f.split('/')[-1],ln,m.group(0),'A aber '+role))
            if b and suf2:
                r2=M[c].get(b)
                if r2 and ((suf2=='E' and r2[1]!='user') or (suf2=='A' and r2[1]!='assistant')): bad.append((f.split('/')[-1],ln,m.group(0),'Ende Rolle '+r2[1]))
print('citations',tot,'problems',len(bad))
for x in bad[:80]: print(x)
