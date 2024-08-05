#if WITH_AUTOMATION_TESTS

#include "Tests/Test_UMG.h"
#include "Tests/PlayerCharacter_Test.h"
#include "EnhancedInputComponent.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/LevelUpWidget.h"
#include "UI/WeaponSelectionWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/WeaponComponent.h"
#include "Components/ExperienceComponent.h"
#include "Components/HealthComponent.h"
#include "UI/LevelUpWidget.h"
#include "UI/StartGameWidget.h"
#include "UI/GameOverWidget.h"
#include "UI/CreditsUserWidget.h"
#include "UI/PlayerHUDWidget.h"
#include "ChibaChabaGameModeBase.h"
#include "Components/PanelWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogWidgetTest, All, All)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWeaponSelectionWidgetCanChooseWeapon, "_ChibaChaba.UMG.WeaponSelectionWidgetCanChooseWeapon",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLevelUpWidgetCanCall, "_ChibaChaba.UMG.LevelUpWidgetCanCall",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStartGameWidgetVisibleOnGame, "_ChibaChaba.UMG.StartGameWidgetVisibleOnGame",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameOverWidgetCanCallOnGame, "_ChibaChaba.UMG.GameOverWidgetCanCallOnGame",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameOverWidgetCanOpenCredits, "_ChibaChaba.UMG.GameOverWidgetCanOpenCredits",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameOverWidgetCanRestartGame, "_ChibaChaba.UMG.GameOverWidgetCanRestartGame",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreditsWidgetCanCloseOnGame, "_ChibaChaba.UMG.CreditsWidgetCanCloseOnGame",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPlayerHUDWidgetsVisibleOnGame, "_ChibaChaba.UMG.PlayerHUDWidgetsVisibleOnGame",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

namespace
{
/**
 * Clicked the specified button in the given widget.
 * @param World The world context.
 * @param NameButton The name of the button to click.
 * @return Whether the button was successfully clicked
 */
bool ClickWidgetButton(const UUserWidget* Widget, const FName& NameButton)
{
	if (!Widget || !Widget->WidgetTree) return false;

	const FName Name = NameButton;
	const UWidget* FoundWidget = Widget->WidgetTree->FindWidget(Name);

	if (!FoundWidget) return false;

	const auto* Button = Cast<UButton>(FoundWidget);
	if (!Button) return false;

	Button->OnClicked.Broadcast();
	return true;
}
}  // namespace

bool FWeaponSelectionWidgetCanChooseWeapon::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get player char
	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get Player Char Weapon Component
	auto* WeaponComponent = Char->FindComponentByClass<UWeaponComponent>();
	if (!TestNotNull("WeaponComponent not Exists", WeaponComponent)) return false;
	// Set init count Player Char weapon;
	const int32 WeaponCountBefore = WeaponComponent->GetAddedWeapons().Num();

	// Get player char exp component
	auto* ExperienceComponent = Char->FindComponentByClass<UExperienceComponent>();
	if (!TestNotNull("ExperienceComponent not Exists", ExperienceComponent)) return false;

	// add exp to Char
	ExperienceComponent->AddExperience(100);

	// Get Weapon Selection Widget
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UWeaponSelectionWidget::StaticClass(), false);
	TestTrueExpr(Widgets.Num() > 0);

	// Choose in Weapon in the Weapon Selection Widget
	for (const auto* widget : Widgets)
	{
		const FName Name = "ChooseButton";
		TestTrueExpr(ClickWidgetButton(widget, Name));
	}

	// Check init count Player Char weapon is Changed;
	const int32 WeaponCountAfter = WeaponComponent->GetAddedWeapons().Num();
	TestTrueExpr(WeaponCountBefore < WeaponCountAfter);

	return true;
}

bool FLevelUpWidgetCanCall::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get player char
	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get ULevelUpWidget
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, ULevelUpWidget::StaticClass(), false);
	TestTrueExpr(Widgets.Num() > 0);

	// check visibility levelUpWidget
	for (const auto* widget : Widgets)
	{
		if (!TestNotNull("ULevelUpWidget not valid", widget)) return false;
		TestTrueExpr(widget->GetVisibility() == ESlateVisibility::Hidden);
	}
	// Get Player Char Exp Component
	auto* ExperienceComponent = Char->FindComponentByClass<UExperienceComponent>();
	if (!TestNotNull("ExperienceComponent not Exists", ExperienceComponent)) return false;

	// Add Exp to player char
	ExperienceComponent->AddExperience(100);

	// Check LevelUpWidget Visibility = Visible
	for (const auto* widget : Widgets)
	{
		if (!TestNotNull("ULevelUpWidget not valid", widget)) return false;
		const auto* LevelUpWidget = Cast<ULevelUpWidget>(widget);
		if (!TestNotNull("LevelUpWidget not Exists", LevelUpWidget)) return false;
		TestTrueExpr(LevelUpWidget->GetVisibility() == ESlateVisibility::Visible);
	}

	return true;
}

