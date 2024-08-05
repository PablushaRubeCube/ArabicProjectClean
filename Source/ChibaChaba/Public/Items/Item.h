// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UStaticMeshComponent;

UCLASS()
class CHIBACHABA_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

protected:
	virtual void BeginPlay() override;
};
