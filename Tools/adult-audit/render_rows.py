#!/usr/bin/env python3
"""Status heuristics, redaction and register cross-reference per question.
Run inside the work directory (cwd) that contains msgs35.json etc. Environment: SPN_ADULT_DOCS = path to Docs/Game/Adult."""
import os
import json,re,collections,sys
d=json.load(open('matrix2.json')); msgs={x[0]:x for x in json.load(open('msgs35.json'))}
REG=os.path.join(os.environ['SPN_ADULT_DOCS'],'14-Entscheidungsregister.md')
TITLE={'R1':'Runde 1: Gesamtrichtung','R2':'Runde 2: Content-Struktur und Rollen','R3':'Runde 3: Kategorien-Struktur','R4':'Runde 4: Präferenzprofile','R5':'Runde 5: Beziehung, Attraktion','R6':'Runde 6: Initiative, Flirt','R7':'Runde 7: Orte, Öffentlichkeit, Observer','R8':'Runde 8: Pakete, Modularität','R9A':'Runde 9A: Präferenz-DNA','R9B.1':'Runde 9B.1: Kern/Gedächtnis','R9B.2':'Runde 9B.2: Lernen, Dates, Beziehungen','R9B.3':'Runde 9B.3: Langzeit, Alter, Tod','R9C':'Runde 9C: Weltsimulation, Speicher','R10A':'Runde 10A: Körperprofil','R10B':'Runde 10B: Skelett, Kontakt','KR1':'High-Fidelity Body Response 1–20','KR2':'High-Fidelity Body Response 21–40','R10S':'Runde 10: Szenensystem','R11':'Runde 11: Freeform-Engine','R11B':'Runde 11B: Ergänzungen','R12':'Runde 12: Replay, Aufnahme, Streaming','R13':'Runde 13: Ablauf, Reaktionen','R14':'Runde 14: Körper, Tattoos','R14B':'Runde 14B: Piercings, Körpermodifikation','R15A':'Runde 15A: Präferenzmodell','R15B':'Runde 15B: Gruppen, Branche, Medien','R15C':'Runde 15C: Orte, Öffentlichkeit, Produktion','R15D':'Runde 15D: Praktiken-Präferenzen, Fesseln, Rollenspiel','R15E':'Runde 15E: Solo, Medien, Dienstleistung','R15F':'Runde 15F: Gesundheit, Hygiene','R15G':'Runde 15G: Orientierung, Dynamik','R15H':'Runde 15H: Kompatibilität, Statistik','R16A':'Runde 16A: Master-Taxonomie','R16B':'Runde 16B: Basiskategorien','R16C':'Runde 16C: Positionen','R16D':'Runde 16D: Steuerung, Intensität','R16E':'Runde 16E: Reaktionen','R16F':'Runde 16F: Gewebe, Verletzung','R16G':'Runde 16G: NPC-Verhalten','R16H':'Runde 16H: UI, Einstellungen, Technik','R16I':'Runde 16I: Registry I','R16J':'Runde 16J: Unterkategorien','R16K':'Runde 16K: Reaktions-/Outcome-Familien','R16L':'Runde 16L: Empfindung, Erholung','R16M':'Runde 16M: Registry II','R16N':'Runde 16N: Höhepunkt, Kleidung, Solo','R16O':'Runde 16O: Orte','R16P':'Runde 16P: Kombinierte Aktionen','R16Q':'Runde 16Q: Risiko, Körperzustand','R16R':'Runde 16R: Produktions-Locks','PA':'Pre-Audit A1–A15','R17A':'Runde 17A: Tattoo-Grundmodell','R17B':'Runde 17B: Stile, Motive','R17C':'Runde 17C: Künstler, Studios','R17D':'Runde 17D: Prozess, Heilung','R17E':'Runde 17E: Cover-up, Laser','R17F':'Runde 17F: NPC-Tattoo-Kultur','R17G':'Runde 17G: Tattoo-Technik','R17H':'Runde 17H: Piercings, Körpermodifikation','R17I':'Runde 17I: Abschluss','R18A':'Runde 18A: Steuerung','R18B':'Runde 18B: VR','R18C':'Runde 18C: Bedienung','R19A':'Runde 19A: Technik','R19B':'Runde 19B: Speicher, Builds','R20A':'Runde 20A: Production Lock'}
MOD={'R1':'02, 03, 05','R2':'03, 05','R3':'03, 02','R4':'05','R5':'05','R6':'05, 08','R7':'08, 03','R8':'02','R9A':'05','R9B.1':'05','R9B.2':'05','R9B.3':'05, 06','R9C':'05, 02','R10A':'06, 05','R10B':'03, 06','KR1':'06','KR2':'06','R10S':'03, 01','R11':'03, 06','R11B':'03','R12':'04','R13':'03, 06','R14':'06, 07','R14B':'07','R15A':'05','R15B':'05, 08','R15C':'08, 04','R15D':'05, 03','R15E':'05, 08','R15F':'06','R15G':'05','R15H':'05','R16A':'03','R16B':'03','R16C':'03','R16D':'04, 03','R16E':'06','R16F':'06','R16G':'05','R16H':'04, 02','R16I':'03','R16J':'03','R16K':'06, 03','R16L':'06','R16M':'03','R16N':'03, 06','R16O':'08, 03','R16P':'03','R16Q':'06','R16R':'10, 02','PA':'03','R17A':'07','R17B':'07','R17C':'07','R17D':'07','R17E':'07','R17F':'07','R17G':'07','R17H':'07','R17I':'07','R18A':'04','R18B':'04','R18C':'04','R19A':'02','R19B':'02','R20A':'10, 02'}
LEX=re.compile(r'\banal(?:e[nmrs]?|sex|verkehr)?\b|\boral(?:e[nmrs]?|sex)?\b|penetr\w*|vagin\w*|\bpenis\b|fist\w*|\bfaust\b|deep\s?throat\w*|blow\s?job|\bbj\b|face\s?fuck\w*|\bgag(?:ging)?\b|\bpuke\w*|sperm\w*|ejakul\w*|\bcum\w*|pussy|titt\w*|arsch\w*|schwanz|fick\w*|squirt\w*|rimm\w*|cunnil\w*|hand\s?job|fellat\w*|dildo\w*|prolaps\w*|\bkot\b|pissen|würg\w*|erbro\w*|erbrech\w*|kotz\w*|genital\w*|\banus\b|scheide|vulva|klitor\w*|hoden|eichel|masturb\w*|vergewalt\w*|\brape\w*|fickpuppe|\bpimmel\b|\bmuschi\b|\blecken\b|saug\w*|\bnutt\w*|\bfeucht\b|hakenkreuz\w*|siegesrune\w*|\bss-?runen\w*',re.I)
def clean(t):
    t=re.sub(r'\*\*|`|#+|\\-|&#x20;','',t); t=re.sub(r'\s+',' ',t).strip(); return t
