// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ChibaChabaGameInstance.generated.h"

UCLASS()
class CHIBACHABA_API UChibaChabaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UChibaChabaGameInstance();

	/**
	 * @return False if the game was restarted
	 */
	const bool GetFirstLaunch() const;

	/**
	 * set IsFirstLaunch to true after restart
	 */
	void SetFirstLaunch(bool IsFirstFirstLainchGame);

private:
	bool IsFirstLaunch;
};
