// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Item.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}