def trunc(t,n):
    if len(t)<=n: return t
    c=t[:n].rsplit(' ',1)[0]; return c+' …'
def esc(t): return t.replace('|','¦')
VERD=re.compile(r'^(?:ja|jaa|jo|genau|ok|okay|passt|klar|sicher|natürlich|definitiv|gut|stimmt|super|safe|auf jeden fall|ja ja|jap|jep|richtig|sehr gut|perfekt|achso ja)\b',re.I)
NO=re.compile(r'^(?:nein|nö|nee|no)\b',re.I)
UNV=re.compile(r'\b(?:kann (?:schon |es |alles |bei manchen )?sein|kann es geben|mal schauen|kann man schauen|vielleicht)\b',re.I)
OPEN=re.compile(r'weiß (?:ich )?(?:noch )?nicht|weis ich nicht|wie meinst|was ist|was genau|was meinst|genauer|erkläre|erklär|verstehe ich nicht|keine ahnung|welche (?:artists|adult)|wie zb|sag das genauer|wieso|worüber|fällt mir (?:gerade )?nicht|^\?$|^4\?$|\?\s*$',re.I)
GEN=re.compile(r'^(?:ja|ok|okay|perfekt)\b[ ,.!]*(?:passt|genau|gut|sicher|dann|das passt)?[ a-zäöü]{0,25}$',re.I)
def status(txt):
    t=txt.strip()
    if not t: return '⚪','ohne Antwort'
    if NO.match(t): return '❌','nein'
    if VERD.match(t):
        return ('📋','ja mit Zusatz') if len(t)>40 else ('📋','ja')
    if OPEN.search(t) and len(t)<170: return '❓','offen/Rückfrage'
    if UNV.search(t) and len(t)<120: return '🧭','bedingt/unverbindlich'
    return ('📋','Freitext')
