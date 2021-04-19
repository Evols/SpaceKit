// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0


#include "SpaceMovementComponent.h"


#include "SpaceTransformComponent.h"
#include "ReactPhysics/Public/reactphysics3d/reactphysics3d.h"

USpaceMovementComponent::USpaceMovementComponent()
{
}


// Called every frame
void USpaceMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

USpaceTransformComponent* USpaceMovementComponent::GetSpaceUpdatedComponent()
{
	auto* Owner = GetOwner(); if (Owner == nullptr) return nullptr;
	return Cast<USpaceTransformComponent>(Owner->GetComponentByClass(USpaceTransformComponent::StaticClass()));
}
