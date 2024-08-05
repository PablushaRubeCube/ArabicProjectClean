// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "PhysicalWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class CHIBACHABA_API APhysicalWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	APhysicalWeapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision)
	TObjectPtr<UBoxComponent> WeaponCollision;

	virtual void BeginPlay() override;
};