FLAG={('R16A',7):'K1',('R16A',10):'K1',('R16K',275):'K2',('R15G',152):'K5',('R17F',103):'K16',('R17B',24):'',('R17B',25):'',('R17B',26):'',('R2',22):'K1',('R10S',5):'K1',('R10S',11):'K1',('R5',14):'K8',('R9A',45):'K12',('R15A',18):'K4',('R16B',43):'K4',('R15F',137):'E-GE-004',('R15F',138):'E-GE-004',('R15F',139):'E-GE-004',('R15E',121):'K6',('R15E',111):'K1/Bewusstlosigkeit',('R15G',152):'K5',('R15G',153):'K5',('R14',62):'K14',('R17A',20):'K17',('R17F',103):'K16',('R20A',10):'K18',('R16R',428):'OE-82',('R12',21):'delegiert',('R16R',431):'K19',('R15E',102):'K15',('R11',85):'(einvernehmliche Dominanz)',('R2',21):'',('R10S',100):''}
# register
reg=open(REG,encoding='utf8').read()
entries=[]
for ln in reg.split('\n'):
    mm=re.match(r'^\| (E-[A-Z]{2}-\d{3}) \|',ln)
    if not mm: continue
    c=[x.strip() for x in ln.split('|')]
    eid=c[1]; src=c[3]; txt=c[4]+' '+(c[6] if len(c)>6 else '')
    msgset=set()
    for t in re.finditer(r'35#(\d+)[AEK]?(?:\s*[–-]\s*(\d+)[AEK]?)?',src):
        a=int(t.group(1)); b=int(t.group(2)) if t.group(2) else a
        if b-a>60: b=a
        msgset.update(range(a,b+1))
    txt2=re.sub(r'\d{1,2}\.\d{1,2}\.(?:\d{2,4})?','',txt)
    txt2=re.sub(r'35#\d+','',txt2)
    rngs=[]
    for t in re.finditer(r'(?<![\d.])(\d{1,3})\s*[–-]\s*(\d{1,3})(?![\d.])',txt2):
        a,b=int(t.group(1)),int(t.group(2))
        if a<=b<=500: rngs.append((a,b))
    singles=[int(x) for x in re.findall(r'(?:Fragen?|Frage|F\.)\s*(\d{1,3})(?![\d–-])',txt2)]
    entries.append((eid,msgset,rngs,singles))
def lo_ok(prev,p):
    return prev[0]>p['msg']
def reg_for(p,q,amsgs):
    PKM={x['msg'] for x in d}
    direct=set(amsgs)|{x+1 for x in amsgs if x+1 not in PKM}
    viapk={x+1 for x in amsgs if x+1 in PKM}
    qi=[];pk=[]
    for eid,ms,rg,sg in entries:
        hit_dir=bool(ms&direct); hit_pk=bool(ms&viapk)
        if not (hit_dir or hit_pk): continue
        match=any(a<=q<=b for a,b in rg) or q in sg
        if match: qi.append(eid)
        elif hit_dir: pk.append(eid)
    return qi,pk
