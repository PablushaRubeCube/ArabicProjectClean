// Arabic Game. Copyright by Maksim Kremnev. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/Test_Weapons.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"
#include "Engine/Blueprint.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Enemy/EnemyCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(Test_Weapons, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWeaponArrowSpawnLvUpKill, "_ChibaChaba.Weapons.WeaponArrowSpawnLvUpKill",
	EAutomationTestFlags::HighPriority | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWeaponMagicBoltSpawnLvUpKill, "_ChibaChaba.Weapons.WeaponMagicBoltSpawnLvUpKill",
	EAutomationTestFlags::HighPriority | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWeaponFireShieldSpawnLvUpKill, "_ChibaChaba.Weapons.WeaponFireShieldSpawnLvUpKill",
	EAutomationTestFlags::HighPriority | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

namespace
{
/**
 * expected max weapon level is 8. Causes weapon level up 8 times to reach max level
 * @param waiting for a weapon name
 * @param expect weapon component
 * @return Whether the Weapon was successfully LevelUp
 */
bool SpawnMaxLevelWeapon(const FString& WeaponName, UWeaponComponent* WeaponComp)
{

	const FString BPName = WeaponName;
	const UBlueprint* BPWeapon = LoadObject<UBlueprint>(nullptr, *FString(BPName));
	if (!BPWeapon)
	{
		UE_LOG(Test_Weapons, Warning, TEXT("BPWeapon can't loadObject"));
		return false;
	}

	UClass* GeneratedClass = BPWeapon->GeneratedClass;

	// Check if GeneratedClass is a descendant of ABaseWeapon
	if (!GeneratedClass || !GeneratedClass->IsChildOf(ABaseWeapon::StaticClass()))
	{
		UE_LOG(Test_Weapons, Warning, TEXT("GeneratedClass is not derived from ABaseWeapon or is invalid"));
		return false;
	}

	const TSubclassOf<ABaseWeapon> WeaponClass = TSubclassOf<ABaseWeapon>(GeneratedClass);

	if (!WeaponClass)
	{
		UE_LOG(Test_Weapons, Warning, TEXT("WeaponClass is invalid after conversion"));
		return false;
	}

	// make max lv.8 levels of pumping are expected. change if necessary
	for (int32 i = 0; i < 8; i++)
	{
		WeaponComp->AddWeapon(WeaponClass);
	}
	return true;
}
}  // namespace

bool FWeaponArrowSpawnLvUpKill::RunTest(const FString& Parameters)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World don't exist", World)) return false;

	// Get Player Char
	const auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	const auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get Player Char weapon component
	auto* WeaponComp = Char->GetComponentByClass<UWeaponComponent>();
	if (!TestNotNull("WeaponComp not Exists", WeaponComp)) return false;
	if (!TestEqual("Weapon not equal 0", WeaponComp->GetAddedWeapons().Num(), 0)) return false;

	// Get AEnemyCharacter
	TArray<AActor*> InActorsOfClass;
	UGameplayStatics::GetAllActorsOfClass(World, AEnemyCharacter::StaticClass(), InActorsOfClass);
	if (!TestEqual("Don't find any Enemy in begin", InActorsOfClass.Num(), 1)) return false;

	// Spawn and level up weapon from BP
	const FString BPName = "/Script/Engine.Blueprint'/Game/_ChibaChaba/Weapons/Spells/Arrow/BP_Weapon_ArrowRain.BP_Weapon_ArrowRain'";
	TestTrueExpr(SpawnMaxLevelWeapon(BPName, WeaponComp));

	// Check count spawnedWeapon
	if (!TestEqual("Weapon not equal 1", WeaponComp->GetAddedWeapons().Num(), 1)) return false;

	// Check Weapon level equal 8
	for (const auto* Weapon : WeaponComp->GetAddedWeapons())
	{
		if (!TestNotNull("Weapon not Exists", Weapon)) return false;
		if (!TestEqual("Weapon not equal 1", Weapon->GetCurrentLevel(), 8)) return false;
	}

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.f));

	// Kill enemy actor spawned Weapon
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogTemp, Warning, TEXT("World context is not valid"));
				return true;
			}

			CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);

			TArray<AActor*> OutActorsOfClass;
			UGameplayStatics::GetAllActorsOfClass(World, AEnemyCharacter::StaticClass(), OutActorsOfClass);
			TestEqual("Enemy don't die yet", OutActorsOfClass.Num(), 0);
			return true;
		}))

	return true;
}

