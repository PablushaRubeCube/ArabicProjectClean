

#include "Characters/Enemy/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Pickup/Item_Pickup.h"
#include "Characters/Enemy/EnemyAIController.h"
#include "BrainComponent.h"
#include "Components/WeaponComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);

	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RandomMaxSpeedRange = FVector2D(300.f, 650);

	// Set in bp � bp UseAccelerationForPaths = true;
}

void AEnemyCharacter::Death()
{
	Super::Death();

	SetLifeSpan(2.0f);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(Controller);
	if (AIController && AIController->GetBrainComponent())
	{
		AIController->GetBrainComponent()->Cleanup();
	}

	DropLoot();

	// TODO replace with weapon pool or combine weapon and enemy
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto* AttachedActor : AttachedActors)
	{
		AttachedActor->Destroy();
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GetWorld());
	check(GetCharacterMovement());
	check(GetMesh());

	GetCharacterMovement()->MaxWalkSpeed = GetRandomMaxSpeed();
}

float AEnemyCharacter::GetRandomMaxSpeed() const
{
	return FMath::RandRange(RandomMaxSpeedRange.X, RandomMaxSpeedRange.Y);
}

void AEnemyCharacter::DropLoot() const
{
	auto DroopToSpawn = GetRandomDroop();
	if (!DroopToSpawn) return;

	const FActorSpawnParameters SpawnParam;
	GetWorld()->SpawnActor<AItem_Pickup>(DroopToSpawn, GetActorTransform(), SpawnParam);
}

TSubclassOf<AItem_Pickup> AEnemyCharacter::GetRandomDroop() const
{
	if (!(ItemToDrops.Num() > 0)) return nullptr;

	float TotalWeight = 0.0f;

	for (const auto& ItemDrop : ItemToDrops)
	{
		TotalWeight += ItemDrop.Value;
	}

	float RandomPoint = FMath::FRand();

	float AccumulatedWieght = 0.0f;
	for (const auto& ItemDrop : ItemToDrops)
	{
		AccumulatedWieght += ItemDrop.Value / TotalWeight;
		if (RandomPoint <= AccumulatedWieght)
		{
			return ItemDrop.Key;
		}
	}
	return nullptr;
}
