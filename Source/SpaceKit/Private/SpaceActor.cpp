// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceActor.h"
#include "SpaceTransformComponent.h"

// Sets default values
ASpaceActor::ASpaceActor()
{
	SpaceTransform = CreateDefaultSubobject<USpaceTransformComponent>(TEXT("SpaceTransform"));
}
