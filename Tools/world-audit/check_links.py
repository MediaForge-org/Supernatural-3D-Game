#!/usr/bin/env python3
"""Check relative links and Markdown anchors. Usage: check_links.py DIR"""
import re,os,sys,unicodedata
root=sys.argv[1]
files=[os.path.join(dp,f) for dp,_,fs in os.walk(root) for f in fs if f.endswith('.md')]
def slug(h):
    h=h.strip()
    h=re.sub(r'[`*_~]','',h) if False else h
    h=re.sub(r'<[^>]+>','',h)
    h=re.sub(r'\[([^\]]*)\]\([^)]*\)',r'\1',h)
    h=h.lower()
    out=[]
    for ch in h:
        if ch.isalnum() or ch in '-_': out.append(ch)
        elif ch==' ': out.append('-')
        # others dropped
    return ''.join(out)
anchors={}
for f in files:
    s=open(f,encoding='utf8').read()
    in_code=False; a=set(); seen={}
    for line in s.split('\n'):
        if line.startswith('```'): in_code=not in_code
        if in_code: continue
        m=re.match(r'^(#{1,6})\s+(.*)$',line)
        if m:
            sl=slug(m.group(2).replace('`',''))
            n=seen.get(sl,0); seen[sl]=n+1
            a.add(sl if n==0 else f'{sl}-{n}')
    anchors[os.path.abspath(f)]=a
bad=0; total=0
for f in files:
    s=open(f,encoding='utf8').read()
    in_code=False
    for ln,line in enumerate(s.split('\n'),1):
        if line.startswith('```'): in_code=not in_code
        if in_code: continue
        for m in re.finditer(r'\]\(([^)\s]+)\)',line):
            t=m.group(1)
            if re.match(r'^[a-z]+:',t): continue
            total+=1
            path,_,frag=t.partition('#')
            tgt=os.path.abspath(os.path.join(os.path.dirname(f),path)) if path else os.path.abspath(f)
            if not os.path.exists(tgt):
                print('MISSING FILE',f,ln,t); bad+=1; continue
            if frag and tgt.endswith('.md'):
                if frag not in anchors.get(tgt,set()):
                    print('MISSING ANCHOR',os.path.relpath(f,root),ln,t); bad+=1
print('links',total,'bad',bad)
