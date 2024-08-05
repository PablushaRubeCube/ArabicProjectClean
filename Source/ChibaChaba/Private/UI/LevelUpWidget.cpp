// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/LevelUpWidget.h"
#include "Components/VerticalBox.h"
#include "UI/WeaponSelectionWidget.h"
#include "ChibaChabaGameModeBase.h"
#include "Weapons/BaseWeapon.h"
#include "ChiChaCoreTypes.h"

void ULevelUpWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(GetWorld());

	auto* GameMode = Cast<AChibaChabaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) GameMode->OnChangeGameState.AddDynamic(this, &ULevelUpWidget::LevelUp);
}

void ULevelUpWidget::LevelUp(EGameStates State)
{
	if (State != EGameStates::EGS_LevelUp) return;

	UpdateWeaponsStat();
}

// TODO checks may not be done every time
void ULevelUpWidget::UpdateWeaponsStat()
{
	if (!GetWorld() || !BoxContain) return;
	if (!GetWorld()->GetAuthGameMode()) return;

	BoxContain->ClearChildren();

	const auto* GameMode = Cast<AChibaChabaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	TArray<TSubclassOf<ABaseWeapon>> Weapons = GameMode->GetArrayWeapons();
	for (auto& Weapon : Weapons)
	{
		const auto* DefautlWeaponObj = Weapon.GetDefaultObject();
		if (!DefautlWeaponObj) continue;

		UWeaponSelectionWidget* RowWidget = CreateWidget<UWeaponSelectionWidget>(GetWorld(), SelectionWidget);
		if (!RowWidget) continue;

		RowWidget->SetIconeWeapon(DefautlWeaponObj->GetWeaponType().WeaponIcone);
		// RowWidget->SetLevelWeapon(DefautlWeaponObj->GetWeaponType().LevelWeapon);
		RowWidget->SetNameWeapon(DefautlWeaponObj->GetWeaponType().NameWeapon);
		RowWidget->SetOverviewWeapon(DefautlWeaponObj->GetWeaponType().Overview);
		RowWidget->SetWeapon(Weapon);

		BoxContain->AddChild(RowWidget);
	}
}
