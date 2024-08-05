// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Pickup/Item_Pickup.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Characters/PlayerCharacter.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AItem_Pickup::AItem_Pickup()
{
	TimerRate = 0.01f;
	MoveItemCurve = nullptr;
	TotalTimeMove = 1.f;

	PlayerChar = nullptr;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapShere"));
	OverlapSphere->SetSphereRadius(30.0f);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	OverlapSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(OverlapSphere);

	NiagaraPickup = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraPickup"));
	NiagaraPickup->SetupAttachment(GetRootComponent());

	TimerStart = 0.f;

	OverlapParticle = nullptr;
	PickupSound = nullptr;
}

void AItem_Pickup::BeginPlay()
{
	Super::BeginPlay();
	check(GetWorld());
	check(OverlapSphere);
	check(NiagaraPickup);

	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem_Pickup::OnSphereBeginOverlap);
}

void AItem_Pickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (!PlayerChar) return;

	if (OverlapParticle)
	{
		const FName SocketName = NAME_None;
		UNiagaraFunctionLibrary::SpawnSystemAttached(OverlapParticle, OverlapSphere, SocketName,
			FVector::ZeroVector,	// Offset (should be zero)
			FRotator::ZeroRotator,	// Rotation (should be zero)
			EAttachLocation::SnapToTargetIncludingScale, true);
	}

	if (!PickupItem()) return;
}

bool AItem_Pickup::PickupItem()
{
	if (!GetMoveItemLocations()) return false;
	if (!MoveItemCurve) return false;

	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	float Min = 0.f;
	MoveItemCurve->GetTimeRange(Min, TotalTimeMove);

	TimerStart = GetWorld()->GetTimeSeconds();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AItem_Pickup::MoveToTarget, TimerRate, true);
	return true;
}

void AItem_Pickup::MoveToTarget()
{
	UpdateMove(OverlapItemLocation, PlayerChar->GetActorLocation(), MoveItemCurve);

	if ((GetWorld()->GetTimeSeconds() - TimerStart) > TotalTimeMove)
	{
		TimerStart = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		FinishAction();
	}
}

void AItem_Pickup::FinishAction()
{
	if (PickupSound) UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
}

bool AItem_Pickup::GetMoveItemLocations()
{
	if (!PlayerChar) return false;
	OverlapItemLocation = GetActorLocation();
	return true;
}

void AItem_Pickup::UpdateMove(const FVector& StartLocation, const FVector& FinishLocation, const UCurveFloat* Curve)
{
	const float ElapsedTime = GetWorld()->GetTimeSeconds() - TimerStart;
	const float CurveValue = Curve->GetFloatValue(ElapsedTime);
	FVector Start = StartLocation;
	Start.Z = 0.f;
	FVector Finish = FinishLocation;
	Finish.Z = 0.f;
	const FVector NewLocation = StartLocation + (Finish - Start) * CurveValue;
	SetActorLocation(NewLocation);
}
