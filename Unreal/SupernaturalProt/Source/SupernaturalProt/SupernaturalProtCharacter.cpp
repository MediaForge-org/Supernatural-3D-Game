// Copyright Epic Games, Inc. All Rights Reserved.

#include "SupernaturalProtCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Spn/SpnCharacterMovementComponent.h"
#include "Engine/GameInstance.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnSettingsSubsystem.h"
#include "Spn/SpnMovementDiagnostics.h"
#include "Spn/Character/SpnCharacterVisualComponent.h"
#include "Spn/Character/SpnWardrobeComponent.h"
#include "SpnNpcReaction.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Spn/UI/SpnUiText.h"
#include "InputTriggers.h"
#include "SupernaturalProt.h"
#include "Spn/Case/SpnCaseSubsystem.h"

namespace
{
	// Above the template contexts (priority 0), so the runtime interact key wins a conflict.
	constexpr int32 RuntimeInteractContextPriority = 1;

	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInput(const AController* Controller)
	{
		const APlayerController* PlayerController = Cast<APlayerController>(Controller);
		return PlayerController != nullptr ? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()) : nullptr;
	}
}

ASupernaturalProtCharacter::ASupernaturalProtCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USpnCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Speeds, accelerations, braking, turn friction and turn rate, minimum analog speed, air control, step height and the
	// jump velocity come from USpnCharacterMovementComponent::Settings (M2-A / M2-A.1); tune them there in the Character Blueprint.

	// Jumping is an active input only: one jump, no jump held longer for more height, no double jump.
	JumpMaxCount = 1;
	JumpMaxHoldTime = 0.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;

	// Interaction with SPN interactables (E by default, see DefaultInteractKey)
	Interaction = CreateDefaultSubobject<USpnInteractionComponent>(TEXT("Interaction"));

	MovementDiagnostics = CreateDefaultSubobject<USpnMovementDiagnostics>(TEXT("MovementDiagnostics"));

	Visual = CreateDefaultSubobject<USpnCharacterVisualComponent>(TEXT("Visual"));
	Wardrobe = CreateDefaultSubobject<USpnWardrobeComponent>(TEXT("Wardrobe"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASupernaturalProtCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASupernaturalProtCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASupernaturalProtCharacter::LookMouse);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASupernaturalProtCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(ResolveInteractAction(), ETriggerEvent::Started, this, &ASupernaturalProtCharacter::DoInteract);
		EnhancedInputComponent->BindAction(ResolveForceAction(), ETriggerEvent::Started, this, &ASupernaturalProtCharacter::DoForce);
		EnhancedInputComponent->BindAction(ResolveForceAction(), ETriggerEvent::Completed, this, &ASupernaturalProtCharacter::DoForceEnd);
		EnhancedInputComponent->BindAction(ResolveForceAction(), ETriggerEvent::Canceled, this, &ASupernaturalProtCharacter::DoForceEnd);
		Interaction->ForceKey = DefaultForceKey;
		Interaction->PromptKey = DefaultInteractKey; // the on-screen prompt names the key that is actually mapped

		// Sprinting (hold)
		UInputAction* Sprint = ResolveSprintAction();
		EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Started, this, &ASupernaturalProtCharacter::DoSprintStart);
		EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Completed, this, &ASupernaturalProtCharacter::DoSprintEnd);
		EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Canceled, this, &ASupernaturalProtCharacter::DoSprintEnd);

		// Base gait (Walk -> Jog -> Run)
		EnhancedInputComponent->BindAction(ResolveGaitAction(), ETriggerEvent::Started, this, &ASupernaturalProtCharacter::DoCycleGait);

		// First case (NightBuild07): throw a carried object as a lure
		EnhancedInputComponent->BindAction(ResolveThrowAction(), ETriggerEvent::Started, this, &ASupernaturalProtCharacter::DoThrow);

		// Gamepad (developer decision 2026-11-14): Force = Square held, L3 = sprint toggle (tap) / gait change (hold)
		ResolvePadActions();
		if (RuntimeForcePadAction != nullptr)
		{
			EnhancedInputComponent->BindAction(RuntimeForcePadAction, ETriggerEvent::Triggered, this, &ASupernaturalProtCharacter::DoForcePad);
			EnhancedInputComponent->BindAction(RuntimeForcePadAction, ETriggerEvent::Completed, this, &ASupernaturalProtCharacter::DoForceEnd);
			EnhancedInputComponent->BindAction(RuntimeForcePadAction, ETriggerEvent::Canceled, this, &ASupernaturalProtCharacter::DoForceEnd);
		}
		if (RuntimeSprintPadAction != nullptr)
		{
			EnhancedInputComponent->BindAction(RuntimeSprintPadAction, ETriggerEvent::Started, this, &ASupernaturalProtCharacter::DoSprintPadPressed);
			EnhancedInputComponent->BindAction(RuntimeSprintPadAction, ETriggerEvent::Completed, this, &ASupernaturalProtCharacter::DoSprintPadReleased);
			EnhancedInputComponent->BindAction(RuntimeSprintPadAction, ETriggerEvent::Canceled, this, &ASupernaturalProtCharacter::DoSprintPadReleased);
		}
	}
	else
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASupernaturalProtCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickStepAside(DeltaSeconds);
	TickPadSprint(DeltaSeconds);
	UpdateStairCamera(DeltaSeconds);
	TickFootstepNoise(DeltaSeconds);

	if (HideMeshCameraDistance > 0.f && CameraBoom != nullptr && FollowCamera != nullptr && GetMesh() != nullptr)
	{
		const float CameraDistance = FVector::Dist(FollowCamera->GetComponentLocation(), CameraBoom->GetComponentLocation());
		GetMesh()->SetOwnerNoSee(CameraDistance < HideMeshCameraDistance);
		if (Visual != nullptr) { Visual->SetShownOwnerNoSee(CameraDistance < HideMeshCameraDistance); }
	}

	// Camera arm: an obstacle shortens it at once (the spring arm's own collision result, so nothing is ever clipped),
	// the way back out is rate-limited. Without this the arm jumps from a few cm to its full length as soon as the
	// camera swings over the edge of a low obstacle.
	if (CameraBoom != nullptr && FollowCamera != nullptr)
	{
		if (ConfiguredArmLength <= 0.f)
		{
			ConfiguredArmLength = CameraBoom->TargetArmLength;
			ArmLimit = ConfiguredArmLength;
		}
		if (CameraArmRecoverySpeed > 0.f && ConfiguredArmLength > 0.f)
		{
			const float Actual = FVector::Dist(FollowCamera->GetComponentLocation(), CameraBoom->GetComponentLocation());
			if (Actual < ArmLimit - 1.f)
			{
				ArmLimit = FMath::Max(Actual, 10.f);
			}
			else
			{
				ArmLimit = FMath::Min(ConfiguredArmLength, ArmLimit + CameraArmRecoverySpeed * DeltaSeconds);
			}
			CameraBoom->TargetArmLength = ArmLimit;
		}
	}
}

void ASupernaturalProtCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (RuntimeForceMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
		{
			EnhancedInput->RemoveMappingContext(RuntimeForceMappingContext);
		}
	}

	if (RuntimeInteractMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
		{
			EnhancedInput->RemoveMappingContext(RuntimeInteractMappingContext);
		}
	}

	if (RuntimeSprintMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
		{
			EnhancedInput->RemoveMappingContext(RuntimeSprintMappingContext);
		}
	}

	if (RuntimeGaitMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
		{
			EnhancedInput->RemoveMappingContext(RuntimeGaitMappingContext);
		}
	}

	if (RuntimeThrowMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
		{
			EnhancedInput->RemoveMappingContext(RuntimeThrowMappingContext);
		}
	}

	if (RuntimePadMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
		{
			EnhancedInput->RemoveMappingContext(RuntimePadMappingContext);
		}
	}

	Super::EndPlay(EndPlayReason);
}

USpnCharacterMovementComponent* ASupernaturalProtCharacter::GetSpnMovement() const
{
	return Cast<USpnCharacterMovementComponent>(GetCharacterMovement());
}

UInputAction* ASupernaturalProtCharacter::ResolveGaitAction()
{
	if (RuntimeGaitAction == nullptr)
	{
		RuntimeGaitAction = NewObject<UInputAction>(this, TEXT("IA_SpnGaitCycle_Runtime"), RF_Transient);
		RuntimeGaitAction->ValueType = EInputActionValueType::Boolean;

		RuntimeGaitMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_SpnGaitCycle_Runtime"), RF_Transient);
		RuntimeGaitMappingContext->MapKey(RuntimeGaitAction, DefaultGaitCycleKey);
	}

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
	{
		EnhancedInput->AddMappingContext(RuntimeGaitMappingContext, RuntimeInteractContextPriority);
	}
	return RuntimeGaitAction;
}

