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
#include "Spn/SpnInteractionComponent.h"
#include "SupernaturalProt.h"

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

ASupernaturalProtCharacter::ASupernaturalProtCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

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
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASupernaturalProtCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASupernaturalProtCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(ResolveInteractAction(), ETriggerEvent::Started, this, &ASupernaturalProtCharacter::DoInteract);

		// Sprinting (hold)
		UInputAction* Sprint = ResolveSprintAction();
		EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Started, this, &ASupernaturalProtCharacter::DoSprintStart);
		EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Completed, this, &ASupernaturalProtCharacter::DoSprintEnd);
		EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Canceled, this, &ASupernaturalProtCharacter::DoSprintEnd);
	}
	else
	{
		UE_LOG(LogSupernaturalProt, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASupernaturalProtCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HideMeshCameraDistance > 0.f && CameraBoom != nullptr && FollowCamera != nullptr && GetMesh() != nullptr)
	{
		const float CameraDistance = FVector::Dist(FollowCamera->GetComponentLocation(), CameraBoom->GetComponentLocation());
		GetMesh()->SetOwnerNoSee(CameraDistance < HideMeshCameraDistance);
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

	Super::EndPlay(EndPlayReason);
}

UInputAction* ASupernaturalProtCharacter::ResolveSprintAction()
{
	if (RuntimeSprintAction == nullptr)
	{
		RuntimeSprintAction = NewObject<UInputAction>(this, TEXT("IA_SpnSprint_Runtime"), RF_Transient);
		RuntimeSprintAction->ValueType = EInputActionValueType::Boolean;

		RuntimeSprintMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_SpnSprint_Runtime"), RF_Transient);
		RuntimeSprintMappingContext->MapKey(RuntimeSprintAction, DefaultSprintKey);
	}

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput = GetEnhancedInput(GetController()))
	{
		EnhancedInput->AddMappingContext(RuntimeSprintMappingContext, RuntimeInteractContextPriority);
	}
	return RuntimeSprintAction;
}

void ASupernaturalProtCharacter::DoSprintStart()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (SpeedBeforeSprint <= 0.f)
	{
		SpeedBeforeSprint = Movement->MaxWalkSpeed;
	}
	Movement->MaxWalkSpeed = SprintSpeed;
}

void ASupernaturalProtCharacter::DoSprintEnd()
{
	if (SpeedBeforeSprint > 0.f)
	{
		GetCharacterMovement()->MaxWalkSpeed = SpeedBeforeSprint;
		SpeedBeforeSprint = 0.f;
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

void ASupernaturalProtCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ASupernaturalProtCharacter::DoMove(float Right, float Forward)
{
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
