// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_AUTOMATION_TESTS

#include "Tests/TestCharacter.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/PlayerCharacter_Test.h"
#include "EnhancedInputComponent.h"
#include "Components/HealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterTest, All, All)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanMove, ("_ChibaChaba.Character.CharacterCanMove"),
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanTakeDamage, ("_ChibaChaba.Character.CharacterCanTakeDamage"),
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanDie, ("_ChibaChaba.Character.CharacterCanDie"),
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeHeal, ("_ChibaChaba.Character.CharacterCanBeHeal"),
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

bool FCharacterCanMove::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get Player Char
	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get Player Char init Location
	const FVector CharLocation = Char->GetActorLocation();

	// Move Player Char
	const bool WantedAction = FindAction(Char->InputComponent, ETriggerEvent::Triggered, "IA_Move");
	if (WantedAction)
	{
		const double StartTime = FPlatformTime::Seconds();
		ADD_LATENT_AUTOMATION_COMMAND(FUntilCommand([=]() -> bool { return TestUtils::MoveCharacter(Char, StartTime); },
			[=]() -> bool { return false; },  // This callback will not be used because we do not wait for a timeout.
			3.0f));
	}

	// Check Player location changed
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!Char->IsValidLowLevel())
			{
				UE_LOG(LogCharacterTest, Warning, TEXT("Char not valid"));
				return true;
			}
			TestNotEqual("CharLocation not change", Char->GetActorLocation(), CharLocation);
			return true;
		}));

	return true;
}

bool FCharacterCanTakeDamage::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get Player Char
	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get Player Char health component
	const auto* HealthComponent = Char->FindComponentByClass<UHealthComponent>();
	if (!TestNotNull("Char health component not Exists", HealthComponent)) return false;

	// Get init Health
	const float StartTestHealth = HealthComponent->GetHealthPercent();

	// Apply Damage
	const float Damage = 10.0f;
	UGameplayStatics::ApplyDamage(Char, Damage, nullptr, nullptr, UDamageType::StaticClass());

	// Get Health after Damage
	const float DamagedTestHealth = HealthComponent->GetHealthPercent();

	// Check Player Health Changed
	TestNotEqual("Damage not Apply", StartTestHealth, DamagedTestHealth);
	return true;
}

bool FCharacterCanDie::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get PlayerChar
	auto* Char = Cast<APlayerCharacter_Test>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get HealthComponent
	const auto* HealthComponent = Char->FindComponentByClass<UHealthComponent>();
	if (!TestNotNull("Char health component not Exists", HealthComponent)) return false;

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

	// Set maximum damage to ensure the Char dies
	const float Damage = (HealthComponent->GetHealthPercent() * 100.0f);
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!Char->IsValidLowLevel())
			{
				UE_LOG(LogCharacterTest, Warning, TEXT("Char not valid"));
				return true;
			}
			UGameplayStatics::ApplyDamage(Char, Damage, nullptr, nullptr, UDamageType::StaticClass());
			return true;
		}));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));

	// Check Char is dead
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!HealthComponent->IsValidLowLevel())
			{
				UE_LOG(LogCharacterTest, Warning, TEXT("HealthComponent not valid"));
				return true;
			}
			if (HealthComponent->GetHealthPercent() > 0.01f)
			{
				UE_LOG(LogCharacterTest, Warning, TEXT("Char health > 0, Health = %f"), HealthComponent->GetHealthPercent());
			}

			return true;
		}));

	return true;
}

bool FCharacterCanBeHeal::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get Player Char
	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get Player Char health component
	auto* HealthComponent = Char->FindComponentByClass<UHealthComponent>();
	if (!TestNotNull("Char health component not Exists", HealthComponent)) return false;

	// Get init Health
	const float HealthPercent = HealthComponent->GetHealthPercent();

	// Apply Damage
	const float Damage = 11.0f;
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!Char->IsValidLowLevel())
			{
				UE_LOG(LogCharacterTest, Warning, TEXT("Char not valid"));
				return true;
			}
			UGameplayStatics::ApplyDamage(Char, Damage, nullptr, nullptr, UDamageType::StaticClass());
			return true;
		}));

	// Check Health changed after Damage
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!HealthComponent->IsValidLowLevel())
			{
				UE_LOG(LogCharacterTest, Warning, TEXT("HealthComponent not valid"));
				return true;
			}
			TestNotEqual("HealNotChange", HealthComponent->GetHealthPercent(), (HealthPercent - Damage));
			return true;
		}));

	// Heal Char
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!HealthComponent->IsValidLowLevel())
			{
				UE_LOG(LogCharacterTest, Warning, TEXT("HealthComponent not valid"));
				return true;
			}
			HealthComponent->Heal(HealthComponent->GetMaxHealth());
			return true;
		}));

	// Check Player Health after Heal
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!FMath::IsNearlyEqual(HealthComponent->GetHealthPercent(), 1.0f))
			{
				UE_LOG(LogCharacterTest, Error, TEXT("Health is not full"));
			}
			return true;
		}));

	return true;
}

#endif