// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceActor.generated.h"


class USpaceTransformComponent;

UCLASS()
class SPACEKIT_API ASpaceActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpaceActor();

public:

	UPROPERTY(Category = "Space", VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "SpaceTransform", AllowPrivateAccess = "true"))
	USpaceTransformComponent* SpaceTransform;
	
};
