// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceTransformComponent.h"
#include "ReactPhysics/Public/reactphysics3d/reactphysics3d.h"

USpaceTransformComponent::USpaceTransformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_EndPhysics;

#if WITH_EDITOR
	bTickInEditor = true;
#endif

}


// Called when the game starts
void USpaceTransformComponent::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	CachedLocation = Location.ToFVector();
	CachedRotation = Rotation.ToFRotator();
#endif
}


// Called every frame
void USpaceTransformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* Actor = GetOwner();
	if (Actor)
	{
#if WITH_EDITOR
		{
			const auto NewLocation = Actor->GetActorLocation();
			if (NewLocation != CachedLocation)
			{
				Location = FVectorFixed(NewLocation);
				CachedLocation = NewLocation;
			}

			const auto NewRotation = Actor->GetActorRotation();
			if (NewRotation != CachedRotation)
			{
				Rotation = FRotatorFloat(NewRotation);
				CachedRotation = NewRotation;
			}
		}
#endif

		Actor->SetActorLocation(Location.ToFVector());
		Actor->SetActorRotation(Rotation.ToFRotator());
	}
}
