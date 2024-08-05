// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Pickup/Heal/Item_Heal.h"
#include "Characters/PlayerCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

AItem_Heal::AItem_Heal()
{
	HealValue = 10.0f;
}

void AItem_Heal::BeginPlay()
{
	Super::BeginPlay();
}

void AItem_Heal::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!OtherActor) return;
	APlayerCharacter* OverlapChar = Cast<APlayerCharacter>(OtherActor);

	if (!OverlapChar) return;
	UHealthComponent* HealthComponent = OverlapChar->FindComponentByClass<UHealthComponent>();

	if (!HealthComponent) return;
	HealthComponent->Heal(HealValue);

	SetLifeSpan(2.f);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	NiagaraPickup->SetVisibility(false);
}
