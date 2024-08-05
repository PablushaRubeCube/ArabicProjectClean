#include "Items/Pickup/Experience/Item_Experience.h"
#include "Components/SphereComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Components/ExperienceComponent.h"

AItem_Experience::AItem_Experience()
{
	Experience = 10.f;
}

float AItem_Experience::GetExperience() const
{
	return Experience;
}

void AItem_Experience::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!OtherActor) return;
	PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (!PlayerChar) return;
}

void AItem_Experience::FinishAction()
{
	Super::FinishAction();

	if (PlayerChar)
	{
		auto* ExpComp = PlayerChar->GetComponentByClass<UExperienceComponent>();
		if (ExpComp) ExpComp->AddExperience(GetExperience());
	}

	Destroy();
}
