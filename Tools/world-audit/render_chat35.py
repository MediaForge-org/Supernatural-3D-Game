#!/usr/bin/env python3
"""Erzeugt Modul 27 (Abdeckungsprotokoll Chat 35, Nachrichten 99-875).
Usage: python3 render_chat35.py <msgs35.json> <chat35_read_log.json> <Docs/Game/World> <Ausgabedatei>
Die Klassen stammen aus dem Lesenachweis (chat35_read_log.json); alle Nutzernachrichten wurden angezeigt (gekürzt oder voll),
Assistentennachrichten nur dort, wo sie im Lesenachweis stehen. Die Welt-Stichworttreffer sind eine Zählung, kein Leseersatz."""
import json, re, sys, glob, os, collections
msgs = {x[0]: x for x in json.load(open(sys.argv[1]))}
log = json.load(open(sys.argv[2])); d = sys.argv[3]
a, b = log["range"]
kw = re.compile(r"km²|km2|Metropolis|Untergrund|Stadt|Städte|Hölle|Fegefeuer|Himmel|Zeitreise|Motorsport|Rennstrecke|Fahrzeug|Auto|Bahn|Zug|Straße|Autobahn|Flughafen|Berg|See|Fluss|Wald|Küste|Insel|Weltkarte|Map\b|Realm|Reich", re.I)
cls = {}
def cut(i):
    for lo, hi, c in log["user_cut_by_range"]:
        if lo <= i <= hi: return c
for i in range(a, b + 1):
    x = msgs.get(i)
    if not x: continue
    if x[1] == "user":
        if i in log["user_full"] or len(x[3]) <= cut(i): cls[i] = "U1"   # vollständig gelesen
        else: cls[i] = "U2"                                              # gekürzt gelesen, Rest per Stichwort
    else:
        cls[i] = "A1" if i in log["assistant_context"] else "A2" if i in log["assistant_heads"] else "A3"
name = {"U1": "Nutzer, vollständig gelesen", "U2": "Nutzer, Anfang gelesen, Rest per Stichwortsuche", "A1": "Assistent, Kontext gelesen (Auszüge)", "A2": "Assistent, nur Überschriften/Anfang", "A3": "Assistent, nur Stichwortzählung"}
reg = collections.defaultdict(set)
for line in open(glob.glob(d + "/20-*.md")[0], encoding="utf-8"):
    m = re.match(r"\| (W-[A-Z]+-\d+) \|", line)
    if m:
        for n in re.findall(r"35#(\d+)[EA]", line):
            if a <= int(n) <= b: reg[int(n)].add(m.group(1))
out = []; P = out.append
P("# Welt 27: Abdeckungsprotokoll Chat 35 (Nachrichten 99–875)\n")
P("> [!IMPORTANT]\n> **Generiertes Modul** (`Tools/world-audit/render_chat35.py` mit `chat35_read_log.json`). Es hält fest, welche Nachrichten in welcher Tiefe geprüft wurden. Assistentenantworten sind **keine** Belege für Entwicklerentscheidungen; sie liefern nur Kontext. Statuszeichen: [Legende](README.md#statuslegende).\n")
P("Teil der Welt-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md).\n")
P("## 1. Ergebnis\n")
c = collections.Counter(cls.values())
P(f"Im Bereich {a}–{b} liegen {len(cls)} Nachrichten ({sum(v for k, v in c.items() if k[0]=='U')} vom Entwickler, {sum(v for k, v in c.items() if k[0]=='A')} vom Assistenten).\n")
P("| Klasse | Bedeutung | Nachrichten |\n|---|---|---:|")
for k in ("U1", "U2", "A1", "A2", "A3"): P(f"| {k} | {name[k]} | {c[k]} |")
P("")
us = [i for i in cls if cls[i][0] == "U"]
P(f"Alle {len(us)} Entwicklernachrichten wurden angezeigt und auf Weltaussagen gesichtet. Der überwiegende Teil betrifft das Adult-System, Modding, Werkzeuge und Archivarbeit (in `Docs/Game/Adult/` beziehungsweise nicht weltrelevant). Weltrelevant waren {len(reg)} Nachrichten mit Registerbezug (siehe Abschnitt 3).\n")
P("**Grenzen:** Die Assistentenantworten der Klasse A3 ({} Nachrichten, rund {:.2f} MB Text) wurden nur nach Stichwörtern gezählt; Treffer mit hoher Zahl wurden gelesen (A1/A2). Eine Aussage nur dort wäre nicht erfasst, wäre aber ohnehin nur ein Vorschlag, solange keine Entwicklerantwort folgt.\n".format(c["A3"], sum(len(msgs[i][3]) for i in cls if cls[i] == "A3") / 1e6))
P("---\n\n## 2. Nachrichten nach Klasse und Bereich\n")
def role(i): return 'E' if msgs[i][1] == 'user' else 'A'
for k in ("U2", "A1", "A2"):
    P(f"**{k} ({name[k]}):** " + ", ".join(f"35#{i}{role(i)}" for i in sorted(i for i in cls if cls[i] == k)) + "\n")
P("**U1:** alle übrigen Entwicklernachrichten im Bereich. **A3:** alle übrigen Assistentennachrichten im Bereich.\n")
P("---\n\n## 3. Nachrichten mit Registerbezug\n")
P("| Nachricht | Datum | Klasse | Register |\n|---|---|---|---|")
for n in sorted(reg):
    P(f"| 35#{n}{role(n)} | {msgs[n][2][:10]} | {cls.get(n, '–')} | {', '.join(sorted(reg[n]))} |")
P("\n---\n\n## 4. Gelesene Assistentenantworten (Kontext)\n")
P("Diese Antworten wurden gelesen, um die Zustimmung des Entwicklers einordnen zu können: " + ", ".join(f"35#{i}A" for i in sorted(log["assistant_context"])) + ".\n")
P("Nur Überschriften oder Anfang gelesen: " + ", ".join(f"35#{i}A" for i in sorted(log["assistant_heads"])) + ".\n")
P("---\n\n## 5. Nach Datum\n")
bd = collections.Counter(msgs[i][2][:10] for i in cls)
P("| Datum | Nachrichten |\n|---|---:|")
for k in sorted(bd): P(f"| {k} | {bd[k]} |")
open(sys.argv[4], "w", encoding="utf-8").write("\n".join(out) + "\n")
