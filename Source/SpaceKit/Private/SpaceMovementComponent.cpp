// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0


#include "SpaceMovementComponent.h"

#include "SpaceGameState.h"
#include "SpaceTransformComponent.h"
#include "ReactPhysics/Public/reactphysics3d/reactphysics3d.h"


USpaceMovementComponent::USpaceMovementComponent()
{
	bWantsInitializeComponent = true;
	SpaceMass = 500.0_fl;
}

void USpaceMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	auto* PhysicsWorld = USpaceGameStateComponent::GetPhysicsWorld(GetWorld()); if (!PhysicsWorld) return;
	auto* SpaceUpdatedComponent = GetSpaceUpdatedComponent(); if (!SpaceUpdatedComponent) return;

	const auto transform = reactphysics3d::Transform(
		reactphysics3d::Vector3(SpaceUpdatedComponent->Location),
		reactphysics3d::Quaternion(FQuatFloat(SpaceUpdatedComponent->Rotation))
	);

	PhysicsBody = PhysicsWorld->createRigidBody(transform);
	PhysicsBody->setMass(SpaceMass);
}


// Called every frame
void USpaceMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	;
}

reactphysics3d::RigidBody* USpaceMovementComponent::GetPhysicsBody()
{
	return PhysicsBody;
}

USpaceTransformComponent* USpaceMovementComponent::GetSpaceUpdatedComponent()
{
	auto* Owner = GetOwner(); if (Owner == nullptr) return nullptr;
	return Cast<USpaceTransformComponent>(Owner->GetComponentByClass(USpaceTransformComponent::StaticClass()));
}
