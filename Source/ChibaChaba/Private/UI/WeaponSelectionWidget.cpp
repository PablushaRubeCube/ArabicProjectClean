// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WeaponSelectionWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Weapons/BaseWeapon.h"
#include "Characters/PlayerCharacter.h"
#include "Components/WeaponComponent.h"
#include "ChibaChabaGameModeBase.h"

void UWeaponSelectionWidget::SetIconeWeapon(UTexture2D* Icone)
{
	if (Icone && IconeWeapon) IconeWeapon->SetBrushFromTexture(Icone);
}

void UWeaponSelectionWidget::SetLevelWeapon(const FText& Level)
{
	if (LevelWeapon) LevelWeapon->SetText(Level);
}

void UWeaponSelectionWidget::SetNameWeapon(const FText& Name)
{
	if (NameWeapon) NameWeapon->SetText(Name);
}

void UWeaponSelectionWidget::SetOverviewWeapon(const FText& Overview)
{
	if (OverviewWeapon) OverviewWeapon->SetText(Overview);
}

void UWeaponSelectionWidget::SetWeapon(TSubclassOf<ABaseWeapon> ChooseWeapon)
{
	Weapon = ChooseWeapon;
}

TSubclassOf<ABaseWeapon> UWeaponSelectionWidget::GetWeapon() const
{
	return Weapon;
}

void UWeaponSelectionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(GetWorld());

	ChooseButton->OnClicked.AddDynamic(this, &UWeaponSelectionWidget::ChooseWeapon);
}

void UWeaponSelectionWidget::ChooseWeapon()
{
	auto* Char = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (!Char) return;

	auto* WeaponComp = Char->GetComponentByClass<UWeaponComponent>();
	if (!WeaponComp) return;
	WeaponComp->AddWeapon(GetWeapon());

	auto* GameMode = Cast<AChibaChabaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) GameMode->ClearPause();
}
