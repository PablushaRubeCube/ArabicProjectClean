// Arabic Game. Copyright by Maksim Kremnev. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/Test_Enemy.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyCharacter.h"
#include "Components/HealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemyTest, All, All)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnemyCanMove, ("_ChibaChaba.Enemy.EnemyCanMove"),
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnemyCanTakeDamage, ("_ChibaChaba.Enemy.EnemyCanTakeDamage"),
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnemyrCanDie, ("_ChibaChaba.Enemy.EnemyCanDie"),
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

bool FEnemyCanMove::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Find an enemy character
	AEnemyCharacter* Enemy = FindActorOfClass<AEnemyCharacter>(World, AEnemyCharacter::StaticClass());
	if (!TestNotNull("Enemy not Exists", Enemy)) return false;

	// Get initial enemy location
	const FVector EnemyLocation = Enemy->GetActorLocation();

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

	// check enemy location. The enemy location should have changed
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!Enemy->IsValidLowLevel())
			{
				UE_LOG(LogEnemyTest, Warning, TEXT("Enemy not valid"));
				return true;
			}
			TestNotEqual("EnemyLocation not change", Enemy->GetActorLocation(), EnemyLocation);
			return true;
		}));

	return true;
}

bool FEnemyCanTakeDamage::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Find an enemy character
	AEnemyCharacter* Enemy = FindActorOfClass<AEnemyCharacter>(World, AEnemyCharacter::StaticClass());
	if (!TestNotNull("Enemy not Exists", Enemy)) return false;

	// Get enemy health component
	const auto* HealthComponent = Enemy->FindComponentByClass<UHealthComponent>();
	if (!TestNotNull("Enemy health component not Exists", HealthComponent)) return false;

	// get enemy health
	const float StartTestHealth = HealthComponent->GetHealthPercent();

	// damage enemy
	const float Damage = 10.0f;
	UGameplayStatics::ApplyDamage(Enemy, Damage, nullptr, nullptr, UDamageType::StaticClass());

	// get new current enemy health
	const float DamagedTestHealth = HealthComponent->GetHealthPercent();

	// check change health
	TestNotEqual("Damage not Apply", StartTestHealth, DamagedTestHealth);
	return true;
}

bool FEnemyrCanDie::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Find an enemy character
	AEnemyCharacter* Enemy = FindActorOfClass<AEnemyCharacter>(World, AEnemyCharacter::StaticClass());
	if (!TestNotNull("Enemy not Exists", Enemy)) return false;

	// get enemy health component
	const auto* HealthComponent = Enemy->FindComponentByClass<UHealthComponent>();
	if (!TestNotNull("Enemy health component not Exists", HealthComponent)) return false;

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

	// Set maximum damage to ensure the enemy dies
	const float Damage = (HealthComponent->GetHealthPercent() * 100.0f);

	// Apply damage
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!Enemy->IsValidLowLevel())
			{
				UE_LOG(LogEnemyTest, Warning, TEXT("Enemy not valid"));
				return true;
			}
			UGameplayStatics::ApplyDamage(Enemy, Damage, nullptr, nullptr, UDamageType::StaticClass());
			return true;
		}));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));

	// Check if enemy health is less than or equal to 0
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!HealthComponent->IsValidLowLevel())
			{
				UE_LOG(LogEnemyTest, Warning, TEXT("HealthComponent not valid"));
				return true;
			}
			if (HealthComponent->GetHealthPercent() > 0.01f)
			{
				UE_LOG(LogEnemyTest, Warning, TEXT("Enemy health > 0, Health = %f"), HealthComponent->GetHealthPercent());
			}

			return true;
		}));

	return true;
}

#endif