bool FStartGameWidgetVisibleOnGame::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get UStartGameWidget
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UStartGameWidget::StaticClass(), false);
	TestTrueExpr(Widgets.Num() > 0);

	// Check UStartGameWidget Visibility = Visible
	for (const auto* widget : Widgets)
	{
		if (!TestNotNull("UStartGameWidget not valid", widget)) return false;
		const auto* StartGameWidget = Cast<UStartGameWidget>(widget);
		if (!TestNotNull("StartGameWidget not Exists", StartGameWidget)) return false;
		TestTrueExpr(StartGameWidget->GetVisibility() == ESlateVisibility::Visible);
	}

	return true;
}

bool FGameOverWidgetCanCallOnGame::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get player char
	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get Player Char Health Component
	auto* HealthComponent = Char->FindComponentByClass<UHealthComponent>();
	if (!TestNotNull("Char health component not Exists", HealthComponent)) return false;

	// Set maximum damage to ensure the Char dies
	const float Damage = (HealthComponent->GetHealthPercent() * 100.0f);

	// Apply Damage Char
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!Char->IsValidLowLevel())
			{
				UE_LOG(LogWidgetTest, Warning, TEXT("Char not valid"));
				return true;
			}
			UGameplayStatics::ApplyDamage(Char, Damage, nullptr, nullptr, UDamageType::StaticClass());
			return true;
		}));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

	// Check GameOverWidget is visble after death Char
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogWidgetTest, Warning, TEXT("World not valid"));
				return true;
			}
			TArray<UUserWidget*> Widgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UGameOverWidget::StaticClass(), false);

			for (const auto* widget : Widgets)
			{
				if (!TestNotNull("UGameOverWidget not valid", widget)) continue;
				const auto* GameOverWidget = Cast<UGameOverWidget>(widget);
				if (!TestNotNull("GameOverWidget not Exists", GameOverWidget)) continue;
				TestTrueExpr(GameOverWidget->GetVisibility() == ESlateVisibility::Visible);
			}
			return true;
		}));

	return true;
}

bool FGameOverWidgetCanOpenCredits::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get Auth GameMode
	auto* AuthGameMode = World->GetAuthGameMode();
	if (!TestNotNull("AuthGameMode don't exist", AuthGameMode)) return false;

	// Get specific GameMode
	auto* GameMode = Cast<AChibaChabaGameModeBase>(AuthGameMode);
	if (!TestNotNull("GameMode not Exists", GameMode)) return false;

	// Call GameOver state
	GameMode->GameOver();

	// Open gameover widget and choose credits widget
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogWidgetTest, Warning, TEXT("World not valid"));
				return true;
			}
			TArray<UUserWidget*> Widgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UGameOverWidget::StaticClass(), false);
			TestTrueExpr(Widgets.Num() > 0);

			for (const auto* widget : Widgets)
			{
				const FName Name = "Credits";
				TestTrueExpr(ClickWidgetButton(widget, Name));
			}
			return true;
		}));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.1f));
	// credits widget is open
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogWidgetTest, Warning, TEXT("World not valid"));
				return true;
			}
			TArray<UUserWidget*> Widgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UCreditsUserWidget::StaticClass(), false);
			TestTrueExpr(Widgets.Num() > 0);

			for (const auto* widget : Widgets)
			{
				if (!TestNotNull("UCreditsUserWidget not valid", widget)) continue;
				if (TestEqual("CreditsUserWidget Visibility not == Visible", widget->GetVisibility(), ESlateVisibility::Visible)) continue;
			}
			return true;
		}));
	return true;
}