UInputAction* ASupernaturalProtCharacter::ResolveThrowAction()
{
	if (RuntimeThrowAction == nullptr)
	{
		RuntimeThrowAction = NewObject<UInputAction>(this, TEXT("IA_SpnThrow_Runtime"), RF_Transient);
		RuntimeThrowAction->ValueType = EInputActionValueType::Boolean;

		RuntimeThrowMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_SpnThrow_Runtime"), RF_Transient);
		RuntimeThrowMappingContext->MapKey(RuntimeThrowAction, DefaultThrowKey);
	}

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
	{
		EnhancedInput->AddMappingContext(RuntimeThrowMappingContext, RuntimeInteractContextPriority);
	}
	return RuntimeThrowAction;
}

void ASupernaturalProtCharacter::DoThrow()
{
	if (USpnCaseSubsystem* Case = GetWorld() != nullptr ? GetWorld()->GetSubsystem<USpnCaseSubsystem>() : nullptr)
	{
		Case->Throw(this);
	}
}

UInputAction* ASupernaturalProtCharacter::ResolveSprintAction()
{
	if (RuntimeSprintAction == nullptr)
	{
		RuntimeSprintAction = NewObject<UInputAction>(this, TEXT("IA_SpnSprint_Runtime"), RF_Transient);
		RuntimeSprintAction->ValueType = EInputActionValueType::Boolean;

		RuntimeSprintMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_SpnSprint_Runtime"), RF_Transient);
		RuntimeSprintMappingContext->MapKey(RuntimeSprintAction, DefaultSprintKey);   // keyboard: hold; the gamepad key has its own action (ResolvePadActions)
	}

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
	{
		EnhancedInput->AddMappingContext(RuntimeSprintMappingContext, RuntimeInteractContextPriority);
	}
	return RuntimeSprintAction;
}

void ASupernaturalProtCharacter::DoSprintStart()
{
	if (USpnCharacterMovementComponent* Movement = GetSpnMovement())
	{
		Movement->SetSprintRequested(true);
	}
}

void ASupernaturalProtCharacter::DoSprintEnd()
{
	bPadSprintToggled = false; // releasing Shift ends a sprint the pad switched on as well
	if (USpnCharacterMovementComponent* Movement = GetSpnMovement())
	{
		Movement->SetSprintRequested(false);
	}
}

void ASupernaturalProtCharacter::DoCycleGait()
{
	if (USpnCharacterMovementComponent* Movement = GetSpnMovement())
	{
		Movement->CycleBaseGait();
	}
}

