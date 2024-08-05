// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ExperienceComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChiChaCoreTypes.h"
#include "ChibaChabaGameModeBase.h"

APlayerCharacter::APlayerCharacter()
{
	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());
	AttackSphere->SetSphereRadius(150.f);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 0.f;
	SpringArmComponent->bDoCollisionTest = (false);

	ToggleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ToggleCamera"));
	ToggleCamera->SetupAttachment(SpringArmComponent);
	ToggleCamera->SetWorldRotation(FRotator(-60.0f, 0.0f, 0.0f));
	ToggleCamera->SetWorldLocation(FVector(-555.f, 0.f, 960.f));

	GameCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	GameCamera->SetupAttachment(SpringArmComponent);
	GameCamera->SetWorldRotation(FRotator(-40.f, 180.f, 0.f));
	GameCamera->SetWorldLocation(FVector(300.f, 0.f, 170.f));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
	HealthWidgetComponent->SetGenerateOverlapEvents(false);

	ExperienceComponent = CreateDefaultSubobject<UExperienceComponent>(TEXT("ExperienceComponent"));

	MappingContext = nullptr;
	MoveAction = nullptr;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 480.f);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GetWorld());

	check(AttackSphere);
	check(SpringArmComponent);
	check(ToggleCamera);
	check(GameCamera);
	check(HealthWidgetComponent);
	check(ExperienceComponent);
	check(GetCharacterMovement());

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Death()
{
	Super::Death();

	auto* GameMode = Cast<AChibaChabaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) GameMode->GameOver();

	// TODO stop weapon play when char dead
}
