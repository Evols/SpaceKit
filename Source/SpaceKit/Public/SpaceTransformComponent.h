// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "SpaceKitPrecision/Public/VectorFixed.h"
#include "SpaceKitPrecision/Public/RotatorFloat.h"

#include "SpaceTransformComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEKIT_API USpaceTransformComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="SpaceTransform")
	FVectorFixed Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="SpaceTransform")
	FRotatorFloat Rotation;

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