bool FGameOverWidgetCanRestartGame::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get Auth GameMode
	auto* AuthGameMode = World->GetAuthGameMode();
	if (!TestNotNull("AuthGameMode don't exist", AuthGameMode)) return false;

	// Get specific GameMode
	auto* GameMode = Cast<AChibaChabaGameModeBase>(AuthGameMode);
	if (!TestNotNull("GameMode not Exists", GameMode)) return false;

	// Call GameOver State
	GameMode->GameOver();

	// get UGameOverWidget and press Restart Button
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogWidgetTest, Warning, TEXT("World not valid"));
				return true;
			}
			TArray<UUserWidget*> Widgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UGameOverWidget::StaticClass(), false);
			TestTrueExpr(Widgets.Num() > 0);

			for (const auto* widget : Widgets)
			{
				const FName Name = "Restart";
				TestTrueExpr(ClickWidgetButton(widget, Name));
			}
			return true;
		}));

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));

	// get UPlayerHUDWidget and check Visibility = Visible
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			auto* World = GetTestGameWorld();
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogWidgetTest, Warning, TEXT("World not valid"));
				return true;
			}
			if (TestNotNull("World not Exists", World))
			{
				TArray<UUserWidget*> Widgets;
				UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UPlayerHUDWidget::StaticClass(), false);
				TestTrueExpr(Widgets.Num() > 0);

				for (const auto* widget : Widgets)
				{
					if (!TestNotNull("PlayerHUDWidget not valid", widget)) continue;
					const auto* PlayerHUDWidget = Cast<UPlayerHUDWidget>(widget);
					if (!TestNotNull("PlayerHUDWidget not Exists", PlayerHUDWidget)) continue;
					TestTrueExpr(PlayerHUDWidget->GetVisibility() == ESlateVisibility::Visible);
				}
			}
			return true;
		}));
	return true;
}

bool FCreditsWidgetCanCloseOnGame::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get Auth GameMode
	auto* AuthGameMode = World->GetAuthGameMode();
	if (!TestNotNull("AuthGameMode don't exist", AuthGameMode)) return false;

	// Get specific GameMode
	auto* GameMode = Cast<AChibaChabaGameModeBase>(AuthGameMode);
	if (!TestNotNull("GameMode not Exists", GameMode)) return false;

	// Call OpenCreits State
	GameMode->OpenCredits();

	// get UCreditsUserWidget
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UCreditsUserWidget::StaticClass(), false);
	TestTrueExpr(Widgets.Num() > 0);

	// check  UCreditsUserWidget Visibility = Visible
	for (const auto* widget : Widgets)
	{
		if (!TestNotNull("UCreditsUserWidget not valid", widget)) return false;
		if (!TestEqual("CreditsUserWidget Visibility not == Visible", widget->GetVisibility(), ESlateVisibility::Visible)) return false;
	}

	// close credits widget
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogWidgetTest, Warning, TEXT("World not valid"));
				return true;
			}
			TArray<UUserWidget*> Widgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UCreditsUserWidget::StaticClass(), false);
			TestTrueExpr(Widgets.Num() > 0);

			for (const auto* widget : Widgets)
			{
				const FName Name = "Back";
				TestTrueExpr(ClickWidgetButton(widget, Name));
			}
			return true;
		}));

	// get UGameOverWidget and check Visibility = Visible
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogWidgetTest, Warning, TEXT("World not valid"));
				return true;
			}
			TArray<UUserWidget*> Widgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UGameOverWidget::StaticClass(), false);
			TestTrueExpr(Widgets.Num() > 0);

			for (const auto* widget : Widgets)
			{
				if (!TestNotNull("GameOverWidget not valid", widget)) continue;
				if (!TestEqual("GameOverWidget Visibility not == Visible", widget->GetVisibility(), ESlateVisibility::Visible)) continue;
			}
			return true;
		}));
	return true;
}

bool FPlayerHUDWidgetsVisibleOnGame::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get Auth GameMode
	auto* AuthGameMode = World->GetAuthGameMode();
	if (!TestNotNull("AuthGameMode don't exist", AuthGameMode)) return false;

	// Get specific GameMode
	auto* GameMode = Cast<AChibaChabaGameModeBase>(AuthGameMode);
	if (!TestNotNull("GameMode not Exists", GameMode)) return false;

	// Call StartGame State
	GameMode->StartGame();

	// Get ChildWidgets of UPlayerHUDWidget and check visibility = SelfHitTestInvisible
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UPlayerHUDWidget::StaticClass(), false);
	TestTrueExpr(Widgets.Num() > 0);

	for (const auto* widget : Widgets)
	{
		if (!TestNotNull("PlayerHUDWidget not valid", widget)) return false;

		TArray<UWidget*> ChildWidgets;
		widget->WidgetTree->GetAllWidgets(ChildWidgets);
		TestTrueExpr(ChildWidgets.Num() > 0);

		for (const auto* WidgetChildren : ChildWidgets)
		{
			if (!TestNotNull("WidgetChildren not Exists", WidgetChildren)) return false;
			TestTrueExpr(WidgetChildren->GetVisibility() == ESlateVisibility::SelfHitTestInvisible);
		}
	}
	return true;
}
#endif