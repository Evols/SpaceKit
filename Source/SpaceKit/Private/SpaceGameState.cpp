// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0.

#include "SpaceGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "SpaceMovementComponent.h"
#include "SpaceKit/Public/SpaceTransformComponent.h"
#include "ReactPhysics/Public/reactphysics3d/reactphysics3d.h"


USpaceGameStateComponent::USpaceGameStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;

	PhysicsCommon = new reactphysics3d::PhysicsCommon();
}

USpaceGameStateComponent::~USpaceGameStateComponent()
{
	delete PhysicsCommon;
}

void USpaceGameStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* World = GetWorld(); if (!World) return;

	for (TActorIterator<AActor> ActorIt(World, AActor::StaticClass()); ActorIt; ++ActorIt)
	{
		auto* Actor = *ActorIt;
		auto* SpaceTransformComponent = Cast<USpaceTransformComponent>(Actor->GetComponentByClass(USpaceTransformComponent::StaticClass()));
		auto* SpaceMovementComponent = Cast<USpaceMovementComponent>(Actor->GetComponentByClass(USpaceMovementComponent::StaticClass()));
		if (SpaceTransformComponent == nullptr || SpaceMovementComponent == nullptr) continue;

		UE_LOG(LogTemp, Log, TEXT("USpaceGameStateComponent::TickComponent: %s"), *Actor->GetName());
	}
}

void USpaceGameStateComponent::BeginPlay()
{
	Super::BeginPlay();

	PhysicsWorld = PhysicsCommon->createPhysicsWorld();
}

void USpaceGameStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

ASpaceGameState::ASpaceGameState()
{
	SpaceGameStateComponent = CreateDefaultSubobject<USpaceGameStateComponent>(TEXT("SpaceGameStateComponent"));
}
