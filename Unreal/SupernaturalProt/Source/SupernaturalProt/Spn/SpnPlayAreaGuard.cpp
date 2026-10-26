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

FVector ASpnPlayAreaGuard::ChooseReturnLocation(const FVector& From) const
{
	FVector Best = SafeLocation;
	double BestDistance = TNumericLimits<double>::Max();
	for (const FVector& Point : ResetPoints)
	{
		const double Distance = FVector::DistSquared2D(From, Point);
		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			Best = Point;
		}
	}
	return Best;
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
	const FVector Target = ChooseReturnLocation(Pawn->GetActorLocation());
	Pawn->TeleportTo(Target, Pawn->GetActorRotation(), /*bIsATest*/ false, /*bNoCheck*/ true);
	LastReturnLocation = Target;
	++ReturnCount;
	UE_LOG(LogSupernaturalProt, Warning, TEXT("'%s' left the play area and was put back at %s"), *GetNameSafe(Pawn), *Target.ToString());
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