out=collections.OrderedDict()
stats={}
allrows=[]
for p in d:
    lab=p['label']; rows=[]; cnt=collections.Counter(); pkreg=set()
    amsgs_all=sorted({k for v in p['ans'].values() for k,_ in v})
    for q,qt in p['qs']:
        a=p['ans'].get(str(q),[])
        # dedupe & order
        seen=set(); aa=[]
        for k,t in sorted(a,key=lambda x:x[0]):
            t=clean(t)
            if (k,t) in seen: continue
            seen.add((k,t)); aa.append((k,t))
        aa=[x for x in aa if x[1]]
        final=aa[-1][1] if aa else ''
        st,stl=status(final)
        pausch=None
        if st in ('❓','🧭') and aa:
            lastk=aa[-1][0]
            for k2 in range(lastk+1,p['nxt']):
                x2=msgs.get(k2)
                if not(x2 and x2[1]=='user'): continue
                tx=x2[3].strip().rstrip('-').strip()
                if len(tx)>22 or not GEN.match(tx) or re.search(r'\d|nacht|bin da|schlafen',tx,re.I): continue
                prev=msgs.get(k2-1)
                if prev and prev[1]=='assistant' and re.search(r'(?<![\d.])%d(?![\d])'%q,prev[3][:6000]) and lo_ok(prev,p):
                    pausch=k2;break
            if pausch: st,stl='📋','Rückfrage → pauschal bestätigt (35#%d)'%pausch
        # multi-answer: if first positive and later different, mark
        if len(aa)>1: stl+=' (nachgereicht/geändert)'
        fl=FLAG.get((lab,q),'')
        if lab=='R12' and q>=21: fl='delegiert'
        if fl=='delegiert': st,stl='🧭','delegiert'
        qq=clean(qt)
        if LEX.search(qq): qshow='[Wortlaut explizit; nicht wiedergegeben – Themenfamilie: '+TITLE[lab].split(': ',1)[-1]+']'
        else: qshow=trunc(qq,120)
        parts=[]
        for k,t in aa:
            t=re.sub(r'jungen mädchen und buben','jungen Personen [Originalwortlaut s. 35#749; Dokumentation: nur Erwachsene]',t,flags=re.I)
            if (lab,q)==('R15G',152):
                parts.append((k,'[abwertende Äußerung zur Orientierung; Wortlaut in der Quelle; siehe K5]')); continue
            if LEX.search(t):
                lead=t.split(' ')[0] if VERD.match(t) or NO.match(t) else ''
                s=('['+lead+' …; Details explizit, nicht wiedergegeben]') if lead else '[Antwort enthält explizite Details; nicht wiedergegeben]'
            else: s=trunc(t,130)
            parts.append((k,s))
        if not parts: ashow='–'
        elif len(parts)==1: ashow=parts[0][1]
        else: ashow=' → '.join(f'{s} (35#{k})' for k,s in parts)
        ak=','.join(f'35#{k}' for k,_ in aa) or '–'
        src=f'35#{p["msg"]}'+(' → '+ak if aa else '')
        qi,pk=reg_for(p,q,[k for k,_ in aa])
        pkreg.update(pk); pkreg.update(qi)
        regs=', '.join(qi[:3])+(f' (+{len(qi)-3})' if len(qi)>3 else '') if qi else '–'
        if st=='⚪' : regs='–'
        cnt[st+' '+stl.split(' (')[0]]+=1
        rows.append((q,esc(qshow),esc(ashow),src,f'{st} {stl}'+(f' · {fl}' if fl and fl!='delegiert' else ''),regs))
        allrows.append(dict(label=lab,q=q,ans=final,status=st,stl=stl,regs=qi,pk=pk,msgs=[k for k,_ in aa]))
    stats[lab]=(cnt,sorted(pkreg),amsgs_all)
    out[lab]=(p,rows)
json.dump(allrows,open('allrows.json','w'),ensure_ascii=False)
import pickle; pickle.dump((out,stats,TITLE,MOD),open('render.pkl','wb'))
# quick overview
tot=collections.Counter()
for l,(c,_,_) in stats.items(): tot.update({k.split(' ')[0]:v for k,v in c.items()})
print(tot)
