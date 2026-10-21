#!/usr/bin/env python3
"""Manual overrides (documented unanswered/pauschal cases) and message-to-register map.
Run inside the work directory (cwd) that contains msgs35.json etc. Environment: SPN_ADULT_DOCS = path to Docs/Game/Adult."""
import os
import json,re,collections
REG=os.path.join(os.environ['SPN_ADULT_DOCS'],'14-Entscheidungsregister.md')
d=json.load(open('matrix.json')); msgs={x[0]:x for x in json.load(open('msgs35.json'))}
# manual additions
def add(label,q,msg,text):
    for p in d:
        if p['label']==label:
            p['ans'].setdefault(str(q),[]).append((msg,text))
add('R17A',13,751,'Achso ja'); 
for q in (24,25,26): add('R17B',q,761,'(Block 23–26) es soll alles geben können was man sich vorstellen kann, Blackout wählbar, beim Tätowierer Vorschau von sich: Größe, Motiv und mehr')
for q in (27,28): add('R17B',q,763,'ja (in 761 versehentlich gelöscht, in 763 nachgereicht)')
add('R18A',4,791,'4?')
for q in range(81,101): add('R17E',q,771,'ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort)')
# fix: R17B 23 may be existing; dedupe later
# register map
reg=open(REG,encoding='utf8').read()
idmap=collections.defaultdict(set)
for ln in reg.split('\n'):
    mm=re.match(r'^\| (E-[A-Z]{2}-\d{3}) \|',ln)
    if not mm: continue
    eid=mm.group(1); cells=[c.strip() for c in ln.split('|')]
    src=cells[3] if len(cells)>3 else ''
    for t in re.finditer(r'35#(\d+)[AEK]?(?:\s*[–-]\s*(\d+)[AEK]?)?',src):
        a=int(t.group(1)); b=int(t.group(2)) if t.group(2) else a
        if b-a>60: b=a
        for k in range(a,b+1): idmap[k].add(eid)
# also single numbers in list after # e.g. "35#444E, 35#446E, 35#448E–454E"
json.dump({k:sorted(v) for k,v in idmap.items()},open('idmap.json','w'))
json.dump(d,open('matrix2.json','w'),ensure_ascii=False)
print(len(idmap), sum(len(v) for v in idmap.values()))
