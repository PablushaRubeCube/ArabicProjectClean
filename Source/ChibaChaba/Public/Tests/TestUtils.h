#pragma once

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Tests/PlayerCharacter_Test.h"
#include "EnhancedInputComponent.h"

class TestUtils
{
public:
	/**
	 * calls the character's movement function
	 * @param waiting for a test character
	 * @param start of function call
	 * @return false while we are calling the function, true when complete.
	 */
	static bool MoveCharacter(APlayerCharacter_Test* Char, const double StartTime)
	{
		const double NewTime = FPlatformTime::Seconds();
		if (NewTime - StartTime <= 5.0f)  // Checking the past tense
		{
			FInputActionValue InputActionValue = FVector2D(1.0f, 0.0f);
			Char->Move(InputActionValue);
			return false;  // We continue calling the function for 5 seconds
		}
		else
		{
			return true;  // Successfully complete the latent command after 5 seconds
		}
	}

private:
};

/**
 * Get the test game world.
 * @return The game world.
 */
static UWorld* GetTestGameWorld()
{
	UWorld* TestWorld = nullptr;
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if (((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game)) && (Context.World() != NULL))
		{
			TestWorld = Context.World();
			break;
		}
	}

	if (!TestWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("Test game world not found."));
	}

	return TestWorld;
}

/**
 * calls the character's movement function
 * @param Character InputComponent
 * @param expected ETriggerEvent of button
 * @param the name of the button we want to call the trigger
 * @return true if ActionName was successfully found
 */
static bool FindAction(const UInputComponent* InputComponent, const ETriggerEvent& FindTriggerEvent, const FString& FindActionName)
{
	const UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(InputComponent);
	for (auto& ActionBinding : InputComp->GetActionEventBindings())
	{
		const ETriggerEvent TriggerEvent = ActionBinding->GetTriggerEvent();
		const FString ActionName = ActionBinding->GetAction()->GetName();

		if (TriggerEvent == FindTriggerEvent && ActionName == FindActionName)
		{
			return true;
		}
	}
	return false;
}

/**
 * Finds an actor of the specified class in the given world.
 * @param World The world context.
 * @param ActorClass The class of the actor to find.
 * @return A pointer to the found actor, or nullptr if not found.
 */
template <typename T>
T* FindActorOfClass(const UWorld* World, TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> AllActorsOfClass;
	UGameplayStatics::GetAllActorsOfClass(World, ActorClass, AllActorsOfClass);
	if (AllActorsOfClass.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No actors of class %s found in the map"), *ActorClass->GetName());
		return nullptr;
	}

	for (auto* Actor : AllActorsOfClass)
	{
		T* TypedActor = Cast<T>(Actor);

		if (TypedActor) return TypedActor;
	}

	UE_LOG(LogTemp, Error, TEXT("Actor of class %s not found"), *ActorClass->GetName());
	return nullptr;
}

#endif