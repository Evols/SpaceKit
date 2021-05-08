// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0


#include "SpaceMovementComponent.h"


#include "QuatFloat.h"
#include "SpaceGameState.h"
#include "SpaceTransformComponent.h"


USpaceMovementComponent::USpaceMovementComponent()
{
	bWantsInitializeComponent = true;
	SpaceMass = 500.0_fl;
}

void USpaceMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	auto* SpaceUpdatedComponent = GetSpaceUpdatedComponent(); if (!SpaceUpdatedComponent) return;
}


// Called every frame
void USpaceMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	;
}

USpaceTransformComponent* USpaceMovementComponent::GetSpaceUpdatedComponent()
{
	auto* Owner = GetOwner(); if (Owner == nullptr) return nullptr;
	return Cast<USpaceTransformComponent>(Owner->GetComponentByClass(USpaceTransformComponent::StaticClass()));
}
