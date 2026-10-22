# Welt 13: Fahrzeuge und Fahrzeugklassen

> [!IMPORTANT]
> **Es gibt keine Fahrzeugliste, keine Marken, keine Modelle und keine Werte.** Belegt sind Klassen, Anforderungen und Richtungen. Nichts ist implementiert. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (FZ).

Teil der Welt-Dokumentation ([Index](README.md)). Fahrphysik: [Modul 14](14-Fahrphysik-und-Fahrzeuginteraktion.md); Motorsport: [Modul 15](15-Motorsport-und-Rennaktivitaeten.md).

**Inhalt**

1. [Klassen laut Planung](#1-klassen-laut-planung)
2. [Fahrzeugidentität und Ausstattung](#2-fahrzeugidentität-und-ausstattung)
3. [Getriebe und Antrieb](#3-getriebe-und-antrieb)
4. [Besitz, Markt, Wartung](#4-besitz-markt-wartung)
5. [Kraftstoffe und Energie](#5-kraftstoffe-und-energie)
6. [Fahrzeug als Hunter-Basis](#6-fahrzeug-als-hunter-basis)
7. [Luft, Wasser, Schiene](#7-luft-wasser-schiene)
8. [Persistenz](#8-persistenz)
9. [Nicht belegt](#9-nicht-belegt)

---

## 1. Klassen laut Planung

Quelle M-WB17 Block 5 (📋 als Planung), Entwicklerbestätigung 23#1025E:

| Klasse | Aussage |
|---|---|
| Autos | breites Spektrum über Jahrzehnte; Gebrauchtmarkt |
| Motorräder | vorhanden; im Rennsport gleich tief |
| ATV/UTV, Schneemobile | vorhanden |
| Fahrräder, E-Scooter | vorhanden |
| Lkw, Fracht | Güterverkehr per Lkw, Bahn, Hafen; Trucking als Job |
| Taxi, Rideshare, Lieferung | Jobs |
| Boote | besitzbar, an Marinas; Tauchen per Boot |
| Kleinflugzeuge | spielerflugbar |
| Hubschrauber | besitzbar, meist von Piloten geflogen (23#1025E Nr. 34) |
| Airliner | Passagierverkehr |
| Rennfahrzeuge | alle Klassen des Motorsports ([Modul 15](15-Motorsport-und-Rennaktivitaeten.md)) |
| Spezialfahrzeuge | hohe Berge nur mit Spezialfahrzeugen oder Gondeln (23#821E) |
| Schienenfahrzeuge | langfristig fahrbar mit Berechtigung (D15-RW) |

**Nicht belegt:** Fahrzeugliste, fiktive Marken, Modelle, Baujahre, Trims, Sondereinsatzfahrzeuge der Hunter, Panzer/Militär. Offen laut M-OD: Startsortiment, Marken, Modelljahre, Trims.

---

## 2. Fahrzeugidentität und Ausstattung

- Identität aus Modell, Trim, Motor, Getriebe, Antrieb, Baujahr, Optionen (M-B38).
- Varianten für Rallye, Homologation, Rennstrecke, Gelände, Leistung, Flotte (M-B38).
- Reifenmodell: Mischung, Temperatur, Druck, Verschleiß, Nässe (M-PD).
- Supersportwagen sind **nicht** magisch geländetauglich; Geländetauglichkeit variiert real.

---

## 3. Getriebe und Antrieb

- Standard ist manuell mit Kupplung, Schleifpunkt, Abwürgen; andere Versionen kaufbar; Automatik extra (23#613E).
- Das Getriebe gehört zum gekauften Trim; Modellfamilien können Manuell, Automatik, DCT, Sequentiell haben (M-PD §Vehicles/racing).
- **Für jedes Fahrzeug existiert eine manuelle Variante** plus passende Automatiken (D15-RV).
- Kupplung ist deaktivierbar, Schalten bleibt (23#1025E Nr. 14).
- Antriebsarten (Verbrenner, Elektro): E-Laden genannt (M-PD); Hybrid/Wasserstoff nicht belegt.

---

## 4. Besitz, Markt, Wartung

- Gebrauchtmarkt mit Laufleistung, Historie, Verschleiß, versteckten Mängeln, Modifikationen (M-WB17).
- Verschleiß: Motor, Öl, Kühlung, Getriebe, Kupplung, Differenzial, Batterie, Bremsen, Reifen, Federung.
- Tuning: Leistungstuning wichtiger als Kosmetik, ‚nicht so viel wie echtes Performance Tuning‘ (23#1025E Nr. 13).
- Werkstätten: Reparatur, Leistung, Rennen, Gelände, Karosserie (M-PD).
- Früh einfache Reparatur und einfaches Tuning, langfristig nahezu vollständige Werkstattsimulation (D15-RV).
- Abschleppen, Winde, Bergung; Fahrzeug-App und Anlieferung per Telefon; Fahrzeugvertrautheit ohne magische Werte (M-PD).
- Valet-Service an passenden Orten (M-WB17).
- Fahrzeug im Safe/HQ-Bargeld: nicht verloren (23#613E).
- Pay-to-win: Rennsport davor geschützt (M-PD); optionaler Echtgeldkauf nur zum Abkürzen (23#575E).

---

## 5. Kraftstoffe und Energie

Benzinsorten, Diesel, E85 (‚109 Oktan‘, mehr Leistung; 23#1025E Nr. 11) und Rennkraftstoffe; E85 oder hochoktanig kann 50+ PS bringen, wo Motor und Tune es erlauben (M-WB17). Realer Verbrauch. Tankstellen und E-Ladeinfrastruktur sind als Weltobjekte plausibel (Tankstellen in Wildnis genannt, 35#96A), Zahl offen.

---

## 6. Fahrzeug als Hunter-Basis

Der Kofferraum ist die mobile Ausrüstungsbasis (M-PD); Inventar im Fahrzeug; Fahrzeuge bleiben am Fallort stehen (Prompt-Entwurf, nicht Entscheidung); Verfolgen mit Fahrzeugen als Ermittlungsmethode (M-LATE); Kennzeichen als Beweis (M-PD §Cash/law).

---

## 7. Luft, Wasser, Schiene

Siehe [Modul 09](09-Strassen-Schiene-und-Verkehrsinfrastruktur.md). Boote: Speedboote, Segeln, Yachten, Jetski, Wasserski; Flugzeuge: Passagierflug mit Airlinern; Heliports, Krankenhaus-Helipads (35#96A, 💡).

---

## 8. Persistenz

Fahrzeuge sind persistente Entitäten mit stabiler ID (M-PD §World state: Fahrzeuge werden im Weltzustand geführt). Das Schema ist als Prompt-Entwurf (`009_create-persistent-vehicle-state-record`) und nicht als Entscheidung genannt.

---

## 9. Nicht belegt

Fahrzeugmenge, Preise, Marken, Hunter-Spezialfahrzeuge, Schadenstiefe (offen, W-FZ-013), Stationsfahrzeuge der Polizei, Sonderfahrzeuge der Untergrundstädte, Fahrzeuge der Reiche. Keine dieser Aussagen darf aus dieser Dokumentation abgeleitet werden.

**Ergänzung aus Chat 35:** Der Entwickler will Fahrzeuge nicht mit echten Namen („autos würde ich nicht so nennen wie in echt“, 35#717E, W-FZ-017); die Festlegung aller Autos und Marken steht noch aus (35#699E). Der Assistent schlug am 23.09. vor, zuerst Taxonomie, Markenstrategie, Klassen, Epochen und Regionen zu klären und erst dann Modelle (35#700A, W-FZ-018, 💡).
