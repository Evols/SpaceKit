// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0.

#include "SpaceGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "SpaceKit/Public/SpaceTransformComponent.h"


USpaceGameStateComponent::USpaceGameStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
}

void USpaceGameStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* World = GetWorld(); if (!World) return;

	for (TActorIterator<AActor> ActorIt(World, AActor::StaticClass()); ActorIt; ++ActorIt)
	{
		auto* Actor = *ActorIt;
		auto* SpaceTransformComponent = Cast<USpaceTransformComponent>(Actor->GetComponentByClass(USpaceTransformComponent::StaticClass()));
		if (!SpaceTransformComponent) continue;

		UE_LOG(LogTemp, Log, TEXT("USpaceGameStateComponent::TickComponent: %s"), *Actor->GetName());
	}
}
