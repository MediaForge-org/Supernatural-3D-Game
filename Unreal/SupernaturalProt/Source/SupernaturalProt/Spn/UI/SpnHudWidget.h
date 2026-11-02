#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Spn/UI/SpnUiNotify.h"

#include "SpnHudWidget.generated.h"

class UBorder;
class UTextBlock;
class UVerticalBox;
class USpnMapViewWidget;
class USpnMapSubsystem;

// The unified gameplay HUD (Night Build 01, work package I): compass, minimap, interaction prompt, status notifications and the contextual hints.
// ENGINEERING PROPOSAL (layout, wording). It never takes input (every element is hit-test invisible) and shows nothing that is not implemented:
// no health, ammo or quest data. It reads the shared USpnMapSubsystem; the minimap and the compass are views of the same state as the world map.
UCLASS()
class SUPERNATURALPROT_API USpnHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void PushNotification(const FText& Text, SpnUi::ENote Kind);
	void SetPrompt(const FString& Text);
	/** Hints name controller buttons when the last input came from a gamepad. */
	void SetUsingGamepad(bool bValue);
	/** Hides the HUD while a full-screen screen (map, pause) is up. */
	void SetSuppressed(bool bValue);

	// --- inspection (tests read what is really on screen) ---
	FString GetPromptString() const;
	bool IsPromptVisible() const;
	int32 GetNotificationCount() const { return Notes.Num(); }
	FString GetNotificationString(int32 Index) const;
	bool IsMinimapVisible() const;
	bool IsCompassVisible() const;
	FString GetNavigationString() const;
	FString GetHintString() const;
	USpnMapViewWidget* GetMinimap() const { return Minimap; }
	USpnMapViewWidget* GetCompass() const { return Compass; }
	int32 GetMinimapRebuilds() const { return MinimapRebuilds; }

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	void BuildTree();
	void RefreshNotes();
	void RefreshHint();

	struct FNote { FText Text; SpnUi::ENote Kind = SpnUi::ENote::Info; double Expires = 0.0; };

	TSharedPtr<SWidget> HeldSlate;
	TArray<FNote> Notes;
	double Clock = 0.0;
	bool bGamepad = false;
	bool bSuppressed = false;
	bool bHintDirty = true;
	FString HintCache;

	// minimap rebuild policy: only when something that changes the picture changed
	FVector LastLocation = FVector(1e18);
	float LastYaw = 1e9f;
	uint64 LastRevision = ~0ull;
	uint64 LastRouteGeneration = ~0ull;
	float LastZoom = -1.f;
	bool LastHeadingUp = false;
	double LastRebuild = -1e9;
	int32 MinimapRebuilds = 0;

	UPROPERTY(Transient) TObjectPtr<USpnMapViewWidget> Minimap;
	UPROPERTY(Transient) TObjectPtr<USpnMapViewWidget> Compass;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> MinimapCaption;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> MinimapColumn;
	UPROPERTY(Transient) TObjectPtr<UBorder> CompassBox;
	UPROPERTY(Transient) TObjectPtr<UBorder> PromptBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> PromptText;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> NoteBox;
	UPROPERTY(Transient) TArray<TObjectPtr<UTextBlock>> NoteLines;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintText;
};