void ASupernaturalProtCharacter::ResolvePadActions()
{
	if (RuntimePadMappingContext == nullptr)
	{
		RuntimePadMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_SpnPad_Runtime"), RF_Transient);
		if (DefaultForceGamepadKey.IsValid())
		{
			RuntimeForcePadAction = NewObject<UInputAction>(this, TEXT("IA_SpnForcePad_Runtime"), RF_Transient);
			RuntimeForcePadAction->ValueType = EInputActionValueType::Boolean;
			RuntimeForcePadAction->bConsumeInput = false;   // Square must still reach Interact (the tap); this context is evaluated first
			FEnhancedActionKeyMapping& Mapping = RuntimePadMappingContext->MapKey(RuntimeForcePadAction, DefaultForceGamepadKey);
			UInputTriggerHold* Hold = NewObject<UInputTriggerHold>(RuntimePadMappingContext);
			Hold->HoldTimeThreshold = ForcePadHoldSeconds;
			// not one-shot: a one-shot hold reports Completed right after it fires, which ended the break at once. Triggered repeats while held;
			// DoForcePad starts only when nothing is being forced yet.
			Hold->bIsOneShot = false;
			Mapping.Triggers.Add(Hold);
		}
		if (DefaultSprintGamepadKey.IsValid())
		{
			RuntimeSprintPadAction = NewObject<UInputAction>(this, TEXT("IA_SpnSprintPad_Runtime"), RF_Transient);
			RuntimeSprintPadAction->ValueType = EInputActionValueType::Boolean;
			RuntimePadMappingContext->MapKey(RuntimeSprintPadAction, DefaultSprintGamepadKey);
		}
	}
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
	{
		EnhancedInput->AddMappingContext(RuntimePadMappingContext, RuntimeInteractContextPriority + 1);
	}
}

void ASupernaturalProtCharacter::DoForcePad()
{
	// Square is Interact too: the hold only means "force" where forcing is offered (a locked, breakable door in focus)
	if (Interaction != nullptr && !Interaction->IsForcing() && Interaction->HasForceTarget())
	{
		Interaction->BeginForce();
	}
}

void ASupernaturalProtCharacter::DoSprintPadPressed()
{
	bPadSprintDown = true;
	bPadHoldUsed = false;
	PadSprintDownTime = GetWorld() != nullptr ? GetWorld()->GetTimeSeconds() : 0.0;
}

void ASupernaturalProtCharacter::DoSprintPadReleased()
{
	if (!bPadSprintDown)
	{
		return;
	}
	bPadSprintDown = false;
	if (bPadHoldUsed)
	{
		return; // the hold changed the gait; the release does nothing else
	}
	// a tap: sprint on / off
	bPadSprintToggled = !bPadSprintToggled;
	PadSprintIdleSeconds = 0.f;
	if (USpnCharacterMovementComponent* Movement = GetSpnMovement())
	{
		Movement->SetSprintRequested(bPadSprintToggled);
	}
}

void ASupernaturalProtCharacter::TickPadSprint(float DeltaSeconds)
{
	if (bPadSprintDown && !bPadHoldUsed && GetWorld() != nullptr && GetWorld()->GetTimeSeconds() - PadSprintDownTime >= GaitHoldSeconds)
	{
		bPadHoldUsed = true;
		if (USpnCharacterMovementComponent* Movement = GetSpnMovement())
		{
			Movement->CycleBaseGait();
			const TCHAR* Key = Movement->GetBaseGait() == ESpnGait::Walk ? TEXT("ui.gait.walk") : Movement->GetBaseGait() == ESpnGait::Run ? TEXT("ui.gait.run") : TEXT("ui.gait.jog");
			SpnUi::Notify(this, FText::Format(FSpnUiText::Get(TEXT("ui.gait.changed")), FSpnUiText::Get(Key)));
		}
	}
	if (!bPadSprintToggled)
	{
		return;
	}
	// the toggled sprint ends when the player lets go of the stick (or stops) for a moment
	const UCharacterMovementComponent* Movement = GetCharacterMovement();
	const bool bIdle = Movement == nullptr || Movement->GetCurrentAcceleration().IsNearlyZero();
	PadSprintIdleSeconds = bIdle ? PadSprintIdleSeconds + DeltaSeconds : 0.f;
	if (PadSprintIdleSeconds >= SprintToggleReleaseSeconds)
	{
		bPadSprintToggled = false;
		if (USpnCharacterMovementComponent* SpnMovement = GetSpnMovement())
		{
			SpnMovement->SetSprintRequested(false);
		}
	}
}

