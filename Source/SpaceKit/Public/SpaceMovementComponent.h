// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"

#include "SpaceKitPrecision/Public/RotatorFloat.h"

#include "SpaceMovementComponent.generated.h"


namespace reactphysics3d
{
	class RigidBody;
}

class USpaceTransformComponent;

/**
 * Movement component for SpaceKit actors
 */
UCLASS()
class SPACEKIT_API USpaceMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:

	USpaceMovementComponent();

	virtual void InitializeComponent() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, Category="SpaceKit")
	FVectorFloat SpaceVelocity;

	reactphysics3d::RigidBody* GetPhysicsBody();
	
protected:

	UFUNCTION()
	USpaceTransformComponent* GetSpaceUpdatedComponent();

private:

	reactphysics3d::RigidBody* PhysicsBody;
	
};
