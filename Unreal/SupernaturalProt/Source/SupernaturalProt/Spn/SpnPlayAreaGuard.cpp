#include "Spn/SpnPlayAreaGuard.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "SupernaturalProt.h"

ASpnPlayAreaGuard::ASpnPlayAreaGuard()
{
	PrimaryActorTick.bCanEverTick = false;

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	SetRootComponent(Volume);
	Volume->SetBoxExtent(BoxExtent);
	Volume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Volume->SetCollisionResponseToAllChannels(ECR_Ignore);
	Volume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Volume->SetGenerateOverlapEvents(true);
	Volume->SetHiddenInGame(true);
	Volume->OnComponentEndOverlap.AddDynamic(this, &ASpnPlayAreaGuard::HandleEndOverlap);
}

void ASpnPlayAreaGuard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Volume->SetBoxExtent(BoxExtent);
}

void ASpnPlayAreaGuard::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// BoxExtent may have been set by a script after the volume was serialized with its default size.
	Volume->SetBoxExtent(BoxExtent);
}

bool ASpnPlayAreaGuard::IsInside(const AActor* Actor) const
{
	return Actor != nullptr && Volume->Bounds.GetBox().IsInside(Actor->GetActorLocation());
}

bool ASpnPlayAreaGuard::ReturnToSafeLocation(APawn* Pawn)
{
	if (Pawn == nullptr || !HasAuthority())
	{
		return false;
	}

	if (const ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
	}
	Pawn->TeleportTo(SafeLocation, Pawn->GetActorRotation(), /*bIsATest*/ false, /*bNoCheck*/ true);
	++ReturnCount;
	UE_LOG(LogSupernaturalProt, Warning, TEXT("'%s' left the play area and was put back at %s"), *GetNameSafe(Pawn), *SafeLocation.ToString());
	return true;
}

void ASpnPlayAreaGuard::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	// The capsule can also leave a part of the box while the pawn is still inside; only act when its location is outside.
	if (Pawn != nullptr && HasAuthority() && !IsInside(Pawn))
	{
		ReturnToSafeLocation(Pawn);
	}
}
