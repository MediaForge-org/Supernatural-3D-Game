// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputCoreTypes.h"
#include "Logging/LogMacros.h"
#include "SupernaturalProtCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpnInteractionComponent;
class USpnMovementDiagnostics;
class USpnCharacterMovementComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ASupernaturalProtCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Finds and uses interactables in front of the character (SPN placeholder interaction) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpnInteractionComponent* Interaction;

	/** Opt-in movement recorder (inactive until spn.MoveDiag 1 / bEnabled): frame data for the jump and step-up investigation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpnMovementDiagnostics* MovementDiagnostics;

	/** Runtime fallback used while InteractAction is not set: an Enhanced Input action and context mapping DefaultInteractKey */
	UPROPERTY(Transient)
	UInputAction* RuntimeInteractAction;

	UPROPERTY(Transient)
	UInputMappingContext* RuntimeInteractMappingContext;

	UPROPERTY(Transient)
	UInputAction* RuntimeForceAction;

	UPROPERTY(Transient)
	UInputMappingContext* RuntimeForceMappingContext;

	/** Runtime sprint action and context (hold DefaultSprintKey), created in code like the interact fallback */
	UPROPERTY(Transient)
	UInputAction* RuntimeSprintAction;

	UPROPERTY(Transient)
	UInputMappingContext* RuntimeSprintMappingContext;

	/** Runtime action and context that cycle the base gait (Walk, Jog, Run), created in code like the sprint fallback */
	UPROPERTY(Transient)
	UInputAction* RuntimeGaitAction;

	UPROPERTY(Transient)
	UInputMappingContext* RuntimeGaitMappingContext;

	/** The boom's arm length as authored (read on the first tick) and the current cap that CameraArmRecoverySpeed grows back */
	float ConfiguredArmLength = 0.f;
	float ArmLimit = 0.f;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Interact Input Action. If set, map it in an Input Mapping Context yourself; if empty, DefaultInteractKey is mapped at runtime */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;

	/** Key for the runtime interact mapping, used only while InteractAction is empty (placeholder binding) */
	UPROPERTY(EditAnywhere, Category="Input")
	FKey DefaultInteractKey = EKeys::E;

	/**
	 * Gamepad key for Interact (DualSense Square = Gamepad_FaceButton_Left), mapped at runtime with DefaultInteractKey. Interaction and a normal
	 * attack are separate actions on separate keys (there is no attack action in this character yet); a test keeps the key sets disjoint.
	 */
	UPROPERTY(EditAnywhere, Category="Input")
	FKey DefaultInteractGamepadKey = EKeys::Gamepad_FaceButton_Left;

	/** Keys for the Force verb (break a locked door open): F, and DualSense Triangle (Gamepad_FaceButton_Top). Mapped at runtime. */
	UPROPERTY(EditAnywhere, Category="Input")
	FKey DefaultForceKey = EKeys::F;

	UPROPERTY(EditAnywhere, Category="Input")
	FKey DefaultForceGamepadKey = EKeys::Gamepad_FaceButton_Top;

	/** Key held to sprint (placeholder binding, mapped at runtime) */
	UPROPERTY(EditAnywhere, Category="Input")
	FKey DefaultSprintKey = EKeys::LeftShift;

	/**
	 * Gamepad key held to sprint (placeholder binding, mapped at runtime; L3 is the left thumbstick press for Xbox-compatible pads and
	 * the DualSense). The combat documentation has no sprint binding for pads yet (Kampfsystem 06); set this to "Invalid" to map none.
	 */
	UPROPERTY(EditAnywhere, Category="Input")
	FKey DefaultSprintGamepadKey = EKeys::Gamepad_LeftThumbstick;

	/** While the camera is closer than this to its pivot (squeezed by a wall behind the character), the character mesh is hidden
	 *  in the owner's view so the camera does not look into the body. 0 disables it. Placeholder value, cm. */
	UPROPERTY(EditAnywhere, Category="Camera", meta = (ClampMin = "0"))
	float HideMeshCameraDistance = 70.f;

	/** How fast the camera arm may grow back after an obstacle shortened it, cm/s. Shortening is never smoothed (collision
	 *  safety first); only the way back out is, so the camera does not pop from "squeezed" to "far and high". 0 = instant. Placeholder value. */
	UPROPERTY(EditAnywhere, Category="Camera", meta = (ClampMin = "0"))
	float CameraArmRecoverySpeed = 300.f;

	/**
	 * Stair camera smoothing (Night Build 01, A1): time constant (seconds) of the exponential decay that lets the VISUAL camera follow a step
	 * instead of snapping with the capsule. 0 = off. Only the camera boom is offset; the capsule and the physical position are never touched.
	 * Disabled while airborne (jumps and falls stay immediate) and for vertical jumps larger than twice the step height (teleports, ledges).
	 */
	UPROPERTY(EditAnywhere, Category="Camera", meta = (ClampMin = "0", ClampMax = "1"))
	float StairCameraSmoothingSeconds = 0.14f;

	/** Largest lag (cm) the smoothing may build up; the camera never trails further than this behind the capsule. */
	UPROPERTY(EditAnywhere, Category="Camera", meta = (ClampMin = "0", ClampMax = "100"))
	float StairCameraMaxLag = 24.f;

	/** Key that cycles the base gait Walk -> Jog -> Run (placeholder binding, mapped at runtime) */
	UPROPERTY(EditAnywhere, Category="Input")
	FKey DefaultGaitCycleKey = EKeys::V;

