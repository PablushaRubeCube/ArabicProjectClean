// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	bUseControllerRotationYaw = false;

	MaterialChangeRate = 0.1f;
	TotalTimeMaterialChange = 2.0f;
	MontageAttack = nullptr;
	MaterialChangeStartTime = 0.0f;

	StartDie = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GetWorld());
	check(HealthComponent);
	check(WeaponComponent);
	check(GetCapsuleComponent());
	check(GetCharacterMovement());
	check(GetMesh());

	CreateDymMaterial();
}

const UAnimMontage* ABaseCharacter::GetMontageAttack() const
{
	return MontageAttack;
}

void ABaseCharacter::Death()
{
	if (StartDie) return;
	StartDie = true;

	DissolveChar();
	PlayDeathAnimation();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::CreateDymMaterialArray()
{
	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		auto* DymMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
		if (DymMaterial) DymMaterials.Add(DymMaterial);
	}
}

void ABaseCharacter::SetDymMaterialParam()
{
	const FLinearColor& DissolveColor = FLinearColor::Green;
	for (auto& DymMaterial : DymMaterials)
	{
		if (!DymMaterial) break;
		DymMaterial->SetVectorParameterValue("Color", DissolveColor);
		DymMaterial->SetScalarParameterValue("Emissive_Mult", 150.f);
	}
}

void ABaseCharacter::CreateDymMaterial()
{
	CreateDymMaterialArray();
	SetDymMaterialParam();
}

void ABaseCharacter::ChangeDymMaterialApperance()
{
	const float TimePassed = ((GetWorld()->GetTimeSeconds() - MaterialChangeStartTime) / TotalTimeMaterialChange);
	const float Appearance = FMath::Lerp(1.0f, 0.0f, TimePassed);
	for (auto& DymMaterial : DymMaterials)
	{
		if (!DymMaterial) continue;
		DymMaterial->SetScalarParameterValue("Appearance", Appearance);
	}
	if (TimePassed > TotalTimeMaterialChange) GetWorldTimerManager().ClearTimer(MaterialTimer);
}

void ABaseCharacter::DissolveChar()
{
	GetWorldTimerManager().SetTimer(MaterialTimer, this, &ABaseCharacter::ChangeDymMaterialApperance, MaterialChangeRate, true);
	MaterialChangeStartTime = GetWorld()->GetTimeSeconds();
}

void ABaseCharacter::PlayDeathAnimation() const
{
	auto* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && MontagesDeath.Num() > 0)
	{
		const int32 RandomMontageIndex = FMath::RandRange(0, MontagesDeath.Num() - 1);
		if (!MontagesDeath[RandomMontageIndex]) return;
		AnimInstance->Montage_Play(MontagesDeath[RandomMontageIndex]);
	}
}