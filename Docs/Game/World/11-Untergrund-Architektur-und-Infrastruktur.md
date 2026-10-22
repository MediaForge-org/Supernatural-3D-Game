# Welt 11: Untergrund – Architektur, Stabilisierung und Infrastruktur

> [!IMPORTANT]
> **Die Quellen enthalten kaum Architektur.** Dieses Modul sagt, was belegt ist, was nur Rechenbeispiel oder Vorschlag ist und was fehlt. Es erfindet keine Tragwerke. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (UG).

Teil der Welt-Dokumentation ([Index](README.md)). Lore: [Modul 10](10-Untergrundzivilisation.md).

**Inhalt**

1. [Befund zur Quellenlage](#1-befund-zur-quellenlage)
2. [Was belegt ist](#2-was-belegt-ist)
3. [Flächen- und Dichtebeispiele](#3-flächen--und-dichtebeispiele)
4. [Kapazitätsprüfungen nach der Footprint-Wahl](#4-kapazitätsprüfungen-nach-der-footprint-wahl)
5. [Naturräume](#5-naturräume)
6. [Netz, Zugang, Sicherheit](#6-netz-zugang-sicherheit)
7. [Technik und Streaming](#7-technik-und-streaming)
8. [Offene Punkte](#8-offene-punkte)

---

## 1. Befund zur Quellenlage

Die Aufgabenstellung nennt für die zentrale Höhle ‚fortschrittliche Materialien, energetische Stabilisierung und physische Tragstrukturen‘. **Dazu gibt es im Master keinen Treffer** (Suche in Kanon, Deltas 14.–19.10. und Chats 43/48). Die Aussage steht nur in der Aufgabenstellung und ist hier **nicht** als Quelle geführt. Belegt ist nur: ‚außergewöhnlich fortgeschrittene Infrastruktur, fiktives Freie-Energie-System‘ (D14-UC) und ‚Vertikale Ebenen, Höhlengeometrie, Biom, Ressourcen sind Designaufgaben‘ (D14-UC Grenze 5).

---

## 2. Was belegt ist

| Aussage | Status | Quelle |
|---|:---:|---|
| Horizontale Fläche der größten Stadt 50–100 km² (tentativ) | 🧭 | M-AD1015 |
| Mehrere Städte; Transport per Schiene oder Ähnlichem | 📋 | D14-UC |
| Bunker als gesicherte Teilzone; eigener Garten im Bunker | 📋 | D14-HB; 48#209E |
| Außergewöhnliche Infrastruktur, fiktive Energie | 📋 / 🔭 | D14-UC |
| Unterirdischer Fluss, Seen, Höhlenökosysteme, offene Naturzonen erwünscht | 📋 Richtung | D14-UC |
| KI, Roboter, Aufgabenautomation in der Infrastruktur (U32) | 📋 | D15-U |
| Kommunikation: Geräte, Hologramme, intelligente Räume, optionale neurale Links (U36) | 📋 | D17-U |
| Weitere Untergrundanlagen anderswo | 📋 Richtung | D14-UC |

---

## 3. Flächen- und Dichtebeispiele

Rechenbeispiele des Assistenten (D14-PS), **keine Empfehlung, keine Messung**:

| Footprint | mittlere Dichte | Bevölkerung |
|---:|---:|---:|
| 20 km² | 15.000/km² | 300.000 |
| 30 km² | 20.000/km² | 600.000 |
| 50 km² | 20.000/km² | 1.000.000 |
| 50 km² | 10.000/km² | 500.000 |
| 100 km² | 10.000/km² | 1.000.000 |

Drei getrennte Maße: horizontaler Footprint (km²), summierte Nutzfläche über Ebenen, mittlere Einwohnerzahl je Footprint-km² (einschließlich Freiflächen und Infrastruktur). Frühere Optionen A bis E (10–20, 20–50, 50–100, über 100 km², offen) wurden angeboten; gewählt wurde die Spanne 50–100 km² als tentativ (M-AD1015).

**Rechnung (nicht Quelle):** Mit 50–100 km² Footprint und der Bevölkerungsspanne 300.000–1.000.000 ergeben sich mittlere Dichten von 3.000 bis 20.000 Einwohnern je km². Die Obergrenze ähnelt der Beispielzeile 50 km² / 1 Mio.; die Untergrenze ist deutlich lockerer. Welche Kombination gilt, ist offen. Zum Vergleich ist die größte oberirdische Stadt des Kandidaten 110 km² groß.

---

## 4. Kapazitätsprüfungen nach der Footprint-Wahl

Aufgabenliste aus D14-PS (💡):

1. Kavernen, Wohnebenen, Landwirtschaft, Ökosystem/Wasser, Technik, Verkehrstrassen trennen.
2. Mindestversorgung (Nahrung, Wasser, Luft, Abfall, Wärmeabfuhr, Medizin, Notunterkunft, Energie) in fiktiven Begriffen.
3. Dichte auf Bezirks- und Stadtebene prüfen.
4. Kontrollierter Zugang zwischen Fernverkehr und abgeschotteten Wohnbezirken.
5. NPC-Simulations-LOD und Crowd-Budgets **unabhängig** von der Lore-Bevölkerung (nicht alle gleichzeitig aktiv).
6. Obere und untere Footprints in getrennten GIS-Schichten führen.

---

## 5. Naturräume

Unterirdischer Fluss, Seen, Vegetation: Quelle, Entwässerung, Grundwasseraustausch, Licht, Belüftung, Chemie und ökologische Pflege sind ungeklärt; **kein bewiesenes, ewiges Ökosystem erfinden** (D14-PS). Ein Bunkergarten ist belegt (48#209E).

---

## 6. Netz, Zugang, Sicherheit

- Netz: mehrere Städte, Schiene o. Ä.; Versorgung, Wartung, Evakuierung und medizinische Notfahrten ohne pauschale Besucherrechte (D14-NS, 💡).
- Privatsphäre bleibt Invariante: Infrastruktur-KI heißt keine Massenüberwachung (D15-U, D16-AI).
- U35: Regeln je KI und Aufgabe; kritische Infrastruktur mit zusätzlichen Sicherungen (D17-U).
- Vorschläge (D17-AUT, D17-PRI, 💡): fähigkeitsbasierte Berechtigungen, isolierte Steuerdomänen (Energie, Wasser, Luft, Verkehr, Medizin, Labor), konservative Sicherheitsmodi bei Verbindungsverlust, Prüfbarkeit ohne Bürgertracking, Weigerung der Abschaltung einer möglicherweise bewussten KI als soziale Frage.
- **U37** (internes Netz und Oberflächen-Internet) ist offen; kein Gateway vorab implementieren.

---

## 7. Technik und Streaming

Nur Fragen sind belegt: GIS-Darstellung und Streaming/LOD vertikal gestapelter Räume (D14-OQ Nr. 13); Performance der Offscreen-Simulation und deterministische Resynchronisation (D15-OQ Nr. 9). Vorschläge zu `NetworkRealm`-Platzhaltern und Fehlertests (D17 Implementierungsnotizen) sind unverbindlich. Unreal-Seite: [Modul 18](18-Unreal-Landscape-World-Partition-und-Streaming.md).

---

## 8. Offene Punkte

Footprint innerhalb 50–100 km²; Ebenenzahl; Kavernenhöhe; Tragwerk und „Stabilisierung“ (nicht belegt); Energie; Belüftung/Wärme; Stadtzahl; Verbindung zur Oberflächenbahn; Ebenen-Layout des Bunkers; Eingänge; Wasserkreislauf; Katastrophenfälle.
