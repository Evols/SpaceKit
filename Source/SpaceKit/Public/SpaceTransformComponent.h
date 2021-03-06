// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SpaceKitPrecision/Public/RotatorFloat.h"

#include "SpaceTransformComponent.generated.h"


/**
 * This enables actor that possess it will have its location and rotation in FVectorFloat (precise) instead of FVector (single precision).
 * Note that this shouldn't be spawned in a BlueprintClass, because for some reason, you can't edit the position if you do that.
 */
UCLASS()
class SPACEKIT_API USpaceTransformComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="SpaceTransform")
	FVectorFloat Location;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="SpaceTransform")
	FRotatorFloat Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="SpaceTransform")
	FRealFloat Mass;

public:

	USpaceTransformComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
private:

#if WITH_EDITOR
	FVector CachedLocation;
	FRotator CachedRotation;
#endif

};
