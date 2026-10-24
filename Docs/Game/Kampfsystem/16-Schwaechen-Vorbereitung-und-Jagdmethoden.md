# Kampfsystem 16: Schwächen, Vorbereitung und Jagdmethoden

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul beschreibt, wie Recherche, Vorbereitung und die individuelle Schwäche eines Wesens den Kampf bestimmen. Es **verweist** auf die Kreaturen- und Ermittlungs-Register (`K-…`, `E-…`) und legt keine Schwächen einer konkreten Art fest. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Kreaturenregeln: [Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md), [12](../Kreaturen/12-Jagd-Rituale-und-verfluchte-Objekte.md), [14](../Kreaturen/14-Forschung-und-Bestiarium.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Von der Recherche zur Konfrontation](#3-von-der-recherche-zur-konfrontation)
4. [Wissen, Irrtum und Rückmeldung im Kampf](#4-wissen-irrtum-und-rückmeldung-im-kampf)
5. [Jagdmethoden im Überblick](#5-jagdmethoden-im-überblick)
6. [Gegenüberstellung offener Lösungsansätze](#6-gegenüberstellung-offener-lösungsansätze)
7. [Schnittstellen und Fehlerfälle](#7-schnittstellen-und-fehlerfälle)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Die Kernschleife ist beschlossen:** Ankunft, Dialog, Ermittlung, Recherche, Vorbereitung, Konfrontation unter Ausnutzung der Schwäche, dauerhafte Konsequenzen (Spielvision §2.2, Vertical Slice als Zielbild).
2. **Eine Schwäche ist mehr als Bonusschaden:** Sie kann tödlich, schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, enthüllend oder fähigkeitsstörend wirken (`K-KO-008`).
3. **Der Spieler weiß nur, was er erforscht hat.** Das Archiv zeigt Wissen, keine Debug-Werte; Wissen kann falsch sein, Begleiter irren (`K-FO-001`, `K-FO-003`).
4. **Zu einem Wesen gibt es mehrere Wege** (töten, vertreiben, bannen, fangen, retten, verhandeln); Vorbereitung wird belohnt, Improvisation bleibt möglich (`E-FA-009`, `K-KO-014`).
5. **Offen:** Schwellen für Hypothese und Bestätigung, Kosten und Risiko von Ritualen, Zahl und Art der Wege je Wesen (`K-FO-012`, `K-JA-011`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-SJ-001 | **Die Kernschleife ist Ankunft → Dialog → Ermittlung → Recherche → Vorbereitung → Konfrontation unter Ausnutzung der Schwäche → dauerhafte Konsequenzen** | 🧭 RICHTUNG | Spielvision §2.2 (Zielwerte vorläufig) | ERM KRE | – |
| COMBAT-DEC-SJ-002 | **Die richtige Waffe, Munition und Kreaturschwäche sind Kernstück der Jagd;** ein unbekanntes Wesen kann extrem gefährlich sein, dasselbe mit guter Vorbereitung kontrollierbar | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#85A` P1 | KRE WAF | – |
| COMBAT-DEC-SJ-003 | **Schwächen haben Wirkungsarten** (tödlich, schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, enthüllend, fähigkeitsstörend) | 📋 BESCHLOSSEN | `K-KO-008` | KRE MUN | – |
| COMBAT-DEC-SJ-004 | **Eine Schwäche kann bei Ausnahme-Individuen teilweise versagen** | 📋 BESCHLOSSEN | `K-KL-009` | KRE | Prüfziel für Tests |
| COMBAT-DEC-SJ-005 | **Das Archiv zeigt das Wissen des Hunters, keinen Debugger;** Statuswörter Confirmed, Suspected, Disputed, Disproven, Historical/Unverified | 📋 BESCHLOSSEN | `K-FO-001`, `K-FO-002` | UI ERM | – |
| COMBAT-DEC-SJ-006 | **Wissen kann falsch sein, Begleiter irren;** Monsterwissen stammt aus Vorwissen, Forschung, Notizen und unzuverlässigen Quellen | 📋 BESCHLOSSEN | `K-FO-003`, `K-FO-004` | ERM NPC | – |
| COMBAT-DEC-SJ-007 | **Umfassende freie Monsterforschung** (Beobachten, Spuren, Proben, Tests) ist vorgesehen | 📋 BESCHLOSSEN | `K-FO-005` | ERM KRE | – |
| COMBAT-DEC-SJ-008 | **Methoden zum Töten verlangen Recherche oder eigenes Testen;** es gibt mehrere Methoden, Fangen und Verhör sind möglich | 📋 BESCHLOSSEN | `E-FA-013`; `K-KO-014` | ERM KRE | – |
| COMBAT-DEC-SJ-009 | **Auflösungsarten:** töten, vertreiben, bannen, fangen, Fluch brechen, retten, Artefakt zerstören oder sichern, verhandeln, überleben, evakuieren | 📋 BESCHLOSSEN | `E-FA-009`; `M-PD` | ERM KRE | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-SJ-010 | **Rituale brauchen Zutaten, Vorbereitung, Zeit und Ort;** Fehler haben Folgen; keine Quick-Time-Rituale als Standard | 📋 BESCHLOSSEN | `K-JA-001`, `K-JA-003`, `K-JA-004` | KRE STE | Kosten: `K-JA-011` ❓ |
| COMBAT-DEC-SJ-011 | **Schutzmaßnahmen sind real, nicht absolut und zerstörbar** | 📋 BESCHLOSSEN | `K-JA-005` | KRE | – |
| COMBAT-DEC-SJ-012 | **Vorbereitung vor der Jagd:** Archiv → Hypothese → Ausrüstung aus Kofferraum oder Unterschlupf → Einsatz mit begrenztem Körperinventar | 🧭 RICHTUNG | `23#85A` P10; `23#86E` P10; `COMBAT-DEC-WA-004` | WAF ERM | [Modul 09](09-Waffenklassen-und-Ausruestung.md) |
| COMBAT-DEC-SJ-013 | **Hunter-Munition wirkt nur bei passender Schwäche** | 📋 BESCHLOSSEN | `23#155E` P5; `COMBAT-DEC-MU-004` | MUN KRE | – |
| COMBAT-DEC-SJ-014 | **Resistenz ist intern gestuft; die Oberfläche zeigt beobachtete Belege** (zum Beispiel ‚Kugeln wirkten kaum‘), keine Prozentwerte | 📋 BESCHLOSSEN | `K-KO-009`; Spielvision §9.2 | UI KRE | `K-KO-018` ❓ |
| COMBAT-DEC-SJ-015 | **Eine Falschklassifikation kann zu einer wirkungslosen Vorbereitung führen** (Folge der Unsicherheit des Wissens) | 🧩 ABGELEITET | Prämissen: `K-FO-003`, `COMBAT-DEC-SJ-002` | KRE ERM | Prüfziel: Szenario 5 ([Modul 26](26-Querschnittsszenarien.md)) |
| COMBAT-DEC-SJ-016 | **Der Kampf selbst liefert Wissen:** Reaktionen des Wesens auf Waffen und Methoden fließen in Archiv und Notizen, ohne Schwellen zu behaupten | 💡 VORSCHLAG | Anwendung von `K-FO-004` und `K-KI-013` | ERM KI | Schwellen: `CQ-67` |
| COMBAT-DEC-SJ-017 | **Schwellen für Hypothese und Bestätigung sowie der Umgang mit falscher Klassifikation** sind nicht festgelegt | ❓ OFFEN | `K-FO-012`; `M-OD` | ERM | `CQ-67` |
| COMBAT-DEC-SJ-018 | **Kosten- und Risikomodell der Rituale** ist nicht festgelegt | ❓ OFFEN | `K-JA-011` | KRE | `CQ-68` |
| COMBAT-DEC-SJ-019 | **Rückmeldung im Kampf über die Wirksamkeit** (Wirkungsanzeige, Hinweise, Audio) ist nicht entschieden | ❓ OFFEN | `K-KO-018`; `COMBAT-DEC-ST-007` | UI | `CQ-69` |

---

## 3. Von der Recherche zur Konfrontation

```text
Hinweise (Ermittlung) -> Hypothese (Archiv: Suspected)
  -> Vorbereitung: Methode wählen (Waffe, Munition, Ritual, Falle, Bann)
  -> Konfrontation: Methode wirkt, wirkt teilweise oder versagt
  -> Beobachtung: Archiv aktualisiert (Confirmed | Disputed | Disproven)
  -> Folgen: Wesen tot, vertrieben, gefangen, geflohen -> Weltzustand
```

Jeder Schritt hat eine Quelle: Hypothese `K-FO-002`, Vorbereitung `COMBAT-DEC-SJ-012`, Wirkung `COMBAT-DEC-SJ-003`, Folgen [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md). Die Kette ist Entwurf (💡); die Einzelaussagen sind beschlossen.

---

## 4. Wissen, Irrtum und Rückmeldung im Kampf

1. **Eigene Beobachtung zählt:** Dass eine Methode wirkt, zeigt sich am Verhalten des Wesens (Rückzug, Verlangsamung, Schmerzreaktion, Verschwinden).
2. **Irrtum ist ein Spielinhalt:** Eine falsche Hypothese kostet Munition, Zeit und Gesundheit (🧩 `COMBAT-DEC-SJ-015`).
3. **Keine Anzeige der Schwäche:** Das Spiel bezeichnet weder Schwäche noch Resistenz mit Zahlen (`COMBAT-DEC-SJ-014`, `COMBAT-DEC-GP-003`).
4. **Begleiter nennen Unbestätigtes nicht als Tatsache** (`K-FO-011`, 💡).

---

## 5. Jagdmethoden im Überblick

Die Methoden sind Themen aus den Quellen, keine Liste:

| Methode | Quelle | Modul |
|---|---|---|
| Schusswaffe mit passender Munition | `M-PD`, `K-KO-008` | 05, 10 |
| Nahkampfwaffe mit besonderem Material | `23#87A`, `K-JA-010` | 04, 09 |
| Ritual, Bann, Schutzzeichen | `K-JA-001` bis `-005` | Kreaturen 12 |
| Fallen, Fesselung, Lebendfang | `K-CO-001` bis `-005` | [Modul 17](17-Fallen-Fesselung-und-Lebendfang.md) |
| Verhandlung, Vertreiben, Retten | `E-FA-009`, `K-GS-003` | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| Feuer, Verbrennen von Überresten | `23#85A` P2 (Beispiel) | Kreaturen 12 |
| Umgebung und Improvisation | `K-KO-014` | 04 |

---

## 6. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Rückmeldung der Wirkung (`CQ-69`) | A: Nur Verhalten und Audio | passt zu ‚keine Zahlen‘ | schwer lesbar |
| | B: Archivnotiz nach dem Kampf | erhält Lernen | spät |
| | C: Optionaler Hinweis je Schwierigkeit | zugänglich | kollidiert mit Detektiv-Prinzip |
| Schwellen der Bestätigung (`CQ-67`) | A: Zahl von Belegen | nachprüfbar | mechanisch |
| | B: Qualität der Belege | glaubwürdig | schwerer zu testen |
| Rituale im Kampf (`CQ-68`) | A: Vor dem Kampf abgeschlossen | planbar | wenig Kampfnähe |
| | B: Während des Kampfes fortsetzbar und störbar | dramatisch | verlangt Eingabe ohne QTE |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 7. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Archiv und zur Ermittlung | Wissensstatus, Notizen |
| zur Munition und Waffe | Wirkungsliste |
| zur KI | Reaktion auf Methoden, Lernen aus Beobachtung |
| zu Fallen | Methode „Fangen“ |
| zur Persistenz | Archivstände, Folgen |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Methode wirkt nicht, Spieler hat nur eine Waffe | Flucht, Improvisation, Rückzug bleiben möglich | 🧭 |
| Archiv widerspricht der Beobachtung | Status ‚Disputed‘ | 📋 |
| Mod ändert eine Schwäche | additiv, Konfliktmeldung | 💡 |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-67` | Schwellen für Hypothese und Bestätigung; Lernen aus dem Kampf | `COMBAT-DEC-SJ-016`, `-017` | ja |
| `CQ-68` | Kosten, Risiko und Kampfnähe von Ritualen | `COMBAT-DEC-SJ-018` | ja |
| `CQ-69` | Rückmeldung der Wirksamkeit im Kampf | `COMBAT-DEC-SJ-019` | ja |
