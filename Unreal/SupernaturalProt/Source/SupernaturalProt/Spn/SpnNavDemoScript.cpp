#include "Spn/SpnNavDemo.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformMisc.h"
#include "Misc/CommandLine.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "Spn/UI/SpnMapScreen.h"
#include "Spn/UI/SpnMapView.h"
#include "Spn/UI/SpnUiSubsystem.h"

// The scripted tech-demo run (NightBuild03). Every number shown on screen is read from the running systems at that moment (route lengths from the
// Core route, status from the NavigationSession); nothing is typed in. Camera paths are authored in map metres + height above ground.
// The map interactions use the same entry points as the mouse (the map view's click delegate = right click = waypoint) and the same subsystem call
// the X key makes (SetRoadClosed). The vehicle is the demo autopilot of ASpnNavDemo, not a driving simulation.

namespace
{
	FString Km(double Metres)
	{
		return Metres >= 1000.0 ? FString::Printf(TEXT("%.2f km"), Metres / 1000.0) : FString::Printf(TEXT("%d m"), FMath::RoundToInt(Metres));
	}
}

void ASpnNavDemo::BuildScript()
{
	auto At = [this](double Time, TFunction<void()> Action)
	{
		FStep Step;
		Step.Time = Time;
		Step.Action = MoveTemp(Action);
		Steps.Add(MoveTemp(Step));
	};
	auto Ui = [this]() -> USpnUiSubsystem*
	{
		UGameInstance* Instance = GetGameInstance();
		return Instance != nullptr ? GetUi() : nullptr;
	};
	auto View = [Ui]() -> SSpnMapView*
	{
		USpnUiSubsystem* U = Ui();
		USpnMapScreen* Screen = U != nullptr ? U->GetMapScreen() : nullptr;
		return Screen != nullptr && Screen->GetMapView() != nullptr ? Screen->GetMapView()->GetView() : nullptr;
	};
	FVector2D EastTarget = FVector2D::ZeroVector, IslandTarget = FVector2D::ZeroVector;
	Map->GetSyntheticPosition(TEXT("eastStreet"), EastTarget);
	Map->GetSyntheticPosition(TEXT("islandTarget"), IslandTarget);

	// --- camera path: overview, then a low flight through the region (map metres, metres above ground) ---
	TArray<FCamKey> Keys;
	auto Key = [&Keys](double T, double Ex, double Ey, double Eh, double Lx, double Ly, double Lh)
	{
		FCamKey K;
		K.Time = T;
		K.Metres = FVector2D(Ex, Ey);
		K.HeightM = Eh;
		K.LookMetres = FVector2D(Lx, Ly);
		K.LookHeightM = Lh;
		Keys.Add(K);
	};
	Key(0, -1100, -1000, 420, 1000, 300, 0);
	Key(6, 1000, -1250, 380, 1000, 300, 0);
	Key(12, 2700, -800, 250, 1800, 100, 0);
	Key(18, 2150, -230, 70, 2000, 0, 0);
	Key(24, 1700, -140, 30, 1300, 60, 5);
	Key(30, 1200, 980, 32, 1000, 1200, 6);
	Key(36, 940, 1120, 11, 1060, 1200, 3);
	Key(42, 1000, -680, 26, 1000, -800, 8);
	Key(48, 450, -70, 12, 900, 0, 2);
	Key(54, 300, -45, 9, 520, 0, 2);
	Key(60, 300, -45, 9, 520, 0, 2);
	SetCameraPath(Keys);

	// ------------------------------------------------------------ intro + scene 1: the test world
	At(0.0, [this]
	{
		Mark(TEXT("intro"));
		UseCamera(true);
		Title(TEXT("SUPERNATURAL - MAP & NAVIGATION TECH DEMO"), TEXT("Meilenstein M2  |  Night Build 03  |  Unreal Engine 5.8.4"),
			TEXT("Synthetische Testregion mit Platzhalterdaten - keine Spielwelt, keine Geografie. Karte und Welt nutzen dieselben Core-Daten."), 6.0);
	});
	At(7.0, [this] { Mark(TEXT("scene1")); Caption(TEXT("Die Testwelt"), TEXT("Straßen, Kreuzungen, Kreisverkehr, Brücken, ein Tunnel, Gebäude, Wald, Wasser - eine einfache 3D-Darstellung der Core-Geometrie."), 6.0); });
	At(14.0, [this] { Caption(TEXT("Straßenklassen"), TEXT("Autobahn mit Rampen, Hauptstraße, Wohnstraßen, Feld- und Waldwege, Fußwege."), 5.5); });
	At(20.0, [this] { Caption(TEXT("Kreisverkehr & Einbahnstraßen"), TEXT("Gegen den Uhrzeigersinn befahrbar; Abbiegeverbot und Sackgassen sind im Routing berücksichtigt."), 5.5); });
	At(26.0, [this] { Caption(TEXT("Wasser als Hindernis"), TEXT("Der Fluss ist nur über Brücke oder Tunnel überquerbar - nicht per Luftlinie."), 5.5); });
	At(32.0, [this] { Caption(TEXT("Brücke"), TEXT("Hauptstraße über den Fluss (Deck, Geländer, Pfeiler). Daneben eine Fußgängerbrücke."), 5.5); });
	At(38.0, [this] { Caption(TEXT("Tunnel"), TEXT("Autobahn unter dem Fluss, nur über Rampen erreichbar (Darstellung als geschlossene Röhre: der Core kennt noch keine Höhen)."), 6.0); });
	At(44.0, [this] { Caption(TEXT("Gebäude mit Eingängen"), TEXT("Grundrisse aus MapGeometry; gelbe Marker zeigen die Eingänge, an denen Routen enden."), 5.5); });
	At(50.0, [this] { Caption(TEXT("Spielerposition"), TEXT("Wir starten auf der Weststraße. Als Nächstes: die Weltkarte."), 4.5); });

	// ------------------------------------------------------------ scene 2: the world map
	At(56.0, [this, Ui, View]
	{
		Mark(TEXT("scene2_map"));
		UseCamera(false);
		if (USpnUiSubsystem* U = Ui()) { U->OpenMap(); }
		AnimateMap(FVector2D(1000.0, 300.0), 5.5, 2.5);
		Caption(TEXT("Weltkarte"), TEXT("Karte öffnen (M): Straßen nach Klasse, Gebäudegrundrisse, Wasser, Wälder und Brücken - gezeichnet aus den Core-Kartendaten."), 6.0);
	});
	At(63.0, [this] { AnimateMap(FVector2D(1000.0, 1200.0), 1.3, 4.0); Caption(TEXT("Zoom & Verschieben"), TEXT("Brücke mit heller Umrandung; der Fluss darunter."), 5.0); });
	At(69.0, [this] { AnimateMap(FVector2D(2000.0, 0.0), 0.8, 4.0); Caption(TEXT("Kreisverkehr"), TEXT("Einbahnring mit Zufahrten und Rampen zur Autobahn."), 5.0); });
	At(75.0, [this] { AnimateMap(FVector2D(1000.0, -800.0), 1.4, 4.0); Caption(TEXT("Tunnel"), TEXT("Abgedunkelt dargestellt; die Autobahn unterquert den Fluss."), 5.0); });
	At(81.0, [this] { AnimateMap(FVector2D(560.0, 20.0), 0.45, 4.0); Caption(TEXT("Gebäude"), TEXT("Grundrisse mit Eingängen (gelb) - hier Laden, Lager, Reihenhäuser."), 5.0); });
	At(87.0, [this] { AnimateMap(FVector2D(1000.0, 300.0), 4.5, 3.0); });

	// ------------------------------------------------------------ scene 3: waypoint and routing
	At(90.0, [this] { Mark(TEXT("scene3_waypoint")); Caption(TEXT("Wegpunkt setzen"), TEXT("Rechtsklick auf die Karte (Ziel: Oststraße, auf der anderen Flussseite). Die Route kommt aus dem Core-Routing (A*)."), 5.5); });
	At(93.0, [this, View, EastTarget]
	{
		if (SSpnMapView* V = View()) { V->OnMapClick.ExecuteIfBound(EastTarget, true); }
	});
	At(96.0, [this]
	{
		FSpnRouteView Route;
		if (Map->GetRoute(Route))
		{
			RouteInitialM = Route.LengthMetres;
			StraightInitialM = Route.StraightLineMetres;
			Mark(FString::Printf(TEXT("route_set length=%.1f straight=%.1f"), Route.LengthMetres, Route.StraightLineMetres));
			Caption(TEXT("Echte Straßenroute"), FString::Printf(TEXT("%s auf der Straße (über die Nordbrücke) - die Luftlinie wären nur %s. Orange: Route, mit Restdistanz im rechten Panel."),
				*Km(Route.LengthMetres), *Km(Route.StraightLineMetres)), 8.0);
		}
		else
		{
			Mark(TEXT("route_set FAILED"));
		}
	});
	At(105.0, [this, IslandTarget]
	{
		Caption(TEXT("Kein gültiger Weg"), TEXT("Ziel auf der Flussinsel: keine Zufahrt. Die Oberfläche meldet das - es wird keine Luftlinie erfunden."), 6.5);
		int64 Id = 0;
		USpnUiSubsystem* U = GetUi();
		USpnMapScreen* Screen = U != nullptr ? U->GetMapScreen() : nullptr;
		if (Screen != nullptr && Screen->GetMapView() != nullptr && Screen->GetMapView()->GetView() != nullptr)
		{
			Screen->GetMapView()->GetView()->OnMapClick.ExecuteIfBound(IslandTarget, true);
		}
		(void)Id;
	});
	At(106.5, [this]
	{
		APlayerController* PC = GetPlayer();
		const FVector Loc = PC != nullptr && PC->GetPawn() != nullptr ? PC->GetPawn()->GetActorLocation() : FVector::ZeroVector;
		const FSpnNavigationReading R = Map->GetNavigation(Loc, 0.f);
		Mark(FString::Printf(TEXT("island status=%s failure=%s"), LexToString(R.Status), *R.FailureText));
	});
	At(113.0, [this, View, EastTarget]
	{
		Caption(TEXT("Ziel ändern"), TEXT("Ein neuer Wegpunkt ersetzt den alten - die Route wird sofort neu geplant."), 4.5);
		if (SSpnMapView* V = View()) { V->OnMapClick.ExecuteIfBound(EastTarget, true); }
	});
	At(119.0, [this, Ui]
	{
		if (USpnUiSubsystem* U = Ui()) { U->CloseAll(); }
		Mark(TEXT("map_closed"));
	});

	// ------------------------------------------------------------ scene 4: navigation in the world
	At(120.5, [this]
	{
		Mark(TEXT("scene4_drive"));
		Caption(TEXT("Navigation im Spiel"), TEXT("Minimap mit Route, Wegpunkt-Beam in der Welt, Restdistanz oben rechts. Das Fahrzeug ist ein Platzhalter mit Demo-Autopilot (keine Fahrphysik)."), 8.0);
		StartAutopilot(38.f);
	});
	At(131.0, [this] { Caption(TEXT("Restdistanz"), TEXT("Die Anzeige zählt entlang der Straße herunter und folgt dem Fahrzeug."), 5.0); });

	// ------------------------------------------------------------ scene 5: closure and re-route
	At(140.0, [this, Ui]
	{
		Mark(TEXT("scene5_closure"));
		APlayerController* PC = GetPlayer();
		const FVector Loc = PC != nullptr && PC->GetPawn() != nullptr ? PC->GetPawn()->GetActorLocation() : FVector::ZeroVector;
		const double RemainingBefore = Map->GetNavigation(Loc, 0.f).DistanceMetres;
		FSpnRouteView Before;
		Map->GetRoute(Before);
		LengthBefore = RemainingBefore;
		// the same call the X key of the map screen makes
		Map->SetRoadClosed(106, true);
		const FSpnNavigationReading After = Map->GetNavigation(Loc, 0.f);
		LengthAfter = After.DistanceMetres;
		Mark(FString::Printf(TEXT("closure remaining_before=%.1f remaining_after=%.1f status=%s"), RemainingBefore, After.DistanceMetres, LexToString(After.Status)));
		StopAutopilot();
		if (USpnUiSubsystem* U = Ui()) { U->OpenMap(); }
		FVector2D PlayerMetres = FVector2D(900.0, 400.0);
		Map->UnrealToMap(Loc, PlayerMetres);
		AnimateMap(FVector2D(1000.0, 250.0), 4.2, 2.0);
		Caption(TEXT("Straßensperrung"), FString::Printf(TEXT("Die Nordbrücke wird gesperrt (rotes X). Vorher: %s Restweg über die Brücke. Jetzt: %s über Tunnel und Kreisverkehr - automatisch neu berechnet."),
			*Km(RemainingBefore), *Km(After.DistanceMetres)), 9.0);
	});
	At(150.5, [this, Ui]
	{
		if (USpnUiSubsystem* U = Ui()) { U->CloseAll(); }
		Mark(TEXT("closure_map_closed"));
		Caption(TEXT("Umleitung"), TEXT("Das Fahrzeug folgt der neuen Route. Die Fahrt läuft jetzt schneller (Demo-Autopilot 110 m/s), damit der Film kurz bleibt."), 7.0);
		StartAutopilot(110.f);
	});

	// ------------------------------------------------------------ wait for arrival, then outro
	TSharedRef<TFunction<void()>> PollRef = MakeShared<TFunction<void()>>();
	*PollRef = [this, PollRef]()
	{
		APlayerController* PC = GetPlayer();
		const FVector Loc = PC != nullptr && PC->GetPawn() != nullptr ? PC->GetPawn()->GetActorLocation() : FVector::ZeroVector;
		const FSpnNavigationReading R = Map->GetNavigation(Loc, 0.f);
		const bool bTimeout = ScriptTime > 290.0;
		if (R.Status == ESpnNavStatus::Arrived || bTimeout)
		{
			Mark(FString::Printf(TEXT("arrival status=%s timeout=%d"), LexToString(R.Status), bTimeout ? 1 : 0));
			StopAutopilot();
			FStep Outro;
			Outro.Time = ScriptTime + 3.0;
			Outro.Action = [this]
			{
				Mark(TEXT("outro"));
				HideCaption();
				const FString Facts = FString::Printf(TEXT("Karte öffnen, zoomen, verschieben  |  Straßen, Gebäude, Wasser aus MapGeometry  |  Wegpunkt per Klick  |  echte Straßenroute (%s statt %s Luftlinie)  |  Meldung bei unerreichbarem Ziel  |  Minimap mit Route und Wegpunkt-Beam  |  Straßensperrung mit automatischer Neuberechnung (%s -> %s)"),
					*Km(RouteInitialM), *Km(StraightInitialM), *Km(LengthBefore), *Km(LengthAfter));
				Title(TEXT("Belegt in dieser Aufnahme"), Facts,
					TEXT("Einschränkungen: Platzhalterdaten (keine Welt, keine Geografie) | keine Höhendaten: Tunnel als Röhre am Boden | Demo-Autopilot statt Fahrphysik | keine ETA/Verkehr | Koordinaten-Adapter nur für die Demo (kein RC-01-Vertrag) | Zahlen sind unabgestimmte Platzhalter"), 0.0);
			};
			Steps.Add(MoveTemp(Outro));
			FStep End;
			End.Time = ScriptTime + 19.0;
			End.Action = [this]
			{
				Mark(TEXT("end"));
				const FString Report = FString::Printf(TEXT("SPN_NAVDEMO_REPORT\nroute_initial_m=%.1f\nstraight_line_initial_m=%.1f\nremaining_before_closure_m=%.1f\nremaining_after_closure_m=%.1f\nscript_seconds=%.1f\n"), RouteInitialM, StraightInitialM, LengthBefore, LengthAfter, ScriptTime);
				const FString Path = ReportPath.IsEmpty() ? FPaths::ProjectSavedDir() / TEXT("NavDemo/report.txt") : ReportPath;
				FFileHelper::SaveStringToFile(Report, *Path);
				UE_LOG(LogTemp, Display, TEXT("SPN_NAVDEMO_END"));
				if (FParse::Param(FCommandLine::Get(), TEXT("SpnNavDemoQuit"))) { FPlatformMisc::RequestExit(false); }
			};
			Steps.Add(MoveTemp(End));
		}
		else
		{
			FStep Again;
			Again.Time = ScriptTime + 1.0;
			Again.Action = *PollRef;
			Steps.Add(MoveTemp(Again));
		}
	};
	At(158.0, *PollRef);
}