bool FWeaponMagicBoltSpawnLvUpKill::RunTest(const FString& Parameters)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World don't exist", World)) return false;

	// Get Player Char
	const auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	const auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get Player Char weapon component
	auto* WeaponComp = Char->GetComponentByClass<UWeaponComponent>();
	if (!TestNotNull("WeaponComp not Exists", WeaponComp)) return false;
	if (!TestEqual("Weapon not equal 0", WeaponComp->GetAddedWeapons().Num(), 0)) return false;

	// get enemy char from level
	TArray<AActor*> InActorsOfClass;
	UGameplayStatics::GetAllActorsOfClass(World, AEnemyCharacter::StaticClass(), InActorsOfClass);
	if (!TestEqual("Don't find any Enemy in begin", InActorsOfClass.Num(), 1)) return false;

	// Spawn and level up weapon from BP
	const FString BPName = "/Script/Engine.Blueprint'/Game/_ChibaChaba/Weapons/Spells/MagicBolt/BP_WeaponMagicBolt.BP_WeaponMagicBolt'";
	TestTrueExpr(SpawnMaxLevelWeapon(BPName, WeaponComp));

	// Check count spawnedWeapon
	if (!TestEqual("Weapon not equal 1", WeaponComp->GetAddedWeapons().Num(), 1)) return false;

	// Check Weapon level equal 8
	for (const auto* Weapon : WeaponComp->GetAddedWeapons())
	{
		if (!TestNotNull("Weapon not Exists", Weapon)) return false;
		if (!TestEqual("Weapon not equal 1", Weapon->GetCurrentLevel(), 8)) return false;
	}

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.f));

	// Kill enemy actor spawned Weapon
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogTemp, Warning, TEXT("World context is not valid"));
				return true;
			}
			TArray<AActor*> OutActorsOfClass;
			UGameplayStatics::GetAllActorsOfClass(World, AEnemyCharacter::StaticClass(), OutActorsOfClass);
			TestEqual("Enemy don't die yet", OutActorsOfClass.Num(), 0);
			return true;
		}))

	return true;
}

bool FWeaponFireShieldSpawnLvUpKill::RunTest(const FString& Parameters)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World don't exist", World)) return false;

	// Get Player Char
	const auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	const auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// Get Player Char weapon component
	auto* WeaponComp = Char->GetComponentByClass<UWeaponComponent>();
	if (!TestNotNull("WeaponComp not Exists", WeaponComp)) return false;
	if (!TestEqual("Weapon not equal 0", WeaponComp->GetAddedWeapons().Num(), 0)) return false;

	// get enemy char from level
	TArray<AActor*> InActorsOfClass;
	UGameplayStatics::GetAllActorsOfClass(World, AEnemyCharacter::StaticClass(), InActorsOfClass);
	if (!TestEqual("Enemy not equal 1", InActorsOfClass.Num(), 1)) return false;

	// Spawn and level up weapon from BP
	const FString BPName =
		"/Script/Engine.Blueprint'/Game/_ChibaChaba/Weapons/Spells/Rotation/BP_Weapon_Rotating_Fire_Shild.BP_Weapon_Rotating_Fire_Shild'";
	TestTrueExpr(SpawnMaxLevelWeapon(BPName, WeaponComp));

	// Check count spawnedWeapon
	if (!TestEqual("Weapon not equal 1", WeaponComp->GetAddedWeapons().Num(), 1)) return false;

	// Check Weapon level equal 8
	for (const auto* Weapon : WeaponComp->GetAddedWeapons())
	{
		if (!TestNotNull("Weapon not Exists", Weapon)) return false;
		if (!TestEqual("Weapon not equal 1", Weapon->GetCurrentLevel(), 8)) return false;
	}

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.f));

	// Kill enemy actor spawned Weapon
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogTemp, Warning, TEXT("World context is not valid"));
				return true;
			}
			TArray<AActor*> OutActorsOfClass;
			UGameplayStatics::GetAllActorsOfClass(World, AEnemyCharacter::StaticClass(), OutActorsOfClass);
			TestEqual("Enemy don't die yet", OutActorsOfClass.Num(), 0);
			return true;
		}))

	return true;
}
#endif