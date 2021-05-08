// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0


#include "SpaceMovementComponent.h"


#include "QuatFloat.h"
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
	auto* PhysicsCommon = USpaceGameStateComponent::GetPhysicsCommon(GetWorld()); if (!PhysicsWorld) return;
	auto* SpaceUpdatedComponent = GetSpaceUpdatedComponent(); if (!SpaceUpdatedComponent) return;

	const auto transform = reactphysics3d::Transform(
		reactphysics3d::Vector3(SpaceUpdatedComponent->Location),
		reactphysics3d::Quaternion(FQuatFloat(SpaceUpdatedComponent->Rotation))
	);

	// Spawn rigid body
	PhysicsBody = PhysicsWorld->createRigidBody(transform);

	// Set mass
	PhysicsBody->setMass(SpaceMass);

	// Set collider
	const auto collisionTransform = reactphysics3d::Transform(
		reactphysics3d::Vector3(0.0_fl, 0.0_fl, 0.0_fl),
		reactphysics3d::Quaternion(FQuatFloat::Identity)
	);

	const auto* Owner = GetOwner();
	const auto Box = Owner->GetComponentsBoundingBox();
	auto* BoxShape = PhysicsCommon->createBoxShape(/*FVectorFloat(Box.GetExtent())*/FVectorFloat(100.0_fl, 100.0_fl, 100.0_fl));
	UE_LOG(LogTemp, Log, TEXT("USpaceMovementComponent::InitializeComponent() %s -> %s"), *Owner->GetName(), UTF8_TO_TCHAR(BoxShape->to_string().c_str()), UTF8_TO_TCHAR(BoxShape->to_string().c_str()));
	PhysicsBody->addCollider(BoxShape, collisionTransform);
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
