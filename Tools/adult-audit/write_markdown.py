#!/usr/bin/env python3
"""Write the question-answer matrix modules (36-40) into ./stage.
Run inside the work directory (cwd) that contains msgs35.json etc. Environment: SPN_ADULT_DOCS = path to Docs/Game/Adult."""
import os
import pickle,json,collections,os,re
out,stats,TITLE,MOD=pickle.load(open('render.pkl','rb'))
FILES=[('36-Frage-Antwort-Matrix-Runden-1-bis-9.md','Runden 1 bis 9',['R1','R2','R3','R4','R5','R6','R7','R8','R9A','R9B.1','R9B.2','R9B.3','R9C']),
('37-Frage-Antwort-Matrix-Runden-10-bis-13.md','Runden 10 bis 13 (inkl. Körperreaktions-Block)',['R10A','R10B','KR1','KR2','R10S','R11','R11B','R12','R13']),
('38-Frage-Antwort-Matrix-Runden-14-und-15.md','Runden 14 und 15',['R14','R14B','R15A','R15B','R15C','R15D','R15E','R15F','R15G','R15H']),
('39-Frage-Antwort-Matrix-Runde-16.md','Runde 16 (Master-Runde)',['R16A','R16B','R16C','R16D','R16E','R16F','R16G','R16H','R16I','R16J','R16K','R16L','R16M','R16N','R16O','R16P','R16Q','R16R']),
('40-Frage-Antwort-Matrix-Runden-17-bis-20.md','Runden 17 bis 20 und Pre-Audit',['PA','R17A','R17B','R17C','R17D','R17E','R17F','R17G','R17H','R17I','R18A','R18B','R18C','R19A','R19B','R20A'])]
NAMES={'01':'Grundregeln, Alter, Zustimmung','02':'Architektur, Pakete, Technik','03':'Interaktion, Taxonomie, Ablauf','04':'Steuerung, Darstellung, Kamera, Audio','05':'NPCs, Beziehungen, Präferenzen','06':'Körper, Physik, Gesundheit','07':'Charakteranpassung, Tattoos','08':'Welt, Orte, Branche, Wirtschaft','10':'Produktion, Recht, Vertrieb'}
HEAD='''# Adult {nr}: Frage-Antwort-Matrix, {titel}

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dies ist ein **Quellenprotokoll**, keine Spezifikation und kein Kanon. Es verzeichnet je Interviewfrage die (gekürzte) Frage, die Antwort des Entwicklers, die Fundstelle, einen **heuristischen** Status und den Registerbezug. Explizite Formulierungen der Quellen sind **automatisch abstrahiert**; der Wortlaut steht nur in der Primärquelle. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Methode, Grenzen und Gesamtzahlen: [Modul 41](41-Audit-Etappe-2-2.md). Quellenschlüssel: [Modul 13](13-Quelleninventar-und-Abdeckung.md#1-zitierschlüssel). Entscheidungen: [Register (Modul 14)](14-Entscheidungsregister.md). Weitere Matrizen: [36](36-Frage-Antwort-Matrix-Runden-1-bis-9.md), [37](37-Frage-Antwort-Matrix-Runden-10-bis-13.md), [38](38-Frage-Antwort-Matrix-Runden-14-und-15.md), [39](39-Frage-Antwort-Matrix-Runde-16.md), [40](40-Frage-Antwort-Matrix-Runden-17-bis-20.md).

## Lesehilfe

| Spalte | Bedeutung |
|---|---|
| **Nr.** | Fragenummer **innerhalb des Pakets** (Paket = eine Fragenliste des Assistenten; Nummern wiederholen sich zwischen Paketen). |
| **Frage** | erste ca. 120 Zeichen der Frage; bei expliziter Formulierung durch einen Hinweis ersetzt. |
| **Antwort** | Antwort des Entwicklers, auf ca. 130 Zeichen gekürzt. „Bereichsantworten“ (z. B. „1-9 ja, …“) gelten für den ganzen Bereich; dieselbe Antwort erscheint dann mehrfach. `A → B` = spätere Nachreichung. `–` = in den Nachrichten keine Antwort gefunden. |
| **Quelle** | `35#Frage-Nachricht → 35#Antwort-Nachricht`. |
| **Status** | **heuristisch** aus dem Antworttext abgeleitet (siehe unten), nie eine Entscheidung dieser Dokumentation. |
| **Register** | Register-Einträge, deren Text diese Frage **ausdrücklich nennt** (Fragenummer oder Bereich in Aussage/Vermerk) und deren Quelle die Antwortnachricht ist. `–` heißt: **kein Eintrag nennt diese Frage namentlich**. Das bedeutet **nicht**, dass die Aussage fehlt; Paketebene steht im Kopf jedes Abschnitts. |

**Status-Werte:** `📋 ja` / `📋 ja mit Zusatz` / `📋 Freitext` = Zustimmung bzw. inhaltliche Antwort des Entwicklers, **nicht** automatisch zu Zahlen oder Details, die der Assistent als Beispiel nannte. `📋 Rückfrage → pauschal bestätigt` = Rückfrage des Entwicklers, danach ein knappes „Ja/Ok passt“ direkt nach der Erklärung. `🧭 bedingt/unverbindlich` = „kann sein“ o. ä. `🧭 delegiert` = „mach, was du gut findest“. `❓ offen/Rückfrage` = Rückfrage ohne belegte Bestätigung. `❌ nein`. `⚪ ohne Antwort`. Zusätze nach `·` verweisen auf Konflikte ([Modul 19](19-Konflikt-und-Widerspruchsmatrix.md)) oder Register-Einträge.

**Module:** 01 [Grundregeln](01-Grundregeln-Alter-Zustimmung.md) · 02 [Architektur](02-Architektur-Pakete-Technik.md) · 03 [Interaktion](03-Interaktion-Taxonomie-Ablauf.md) · 04 [Steuerung](04-Steuerung-Darstellung-Kamera-Audio.md) · 05 [NPCs](05-NPCs-Beziehungen-Praeferenzen.md) · 06 [Körper](06-Koerper-Physik-Gesundheit.md) · 07 [Charakteranpassung](07-Charakteranpassung-Tattoos-Koerpermodifikation.md) · 08 [Welt](08-Welt-Orte-Branche-Wirtschaft.md) · 10 [Produktion/Recht](10-Produktion-Recht-Vertrieb.md). Die Modulzuordnung gilt **auf Paketebene** (heuristisch, nicht je Frage belegt).

**Inhalt**

{toc}

---
'''
total=collections.Counter()
for fn,titel,labs in FILES:
    nr=fn[:2]
    toc='\n'.join(f'- [{TITLE[l]}](#{re.sub(r"[^a-z0-9äöüß-]","",TITLE[l].lower().replace(" ","-").replace(":",""))})' for l in labs)
    # compute anchors properly later via python slug; simple approach: skip fancy
    lines=[HEAD.format(nr=nr,titel=titel,toc='{TOC}')]
    tocitems=[]
    for lab in labs:
        p,rows=out[lab]; cnt,pkreg,amsgs=stats[lab]
        hdr=f'{TITLE[lab]} (35#{p["msg"]})'
        tocitems.append(hdr)
        c=collections.Counter(r[4].split(' ')[0] for r in rows); total.update(c)
        cs=', '.join(f'{k} {v}' for k,v in sorted(c.items()))
        mods=', '.join(MOD[lab].split(', ')) 
        am=', '.join('35#%d'%k for k in amsgs) or '–'
        pr=', '.join(pkreg[:14])+(f' (+{len(pkreg)-14})' if len(pkreg)>14 else '') if pkreg else '–'
        lines.append(f'\n## {hdr}\n')
        lines.append(f'**Datum:** {p["date"]} · **Fragen:** {p["lo"]}–{p["hi"]} ({len(rows)}) · **Antwortnachrichten:** {am} · **Module (Paketebene):** {mods} · **Status:** {cs}  \n**Register (Paketebene, Quelle überschneidet sich):** {pr}\n')
        lines.append('| Nr. | Frage | Antwort | Quelle | Status | Register |\n|---:|---|---|---|---|---|')
        for r in rows:
            lines.append('| '+' | '.join([str(r[0]),r[1],r[2],r[3],r[4],r[5]])+' |')
    txt='\n'.join(lines)
    def slug(h):
        h=h.lower(); o=[]
        for ch in h:
            if ch.isalnum() or ch in '-_': o.append(ch)
            elif ch==' ': o.append('-')
        return ''.join(o)
    toc='\n'.join(f'- [{h}](#{slug(h)})' for h in tocitems)
    txt=txt.replace('{TOC}',toc)
    os.makedirs('stage',exist_ok=True)
    open('stage/'+fn,'w',encoding='utf8').write(txt+'\n')
    print(fn,txt.count('\n'))
print(total, sum(total.values()))