UInputAction* ASupernaturalProtCharacter::ResolveInteractAction()
{
	if (InteractAction != nullptr)
	{
		return InteractAction;
	}

	// No asset assigned: build a transient Enhanced Input action and context in code,
	// so E works without editing the template's input assets.
	if (RuntimeInteractAction == nullptr)
	{
		RuntimeInteractAction = NewObject<UInputAction>(this, TEXT("IA_SpnInteract_Runtime"), RF_Transient);
		RuntimeInteractAction->ValueType = EInputActionValueType::Boolean;

		RuntimeInteractMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_SpnInteract_Runtime"), RF_Transient);
		RuntimeInteractMappingContext->MapKey(RuntimeInteractAction, DefaultInteractKey);
		if (DefaultInteractGamepadKey.IsValid())
		{
			RuntimeInteractMappingContext->MapKey(RuntimeInteractAction, DefaultInteractGamepadKey);
		}
	}

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
	{
		EnhancedInput->AddMappingContext(RuntimeInteractMappingContext, RuntimeInteractContextPriority);
	}
	return RuntimeInteractAction;
}

void ASupernaturalProtCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

ASupernaturalProtCharacter::FLookSettings ASupernaturalProtCharacter::ReadLookSettings() const
{
	FLookSettings Out;
	if (const UGameInstance* Instance = GetGameInstance())
	{
		if (const USpnSettingsSubsystem* Settings = Instance->GetSubsystem<USpnSettingsSubsystem>())
		{
			Out.Mouse = Settings->Get().MouseSensitivity;
			Out.Gamepad = Settings->Get().GamepadSensitivity;
			Out.bInvertY = Settings->Get().bInvertY;
			Out.bSmoothing = Settings->Get().bCameraSmoothing;
		}
	}
	return Out;
}

void ASupernaturalProtCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	const FLookSettings Settings = ReadLookSettings();

	// route the input
	DoLook(LookAxisVector.X * Settings.Gamepad, LookAxisVector.Y * Settings.Gamepad * (Settings.bInvertY ? -1.f : 1.f));
}

void ASupernaturalProtCharacter::LookMouse(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	const FLookSettings Settings = ReadLookSettings();
	DoLook(LookAxisVector.X * Settings.Mouse, LookAxisVector.Y * Settings.Mouse * (Settings.bInvertY ? -1.f : 1.f));
}

void ASupernaturalProtCharacter::UpdateStairCamera(float DeltaSeconds)
{
	if (CameraBoom == nullptr || DeltaSeconds <= 0.f)
	{
		return;
	}
	if (!bStairBaseKnown)
	{
		StairBaseLocation = CameraBoom->GetRelativeLocation();
		bStairBaseKnown = true;
	}
	const UCharacterMovementComponent* Movement = GetCharacterMovement();
	const float Z = GetActorLocation().Z;
	const bool bEnabled = StairCameraSmoothingSeconds > 0.f && ReadLookSettings().bSmoothing;
	const bool bGrounded = Movement != nullptr && Movement->IsMovingOnGround();
	const float StepLimit = Movement != nullptr ? Movement->MaxStepHeight : 25.f;

	if (bStairInit && bEnabled && bGrounded)
	{
		const float DeltaZ = Z - StairPrevZ;
		// a stair step or a slope: the capsule moved vertically on the ground by a plausible amount in one frame
		if (FMath::Abs(DeltaZ) <= StepLimit * 2.f)
		{
			StairLagZ = FMath::Clamp(StairLagZ + DeltaZ, -StairCameraMaxLag, StairCameraMaxLag);
		}
		else
		{
			StairLagZ = 0.f; // teleport / ledge: no smoothing
		}
	}
	else if (!bGrounded || !bEnabled)
	{
		StairLagZ = 0.f; // airborne (jump, fall) or switched off: the camera is exactly where the capsule puts it
	}
	StairPrevZ = Z;
	bStairInit = true;

	// frame-rate independent exponential decay towards the capsule: lag *= exp(-dt / tau)
	if (bEnabled && bGrounded && StairLagZ != 0.f)
	{
		StairLagZ *= FMath::Exp(-DeltaSeconds / StairCameraSmoothingSeconds);
		if (FMath::Abs(StairLagZ) < 0.05f) { StairLagZ = 0.f; }
	}
	// positive lag = the capsule rose and the camera still trails below: offset the boom DOWN by the lag (visual only)
	CameraBoom->SetRelativeLocation(StairBaseLocation - FVector(0.f, 0.f, StairLagZ));
}

