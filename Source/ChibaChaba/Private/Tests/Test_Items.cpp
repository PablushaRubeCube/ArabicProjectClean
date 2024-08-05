// Arabic Game. Copyright by Maksim Kremnev. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/Test_Items.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Pickup/Heal/Item_Heal.h"
#include "Components/SphereComponent.h"
#include "Tests/PlayerCharacter_Test.h"
#include "EnhancedInputComponent.h"
#include "Components/HealthComponent.h"
#include "Items/Pickup/Experience/Item_Experience.h"
#include "Components/ExperienceComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogItemTest, All, All)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItem_HealCanHeal, "_ChibaChaba.Items.Item_Heal Can Heal",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItem_ExperienceCanAddExp, "_ChibaChaba.Items.Item_Experience can add Exp",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

bool FItem_HealCanHeal::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not exist", World)) return false;

	// get ItemHeal from level. Expect 1 item
	AItem_Heal* ItemHeal = FindActorOfClass<AItem_Heal>(World, AItem_Heal::StaticClass());
	if (!TestNotNull("ItemHeal not exist", ItemHeal)) return false;

	// get overlap sphere item
	const auto* ItemOverlapComponent = ItemHeal->GetComponentByClass<USphereComponent>();
	if (!TestNotNull("ItemOverlapComponent not exist", ItemOverlapComponent)) return false;

	// check overlap settings
	TestTrueExpr(ItemOverlapComponent->GetUnscaledSphereRadius() >= 30.0f);
	TestTrueExpr(ItemOverlapComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
	TestTrueExpr(ItemOverlapComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn) == ECollisionResponse::ECR_Overlap);
	TestTrueExpr(ItemOverlapComponent->GetGenerateOverlapEvents());

	// get player char
	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// ApplyDamage to player char
	const float Damage = 10.0f;
	UGameplayStatics::ApplyDamage(Char, Damage, nullptr, nullptr, UDamageType::StaticClass());

	// get player char health component
	const auto* HealthComponent = Char->FindComponentByClass<UHealthComponent>();
	if (!TestNotNull("HealthComponent not Exists", HealthComponent)) return false;

	// get player char health after damage
	const float HealthAfterDamage = HealthComponent->GetHealthPercent();

	// move char to take item heal
	const bool WantedAction = FindAction(Char->InputComponent, ETriggerEvent::Triggered, "IA_Move");
	if (WantedAction)
	{
		const double StartTime = FPlatformTime::Seconds();
		ADD_LATENT_AUTOMATION_COMMAND(FUntilCommand([=]() -> bool { return TestUtils::MoveCharacter(Char, StartTime); },
			[=]() -> bool { return false; },  // This callback will not be used because we do not wait for a timeout.
			3.0f));
	}

	// check char health after heal item pickup
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!HealthComponent->IsValidLowLevel())
			{
				UE_LOG(LogItemTest, Warning, TEXT("HealthComponent not valid"));
				return true;
			}
			TestTrueExpr(HealthComponent->GetHealthPercent() > HealthAfterDamage);
			return true;
		}));

	return true;
}

bool FItem_ExperienceCanAddExp::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not exist", World)) return false;

	// get AItem_Experience. 1 item expected
	AItem_Experience* Item_Experience = FindActorOfClass<AItem_Experience>(World, AItem_Experience::StaticClass());
	if (!TestNotNull("Item_Experience not exist", Item_Experience)) return false;

	// get overlap sphere item
	const auto* ItemOverlapComponent = Item_Experience->GetComponentByClass<USphereComponent>();
	if (!TestNotNull("ItemOverlapComponent not exist", ItemOverlapComponent)) return false;

	// check overlap settings
	TestTrueExpr(ItemOverlapComponent->GetUnscaledSphereRadius() >= 30.0f);
	TestTrueExpr(ItemOverlapComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
	TestTrueExpr(ItemOverlapComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn) == ECollisionResponse::ECR_Overlap);
	TestTrueExpr(ItemOverlapComponent->GetGenerateOverlapEvents());

	// get player char
	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("PlayerCharacter not Exists", PlayerCharacter)) return false;
	auto* Char = Cast<APlayerCharacter_Test>(PlayerCharacter);
	if (!TestNotNull("Char not Exists", Char)) return false;

	// get player char experience component
	const auto* ExpComp = Char->GetComponentByClass<UExperienceComponent>();
	if (!TestNotNull("ExpComp not exist", ExpComp)) return false;
	const int32 Exp = ExpComp->GetExperience();

	// move char to pickup exp item
	const bool WantedAction = FindAction(Char->InputComponent, ETriggerEvent::Triggered, "IA_Move");
	if (WantedAction)
	{
		const double StartTime = FPlatformTime::Seconds();
		ADD_LATENT_AUTOMATION_COMMAND(FUntilCommand([=]() -> bool { return TestUtils::MoveCharacter(Char, StartTime); },
			[=]() -> bool { return false; },  // This callback will not be used because we do not wait for a timeout.
			3.0f));
	}

	// check char exp changed
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!ExpComp->IsValidLowLevel())
			{
				UE_LOG(LogItemTest, Warning, TEXT("ExpComp not valid"));
				return true;
			}
			TestTrueExpr(ExpComp->GetExperience() > Exp);
			return true;
		}));

	return true;
}

#endif