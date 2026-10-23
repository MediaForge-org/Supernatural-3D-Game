# Kreaturen 23: Testkatalog und Abnahmekriterien

> [!IMPORTANT]
> **Es gibt keine Implementierung, also keine laufenden Tests.** Dieser Katalog leitet aus den **Prüfregeln der Module** Tests ab, die gelten, sobald eine Funktion gebaut wird. Er ist kein Testergebnis. Die Ebene gibt an, wo ein Test laufen könnte: **C** = portable C++20-Bibliothek (kein Unreal), **U** = Unreal-Automation oder Editor, **I** = Inspektion (Daten-/Dokumentationsprüfung), **M** = manuell. Das Verfahren folgt `CLAUDE.md` §5.

Teil der Kreaturen-Dokumentation ([Index](README.md)). Gleiche Methode wie [Ermittlung 23](../Ermittlung/23-Testkatalog-und-Abnahmekriterien.md).

**Inhalt**

1. [T-A: Klassifikation und Individuen](#t-a-klassifikation-und-individuen)
2. [T-B: Körper, Schwächen, Regeneration](#t-b-körper-schwächen-regeneration)
3. [T-C: Geister, Besessenheit, Flüche, Verträge](#t-c-geister-besessenheit-flüche-verträge)
4. [T-D: Höhere Wesen, Kosmologie, Seelen](#t-d-höhere-wesen-kosmologie-seelen)
5. [T-E: Ökologie und Lebenszyklus](#t-e-ökologie-und-lebenszyklus)
6. [T-G: Intelligente Wesen und Gesellschaft](#t-g-intelligente-wesen-und-gesellschaft)
7. [T-K: Monster-KI](#t-k-monster-ki)
8. [T-H: Hunterwelt, Organisationen, Kulte](#t-h-hunterwelt-organisationen-kulte)
9. [T-R: Rituale, Schutz, Artefakte, verfluchte Objekte](#t-r-rituale-schutz-artefakte-verfluchte-objekte)
10. [T-F: Lebendfang, Fallen, Containment](#t-f-lebendfang-fallen-containment)
11. [T-W: Forschung und Bestiarium](#t-w-forschung-und-bestiarium)
12. [T-V: Vorfälle und lebendige Welt](#t-v-vorfälle-und-lebendige-welt)
13. [T-O: Orte](#t-o-orte)
14. [T-P: Persistenz und Modding](#t-p-persistenz-und-modding)
15. [T-N: Nemesis, Rivalität, Konsistenz](#t-n-nemesis-rivalität-konsistenz)
16. [T-I: Inhaltsprüfungen (IP, Spoiler, Sprache)](#t-i-inhaltsprüfungen-ip-spoiler-sprache)
17. [Abnahmekriterien nach Ausbaustufe](#abnahmekriterien-nach-ausbaustufe)

---

## T-A: Klassifikation und Individuen

Gruppe zu [03-Klassifikation-Herkunft-und-Individuen.md](03-Klassifikation-Herkunft-und-Individuen.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-A01 | Existenzform und Herkunft sind getrennte Felder; ein Datensatz mit nur einem gemeinsamen Feld wird abgelehnt | C | `23#307A` P3 |
| T-A02 | Eine Art ohne die acht Unterscheidungen (Ökologie, Verhalten, Spuren, Schwäche, Jagdmethode, Kampf-KI, Lore, NPC-Wirkung) besteht die Prüfliste nicht | C, I | `23#1074A` P155 |
| T-A03 | Keine Spieler-Anzeige und kein Archivfeld enthält ein Level, einen Rang oder eine Stufe für Kreaturen | I | `M-PD` |
| T-A04 | Zwei Individuen derselben Art unterscheiden sich in mindestens einem von Alter, Erfahrung, Temperament, Narben | C | `23#1075E` P152 |
| T-A05 | Alter allein verändert keine Trefferpunkte oder Schadenswerte | C | `23#307A` P7 |
| T-A06 | Erfahrung steigt nur durch ein belegbares Ereignis | C | `23#307A` P7, P15 |
| T-A07 | Ein Ausnahmemerkmal ist im Archiv erst nach Beobachtung sichtbar | C, U | `23#307A` P8 |
| T-A08 | Das Archiv führt Art und Individuum getrennt | C | `23#307A` P30 |
| T-A09 | Keine Zufallserzeugung kombiniert Schwäche, Körper und Fähigkeit | C | `23#307A` P28 |
| T-A10 | Ein Eintrag mit mehreren Existenzformen wird akzeptiert | C | `23#307A` P2 |

---

## T-B: Körper, Schwächen, Regeneration

Gruppe zu [04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-B01 | Spezialmaterial ohne passende Schwäche verursacht keinen Zusatzschaden, aber die physische Verletzung bleibt | C | `23#293A` P3 |
| T-B02 | Ein physisch verletzbares, nicht damit tötbares Wesen bleibt nach normalem Beschuss am Leben | C | `23#293A` P4 |
| T-B03 | Ausfall eines Beins verlangsamt ein Wesen mit funktionierender Anatomie, außer es hat Kompensation | C | `23#307A` P12 |
| T-B04 | Fehlender Schmerz hebt strukturelle Beeinträchtigung nicht auf | C | `23#293A` P6 |
| T-B05 | Regeneration hat sieben Teilregeln und wird bei Unterdrückung gestoppt, wenn die Art das als Wirkungsart hat | C | `23#307A` P9, P13 |
| T-B06 | Narben eines nicht regenerierenden Individuums bleiben nach Speichern und Laden und im nächsten Treffer | C | `23#307A` P14 |
| T-B07 | Das Archiv zeigt keine Resistenzzahl, nur Beobachtung | I | `23#307A` P10 |
| T-B08 | Ein Ausnahmemerkmal ändert die Wirkung einer bekannten Schwäche | C | `23#307A` P8 |
| T-B09 | Ein Schuss auf einen Besessenen verletzt den Wirt, die Entität stirbt nicht dadurch | C | `23#359A` P12 |
| T-B10 | Übernatürliche Sofortheilung ist nicht ohne Quelle (Wesen, Ritual, Artefakt) auslösbar | C | `35#26E` |
| T-B11 | Gore bei Kindern wird nicht dargestellt | I, U | `23#1075E` P148 |

---

## T-C: Geister, Besessenheit, Flüche, Verträge

Gruppe zu [05-Geister-Besessenheit-und-Fluecher.md](05-Geister-Besessenheit-und-Fluecher.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-C01 | Ein Geist mit intaktem Anker kehrt nach Vertreibung zurück | C | `23#359A` P9 |
| T-C02 | Das Zerstören des einzigen Ankers beendet die Bindung | C | `23#359A` P9 |
| T-C03 | Eine unterbrochene Salzlinie schützt nicht mehr | C | `23#370A` P4; `23#359A` P26 |
| T-C04 | Ein Fluch eskaliert nach Plan auch ohne Spieler im Gebiet; Zustand übersteht Speichern und Laden | C | `23#359A` P10 |
| T-C05 | Ein Fluch löst sich über seine definierten Fluchtbedingungen | C | `23#359A` P10 |
| T-C06 | Besessenheitsgrad bestimmt, ob Erinnerungsfragmente entstehen | C | `23#362E` C |
| T-C07 | Die Hypothese 'Geist' bleibt Hypothese, bis Belege sie bestätigen | C | `23#359A` P7 |
| T-C08 | Im Detective Mode erscheint kein Marker auf Anker oder Fluchauslöser | U | `23#370A` P14 |
| T-C09 | Ein Vertrag wird nach seinen Bedingungen ausgewertet, nicht nach Autorenentscheid | C | `23#359A` P15 |
| T-C10 | Ein Tod führt nicht automatisch zu einem Geist | C | `23#458A` P2 |

---

## T-D: Höhere Wesen, Kosmologie, Seelen

Gruppe zu [06-Daemonen-Engel-und-Leviathane.md](06-Daemonen-Engel-und-Leviathane.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-D01 | Vor dem auslösenden Ereignis existiert kein Leviathan auf der Erde | C | `23#468E` |
| T-D02 | Gewöhnliche Waffen verursachen bei Leviathan-Artigen Verletzung, aber nie die Endtötung | C | `23#466E` P21 |
| T-D03 | Der Tod eines Gruppenanführers entfernt keine Mitglieder | C | `23#470E` |
| T-D04 | Eine zweite Tötung im Fegefeuer ist endgültig; das Archiv behauptet keine Rückkehr | C | `23#466E` A |
| T-D05 | Ein Gameplay-Tod des Hauptcharakters löst keine Jenseitsreise aus | C | `23#460A` |
| T-D06 | Ein NPC-Tod erzeugt einen Jenseitseintrag (Ziel oder Geist) | C | `23#460A` |
| T-D07 | Ein lebender Reisender kann ohne Methode keinen Toten abholen | C | `23#458A` P14 |
| T-D08 | Freies Reisen wird erst nach Entdeckung der Methode verfügbar | C | `23#458A` P9 |
| T-D09 | Die Zeitrelation Hölle wird bei Rückkehr auf Weltzustand und Alter angewandt | C | `PE-E1022`; `23#361A` P10 |
| T-D10 | Keine UI zeigt Karma, Seelentrefferpunkte oder eine Himmelswahrscheinlichkeit | I | `23#460A`; `23#361A` P9 |
| T-D11 | Ein Online-Partyjob verschiebt keine fremden Spieler in eine Ebene | C | `23#458A` P25 |

---

## T-E: Ökologie und Lebenszyklus

Gruppe zu [08-Oekologie-Populationen-und-Lebenszyklus.md](08-Oekologie-Populationen-und-Lebenszyklus.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-E01 | Ein getötetes Individuum erscheint nie wieder, außer über den benannten Fegefeuer-Weg | C | `23#1075E` P30, P37 |
| T-E02 | Ein vernichtetes Nest ist im Mindestzeitraum nicht aktiv; Wiederbesiedlung durch andere Individuen | C | `23#1076A` |
| T-E03 | Wanderung wegen Hunter-Druck verschiebt den Fall an einen neuen Ort | C | `23#293A` P20 |
| T-E04 | Ein Vorfall entsteht nur bei erfüllten Aktivitätsbedingungen, mit Vorlauf | C | `23#307A` P20–21 |
| T-E05 | Ein verpasstes Zeitfenster löscht den Fall nicht | C | `23#307A` P22 |
| T-E06 | Ein Monster-Kind existiert nur bei Arten mit Auswahlkennzeichen | C | `23#308E` B |
| T-E07 | Eine Territoriumsänderung ohne Spieler ist nach dem Laden vorhanden | C | `48#144A` |
| T-E08 | Eine Ebene-C-Entscheidung (Rudel verliert Gebiet) erscheint nach Regionsladung in Ebene B/A | C, U | `48#144A`; Modul 08 §11 |

---

## T-G: Intelligente Wesen und Gesellschaft

Gruppe zu [09-Intelligente-Wesen-und-Monstergesellschaft.md](09-Intelligente-Wesen-und-Monstergesellschaft.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-G01 | Ein Wesen mit Menschenidentität hat auffindbare Spuren dieser Identität | C | `23#1074A` P45–48 |
| T-G02 | Ein verschontes Wesen verschwindet nicht zwangsweise und wird nicht zwangsweise Feind | C | `M-PD` |
| T-G03 | Ein NPC-Hunter kann ein verschontes Wesen töten, wenn Wissen und Haltung es erlauben | C | `M-PD` |
| T-G04 | Wissensweitergabe zwischen Wesen braucht einen plausiblen Kanal | C | `M-PD` |
| T-G05 | Ein Wesen lügt nur, wenn seine soziale Intelligenz es erlaubt | C | `23#307A` P4 |
| T-G06 | Ein Vorfall verändert die menschliche Gesellschaft (Polizei, Medien, Wirtschaft) auch ohne Spieler | C | `48#146A` |
| T-G07 | Eine Monstergruppe entwickelt sich im Weltzustand auch ohne Spieler | C | `48#142A` |

---

## T-K: Monster-KI

Gruppe zu [10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-K01 | Die KI kennt die Spielerposition nur aus Wahrnehmung, Gedächtnis oder erlaubter Kommunikation | C | `M-PD`; `23#253A` P1 |
| T-K02 | Ein Lerneffekt entsteht nur aus einem vom Wesen beobachteten Ereignis | C | `23#293A` P12 |
| T-K03 | Ein übernatürlicher Hörreiz erreicht nur das benannte Ziel | C | `23#253A` P9 |
| T-K04 | Bewusste Stille tritt nicht in jeder Begegnung auf | C | `23#253A` P8 |
| T-K05 | Die Taktikwahl variiert und wiederholt nicht fest dieselbe Taktik | C | `23#253A` P6–7 |
| T-K06 | Der KI-Inspektor zeigt Fakten, Verdacht, Suchphase, Taktik und Begründung | M | `23#253A` P16 |
| T-K07 | Ein in einer wirksamen Falle gefangenes Wesen befreit sich nicht durch intelligente KI | C | `48#138A` |
| T-K08 | Dasselbe Individuum erinnert sich nach Speichern und Laden an einen gefundenen Trick | C | `48#139E` |
| T-K09 | Ein Rudelmitglied kennt die Spielerposition nur, wenn die Art einen Kanal hat | C | `M-PD` |
| T-K10 | Die KI nutzt keine Metadaten über den Spieler (keine globale Telemetrie) | C, I | `M-PD` |

---

## T-H: Hunterwelt, Organisationen, Kulte

Gruppe zu [11-Hunterwelt-Organisationen-und-Kulte.md](11-Hunterwelt-Organisationen-und-Kulte.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-H01 | Ein NPC-Hunter kann sterben; sein Tod steht im Weltzustand | C | `23#1075E` P12 |
| T-H02 | Ein NPC-Hunter besitzt nur erworbenes Wissen, auch falsches | C | `23#307A` P26 |
| T-H03 | Ein weitergegebener Fall hat ein Ergebnis, das der Spieler später erfährt | C | `23#1075E` P62–68 |
| T-H04 | Die Dichte der Hunter-Begegnungen für einen Zivil-NPC bleibt unter der Obergrenze (1–2 im Leben) | C | `23#1075E` P1 |
| T-H05 | Die Organisation führt keinen serienspezifischen Namen | I | `23#1076A`; `M-B09` |

---

## T-R: Rituale, Schutz, Artefakte, verfluchte Objekte

Gruppe zu [12-Jagd-Rituale-und-verfluchte-Objekte.md](12-Jagd-Rituale-und-verfluchte-Objekte.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-R01 | Ein Ritual scheitert bei fehlendem Pflichtteil | C | `23#359A` P16 |
| T-R02 | Eine Störung (zerstörtes Symbol) ändert den Ritualausgang | C | `23#359A` P17, P26 |
| T-R03 | Weder Daten noch UI enthalten einen Magiebalken | I | `23#1074A` P100 |
| T-R04 | Seltene Zutaten sind über einen definierten Weg erreichbar | C | `23#466E` P13 |
| T-R05 | Ein verfluchtes Objekt hat eine lückenlose Besitzerkette | C | `23#1074A` P108 |
| T-R06 | Ein weitergegebener Fluch erzeugt am neuen Ort einen Vorfall | C | `23#1074A` P109 |
| T-R07 | Unbekannte Artefakt-Eigenschaften erscheinen als unbekannt | C, I | `23#359A` P19 |
| T-R08 | Ein verfluchtes Objekt im Vault bleibt gesichert, bis es entnommen wird | C | `23#1074A` P113–114 |

---

## T-F: Lebendfang, Fallen, Containment

Gruppe zu [13-Lebendfang-Fallen-und-Containment.md](13-Lebendfang-Fallen-und-Containment.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-F01 | Eine Falle wirkt nur bei Arten, deren Regel sie nennt | C | `48#130A` |
| T-F02 | Eine beschädigte Falle ist unwirksam | C | `48#134A` |
| T-F03 | Regen verschlechtert eine ungeschützte Zeichnung im Freien, nicht im Gebäude | C | `48#134A` |
| T-F04 | Der Fallenzustand schreitet voran, auch wenn der Bereich nicht geladen ist | C | `48#134A` |
| T-F05 | Ein NPC kann eine Falle beschädigen, ohne dass der Spieler es erfährt | C | `48#134A` |
| T-F06 | Ein Gefangener ist ein Individuum im Spielstand und erinnert sich | C | `48#139E` |
| T-F07 | Ein gefangenes Individuum fehlt in der Populationssimulation seiner Gruppe | C | `23#1076A` |
| T-F08 | Es gibt keine Zelle, die für alle Arten gilt | C, I | `48#130A` |

---

## T-W: Forschung und Bestiarium

Gruppe zu [14-Forschung-und-Bestiarium.md](14-Forschung-und-Bestiarium.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-W01 | Der Eintrag zeigt kein Feld ohne Claim mit Inhalt | C, U | `23#295A`; `M-PD` |
| T-W02 | Interne Daten (Gewichte, Formeln, Spawn-Regeln) erscheinen in keiner Spieler-UI | I | `23#295A` |
| T-W03 | Eine bestätigte Schwäche gilt nur mit ihren Bedingungen | C | `23#307A` P8 |
| T-W04 | Ein Experiment mit unsauberem Aufbau kann ein falsches Ergebnis liefern | C | `48#128A` |
| T-W05 | Ein alter Text kann 'Disproven' werden, ohne zu verschwinden | C | `23#307A` P25 |
| T-W06 | Der Begleiter nennt eine nur vermutete Schwäche nicht als Tatsache | C, U | `48#126A` |
| T-W07 | Die Kategorie unbekannter Inhalte bleibt verborgen | U | `M-PD` |
| T-W08 | Wissen wird nicht zwischen Spielständen übertragen | C | `48#108A` |

---

## T-V: Vorfälle und lebendige Welt

Gruppe zu [15-Vorfaelle-und-lebendige-Welt.md](15-Vorfaelle-und-lebendige-Welt.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-V01 | Ein dynamischer Vorfall entsteht aus einem Weltzustand, nicht aus einem Zufallsspawn | C | `23#307A` P21 |
| T-V02 | Vor einem erkannten Fall gibt es mindestens ein schwaches Signal | C | `M-PD` |
| T-V03 | Ohne Marker bleibt jeder Pflichtfall lösbar | C, I | `23#308E` P21 |
| T-V04 | Ein ignorierter Vorfall hat nach Fristablauf mindestens ein Weltzustandsergebnis | C | `23#1074A` P63–68 |
| T-V05 | Ein Fall kann ohne Monster enden (False Positive) | C | `23#1074A` P72 |
| T-V06 | Ein Fall mit gemischter Ursache lässt sich als gemischt erkennen | C | `23#1074A` P73 |

---

## T-O: Orte

Gruppe zu [16-Uebernatuerliche-Orte-und-Geografie.md](16-Uebernatuerliche-Orte-und-Geografie.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-O01 | Kein Immobilienangebot zeigt die Ursache als Klartext | I | `23#1076A` |
| T-O02 | Ein Haus mit Anker behält das Problem nach Renovierung | C | `23#1075E` P119 |
| T-O03 | Ein Friedhof erzeugt nicht automatisch einen Geist | C | `23#1075E` P129 |
| T-O04 | Ein Hotspot erzeugt keinen Kartenmarker | I | `23#1076A` |

---

## T-P: Persistenz und Modding

Gruppe zu [17-Persistenz-Daten-und-Modding.md](17-Persistenz-Daten-und-Modding.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-P01 | Speichern und Laden erhalten Status, Narben, Gedächtnis und Gruppenstruktur eines Individuums | C | `M-PD` |
| T-P02 | Ein getötetes Individuum bleibt nach dem Laden tot | C | `23#1075E` P30 |
| T-P03 | Ein Mod-Monster überschreibt keine Basisdaten | C | `23#293A` P23 |
| T-P04 | Die Zahl der Schreibvorgänge je Individuum bleibt unter der Obergrenze | C | Modul 17 §4.2 |
| T-P05 | Das Journal ist nach Verdichtung gleichwertig zum Original | C | Modul 17 §8 |
| T-P06 | Im Online-Modus sind Mods nicht ladbar | C, M | `23#551E` |

---

## T-N: Nemesis, Rivalität, Konsistenz

Gruppe zu [10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-N01 | Ein überlebender Gegner kehrt mit seinen Narben und dem Wissen zurück, das er beobachtet hat | C | `PE-N1022` C |
| T-N02 | Die Anpassung eines Nemesis bleibt innerhalb der Artregeln und Schwächen | C | Proposal 0010 §4.4 |
| T-N03 | Ein toter Einzelner handelt nicht mehr | C | Proposal 0010 §7 |
| T-N04 | Eine Verletzung ist in jedem System sichtbar, das die Person zeigt | C | Proposal 0010 §7 |
| T-N05 | Ein Zeuge oder Wesen weiß nur, was es beobachtet oder gehört hat | C | Proposal 0010 §7; `M-PD` |
| T-N06 | Eine Fraktion hält kein Gebiet, das ihre Mitglieder nicht erreichen können | C | Proposal 0010 §7 |
| T-N07 | Der Zustand nach Speichern und Laden ist gleich dem Zustand davor | C | Proposal 0010 §7 |
| T-N08 | Nach dem Laden entspricht das Beobachtbare dem persistenten Datensatz, auf die Gegenwart vorgerückt, ohne Rücksetzen wichtiger Ergebnisse | C, U | Proposal 0010 §7 |

---

## T-I: Inhaltsprüfungen (IP, Spoiler, Sprache)

Gruppe zu [01-Quellenlage-und-Methodik.md](01-Quellenlage-und-Methodik.md).

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-I01 | Kein Datensatz der Basisspiel-Arten hat ip_provenance 'series_specific' ohne eigene Variante | I | `23#376A`; `23#379E` |
| T-I02 | Keine Dokumentationsdatei enthält Handlungsdetails der Serie | I | `23#1091E` |
| T-I03 | Jede Kreatur im Basisspiel hat ein Artenblatt gemäß Modul 19 | I | Modul 19 §4 |
| T-I04 | Jede 📋-Zeile des Registers hat eine Quelle, die der Prüfer findet | I | Modul 20 |

---

## Abnahmekriterien nach Ausbaustufe

> 💡 **Vorschlag.** Die Zuordnung folgt den Phasen aus `CLAUDE.md` §4 und ist Entscheidung des Entwicklers.

| Stufe | Inhalt | Tests |
|---|---|---|
| **Phase 0 (Kerntechnik)** | Weltzustand trägt einen Kreaturenstatus (lebt/tot), ein Individuum überlebt Speichern und Laden | T-P01, T-P02, T-E01 |
| **Phase 1 (Vertical Slice)** | ein vollständiger Fall mit einer Art: Vorlauf, Ermittlung, Hypothese, Vorbereitung, Konfrontation, Folgen | T-A, T-B, T-V, T-W, T-K01 bis T-K05 |
| **Phase 2** | mehrere Arten, Gedächtnis, Gruppen, Fallen, Rituale | T-E, T-G, T-K, T-R, T-F, T-N |
| **Phase 3 und später** | Reiche, Seelen, Containment-Netz, NPC-Hunter, Online | T-C, T-D, T-H, T-O, T-P |

**Summe:** 124 Tests in 16 Gruppen.