bool ASupernaturalProtCharacter::StartStepAside(const FVector& Target, float MaxSeconds)
{
	if (bSteppingAside || GetCharacterMovement() == nullptr)
	{
		return false;
	}
	bSteppingAside = true;
	StepTarget = Target;
	StepSeconds = 0.f;
	StepMaxSeconds = FMath::Max(MaxSeconds, 0.1f);
	StepStallSeconds = 0.f;
	StepLastDistance = FVector::Dist2D(GetActorLocation(), Target);
	++StepAsideCount;
	// step back while still facing the door: no turning towards the movement direction for the duration of the step
	bStepRestoreOrient = GetCharacterMovement()->bOrientRotationToMovement;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s: steps aside %.0f cm to (%.0f, %.0f)"), *GetName(), StepLastDistance, Target.X, Target.Y);
	return true;
}

void ASupernaturalProtCharacter::TickStepAside(float DeltaSeconds)
{
	if (!bSteppingAside)
	{
		return;
	}
	StepSeconds += DeltaSeconds;
	const FVector To = FVector(StepTarget.X - GetActorLocation().X, StepTarget.Y - GetActorLocation().Y, 0.f);
	const float Distance = To.Size();
	// Blocked = less than 2 cm closer within a 0.3 s window (something solid is in the way: a wall, a pawn); the door then keeps waiting.
	// A window, not a per-frame speed: the calm step starts from standstill and is slow, and the check must not depend on the frame rate.
	bool bBlocked = false;
	StepStallSeconds += DeltaSeconds;
	if (StepStallSeconds >= 0.3f)
	{
		bBlocked = StepLastDistance - Distance < 2.f;
		StepStallSeconds = 0.f;
		StepLastDistance = Distance;
	}
	if (Distance <= 4.f || StepSeconds >= StepMaxSeconds || bBlocked)
	{
		bSteppingAside = false;
		GetCharacterMovement()->bOrientRotationToMovement = bStepRestoreOrient;
		UE_LOG(LogSupernaturalProt, Log, TEXT("%s: step aside ended %s (%.0f cm left after %.2f s)"), *GetName(),
			Distance <= 4.f ? TEXT("at the target") : bBlocked ? TEXT("blocked") : TEXT("by time"), Distance, StepSeconds);
		return;
	}
	// half input: a calm walking step whatever the base gait is (the movement component maps a small input to a slow pace)
	AddMovementInput(To / Distance, 0.5f);
}

