// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0.

#include "SpaceGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "QuatFloat.h"
#include "SpaceMovementComponent.h"
#include "SpaceKit/Public/SpaceTransformComponent.h"
#include "ReactPhysics/Public/reactphysics3d/reactphysics3d.h"


USpaceGameStateComponent::USpaceGameStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	bWantsInitializeComponent = true;

	PhysicsCommon = new reactphysics3d::PhysicsCommon();
	PhysicsWorld = nullptr;
}

USpaceGameStateComponent::~USpaceGameStateComponent()
{
	delete PhysicsCommon;
}

void USpaceGameStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsWorld) return;
	auto* World = GetWorld(); if (!World) return;

	// Update physics bodies from UE4 components
	for (TActorIterator<AActor> ActorIt(World, AActor::StaticClass()); ActorIt; ++ActorIt)
	{
		auto* Actor = *ActorIt;
		auto* SpaceTransformComponent = Cast<USpaceTransformComponent>(Actor->GetComponentByClass(USpaceTransformComponent::StaticClass()));
		auto* SpaceMovementComponent = Cast<USpaceMovementComponent>(Actor->GetComponentByClass(USpaceMovementComponent::StaticClass()));
		if (SpaceTransformComponent == nullptr || SpaceMovementComponent == nullptr) continue;

		SpaceMovementComponent->GetPhysicsBody()->setTransform(reactphysics3d::Transform(
			reactphysics3d::Vector3(SpaceTransformComponent->Location),
			reactphysics3d::Quaternion(FQuatFloat(SpaceTransformComponent->Rotation))
		));
		SpaceMovementComponent->GetPhysicsBody()->setLinearVelocity(SpaceMovementComponent->SpaceVelocity);
		SpaceMovementComponent->GetPhysicsBody()->setAngularVelocity(SpaceMovementComponent->SpaceAngularVelocity);
		SpaceMovementComponent->GetPhysicsBody()->setMass(SpaceMovementComponent->SpaceMass);

		// UE_LOG(LogTemp, Log, TEXT("Before %s -> %s"), *Actor->GetName(), )
	}

	PhysicsWorld->update(FRealFloat(DeltaTime));

	// Update UE4 components from physics bodies
	for (TActorIterator<AActor> ActorIt(World, AActor::StaticClass()); ActorIt; ++ActorIt)
	{
		auto* Actor = *ActorIt;
		auto* SpaceTransformComponent = Cast<USpaceTransformComponent>(Actor->GetComponentByClass(USpaceTransformComponent::StaticClass()));
		auto* SpaceMovementComponent = Cast<USpaceMovementComponent>(Actor->GetComponentByClass(USpaceMovementComponent::StaticClass()));
		if (SpaceTransformComponent == nullptr || SpaceMovementComponent == nullptr) continue;

		const auto& transform = SpaceMovementComponent->GetPhysicsBody()->getTransform();
		SpaceTransformComponent->Location = transform.getPosition().toVectorFloat();
		SpaceTransformComponent->Rotation = FRotatorFloat(transform.getOrientation().toQuatFloat());

		SpaceMovementComponent->SpaceVelocity = SpaceMovementComponent->GetPhysicsBody()->getLinearVelocity().toVectorFloat();
		SpaceMovementComponent->SpaceAngularVelocity = SpaceMovementComponent->GetPhysicsBody()->getAngularVelocity().toVectorFloat();
	}
}

void USpaceGameStateComponent::InitializeComponent()
{
	Super::InitializeComponent();

	PhysicsWorld = PhysicsCommon->createPhysicsWorld();
}

void USpaceGameStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

reactphysics3d::PhysicsWorld* USpaceGameStateComponent::GetPhysicsWorld(UWorld* World)
{
	if (!World) return nullptr;
	auto* GameState = World->GetGameState(); if (!GameState) return nullptr;
	auto* SpaceGameStateComponent = Cast<USpaceGameStateComponent>(GameState->GetComponentByClass(USpaceGameStateComponent::StaticClass())); if (!SpaceGameStateComponent) return nullptr;
	return SpaceGameStateComponent->PhysicsWorld;
}

reactphysics3d::PhysicsCommon* USpaceGameStateComponent::GetPhysicsCommon(UWorld* World)
{
	if (!World) return nullptr;
	auto* GameState = World->GetGameState(); if (!GameState) return nullptr;
	auto* SpaceGameStateComponent = Cast<USpaceGameStateComponent>(GameState->GetComponentByClass(USpaceGameStateComponent::StaticClass())); if (!SpaceGameStateComponent) return nullptr;
	return SpaceGameStateComponent->PhysicsCommon;
}

ASpaceGameState::ASpaceGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	SpaceGameStateComponent = CreateDefaultSubobject<USpaceGameStateComponent>(TEXT("SpaceGameStateComponent"));
}
