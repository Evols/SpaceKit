// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0.

#include "SpaceGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "QuatFloat.h"
#include "SpaceMovementComponent.h"
#include "SpaceKit/Public/SpaceTransformComponent.h"


USpaceGameStateComponent::USpaceGameStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	bWantsInitializeComponent = true;
}

void USpaceGameStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* World = GetWorld(); if (!World) return;

	// Update physics bodies from UE4 components
	for (TActorIterator<AActor> ActorIt(World, AActor::StaticClass()); ActorIt; ++ActorIt)
	{
		auto* Actor = *ActorIt;
		auto* SpaceTransformComponent = Cast<USpaceTransformComponent>(Actor->GetComponentByClass(USpaceTransformComponent::StaticClass()));
		auto* SpaceMovementComponent = Cast<USpaceMovementComponent>(Actor->GetComponentByClass(USpaceMovementComponent::StaticClass()));
		if (SpaceTransformComponent == nullptr || SpaceMovementComponent == nullptr) continue;

		// UE_LOG(LogTemp, Log, TEXT("Before %s -> %s"), *Actor->GetName(), )
	}

	// Update UE4 components from physics bodies
	for (TActorIterator<AActor> ActorIt(World, AActor::StaticClass()); ActorIt; ++ActorIt)
	{
		auto* Actor = *ActorIt;
		auto* SpaceTransformComponent = Cast<USpaceTransformComponent>(Actor->GetComponentByClass(USpaceTransformComponent::StaticClass()));
		auto* SpaceMovementComponent = Cast<USpaceMovementComponent>(Actor->GetComponentByClass(USpaceMovementComponent::StaticClass()));
		if (SpaceTransformComponent == nullptr || SpaceMovementComponent == nullptr) continue;
	}
}

void USpaceGameStateComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

void USpaceGameStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

ASpaceGameState::ASpaceGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	SpaceGameStateComponent = CreateDefaultSubobject<USpaceGameStateComponent>(TEXT("SpaceGameStateComponent"));
}
