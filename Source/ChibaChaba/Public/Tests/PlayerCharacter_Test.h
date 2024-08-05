// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacter.h"
#include "PlayerCharacter_Test.generated.h"

/**
 *
 */
UCLASS()
class CHIBACHABA_API APlayerCharacter_Test : public APlayerCharacter
{
	GENERATED_BODY()

	friend class TestUtils;
};
