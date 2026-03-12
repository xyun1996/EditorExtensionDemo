// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCustomActor.h"

AMyCustomActor::AMyCustomActor()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize default values
	DisplayLabel = TEXT("MyCustomActor");
	bIsActive = true;
	Priority = 1;

	CustomScale = FVector(1.0f, 1.0f, 1.0f);
	CustomRotation = FRotator::ZeroRotator;
	bUseCustomTransform = false;

	ActorColor = FLinearColor::Red;
	Opacity = 1.0f;
	bVisible = true;

	CustomDataVector = FVector::ZeroVector;
	CustomDataRotation = FQuat::Identity;
}

void AMyCustomActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void AMyCustomActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Handle property changes if needed
	if (PropertyChangedEvent.Property)
	{
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();

		if (PropertyName == GET_MEMBER_NAME_CHECKED(AMyCustomActor, bUseCustomTransform))
		{
			// Apply custom transform when enabled
			if (bUseCustomTransform)
			{
				SetActorScale3D(CustomScale);
				SetActorRotation(CustomRotation);
			}
		}
	}
}
#endif
