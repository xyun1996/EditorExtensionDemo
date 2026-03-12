// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCustomActor.generated.h"

/**
 * Custom Actor class for demonstrating Details Panel customization.
 */
UCLASS(BlueprintType, ClassGroup = "EditorExtensionDemo")
class EDITOREXTENSIONDEMO_API AMyCustomActor : public AActor
{
	GENERATED_BODY()

public:
	AMyCustomActor();

	// Basic Settings Category
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic Settings")
	FString DisplayLabel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic Settings")
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic Settings", meta = (ClampMin = "1", ClampMax = "100"))
	int32 Priority;

	// Transform Settings Category
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform Settings")
	FVector CustomScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform Settings")
	FRotator CustomRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform Settings")
	bool bUseCustomTransform;

	// Visual Settings Category
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Settings")
	FLinearColor ActorColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Settings")
	float Opacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Settings")
	bool bVisible;

	// Data Settings Category
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Settings")
	TArray<FString> CustomTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Settings")
	TMap<FString, float> NamedValues;

	// Example structure for customization
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Data")
	FVector CustomDataVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Data")
	FQuat CustomDataRotation;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Called when any property is changed
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
