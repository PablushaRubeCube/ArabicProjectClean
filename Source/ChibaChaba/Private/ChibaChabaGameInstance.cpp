// Fill out your copyright notice in the Description page of Project Settings.

#include "ChibaChabaGameInstance.h"

UChibaChabaGameInstance::UChibaChabaGameInstance()
{
	IsFirstLaunch = true;
}

const bool UChibaChabaGameInstance::GetFirstLaunch() const
{
	return IsFirstLaunch;
}

void UChibaChabaGameInstance::SetFirstLaunch(bool IsFirstFirstLainchGame)
{
	IsFirstLaunch = IsFirstFirstLainchGame;
}
