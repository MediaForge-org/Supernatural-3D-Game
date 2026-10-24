# Kampfsystem 04: Nahkampf und Kampfstile

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sammelt, was der Entwickler über den Nahkampf entschieden hat (unbewaffnet, bewaffnet, Griffkampf, Bodenkampf, Fight Club, mehrere Gegner, Lernen von Techniken), und stellt Offenes daneben. Technikfamilien sind **Themeninventar**, keine Bewegungsliste. Zahlen (Reichweiten, Zeiten, Schaden) gibt es nicht. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Verteidigung (Blocken, Parieren, Ausweichen, Konter) steht in [Modul 07](07-Ausweichen-Blocken-Parieren-und-Konter.md), Steuerung in [Modul 06](06-Steuerung-und-Eingabekontext.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene Anforderungen](#2-beschlossene-anforderungen)
3. [Struktur des Nahkampfs: drei Ebenen](#3-struktur-des-nahkampfs-drei-ebenen)
4. [Technikfamilien als Themeninventar](#4-technikfamilien-als-themeninventar)
5. [Kampfstile](#5-kampfstile)
6. [Fight Club und Straße](#6-fight-club-und-straße)
7. [Mehrere Gegner, Improvisation, Waffenwechsel](#7-mehrere-gegner-improvisation-waffenwechsel)
8. [Techniken lernen](#8-techniken-lernen)
9. [Gegenüberstellung offener Lösungsansätze](#9-gegenüberstellung-offener-lösungsansätze)
10. [Schnittstellen und Fehlerfälle](#10-schnittstellen-und-fehlerfälle)
11. [Offene Punkte dieses Moduls](#11-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Der Nahkampf ist eines der am genauesten beschriebenen Systeme.** Der Entwickler verlangt einen Hybrid aus Boxen, Tritten, Fußarbeit, Clinch, Ringen, Takedowns, bodennaher Kontrolle, Scrambles und Aufgabegriffen, ausdrücklich ‚nicht UFC 2.0‘ und zugleich Straßenkampf (`23#163E`, `23#165E`, `M-PD` ‚Melee / grappling‘).
2. **Die Tiefe steckt in Zuständen, nicht in Tasten.** Kombinationen sind keine fertigen Animationsketten; Position, Distanz, Griff, Richtung und Körperzustand bestimmen, was möglich ist (`23#171E`, `M-PD`).
3. **Der Nahkampf hängt am Verletzungssystem.** Schläge, Würfe und Aufgabegriffe wirken auf Körperregionen; Verletzungen verändern Kraft, Griff und Balance (`M-PD`, `23#163E` P3–P4, P15–P16).
4. **Das Lernen von Techniken ist ein eigener Kernpunkt** (Chat 35, September 2026): Grundtechniken früh, anspruchsvolle über Training und Lehrer, kein klassisches Upgrade-System (`35#791E`). In Gameplay-Systeme §5.3 und im Master `M-PD` fehlt dieser Teil; er ist nur als ‚🧭, gehört zu einem späteren Kampf-Block‘ vermerkt.
5. **Offen sind Tiefe und Animationsabdeckung** (`M-OD`), die Steuerungsphilosophie (Direct oder Assisted, `35#792A`), die Rolle von Kombinationstasten (Spannung `CK-03`) und die Einzelheiten von Fight Club, Nichttödlichkeit und Lehrsystem.

---

## 2. Beschlossene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-NK-001 | **Hybrider Straßen-/Jäger-Nahkampf:** Schlagen, Treten, Fußarbeit, Distanzmanagement, Finten, Block/Parade/Ausweichen, Clinch, Ringen, Takedowns, Griffkampf, bodennahe Kontrolle, Scrambles, Aufgabegriffe | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#165E`; `23#167E` P1 | STE ANI | – |
| COMBAT-DEC-NK-002 | **Leichte und schwere Angriffe** existieren | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#163E` P1 | STE | Die Belegung der schweren Angriffstaste ist offen (Dreieck, [Modul 06](06-Steuerung-und-Eingabekontext.md)) |
| COMBAT-DEC-NK-003 | **Schlagvarianten in großer Zahl,** mit vielen Kopf- und Körpertreffern, Winkeln, Konterschlägen und Übergängen; ‚mehr Körpertreffer und generell mehr Auswahl‘ | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P2; `23#169E` P1–P2 | ANI | Technikfamilien als Inventar: Abschnitt 4; Umfang: `CQ-11` |
| COMBAT-DEC-NK-004 | **Kombinationen sind keine ununterbrechbaren Animationsketten:** Fehlschläge, Blocks und Konter verändern den Zustand; eine Kombination kann mittendrin zerfallen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#164A`; `23#165E` | STE ANI | Verhältnis zur gelernten Combo-Bibliothek: `CK-03` |
| COMBAT-DEC-NK-005 | **Fußarbeit und Reichweite zählen** (Distanz schließen und öffnen, Winkel wechseln) | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ | BEW ANI | – |
| COMBAT-DEC-NK-006 | **Keine harte Zielaufschaltung als Standard.** Schläge können verfehlen; eine optionale weiche Zielhilfe ist konfigurierbar. Der Entwickler: kein Lock-on, ‚weil man auch Schläge nicht treffen kann‘, aber die Wahl soll der Spieler haben | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#163E` P21 | STE SCH | Stärke und Standardwerte je Stufe: `CQ-15` |
| COMBAT-DEC-NK-007 | **Das System berücksichtigt die Kampfdistanz automatisch** (weit, Boxdistanz, Nahdistanz, Clinch, Boden); es gibt nicht ‚die eine beste Kombination‘ | 📋 BESCHLOSSEN | `23#166A` P1; `23#167E` P1 | STE | Zonengrenzen sind Konfiguration |
| COMBAT-DEC-NK-008 | **Clinch:** Kontrolle über Kopf, Arm, Unterhaken, Überhaken, Körper; Wanddruck, kurze Schläge, Knie, Takedown-Versuche und Verteidigung | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (clinch control, under/overhooks, body/arm control, wall pressure); `23#167E` P5 | BEW ANI | – |
| COMBAT-DEC-NK-009 | **Takedowns aus mehreren Familien** (ringerartig, wurfartig, situative Straßenvarianten) mit Verteidigung (Sprawl, Winkelwechsel); kein Takedown gelingt garantiert, Erfolg hängt von Timing, Position, Balance, Kraft, Können, Verletzung und Gegnerreaktion ab | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (trips/throws/takedowns); `23#167E` P6–P7 (‚passt gut‘, ‚perfekt‘) | BEW ANI | Die Erfolgsfunktion ist Entwurf ([Modul 12](12-Treffermodell-und-Koerperphysik.md)) |
| COMBAT-DEC-NK-010 | **Bodenkampf mit stabilen Positionen und umkämpften Übergängen** (Scrambles), in denen keiner volle Kontrolle hat; ‚nicht immer eindeutige Positionen, sondern so ein Mittelding‘, wie in echten Kämpfen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P8–P9; `23#168A` | BEW ANI | Stellenwert der Positionsliste: Abschnitt 4 |
| COMBAT-DEC-NK-011 | **Aufgabegriffe wirken auf den Körper** (Würgen, Armhebel, Schulterhebel, Beinhebel) und enden je nach Kontext mit Abklopfen, Bewusstlosigkeit oder echter Verletzung; kein Balken | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P17; `23#168A` | MED ANI | Tiefe: `CQ-11` |
| COMBAT-DEC-NK-012 | **Im tödlichen Straßenkampf kann anhaltendes Würgen nach der Bewusstlosigkeit töten;** auch eine Guillotine, aus der der Gegner nicht herauskommt, kann töten | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P17 | MED | Kein eigener Hinrichtungsbefehl (`23#168A`, Vorschlag) |
| COMBAT-DEC-NK-013 | **Fight Club:** gleiches physisches Kampfsystem; Ehrenkodex: niemand tötet oder verletzt schwer; Aufgabegriffe werden bei Abklopfen oder Bewusstlosigkeit gelöst; Verstöße haben ernste soziale, rechtliche und Spielfolgen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#169E` P17; `23#167E` P17 | NPC POL | Folgen im Detail: NK-014 |
| COMBAT-DEC-NK-014 | **Fight-Club-Ausgestaltung (Entwurf des Assistenten):** sofort loslassen bei Abklopfen, Stopp bei Bewusstlosigkeit, keine absichtlichen schweren Verletzungen, kein Weiterangreifen auf Wehrlose, Waffen in normalen Kämpfen verboten, Ringrichter oder Organisatoren greifen ein, Verstoß führt zu Ausschluss, Rufverlust, Feindschaften, Ermittlungen | 💡 VORSCHLAG | `23#170A` (Assistent; vom Entwickler nicht einzeln bestätigt, `23#171E` bezieht sich auf die Grapple-Frage) | NPC POL | Umfang: `CQ-13` |
| COMBAT-DEC-NK-015 | **Jeder Kämpfer und Jäger hat einen eigenen Stil und eigene Vorlieben,** keine starren Klassen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P11; `23#168A` | NPC KI | Datenmodell NK-016 |
| COMBAT-DEC-NK-016 | **Stilprofil als Vorschlag:** je Kämpfer Neigungen über Schlagen, Tritte, Ringen, Bodenkampf, Distanzverhalten (druckvoll/abwartend), keine Level | 💡 VORSCHLAG | `23#168A` (Beispielprofile des Assistenten); `M-P08` (Themeninventar ‚fighter-style-profile‘) | NPC KI | Abschnitt 5 |
| COMBAT-DEC-NK-017 | **Menschliche Gegner können fliehen, erstarren oder sich ergeben,** wenn sie genug eingeschüchtert sind; Furcht und Moral bleiben intern und zeigen sich im Verhalten | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘, ‚Multiple-opponent combat…‘; `23#222E` (Zusatz: ‚ja das wäre möglich wenn man den Gegner richtig Angst macht‘) | KI NPC | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-NK-018 | **Mehrere Gegner greifen nicht höflich nacheinander an:** gleichzeitiger Druck, Flankieren, Greifen, Ausgänge abschneiden, Chancen nutzen | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#221A` P1; `23#222E` P1 (‚das ist in tlou2 auch so glaub ich‘) | KI | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-NK-019 | **Körperkollision, Raumgeometrie und Engstellen gelten auch für Gegner;** sie behindern einander, Realismus wird nicht zum dauerhaften Betäubungs-Lock | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P2–P3 | BEW KI | – |
| COMBAT-DEC-NK-020 | **Griffkampf gegen mehrere Gegner ist erlaubt, aber systemisch gefährlich,** nicht künstlich gesperrt | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘, ‚Multiple-opponent combat…‘; `23#222E` P4; `23#167E` P13 | KI | – |
| COMBAT-DEC-NK-021 | **Improvisierte Gegenstände und die Umgebung** (Flasche werfen, Werkzeug, Wände, Möbel, Türen) können genutzt werden | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P5; `23#167E` P13 (‚wenn man was parat hat eine Flasche werfen, Messer zücken‘) | WAF BEW | Gegenstandsliste offen |
| COMBAT-DEC-NK-022 | **Eine normale Glasflasche zerbricht nach einem harten Schlag;** ein Rest kann als improvisierter Gegenstand bleiben | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P6 (‚Glas ist nach 1 Schlag kaputt‘) | WAF | – |
| COMBAT-DEC-NK-023 | **Wechsel zwischen Schusswaffe, Nahkampf, Messer und Improvisation sind fließend;** extrem enge Pistolenhaltung ist möglich | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P7 | WAF STE | Zugriffszeiten nach Trageort: [Modul 09](09-Waffenklassen-und-Ausruestung.md) |
| COMBAT-DEC-NK-024 | **Gegner können eine Schusswaffe physisch greifen und kontrollieren;** die Mündungsrichtung bleibt im Handgemenge physikalisch bedeutsam | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#162A` P13; `23#163E` P13 (‚ja das wäre stark‘) | WAF ANI | Lange Waffen sind anfälliger (Vorschlag `23#162A`) |
| COMBAT-DEC-NK-025 | **Nahkampfwerkzeuge in Kategorien** (unbewaffnet, kleine, mittlere, große Waffen) mit Zielkonflikten: mehr Wirkung bedeutet langsamer, schwerer, schlechter in engen Räumen, mehr Ausdauerverbrauch; die konkrete Waffenliste folgt später | 🧭 RICHTUNG | `23#162A` P2; `23#163E` P2 (‚die ganzen Waffen werden wir wenn wir die Schusswaffen machen dazu besprechen‘); `M-PD` ‚Gunplay / weapons‘ (Zielkonflikt für Waffen allgemein) | WAF | Liste: [Modul 09](09-Waffenklassen-und-Ausruestung.md), `CQ-33` |
| COMBAT-DEC-NK-026 | **Verletzungen beeinflussen den Nahkampf:** verletzter Arm (schwächerer Schlag, schlechteres Blocken, große Zweihandwaffen kaum brauchbar), Beinverletzung (Balance, Ausweichen, Tritte), Blutverlust (Kraft, Reaktion, Erschöpfung) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (Gliedmaßenfunktion, Waffeneinsatz, Balance, Ausdauer); `23#162A` P15; `23#163E` P15 | MED | – |
| COMBAT-DEC-NK-027 | **Treffer werden anatomisch ausgewertet** (Messerstich: Region, Tiefe, Gewebe, möglicherweise Gefäß oder Organ; Axt: Schneiden plus Energie plus Quetschung; Hammer: stumpfes Trauma, Bruchrisiko) | 📋 BESCHLOSSEN | `23#162A` P3; `23#163E` P3 (‚ja genau‘); `M-PD` ‚Human anatomy…‘ (Traumaarten) | TRF MED | Entwurf: [Modul 12](12-Treffermodell-und-Koerperphysik.md) |
| COMBAT-DEC-NK-028 | **Schläge haben Folgen** (Gesicht: Schmerz, Benommenheit; harter Kopftreffer: Gehirnerschütterung, Bewusstlosigkeit möglich; Tritt gegen Knie: Balanceverlust, Gelenkverletzung; Schlag gegen verletzte Rippen besonders problematisch) | 📋 BESCHLOSSEN | `23#162A` P4; `23#163E` P4; `M-CA0905` (Gehirnerschütterung als Teil der Spielergesundheit) | MED | Wirkstärken sind offen |
| COMBAT-DEC-NK-029 | **Nahkampf braucht Ausdauer,** aber ein trainierter Hunter ermüdet nicht nach wenigen Schlägen; schwere Waffen belasten mehr, Verletzung und Blutverlust wirken mit | 📋 BESCHLOSSEN | `23#162A` P5; `23#163E` P5; `23#116E` P21 | AU | [Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md) |
| COMBAT-DEC-NK-030 | **Finisher nur bei tatsächlicher Handlungsunfähigkeit** und passendem Kontext, Waffe und Wesen; kein permanenter Arcade-Knopf | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (‚Finishers only when target is effectively incapacitated…‘); `23#163E` P22 (‚B, ja warum nicht, aber nur wenn das Monster/Person sich nicht mehr wirklich bewegen kann‘) | ANI KRE | Umfang: `CQ-16` |
| COMBAT-DEC-NK-031 | **Steuerung ist kontextabhängig und lernbar,** der Spieler lernt keine riesigen Kombotabellen; Position, Distanz, Griff, Richtung und Kontext bestimmen die sinnvollen Aktionen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#171E`; `23#172A` (Ausgestaltung des Assistenten, 💡) | STE | Belegung: Modul 06 |
| COMBAT-DEC-NK-032 | **Monster nutzen die menschlichen Nahkampfregeln nur, wo Anatomie und Verhalten es erlauben;** humanoide übernatürliche Gegner können mehr davon nutzen; ein übernatürlich starkes Wesen lässt sich trotz perfekter Position nicht automatisch kontrollieren | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#166A` P14; `23#167E` P14 (‚genau‘) | KRE | [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md) |
| COMBAT-DEC-NK-033 | **Untergrund und Umgebung verändern den Kampf** (Asphalt, Schlamm, Motelzimmer, Treppe, Flur, Auto); ein harter Sturz auf Beton verletzt; Gegner können gegen Kanten gestoßen werden | 📋 BESCHLOSSEN | `23#166A` P12; `23#167E` P12 (‚passt‘) | TRF BEW | – |
| COMBAT-DEC-NK-034 | **Leistung im Nahkampf folgt dem tatsächlichen Körper-, Trainings- und Verletzungszustand;** keine pauschalen Geschlechtsmultiplikatoren oder Sperren; Kraft ist nicht dasselbe wie sichtbare Muskelgröße | 📋 BESCHLOSSEN | Spielvision §6.1; Gameplay-Systeme §9.2; `M-PD` ‚Body / physical response / recoil‘ | MED NPC | Größe und Kraft als Kampfmerkmal: `23#166A` P11 (Vorschlag) |
| COMBAT-DEC-NK-035 | **Grundtechniken sind früh verfügbar** (zum Beispiel Haken, Jab), anspruchsvolle (zum Beispiel ein Spinning Roundhouse Kick) ‚wissen die wenigsten‘ | 📋 BESCHLOSSEN | `35#791E` P3; `35#792A`; `35#797E` | STE | Liste der Grundtechniken offen (`CQ-12`) |
| COMBAT-DEC-NK-036 | **Fortgeschrittene Techniken werden über Training, Gyms, Trainer und andere Kämpfer erlernt;** man kann diese Personen kennenlernen | 📋 BESCHLOSSEN | `35#791E` P5; `35#792A`; `35#798A` | NPC STE | `CQ-12` |
| COMBAT-DEC-NK-037 | **Gelernte Techniken können verbessert werden, aber nicht über ein normales Upgrade-System** (Entwickler: ‚ein krasseres‘); kein pauschales Prozent-Upgrade | 📋 BESCHLOSSEN | `35#791E` P3; `35#794A` (Assistentenzusammenfassung: ‚kein stumpfes +5%-Skilltree-System‘) | STE | Mechanik offen (`CQ-12`) |
| COMBAT-DEC-NK-038 | **Unbekannte Techniken sind nicht voll nutzbar;** im Menü erscheinen höchstens Name oder Hinweis. Nur freigeschaltete Techniken werden mit Beschreibung, Tastenkombination und Vorschau angezeigt | 📋 BESCHLOSSEN | `35#791E` P3, P5; `35#794A` | UI STE | Menü und Combo-Rolle: `CQ-12`, `CK-03` |
| COMBAT-DEC-NK-039 | **Das Kampf-Lernsystem ist vom Interaktionssystem der Adult-Module getrennt;** beide können denselben Eingabe- und Animationsunterbau teilen | 📋 BESCHLOSSEN | `35#797E` (‚bei Kampf soll das so sein wie ich das gesagt habe‘); `35#798A` | STE ANI | Die Hybrid-Steuerung (Aktion plus analoge Kontrolle) gilt dort, nicht im Kampf |
| COMBAT-DEC-NK-040 | **Direct Control gegenüber Assisted Control im Kampf** (freie Körpersteuerung oder aktionsgestützte Combos) ist nicht entschieden; die Fragen bleiben ausdrücklich offen | ❓ OFFEN | `35#791E` P1, P2, P4, P6; `35#792A` (‚1–2 bleiben OPEN‘); `35#794A` | STE | `CQ-02` |
| COMBAT-DEC-NK-041 | **Stealth-Takedowns sind kein Sofort-Tod-Knopf:** bei Menschen sehr wirksam, aber fehlbar; bei Monstern artabhängig, manche erlauben keinen normalen Stealth-Takedown | 📋 BESCHLOSSEN | `23#162A` P18; `23#163E` P18 (‚ja bei Monstern ist das eher schwierig, bei Dämonen oder so würde es gehen‘) | KI KRE | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-NK-042 | **Begleiter greifen im Nahkampf ein** (Monster vom Partner wegziehen, schießen, schlagen; Partner wird niedergerissen und man muss entscheiden); Vertrauen kann die Zusammenarbeit beeinflussen | 🧭 RICHTUNG | `23#162A` P19; `23#163E` P19 (‚passt‘) | NPC KI | Gesamtregeln für Begleiter: `N-CO-006` |
| COMBAT-DEC-NK-043 | **Gore im Nahkampf ist ursachenabhängig** (Messer ≠ Hammer ≠ Axt); sehr brutal und realistisch | 📋 BESCHLOSSEN | `23#162A` P17; `23#163E` P17 (‚ja sehr brutal und realistisch‘) | TRF ANI | Schwellen: `CQ-47` |
| COMBAT-DEC-NK-044 | **Auf Grounded reagieren Gegner im Nahkampf schneller, nutzen Öffnungen, verfolgen nach, haben weniger künstliche Pausen;** Grapples sind gefährlicher; keine künstlichen zehnfachen Lebenspunkte | 📋 BESCHLOSSEN | `23#162A` P20; `23#163E` P20 (‚ja sie sind einfach schlauer, aggressiver‘) | KI SCH | – |
| COMBAT-DEC-NK-045 | **Entwaffnung gilt für Spieler und Gegner** (harter Armtreffer senkt die Griffkraft, die Waffe fällt); die Waffe liegt wirklich auf dem Boden und muss aufgenommen werden | 📋 BESCHLOSSEN | `23#162A` P14; `23#163E` P14; Gameplay-Systeme §2 (lose physische Gegenstände) | WAF PER | Beweis- und Aufräumregeln: [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| COMBAT-DEC-NK-046 | **Gegner sind keine Hitpoint-Puppen:** ein Messer im Unterarm schützt der Mensch den Arm, ein Beintreffer lässt ihn humpeln; bei Monstern hängt es von Anatomie und Schmerzempfinden ab | 📋 BESCHLOSSEN | `23#162A` P16; `23#163E` P16; `23#86E` P3 (humpeln, Bein halten) | KI ANI | Darstellung in späteren Versionen (`23#86E` P3) |
| COMBAT-DEC-NK-047 | **Kämpfe gehen am Boden weiter:** wer niedergeworfen ist, kann treten, blocken, wegkriechen, aufzustehen versuchen oder eine Waffe ziehen; Monster können ihn dort bedrängen | 📋 BESCHLOSSEN | `23#162A` P10; `23#163E` P10 (‚genau‘) | BEW | Eignung der Waffenziehung am Boden: `M-P08` (Themeninventar) |
| COMBAT-DEC-NK-048 | **Befreiung aus einem Griff geschieht ohne Quick-Time-Spam** (Richtung, Position, Kraft, kontextabhängige Aktion, Messer, Tritt, Begleiter greift ein) | 📋 BESCHLOSSEN | `23#162A` P9 (Vorschlag); `23#163E` P9 (‚gut‘) | STE | Eingabe: Modul 06 |
| COMBAT-DEC-NK-049 | **Eine Langwaffe ist ein reales Objekt im Nahkampf:** wegdrücken, Handschutz blocken, Schaftschlag; das kann die Waffe beschädigen (teure Optik) | 📋 BESCHLOSSEN | `23#162A` P12; `23#163E` P12 (‚natürlich‘) | WAF | Waffenschaden: [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md) |

---

## 3. Struktur des Nahkampfs: drei Ebenen

Die Quellen beschreiben den Nahkampf in drei Ebenen, die ineinander übergehen (`23#168A`, Zusammenfassung des Assistenten, nicht beschlossen als Struktur). Die Gliederung dient hier nur der Ordnung:

```text
STRIKING     Stand, Schläge, Tritte, Ellbogen, Knie, Distanzkontrolle
   ↕         Übergänge (Clinch-Eintritt, Takedown-Versuch)
CLINCH/WURF  Griffe, Wandkontrolle, Takedowns, Wurf, Verteidigung
   ↕         Übergänge (Landung, Scramble)
BODEN        stabile Positionen, umkämpfte Zwischenzustände, Aufgabegriffe
daneben:     Waffen, Umgebung, Verletzungen, mehrere Gegner, Monsterregeln
```

**Konsequenz für die Architektur (🧩 ABGELEITET aus `COMBAT-DEC-NK-004`, `-007`, `-010`):** Der Zustand eines Kampfes besteht nicht aus einem Animationsbaum, sondern aus (a) Position und Distanz, (b) Griffzustand, (c) Körperzustand beider Beteiligter. Das entspricht den Hauptmodi Schlagabtausch, Clinch, Boden aus [Modul 03](03-Kampfzustaende-und-Systemarchitektur.md) §4.

---

## 4. Technikfamilien als Themeninventar

> **💡 VORSCHLAG.** Die Listen stammen vom Assistenten (`23#166A`, `23#168A`) und vom Prompt-Entwurf `M-P08`. Der Entwickler fordert ‚mehr Auswahl‘ (`23#167E` P2, `23#169E` P1–P2), aber hat **keine** Technikliste beschlossen. Die Tabelle zeigt, was geplant werden müsste, nicht was kommt.

| Familie | Beispiele aus den Quellen | Quelle der Nennung | Entscheidungsstatus der Familie |
|---|---|---|---|
| Hände | Jab, Cross, Hook, Uppercut, Overhand, Körperhaken, Hammerfist | `23#166A`, `23#168A` | Familie 📋 (Schläge), Einzeltechniken 💡 |
| Ellbogen | horizontal, diagonal, kurz im Clinch | `23#166A` | 💡 |
| Beine | Low Kick, Body Kick, Head Kick, Front/Push Kick, Side Kick, Knie | `23#166A` | 💡 (Tritte als Familie sind 📋 über `M-PD`) |
| Riskante Techniken | Spinning-Techniken, fliegendes Knie nur in passenden Situationen | `23#166A`; `35#791E` P3 (Spinning Roundhouse als Beispiel einer gelernten Technik) | 💡 |
| Stehender Griffkampf | Collar/Upper-Body Control, Underhook, Overhook, Body Lock, Wrist/Arm Control, Back Control im Stand, Wanddruck | `23#166A` | Familie 📋 (`M-PD`), Namen 💡 |
| Takedowns | Single/Double Leg, Body Lock, Trip, Hüftwurf, Fußfeger, Wand- und Möbelsturz | `23#166A` | Familie 📋, Namen 💡 |
| Takedown-Verteidigung | Sprawl, Unterhaken, Winkel | `23#166A` | Familie 📋 (`23#167E` P7) |
| Boden | Mount, Half Guard, Guard, Side Control, Back Control, Turtle, North-South, Standing-over | `23#166A` | Familie 📋 (stabile Positionen, `M-PD`), Liste 💡 |
| Übergänge | Sweep, Guard Pass, Escape, Scramble, Aufstehen | `23#166A`, `23#168A` | Scramble 📋, Rest 💡 |
| Submissions | Chokes, Joint Locks (Arm/Schulter/Bein), Kompression, Kontrolle | `23#166A`, `23#168A` | Familie 📋, Namen 💡 |
| Waffen im Nahkampf | Messer, Machete, Beil, Hammer, Brecheisen, Axt, Hunter-Waffen | `23#162A` | 🧭 (Liste folgt) |

**Nicht beschlossene Annahmen, die diese Familienliste nicht stützt:** eine feste Gesamtzahl von Techniken, Tastenkombinationen je Technik, eine Anzahl an Positionen, ein Katalog an Animationen. Die Tiefe und die Animationsabdeckung sind in `M-OD` ausdrücklich offen (`CQ-11`).

---

## 5. Kampfstile

Quelle: `M-PD` ‚Melee / grappling‘ (individueller Stil statt Klassen), `23#167E` P11, `23#168A`. Die Beispiele des Assistenten:

| Beispiel (💡) | Ausprägung |
|---|---|
| Kämpfer A | Boxen sehr stark, Tritte mittel, Ringen schwach, Boden mittel; druckvoll, aggressiv, viele Körperschläge |
| Kämpfer B | Boxen mittel, Ringen und Jiu-Jitsu sehr stark, Tritte gering; wartet auf Fehler, Takedown, Bodenkontrolle |
| Kämpfer C | tritt-orientiert, viel Distanz, schnelle Fußarbeit, schwach im Clinch |

**Folgen aus den Quellen (🧩 ABGELEITET):**

1. Stile sind **Daten der Figur**, keine Klassen und keine Level (`COMBAT-DEC-GP-003`, `COMBAT-DEC-NK-015`).
2. Sie betreffen NPC-Kämpfer, Gegner, Rivalen und andere Jäger, die ‚im Nahkampf überlegen sein können, während du besser mit Schusswaffen bist‘ (`23#168A`, Formulierung des Assistenten). Persistente Rivalen aus dem Fight Club sind ein Vorschlag (`N-NE-021`).
3. Der Stil einer persistenten Figur gehört zu ihrem Datensatz (Persistenzstufen: [NPCs 03](../NPCs/03-Identitaet-Persistenzstufen-und-Datensatztiefe.md)); wie tief, hängt von der Stufe ab.
4. Der Spielerstil entsteht aus Gewohnheit und Training, nicht aus einer Klassenwahl (`COMBAT-DEC-GP-024`).

---

## 6. Fight Club und Straße

| Aspekt | Fight Club | Straße oder Lebensgefahr |
|---|---|---|
| System | gleiches physisches Kampfsystem (`COMBAT-DEC-NK-013`) | dasselbe |
| Regeln | Ehrenkodex: niemand tötet oder verletzt schwer (📋); weitere Regeln 💡 (`COMBAT-DEC-NK-014`) | keine Schiedsrichter; wer töten will, kann es |
| Aufgabegriffe | werden bei Abklopfen und Bewusstlosigkeit gelöst | können bis zur Bewusstlosigkeit und darüber hinaus anhalten (`COMBAT-DEC-NK-012`) |
| Verstoß | ernste soziale, rechtliche und Spielfolgen (📋 allgemein; Einzelheiten 💡) | Beweise, Zeugen, Strafverfolgung ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)) |
| Zweck | kontrolliertes Lernen und Kämpfen; Orte für Rivalitäten, Trainerkontakte, Ruf, Jägerkontakte, Nebenfälle (alles 💡, `23#164A`, `23#166A`) | – |

**Gegenüberstellung der Fight-Club-Varianten (offen, `CQ-13`):**

| Variante | Inhalt | Vorteil | Nachteil |
|---|---|---|---|
| A: Minimal | ein Ort, ein Regelwerk (Ehrenkodex), Kämpfe gegen NPCs | geringer Aufwand, deckt `COMBAT-DEC-NK-013` | Rivalitäten und Lernen bleiben dünn |
| B: Szene | mehrere Regelwerke (Boxen bis ohne Regeln, `23#166A` Vorschlag), Kämpfer mit Stil und Ruf, Trainer | trägt Lernsystem und Rivalen | hoher Inhalts- und Animationsaufwand |
| C: Nur Trainingsort | Fight Club vor allem als Lernort der Techniken (`35#791E` P5), Wettkampf zweitrangig | verbindet Lernsystem und Welt | keine eigene Szene als Spielinhalt |

Welche Variante gilt, bleibt der Projektleitung vorbehalten.

---

## 7. Mehrere Gegner, Improvisation, Waffenwechsel

Die Quellen beschreiben drei zusammenhängende Verhaltensweisen (`COMBAT-DEC-NK-018` bis `-024`):

1. **Mehrere Gegner:** Sie nutzen Positionierung, nicht einen künstlichen Bonus. Eine enge Tür macht aus drei Gegnern ein kontrollierbares Problem (`23#221A` P2, Beispiel des Assistenten, vom Entwickler mit ‚passt gut‘ bestätigt).
2. **Improvisation:** Nicht jeder Gegenstand ist eine Waffe, aber geeignete Dinge sind verwendbar; sie können kaputtgehen (Glas nach einem Schlag, Holz splittert, Metallstange hält länger; `23#221A` P6, Beispiel).
3. **Waffenwechsel unter Druck:** Pistole behalten (Point Shooting, wegdrücken), Waffe schützen, Messer ziehen, Abstand schaffen, Schaftschlag (`23#221A` P7 als Beispiel; `M-PD` verlangt ‚fluid transitions‘).

**Prüfbarer Kern (🧩 ABGELEITET):** Aus den Quellen folgen vier Verhaltensregeln, die ohne Zahlenwerte testbar sind: Gegner dürfen nicht nacheinander warten (NK-018), zwei Körper belegen nicht denselben Raum (NK-019), Grappling-Eintritt wird nicht gesperrt (NK-020), eine zerbrochene Flasche kann nicht ein zweites Mal zerbrechen (NK-022). [Modul 30](30-Testkatalog-und-Abnahmekriterien.md) übernimmt sie.

---

## 8. Techniken lernen

**Was die Quellen festlegen** (`COMBAT-DEC-NK-035` bis `-039`): Es gibt eine Bibliothek aus Techniken. Grundlagen sind früh verfügbar. Fortgeschrittene Techniken müssen gelernt werden, bei Trainern, in Gyms, von anderen Kämpfern. Unbekannte Techniken sind höchstens als Name sichtbar. Verbesserung ist kein stumpfes Prozent-Upgrade.

**Was fehlt:**

| Fehlende Festlegung | Betroffen | Offene Frage |
|---|---|---|
| Welche Techniken starten freigeschaltet | Einstieg | `CQ-12` |
| Wie Lehrer, Gym und Training die Freischaltung vergeben (Zeit, Geld, Beziehung, Aufgaben) | Weltintegration, Wirtschaft | `CQ-12` |
| Wie ‚Verbesserung ohne Upgrade-System‘ aussieht (Qualität der Ausführung, Variantenfreischaltung, Kombination, Gewohnheit) | Fortschritt | `CQ-06`, `CQ-12` |
| Wie Combo-Bibliothek und kontextabhängige Eingaben zusammenpassen | Steuerung | `CK-03`, `CQ-02` |
| Ob NPCs und Begleiter Techniken lernen und weitergeben | NPC-Gedächtnis | `CQ-12` |

**Zusammenhang mit der Progression:** Die Quellen führen ‚Progression‘ insgesamt als offen (`M-PD` ‚Progression — OPEN FOR DEDICATED DESIGN‘, `M-OD`). Das Lernsystem darf daher **kein** Fortschrittsmodell vorwegnehmen; es ist hier nur als Anforderung an die Steuerung und die Welt geführt.

---

## 9. Gegenüberstellung offener Lösungsansätze

| Offene Frage | Ansatz | Vorteil | Nachteil | Bezug |
|---|---|---|---|---|
| Steuerungsphilosophie im Kampf (`CQ-02`) | **A: Aktionsgestützt.** Bibliothek gelernter Techniken, Auslösung über kontextabhängige Eingaben | lernbar, klare Fortschrittsanzeige (gelernt/unbekannt) | wirkt „abgespielt“, wenn Zustände zu grob sind | `35#791E` P3, P5 |
| | **B: Direkt.** Kontinuierliche Steuerung von Körper und Gliedmaßen | Freiheit, kein Combo-Katalog | hohe Lernkurve, schwer mit Controller | `35#792A` (Beschreibung) |
| | **C: Hybrid** aus Aktion plus analoger Feinsteuerung | verbindet beides | Doppelte Komplexität; für Adult vorgeschlagen (`35#796A`), für Kampf ausdrücklich nicht übernommen (`35#798A`) | – |
| Rolle der Kombinationen (`CK-03`) | **A: Kombinationen als Abkürzungen** über Position und Kontext | passt zu `23#171E` | weniger ‚Combo-Gefühl‘ | `23#171E`, `M-PD` |
| | **B: Kombinationen als sichtbare Bibliothek** mit Tastenkombination | passt zu `35#791E` P3 und `35#798A` | Gefahr der Tastentabelle, Konflikt zu `23#171E` | `35#798A` |
| Technikumfang (`CQ-11`) | **A: Kleiner Kern** (wenige Familien, wenige Varianten) | Animationsaufwand gering | widerspricht ‚mehr Auswahl‘ (`23#167E` P2) | `M-OD` |
| | **B: Große Bibliothek** mit Varianten | trägt Lernsystem | Animationsaufwand sehr hoch, ‚Combination explosion‘ (`M-P14`: eigene Mitigation) | Modul 22 |
| Paar-Animation (Modul 22) | handgebaute Paare / prozedural (IK, Motion Warping) / Hybrid | – | – | `CQ-87` |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 10. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Verletzungsmodell | jede Wirkung (Schlag, Wurf, Hebel, Sturz) meldet Region, Mechanismus und Energie; das Modell bestimmt die Folge ([Modul 12](12-Treffermodell-und-Koerperphysik.md)) |
| zur Ausdauer | jede Handlung verbraucht Belastung; Verletzung und Blutverlust erhöhen sie ([Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md)) |
| zur Steuerung | Absicht plus Kontext ergibt die Handlung ([Modul 06](06-Steuerung-und-Eingabekontext.md)) |
| zu Animation | Paaranimationen, Motion Warping, IK ([Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md)) |
| zu NPCs und Fraktionen | Ruf im Fight Club, Rivalen, Zeugen ([Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md), [20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)) |
| zum Koop | Griffpaare mit zwei Spielern oder Spieler gegen NPC ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) |

| Fehlerfall | Erwartung |
|---|---|
| Gegner stirbt während eines Griffs | Griff löst sich, der Überlebende wird frei, kein hängender Griffzustand |
| Ein Dritter greift in einen Bodenkampf ein | Eingreifer hat Zugriff; bekannte Gefahr gemäß NK-020 |
| Beide Beteiligte verlieren das Bewusstsein | Griff endet, Zustand deterministisch aufgelöst |
| Erlernte Technik wird durch Verletzung unausführbar | Absicht wird mit Grund abgelehnt, nicht stumm ignoriert |
| Griff an nicht unterstützter Kreatur | Absicht wird mit Grund abgelehnt (NK-032) |

(Alle Fehlerfälle 💡 VORSCHLAG als Prüfziele für [Modul 30](30-Testkatalog-und-Abnahmekriterien.md).)

---

## 11. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-11` | Tiefe des Nahkampf-, Ring- und Aufgabegriffsystems und Animationsabdeckung (Entscheidungsstatus §5.6, `M-OD`) | `COMBAT-DEC-NK-003`, `-010`, `-011` | ja |
| `CQ-12` | Lehrsystem: Startumfang, Freischaltung über Gym/Trainer/Kämpfer, ‚Verbesserung ohne Upgrade-System‘, Combo-Bibliothek | `COMBAT-DEC-NK-035` bis `-038` | ja |
| `CQ-13` | Fight Club: Umfang, Regelwerke, Locations, Rivalen, Ausschluss und Folgen | `COMBAT-DEC-NK-013`, `-014` | ja |
| `CQ-14` | Nichttödliche Ausschaltung: Dauer und Erholung der Bewusstlosigkeit, rechtliche Folgen, Umgang mit Wesen | `COMBAT-DEC-NK-011`, `-012` | ja |
| `CQ-15` | Weiche Zielhilfe: Stärke, Standardwerte je Schwierigkeit, Controller gegenüber Maus | `COMBAT-DEC-NK-006` | ja |
| `CQ-16` | Finisher: Umfang, Bedingungen je Wesen und Waffe, Verhältnis zu Kill-Cams | `COMBAT-DEC-NK-030` | ja |