public:

	/** Constructor: uses USpnCharacterMovementComponent (four gaits, configurable jump height) */
	ASupernaturalProtCharacter(const FObjectInitializer& ObjectInitializer);

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Hides the mesh in the owner's view while the camera is squeezed against geometry (HideMeshCameraDistance) */
	virtual void Tick(float DeltaSeconds) override;

	/** Removes the runtime interact mapping again */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Returns the action bound to interact: InteractAction, or the runtime fallback (created and mapped on first use) */
	UInputAction* ResolveInteractAction();

	/** Creates (once) and maps the runtime sprint action */
	UInputAction* ResolveSprintAction();

	/** Creates (once) and maps the runtime gait-cycle action */
	UInputAction* ResolveGaitAction();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input (gamepad stick): scaled by the gamepad sensitivity setting */
	void Look(const FInputActionValue& Value);

	/** Called for mouse looking: scaled by the mouse sensitivity setting */
	void LookMouse(const FInputActionValue& Value);

	/** Player settings (sensitivity, invert Y, smoothing) of this game instance; defaults if there is none */
	struct FLookSettings { float Mouse = 1.f; float Gamepad = 1.f; bool bInvertY = false; bool bSmoothing = true; };
	FLookSettings ReadLookSettings() const;

	/** Applies the vertical camera lag for this frame (visual only) */
	void UpdateStairCamera(float DeltaSeconds);

public:
	/** Current vertical camera lag in cm (positive: the camera is below where the capsule would put it). For tests and diagnostics. */
	FORCEINLINE float GetStairCameraLag() const { return StairLagZ; }
	/** Time constant of the stair camera smoothing in seconds (0 = off). Tests and settings screens. */
	FORCEINLINE float GetStairCameraSmoothingSeconds() const { return StairCameraSmoothingSeconds; }
	FORCEINLINE void SetStairCameraSmoothingSeconds(float Seconds) { StairCameraSmoothingSeconds = FMath::Clamp(Seconds, 0.f, 1.f); }
	/** Capsule-follow smoothing of the camera was reset (teleport / load); the next frame starts without lag. */
	void ResetStairCamera();

private:
	float StairLagZ = 0.f;
	float StairPrevZ = 0.f;
	bool bStairInit = false;
	FVector StairBaseLocation = FVector::ZeroVector;
	bool bStairBaseKnown = false;

protected:

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Handles interact pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoInteract();

	/** Force verb (F / DualSense Triangle): break a locked, breakable door open. Separate from Interact on purpose. */
	virtual void DoForce();
	UInputAction* ResolveForceAction();

	/** Requests the Sprint gait; it applies while the input points forward (predicted through the movement component) */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoSprintStart();

	/** Ends the sprint request: back to the base gait */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoSprintEnd();

	/** Cycles the base gait Walk -> Jog -> Run */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoCycleGait();

public:

	/** Rate limit of the camera arm growing back, cm/s **/
	FORCEINLINE float GetCameraArmRecoverySpeed() const { return CameraArmRecoverySpeed; }
	FORCEINLINE void SetCameraArmRecoverySpeed(float Speed) { CameraArmRecoverySpeed = Speed; } // 0 = instant (tests, tuning)

	/** The authored camera arm length (0 until the first tick) **/
	FORCEINLINE float GetConfiguredArmLength() const { return ConfiguredArmLength; }

	/** Camera distance under which the mesh is hidden in the owner's view (0 = never) **/
	FORCEINLINE float GetHideMeshCameraDistance() const { return HideMeshCameraDistance; }

	/** The movement component with the gait logic; null only if a Blueprint swapped the component class **/
	USpnCharacterMovementComponent* GetSpnMovement() const;

	/** Returns the runtime sprint action; null before input setup **/
	FORCEINLINE UInputAction* GetBoundSprintAction() const { return RuntimeSprintAction; }


	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns the action bound to interact (InteractAction or the runtime fallback); null before input setup **/
	FORCEINLINE UInputAction* GetBoundInteractAction() const { return InteractAction != nullptr ? InteractAction : RuntimeInteractAction; }

	/** The runtime keys of Interact, Force and Sprint (keyboard and gamepad). For the prompt and the key-separation test. */
	struct FSpnGameplayKeys { FKey Interact, InteractPad, Force, ForcePad, Sprint, SprintPad; };
	FSpnGameplayKeys GetGameplayKeys() const { return {DefaultInteractKey, DefaultInteractGamepadKey, DefaultForceKey, DefaultForceGamepadKey, DefaultSprintKey, DefaultSprintGamepadKey}; }
};