void ASupernaturalProtCharacter::DoMove(float Right, float Forward)
{
	if (bSteppingAside)
	{
		return;     // the automatic step owns the movement for its fraction of a second
	}
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ASupernaturalProtCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ASupernaturalProtCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ASupernaturalProtCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ASupernaturalProtCharacter::DoInteract()
{
	// use whatever interactable is in front of the character (authority only)
	Interaction->TryInteract();
}

FVector ASupernaturalProtCharacter::GetCameraBase() const
{
	return bStairBaseKnown ? StairBaseLocation : CameraBoom != nullptr ? CameraBoom->GetRelativeLocation() : FVector::ZeroVector;
}

void ASupernaturalProtCharacter::SetCameraBase(const FVector& RelativeLocation)
{
	StairBaseLocation = RelativeLocation;
	bStairBaseKnown = true;
	if (CameraBoom != nullptr)
	{
		CameraBoom->SetRelativeLocation(StairBaseLocation - FVector(0.f, 0.f, StairLagZ));
	}
}

void ASupernaturalProtCharacter::TickFootstepNoise(float DeltaSeconds)
{
	// Gameplay footsteps (NightBuild06): one noise per FootstepNoiseEveryCm walked on the ground, loudness from the speed (Core FootstepNoiseFor).
	// Authority only: noises are gameplay events, the server's NPCs hear them. No audio asset is involved.
	const UCharacterMovementComponent* Move = GetCharacterMovement();
	if (!bEmitFootstepNoise || !HasAuthority() || Move == nullptr || !Move->IsMovingOnGround() || DeltaSeconds <= 0.f)
	{
		return;
	}
	const float Speed = static_cast<float>(GetVelocity().Size2D());
	float RangeCm = 0.f, Strength = 0.f;
	if (!SpnNpcSense::FootstepNoise(Speed, bIsCrouched != 0, RangeCm, Strength))
	{
		FootstepDistance = 0.f;   // standing still: the next walk starts a fresh stride
		return;
	}
	FootstepDistance += Speed * DeltaSeconds;
	if (FootstepDistance < FootstepNoiseEveryCm)
	{
		return;
	}
	FootstepDistance = FMath::Fmod(FootstepDistance, FMath::Max(FootstepNoiseEveryCm, 1.f));
	const UGameInstance* GameInstance = GetGameInstance();
	USpnNoiseSubsystem* Noise = GameInstance != nullptr ? GameInstance->GetSubsystem<USpnNoiseSubsystem>() : nullptr;
	if (Noise == nullptr)
	{
		return;
	}
	FSpnNoise Step;
	Step.Kind = ESpnNoiseKind::Footstep;
	Step.LocationCm = GetActorLocation() - FVector(0.f, 0.f, GetBodyCollision().HalfHeight);   // at the feet
	Step.RangeCm = RangeCm;
	Step.Strength = Strength;
	Step.SourceId = GetName();
	if (Noise->EmitNoise(Step) != 0)
	{
		++FootstepNoiseCount;
	}
}

void ASupernaturalProtCharacter::CaptureDefaultBodyCollision()
{
	if (bDefaultCollisionCaptured)
	{
		return;
	}
	DefaultCollision = GetBodyCollision();
	DefaultMeshRelative = GetMesh() != nullptr ? GetMesh()->GetRelativeLocation() : FVector::ZeroVector;
	DefaultBaseEyeHeight = BaseEyeHeight;
	bDefaultCollisionCaptured = true;
}

FSpnBodyCollisionProfile ASupernaturalProtCharacter::GetBodyCollision() const
{
	FSpnBodyCollisionProfile P;
	if (const UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		P.Radius = Capsule->GetUnscaledCapsuleRadius();
		P.HalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
	}
	if (const UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		P.CrouchedHalfHeight = Move->GetCrouchedHalfHeight();
	}
	return P;
}

FSpnBodyCollisionProfile ASupernaturalProtCharacter::GetDefaultBodyCollision() const
{
	return bDefaultCollisionCaptured ? DefaultCollision : GetBodyCollision();
}

float ASupernaturalProtCharacter::GetCapsuleCentreShift() const
{
	return GetBodyCollision().HalfHeight - GetDefaultBodyCollision().HalfHeight;
}

bool ASupernaturalProtCharacter::ApplyBodyCollision(const FSpnBodyCollisionProfile& Profile)
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	UCharacterMovementComponent* Move = GetCharacterMovement();
	UWorld* World = GetWorld();
	if (Capsule == nullptr || World == nullptr)
	{
		return false;
	}
	CaptureDefaultBodyCollision();
	const FSpnBodyCollisionProfile Current = GetBodyCollision();
	if (Current.Equals(Profile))
	{
		return true;
	}
	if (bIsCrouched || bSteppingAside || (Move != nullptr && Move->MovementMode == MOVE_Custom))
	{
		UE_LOG(LogSupernaturalProt, Warning, TEXT("%s: body collision %.0f/%.0f not applied - crouched or in a scripted move; the capsule stays %.0f/%.0f"),
			*GetName(), Profile.Radius, Profile.HalfHeight, Current.Radius, Current.HalfHeight);
		return false;
	}

	// the feet stay put: the centre moves by the half-height change. A larger capsule must fit here first (never pushed into a wall / ceiling).
	const float DeltaHalfHeight = Profile.HalfHeight - Current.HalfHeight;
	const FVector NewLocation = GetActorLocation() + FVector(0.f, 0.f, DeltaHalfHeight);
	if (Profile.HalfHeight > Current.HalfHeight || Profile.Radius > Current.Radius)
	{
		FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnBodyCollisionRoom), /*bTraceComplex*/ false, this);
		FCollisionResponseParams Response;
		Capsule->InitSweepCollisionParams(Params, Response);
		if (World->OverlapBlockingTestByChannel(NewLocation, GetActorQuat(), Capsule->GetCollisionObjectType(),
			FCollisionShape::MakeCapsule(Profile.Radius, Profile.HalfHeight), Params, Response))
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s: body collision %.0f/%.0f does not fit here (blocked); the capsule stays %.0f/%.0f"),
				*GetName(), Profile.Radius, Profile.HalfHeight, Current.Radius, Current.HalfHeight);
			return false;
		}
	}

	Capsule->SetCapsuleSize(Profile.Radius, Profile.HalfHeight, /*bUpdateOverlaps*/ false);
	SetActorLocation(NewLocation, /*bSweep*/ false, nullptr, ETeleportType::TeleportPhysics);
	Capsule->UpdateOverlaps();
	if (Move != nullptr)
	{
		Move->SetCrouchedHalfHeight(Profile.CrouchedHalfHeight);
		Move->bJustTeleported = true;
	}
	const float Shift = Profile.HalfHeight - DefaultCollision.HalfHeight;   // centre above the default centre
	if (USkeletalMeshComponent* Body = GetMesh())
	{
		const FVector MeshRelative = DefaultMeshRelative - FVector(0.f, 0.f, Shift);
		Body->SetRelativeLocation(MeshRelative);
		CacheInitialMeshOffset(MeshRelative, Body->GetRelativeRotation());   // the network smoothing restores to this offset
	}
	BaseEyeHeight = DefaultBaseEyeHeight - Shift;
	SetCameraBase(GetCameraBase() - FVector(0.f, 0.f, DeltaHalfHeight));
	ResetStairCamera();   // the teleport by DeltaHalfHeight is not a stair step
	UE_LOG(LogSupernaturalProt, Log, TEXT("%s: body collision radius %.1f, half height %.1f, crouched %.1f (default %.1f/%.1f/%.1f)"), *GetName(),
		Profile.Radius, Profile.HalfHeight, Profile.CrouchedHalfHeight, DefaultCollision.Radius, DefaultCollision.HalfHeight, DefaultCollision.CrouchedHalfHeight);
	return true;
}

