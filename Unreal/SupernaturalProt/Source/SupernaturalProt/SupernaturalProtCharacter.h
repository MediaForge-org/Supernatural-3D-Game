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

	/** Runtime fallback used while InteractAction is not set: an Enhanced Input action and context mapping DefaultInteractKey */
	UPROPERTY(Transient)
	UInputAction* RuntimeInteractAction;

	UPROPERTY(Transient)
	UInputMappingContext* RuntimeInteractMappingContext;

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

public:

	/** Constructor */
	ASupernaturalProtCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Removes the runtime interact mapping again */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Returns the action bound to interact: InteractAction, or the runtime fallback (created and mapped on first use) */
	UInputAction* ResolveInteractAction();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

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

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns the action bound to interact (InteractAction or the runtime fallback); null before input setup **/
	FORCEINLINE UInputAction* GetBoundInteractAction() const { return InteractAction != nullptr ? InteractAction : RuntimeInteractAction; }
};

