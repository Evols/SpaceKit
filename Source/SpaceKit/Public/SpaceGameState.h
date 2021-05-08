// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Components/ActorComponent.h"

#include "SpaceGameState.generated.h"

/**
 *
 */
UCLASS()
class SPACEKIT_API USpaceGameStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USpaceGameStateComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};

/**
 * 
 */
UCLASS()
class SPACEKIT_API ASpaceGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	USpaceGameStateComponent* SpaceGameStateComponent;

	ASpaceGameState();
	
};