void ASupernaturalProtCharacter::ResetStairCamera()
{
	bStairInit = false;
	StairLagZ = 0.f;
	if (CameraBoom != nullptr && bStairBaseKnown)
	{
		CameraBoom->SetRelativeLocation(StairBaseLocation);
	}
}

UInputAction* ASupernaturalProtCharacter::ResolveForceAction()
{
	if (RuntimeForceAction == nullptr)
	{
		RuntimeForceAction = NewObject<UInputAction>(this, TEXT("IA_SpnForce_Runtime"), RF_Transient);
		RuntimeForceAction->ValueType = EInputActionValueType::Boolean;

		RuntimeForceMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_SpnForce_Runtime"), RF_Transient);
		RuntimeForceMappingContext->MapKey(RuntimeForceAction, DefaultForceKey);   // keyboard: F held; the gamepad hold has its own action (ResolvePadActions)
	}

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
	{
		EnhancedInput->AddMappingContext(RuntimeForceMappingContext, RuntimeInteractContextPriority);
	}
	return RuntimeForceAction;
}

void ASupernaturalProtCharacter::DoForce()
{
	// hold to break (a door takes time); a target without duration answers at once
	Interaction->BeginForce();
}

void ASupernaturalProtCharacter::DoForceEnd()
{
	Interaction->EndForce();
}
