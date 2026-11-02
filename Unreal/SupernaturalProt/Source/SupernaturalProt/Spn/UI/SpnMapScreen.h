#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpnMapSubsystem.h"
#include "Spn/UI/SpnUiButton.h"

#include "SpnMapScreen.generated.h"

class UBorder;
class UTextBlock;
class UVerticalBox;
class USpnMapViewWidget;

// The world map screen (Night Build 01, work package B): a UMG screen around the map view of USpnMapViewWidget. ENGINEERING PROPOSAL (layout, keys).
// It reads and writes the shared USpnMapSubsystem only: no marker, waypoint or discovery state lives here.
//
// INPUT (routed by USpnUiSubsystem's pre-processor; none of it is a gameplay binding)
//   mouse     drag = pan, wheel = zoom (around the cursor), left click = select, right click = set waypoint, buttons on the right
//   keyboard  arrows / WASD = pan, Q E or - + = zoom, Tab / Shift+Tab = next / previous entry, Enter or Space = waypoint (selected entry, else the
//             crosshair), Delete = remove the waypoint, C or Home = centre on the player, F = whole area, M or Esc = close
//   gamepad   left stick = pan, L2 / R2 = zoom, Cross = waypoint, Square = remove the waypoint, R1 / L1 = next / previous entry, L3 = centre on player,
//             Triangle or Circle = close
UCLASS()
class SUPERNATURALPROT_API USpnMapScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnCloseRequested);
	FOnCloseRequested OnCloseRequested;

	/** Binds to the map subsystem and the pawn's pose provider. Safe to call again. */
	void Setup(USpnMapSubsystem* InMap);
	/** Called each time the screen is shown: centres on the player, clears the selection. */
	void OnShown();
	bool HandleKey(const FKey& Key, bool bRepeat, bool bShift);
	void HandleAnalog(const FKey& Key, float Value);

	void SetUsingGamepad(bool bValue);

	// --- actions (also used by the buttons and by tests) ---
	/** Waypoint on the selected entry, else at the crosshair / last mouse position. */
	void PlaceWaypoint();
	void RemoveWaypoint();
	/** Road navigation: closes / opens the road segment under the mouse cursor (else the crosshair). Re-plans the route when it uses the road. */
	void ToggleRoadClosure();
	/** Road navigation: vehicle <-> on foot; re-plans to the same waypoint. */
	void ToggleTravelMode();
	void CenterOnPlayer();
	void SelectNext(int32 Step);
	void ClearSelection();

	// --- inspection ---
	FString GetPlayerString() const;
	FString GetSelectionString() const;
	FString GetWaypointString() const;
	FString GetMessageString() const;
	FString GetSelectedPoi() const { return SelectedPoi; }
	int64 GetSelectedMarker() const { return SelectedMarker; }
	USpnMapViewWidget* GetMapView() const { return MapView; }
	FVector2D GetCrosshairMetres() const;
	double GetMetresPerPixel() const;
	bool IsCrosshairShown() const;
	int32 GetLegendCount() const { return LegendCount; }

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	void BuildTree();
	void RefreshTexts();
	void OnMapClick(const FVector2D& Metres, bool bRight);
	void Select(const FString& PoiId, int64 MarkerId);
	void ShowMessage(const FText& Text, const FLinearColor& Color);
	USpnUiButton* NewButton(const FText& Title, TFunction<void()> Action);
	UTextBlock* NewText(int32 Size, bool bBold, const FLinearColor& Color);
	bool GetPlayerPose(FVector2D& OutMetres, double& OutBearing, FVector& OutLocation, float& OutYaw) const;

	TSharedPtr<SWidget> HeldSlate;
	TWeakObjectPtr<USpnMapSubsystem> Map;
	FString SelectedPoi;
	int64 SelectedMarker = 0;
	bool bUsingGamepad = false;
	FVector2D PanInput = FVector2D::ZeroVector;
	FVector2D PanKeys = FVector2D::ZeroVector;
	float ZoomInput = 0.f;
	float ZoomKeys = 0.f;
	double Clock = 0.0;
	double NextTextRefresh = 0.0;
	double MessageUntil = 0.0;
	uint64 LastRevision = ~0ull;
	int32 LegendCount = 0;

	UPROPERTY(Transient) TObjectPtr<USpnMapViewWidget> MapView;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> PlayerText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> SelectionText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> WaypointText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> MessageText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintText;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> ButtonBox;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> LegendBox;
	UPROPERTY(Transient) TArray<TObjectPtr<USpnUiButton>> Buttons;
};
