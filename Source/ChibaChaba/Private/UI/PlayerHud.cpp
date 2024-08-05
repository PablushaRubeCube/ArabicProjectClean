// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PlayerHud.h"
#include "Blueprint/UserWidget.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/LevelUpWidget.h"
#include "ChiChaCoreTypes.h"
#include "ChibaChabaGameModeBase.h"

void APlayerHud::BeginPlay()
{
	Super::BeginPlay();
	check(GetWorld());

	if (GetOwningPawn() && LevelUpWidget && PlayerHUD && StartGameWidget && GameOverWidget && CreditsWidget)
	{
		Widgets.Add(EGameStates::EGS_WaitToStart, CreateWidget<UUserWidget>(GetWorld(), StartGameWidget));
		Widgets.Add(EGameStates::EGS_InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUD));
		Widgets.Add(EGameStates::EGS_LevelUp, CreateWidget<UUserWidget>(GetWorld(), LevelUpWidget));
		Widgets.Add(EGameStates::EGS_GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidget));
		Widgets.Add(EGameStates::EGS_Credits, CreateWidget<UUserWidget>(GetWorld(), CreditsWidget));
	}

	for (auto& Widget : Widgets)
	{
		auto& WidgetValue = Widget.Value;
		if (!WidgetValue) continue;

		WidgetValue->AddToViewport();
		WidgetValue->SetVisibility(ESlateVisibility::Hidden);
	}

	auto* GameMode = Cast<AChibaChabaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) GameMode->OnChangeGameState.AddDynamic(this, &APlayerHud::GameStateChanged);
}

void APlayerHud::GameStateChanged(EGameStates State)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Widgets.Contains(State))
	{
		CurrentWidget = Widgets[State];
	}

	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
