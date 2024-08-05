// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "ChiChaCoreTypes.h"
#include "PlayerCharacter.generated.h"

class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class UExperienceComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class CHIBACHABA_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	/**
	 * Ñauses the player character to die
	 */
	virtual void Death() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> AttackSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> ToggleCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> GameCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets | Health", meta = (BlueprintProtected))
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExperienceComponent")
	TObjectPtr<UExperienceComponent> ExperienceComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	virtual void BeginPlay() override;

	/**
	 * Bind input func for PlayerInputComponent
	 * @param expect PlayerInputComponent for bind input func
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Default func form move character
	 * @param expect value for move char
	 */
	void Move(const FInputActionValue& Value);
};
