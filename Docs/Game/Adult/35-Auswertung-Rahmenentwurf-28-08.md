# Adult 35: Auswertung des Rahmenentwurfs vom 28.08.2026

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Der Rahmenentwurf („Adult Content Framework“, Chat 28) ist ein **Prompt, den ein früherer KI-Assistent für einen anderen KI-Chat schrieb**. Er ist ein **Architekturvorschlag**, kein Kanon, und teils durch spätere Entscheidungen überholt. Dieses Modul sagt, was er enthält, was davon noch trägt und was nicht. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Quelle: Chat 28 (`28#n`), Endfassung `28#25`, Nutzernachrichten 1–24. Gesprächszusammenhang: [Modul 17, Abschnitt 8](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md#8-chat-28-rahmenentwurf-adult-content-framework). Gelesen: Nutzernachrichten 1, 3, 5, 7, 10, 12, 14, 16, 18, 20, 22, 24 vollständig; Assistenten-Nachricht 25 (Endfassung) vollständig; 17, 19, 21, 23 vollständig; frühere Fassungen 2, 4, 6, 9, 11 in Etappe 2.2 auf Sicherheitsregeln und Gliederungsentwicklung geprüft, nicht Zeile für Zeile verglichen ([Modul 13](13-Quelleninventar-und-Abdeckung.md)).

**Inhalt**

1. [Einordnung](#1-einordnung)
2. [Aufbau des Entwurfs](#2-aufbau-des-entwurfs)
3. [Sicherheitsregeln des Entwurfs](#3-sicherheitsregeln-des-entwurfs)
4. [Abgleich mit dem heutigen Stand](#4-abgleich-mit-dem-heutigen-stand)
5. [Weiterhin verwendbare Ideen](#5-weiterhin-verwendbare-ideen)
6. [Überholt oder ersetzt](#6-überholt-oder-ersetzt)
7. [Technische Teile im Überblick](#7-technische-teile-im-überblick)
8. [Roadmap des Entwurfs](#8-roadmap-des-entwurfs)
9. [Ergebnis](#9-ergebnis)

---

## 1. Einordnung

| Punkt | Inhalt |
|---|---|
| Datum | 28.08.2026, 25 Nachrichten, ca. 362.000 Zeichen |
| Charakter | Prompt für einen **anderen** KI-Chat („Technical Design Document“), kein Spieldesign |
| Status | **PROPOSAL**; teils überholt |
| Nutzerwünsche | mehr/andere Kategorien, „wesentlich granularer“ als vier grobe Tags, „du kannst doch Hardcore dazu schreiben“, ‚andeuten, was gemeint ist‘ |
| Reaktion des Assistenten | Aufnahme einer abstrakten Taxonomie; **Ablehnung codierter Umschreibungen konkreter Praktiken** (28#23) |
| Spätere Kritik (23#525–531) | Zustimmung kommt in der Roadmap zu spät; Prompt zu groß als Einzelprompt; Overengineering-Gefahr |

## 2. Aufbau des Entwurfs

Die Endfassung (28#25) beschreibt (Ausgabeformat A–AJ):

| Teil | Inhalt |
|---|---|
| Abschnitt 0 | Sicherheitsregeln (siehe Abschnitt 3) |
| Framework | **General Character Interaction Framework** (Social, Affection, Assistance, Rest, Romance, Intimacy) mit gemeinsamer Infrastruktur; Adult als spezialisierte Content-Schicht |
| Pipeline | Interaction Request → Eligibility → Consent → Preferences → Relationship → World Context → Environment Query → Surface Query → Animation Query → Hard Filter → Scoring → AnimationSet → State Machine → Session |
| Taxonomie | Root `Interaction.Content` (Social, Affection, Romantic, Sensual, Adult); Adult-Root mit NonPenetrative, Manual, Oral, Vaginal, Anal, Solo, Mutual, MultiParticipant, MixedCategory, Transition, Rest, OtherSupported; Root > Subtyp > Variante |
| Profile | `AdultContentProfile` Standard/Advanced/Hardcore/ExtremeSupported; „Hardcore“ als eigenständige Content-Profil-Klasse mit abstrakten Variantfamilien; Consent pro Profil getrennt |
| Profildimensionen | ContactDepth, ContactComplexity, MotionProfile, TimingProfile, PacingProfile, DistanceProfile, ContactAlignment |
| Positionen | PositionProfile (Posture je Teilnehmer, Orientation, Elevation, Support, Contact, Blocking, Surface, RoleLayout) |
| Darstellung | Presentation Modes (Disabled, RomanceOnly, FadeToBlack, Implied, CinematicConcealed, Reduced, FullAllowed); Nudity Off/Reduced/Full |
| Zustimmung | ConsentState (Unknown, PotentiallyInterested, Offered, Considering, Accepted, Declined, Withdrawn, Expired, TemporarilyUnavailable); `FConsentGrant`; ConsentScope-Stufen; Neuprüfung bei Kategorie-/Profilwechsel, neuem Teilnehmer, Intensitätsänderung, längerer Unterbrechung; Widerruf → Critical Interrupt → Safe Transition → Exit → Cleanup |
| Anforderungen | Participant Requirements (AdultVerified, Conscious, CapableOfConsent, ConsentGranted, Available, NotInCombat, NotRestrained, NotCriticalInjury, NotStoryLocked, EnvironmentCompatible, InteractionCompatible) |
| Präferenzen | `PreferenceLevel` (Preferred, Allowed, Neutral, Disliked, Never) + Regel-Engine (`FGameplayTagQuery`, `bHardBoundary`) |
| Zustandsautomat | ca. 28 Zustände (Unavailable … Completed), Failure States, Pause-Typen, Dialogue Windows (Dialog nur in Pausen), Post-Interaction |
| Umgebung | Environment Types, PrivacyLevel (Public … Isolated), SafetyLevel, Surfaces (`UInteractionSurfaceComponent` statt `SexFurnitureComponent`), Reservations, Alignment + Fallback |
| Animation | `CharacterAnimationProfile`, Query/Hard Filter/Scoring (Skeleton, Height, Body, Position, Environment, Relationship, Mood, Preference, ContentProfile, Variant, Intensity, Motion, TransitionQuality, RecentUsePenalty, Variation, Authoring, PerformanceCost), Transitions als First-Class Assets, Missing-Transition-Fallback, Sync Markers |
| Kleidung/Kamera/Audio | Clothing States, Camera Director (+Constraints, Shot Definitions, Player Camera Options), Audio-Architektur |
| Eingabe | Continue, Pause, Resume, Stop, Skip; Stop mit ImmediateSafeExit; Skip löst narrativen Outcome auf; Interruption Reasons + Priorität; Witness System (LOS, Distance, Room, Doors, Walls, Audio, Lighting, Visibility, Privacy, Relationship, Faction); NPC-AI-Inputs |
| Mehrspieler | Singleplayer/Online getrennt; Online als ‚langfristiges festes Ziel‘: Server Authority, Player-Player Consent separat, Request Flow, Request Permissions (Everyone/Friends/Party/Trusted/Nobody), Anti-Spam, Block, Client-spezifische Darstellung, Observer, Content-Off-Client, Datenminimierung, Interest Management, Replication, Latenz, Ready Check, Disconnect, Server Crash, Transaktionsgrenzen, Private Instance, Griefing, Matchmaking (kein Adult-Matchmaking), Cross-Server |
| Einstellungen | Adult Content Off/Romance Only/Enabled; Profile; Nudity; Presentation; Adult Audio; Cinematic Camera; Player Camera Control; Adult Dialogue; Request-Einstellungen; Accessibility |
| Paketierung | Optional Content Pack (`/Game/AdultContent`), Primary Asset Manager, Preloading, World Partition |
| Speicherung | Persistieren: Relationship, Stage, Memories, Preferences, Story Flags, Outcomes; **nicht**: Loops, Frames, Kamera, IK; Online-DB; Character Memory (`FRelationshipMemory`) |
| Datenschutz | Privacy by Design, minimale Telemetrie |
| Modding | moddbar: Definitions, AnimationSets, PositionPresets, VariantTags, ContentProfiles, Transitions, Camera/Audio/Dialogue/Environment/Surface/Compatibility Profiles; **nicht moddbar:** AdultEligibility, ConsentRequirement, PlayerOptOut, ServerAuthority, BlockRules, RequestPermissions, SessionSecurity |
| Grenze C++/Daten/Blueprint | nicht Blueprint-only: Consent, Server Authority, Session Lifecycle, Eligibility, Replication, Persistence, Block Rules, Request Permissions |
| Physik | ‚Primary: authored animation, Physik nur unterstützend‘ |
| Robustheit | Cache-Architektur, Failure Handling, Fallback Ladder (Content-Profil-Fallback nur wenn explizit erlaubt), No Softlocks, Debug Overlay, Editor Tools, Authoring Workflow, Batch Authoring, CI/Cook Validation (21 Checks), Automated Test Matrix, Network Load Testing (100/500/1000 Sessions, keine unrealistischen Behauptungen), Security, Stable IDs, Localization |
| Profile für Spieler | Content Profiles (FullContent, Reduced, RomanceOnly, NoAdult) ≠ `AdultContentProfile` |
| Konsequenzen | nicht „+50 Relationship“, sondern narrativ |
| Kombinationen | keine Kombinationsexplosion: nur autorisierte AnimationSets |
| Roadmap | 21 Phasen |

## 3. Sicherheitsregeln des Entwurfs

Abschnitt 0 der Endfassung (28#25), Wortlaut sinngemäß:

- Teilnehmer sind **eindeutig erwachsen, bei Bewusstsein, handlungs- und entscheidungsfähig** und können **aktiv zustimmen und jederzeit zurücknehmen**.
- **Nicht unterstützt:** Minderjährige; nicht-einvernehmliche Interaktionen; Bewusstlose; Schlafende als aktive Teilnehmer; Story-/Quest-Zwang; Relationship-Werte oder KI-Ziele als Consent-Ersatz; Party-Leader-/Admin-Consent.
- Mods dürfen diese Regeln **nicht** deaktivieren.

Diese Regeln stimmen mit dem Sicherheitskern ([Modul 01](01-Grundregeln-Alter-Zustimmung.md)) überein und gehören zu den Teilen des Entwurfs, die **fortbestehen**.

## 4. Abgleich mit dem heutigen Stand

| Entwurf | Heute | Status |
|---|---|---|
| Authored Animation primär, Physik nur unterstützend | **Freeform-Engine** mit Pose/Intent → Kontaktgraph → Ganzkörper-IK → Weichgewebe (Interview 11./13.09.) | 🕰️ überholt |
| `AdultContentProfile` mit Hardcore als Katalogklasse | **Hardcore = Preset** über mehrere Dimensionen (35#187) | 🕰️ ersetzt |
| Online-Adult mit Request-Flow | **Offizielles Online ohne Mods** (23#551); Adult-Paket dort nicht verfügbar | 🕰️ ersetzt, Teile bleiben für Koop |
| Presentation Modes (FadeToBlack, FullAllowed …) im Kern | **Basisspiel nichtexplizit; Paket getrennt** (23#539, K§16–17) | 🕰️ ersetzt |
| UE-spezifische Typen (Gameplay Tags, Primary Asset) im Kern | **engine-neutraler Kern** ohne Unreal-Typen (35#176) | 🕰️ ersetzt |
| Zustimmungszustände (9) und Participant Requirements | übernommen als Vorlage (E-SK-026/027) | ✅ Vorlage bleibt |
| Persistenzgrundsatz (Ergebnisse statt Frames) | übernommen (E-LE-002) | 📋 |
| Modding-Trennung (moddbar/nicht moddbar) | übernommen (E-SK-004, E-PA-035) | 📋 |
| Blueprint-Grenze (Sicherheitsobjekte nie Blueprint-only) | übernommen (E-PA-025) | 📋 |
| Fallback Ladder, No Softlocks | übernommen (E-SO-021) | 📋 |
| Zustandsautomat mit Pausen, Dialogfenstern | übernommen (E-IA-041), vereinfachte Fassung in [Modul 22](22-Spezifikation-Interaktionsengine.md) | 🧭 |
| Witness System | übernommen (E-WE-021) | 🧭 |
| Stable IDs | übernommen, ohne Version (E-SP-007) | 🧭 |
| `PreferenceLevel` Preferred…Never | ersetzt durch Stufenskala neugierig…harte Grenze (E-NP-004) und mehrdimensionales Profil | 🕰️ ersetzt |
| Roadmap mit Consent in Phase 10 | Kritik: Consent muss früh kommen (23#525) | 🕰️ ersetzt |

## 5. Weiterhin verwendbare Ideen

| Idee | Verwendung |
|---|---|
| General Character Interaction Framework (Social/Affection/Assistance/Rest/Romance/Intimacy) | gemeinsame Infrastruktur für Umarmen, Tragen, Medizin, Ringen (E-SO-023) |
| Pipeline mit festen Stufen und Hard Filter vor Scoring | [Gate-Kette](20-Spezifikation-Regelkern-Zustimmung.md#3-prüfkette-gate-reihenfolge) |
| `FConsentGrant` mit Scope | [Zustimmungsdatensatz](20-Spezifikation-Regelkern-Zustimmung.md#62-datensatz) |
| Withdrawal-Kette Critical Interrupt → Safe Transition → Exit → Cleanup | `HardStop`-Ablauf |
| Surfaces statt Sex-Möbel-Komponenten | Interaktionsflächen (E-SO-033) |
| Reservations | Plätze/Flächen |
| Transitions als First-Class Assets | Übergänge (K§18) |
| Sync Markers | Animations-Synchronisation |
| Interruption Reasons mit Priorität | Scheduler ([Modul 22, Abschnitt 13](22-Spezifikation-Interaktionsengine.md#13-interaktionsplaner-scheduler)) |
| Content Settings und Accessibility | [Modul 27](27-Spezifikation-Steuerung-UI-VR-Replay.md) |
| Cook-/CI-Validierung (21 Checks) | [Build-Validator](28-Spezifikation-Speicherung-Mods-Builds.md#3-pakete-und-builds) |
| Network Load Testing (ohne unrealistische Behauptungen) | [Modul 29](29-QA-Testkatalog.md) |
| Konsequenzen narrativ statt Zahlen | [NP-GD-05](24-Spezifikation-NPC-Gesellschaft-und-Konsequenzen.md#8-gedächtnis) |
| Keine Kombinationsexplosion durch autorisierte Sets | in der Freeform-Engine nicht mehr nötig; bleibt als Prinzip für Mods |
| Privacy by Design, minimale Telemetrie | [Modul 28, Abschnitt 9](28-Spezifikation-Speicherung-Mods-Builds.md#9-sicherheit-und-datenschutz) |

## 6. Überholt oder ersetzt

Neben Abschnitt 4:

- **Authored-Animation-Zentrierung:** Das Interview (R10–R11) entschied sich für Freeform. Der Rahmenentwurf bleibt als Beispiel für Hybrid-Ansätze lesbar.
- **Fester Katalog je Profil:** Die Kategorien sind Daten in einer Registry ohne feste Enums (E-IA-005).
- **Mehrspieler mit breitem Online-Betrieb:** nicht mehr Ziel im offiziellen Online.
- **Darstellungsmodi im Kern:** durch Paketgrenze ersetzt.
- **Zählungen und Aufwandsangaben** (21 Phasen; 800–1.200 Arbeitspakete): Schätzungen des Assistenten (23#532–535).
- **Codierte Umschreibungen:** nicht übernommen ([E-XX-022](14-Entscheidungsregister.md#17-xx-nicht-freigegeben-historisch-überholt)).

## 7. Technische Teile im Überblick

| Bereich | Entwurf | Hinweis |
|---|---|---|
| Datenmodell | UE-Typen (`F…`, `U…`) | nur als Beispiel, engine-neutral umzusetzen |
| Replikation | Iris/Replication Graph | Entscheidung über Netzwerkarchitektur steht aus (CLAUDE.md §6) |
| Ladevorgänge | Primary Asset Manager, World Partition | an Unreal-Version gebunden (ungeprüft) |
| Tools | Editor, Batch Authoring | Ideen für den Mod-Editor |
| Tests | 21 Cook-Checks | als Anregung für Build-Validator |

## 8. Roadmap des Entwurfs

Der Entwurf nennt 21 Phasen mit Zustimmung in Phase 10. Die Kritik (23#525–531) bemängelte die späte Zustimmung. **Der heutige Stand** (diese Dokumentation) legt die Reihenfolge der Entwicklung nicht fest ([Modul 32, Abschnitt 7](32-Systemabhaengigkeiten.md#7-zusammenhang-mit-der-entwicklungsphasenplanung)), sieht aber den Regelkern ausdrücklich **vor** Inhalt vor ([Modul 30, Abschnitt 10](30-Abnahmekriterien.md#10-gesamtabnahme-je-entwicklungsstufe)).

## 9. Ergebnis

1. Der Rahmenentwurf bleibt eine **wertvolle Strukturvorlage**, vor allem für Zustimmung, Zustandsautomat, Fallback, Modding-Grenzen und Qualitätssicherung.
2. Er ist **kein** Spezifikationsstand; die Interviewrunden und Kanonentscheidungen haben ihn in mehreren Punkten überholt.
3. Seine UE-spezifischen Typen sind für den engine-neutralen Kern **nicht** zu übernehmen.
4. Die früheren Fassungen (28#2, 4, 6, 9, 11) sind je eine Überarbeitung desselben Prompts. **Etappe 2.2:** Alle enthalten dieselben Sicherheitsausschlüsse (Fassung 2: nur eindeutig Erwachsene, keine Minderjährigen, keine nicht-einvernehmlichen Inhalte, keine schlafenden/bewusstlosen Figuren; Fassung 4 ergänzt Party-Leader-Autorität, Fassung 6 Story-/Quest-Zwang, Fassung 9 Admin-Consent). Die Gliederung wuchs von 58 Überschriften (Fassung 2) über 166 (Fassung 6) und 221 (Fassung 9) auf 246 (Endfassung); ab Fassung 15 kamen Abschnitte zu mehrachsiger Taxonomie, Root-Kategorien und Profilen hinzu. Ein vollständiger Zeilenvergleich steht aus.
